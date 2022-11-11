#include "stdafx.h"
#include "Endox.h"
#include "PrinterSettings.h"

#include "MyPrintDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CPrinterSettings::CPrinterSettings()
{
	m_hDevMode = m_hDevNames = NULL;
	m_hSaveDevMode = NULL;
	m_hSaveDevNames = NULL;
	m_strPrinterName = theApp.GetMessageString(IDS_PRINTER_NOT_SET); // PRINTERNAME_UNDEFINED;
	// m_strdirname = theApp.m_strDefaultDir + "\\" + PRINTERSETTINGS_DIRECTORYNAME;
}

/*
CPrinterSettings::CPrinterSettings(const CString dirname)
{
	CPrinterSettings();
	m_strdirname = dirname;
}
*/

CPrinterSettings::~CPrinterSettings()
{
	if (m_hDevMode)  GlobalFree(m_hDevMode);
	if (m_hDevNames) GlobalFree(m_hDevNames);
	RestorePrinter();
}

void CPrinterSettings::operator=(const CPrinterSettings* src)
{
	VERIFY(src != NULL);
	m_hDevMode = CopyHandle(src->m_hDevMode);
	m_hDevNames = CopyHandle(src->m_hDevNames);
	m_strPrinterName = src->m_strPrinterName;
	// m_strdirname = src->m_strdirname;
	m_strfilename = src->m_strfilename;
}

void CPrinterSettings::operator=(const CPrinterSettings& src)
{
	*this = &src;
}

//
// check to see if our printer (e.g. m_strprintername)
// is still available
//
BOOL CPrinterSettings::IsPrinterAvailable(LPCTSTR pszDeviceName)
{
	HANDLE hPrinter;
	if (OpenPrinter((char *) pszDeviceName, &hPrinter, NULL) == FALSE)
		return FALSE;
	ClosePrinter(hPrinter);
	return TRUE;
}


//
// change settings for global printer
//
// pszDeviceName is the name of your
// printer e.g. "Epson Stylus Color 750"
//
BOOL CPrinterSettings::SetPrinterDevice(LPCTSTR pszDeviceName) const
{
	HANDLE hPrinter;
	int ret = 0;
	PRINTER_DEFAULTS pd;
	
	ZeroMemory(&pd, sizeof(pd)); 
	// because of SetPrinter we need all access
	pd.DesiredAccess = PRINTER_ALL_ACCESS;
	if (OpenPrinter((char *) pszDeviceName, &hPrinter, &pd) == FALSE)
		return FALSE;

	DWORD dwBytesReturned, dwBytesNeeded;
	GetPrinter(hPrinter, 2, NULL, 0, &dwBytesNeeded);
	PRINTER_INFO_2* p2 = (PRINTER_INFO_2*)GlobalAlloc(GPTR,dwBytesNeeded);
	if (GetPrinter(hPrinter, 2, (LPBYTE)p2, dwBytesNeeded,&dwBytesReturned) == 0) {
		GlobalFree(p2);
		ClosePrinter(hPrinter);
		return FALSE;
	}
	// Lock handle for DEVMODE and copy DEVMODE data to PRINTER_INFO_2.
	// (Instead you can change DEVMODE-fields at p2->pDevMode directly)
	DEVMODE* pDevMode = (DEVMODE*) GlobalLock(m_hDevMode);
	int dwsize = sizeof(*p2->pDevMode);
	//CopyMemory(p2->pDevMode,pDevMode, sizeof(*p2->pDevMode));
	CopyDevmode(p2->pDevMode,pDevMode);
	// same for DEVNAMES.
	DEVNAMES* lpDevNames = (LPDEVNAMES)GlobalLock(m_hDevNames);
    LPTSTR lpszDeviceName = (LPTSTR )lpDevNames + lpDevNames->wDeviceOffset;
	// check to see if our settings are valid
	ret = DocumentProperties(NULL,hPrinter,lpszDeviceName,NULL,pDevMode,DM_IN_BUFFER);
	p2->pSecurityDescriptor = NULL;
//	p2->Attributes = 0;
	p2->Priority = 0;
	// change settings
	if (IDOK == ret)
		ret = SetPrinter(hPrinter,2,(LPBYTE)p2,0);
	GlobalFree(p2);             // free PRINTER_INFO_2.
	GlobalUnlock(m_hDevMode);	
	GlobalUnlock(m_hDevNames);
	ClosePrinter(hPrinter);
	// Notification: settings changed
	SendMessage(HWND_BROADCAST, WM_DEVMODECHANGE, 0L,
               (LPARAM)pszDeviceName);
	return ret;
}


//
// Show the printer common dialog
// Accept and store all settings
//
// This code is predominantly from a microsoft sample:
// HOWTO: Implementing an Application-Defined Default Printer 
// Article ID: Q193103 
//
CString CPrinterSettings::PrinterSetup(CWnd* pWnd, CString strTitle)
{
	// Ask the user which printer to use.
	ASSERT(pWnd);
	
	CMyPrintDialog pd(FALSE,PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE | PD_NOSELECTION, pWnd);

	// Make sure we don't accidentally create a device context
	pd.m_pd.Flags &= ~PD_RETURNDC;   // Reset flag set by constructor.

	// Force the CMyPrintDialog to use our device mode & name.
	if (m_hDevMode)
		pd.m_pd.hDevMode  = CopyHandle(m_hDevMode);
	if (m_hDevNames)
		pd.m_pd.hDevNames = CopyHandle(m_hDevNames);

	// Display the dialog box and let the user make their selection.

	if (pd.DoModal() == IDOK)	{
		// The user clicked OK
		// (and POSSIBLY changed printers).
		// In any case, the CMyPrintDialog logic made a copy of the original
		// DEVMODE/DEVNAMES that we passed it and applied the user's
		// changes to that copy and discarded the original copy we passed
		// it. (NOTE: If the user had clicked CANCEL instead, the original
		// values we passed would have been returned unchanged).
		if (m_hDevMode)
			GlobalFree(m_hDevMode);                      // Free old copies.
		if (m_hDevNames)
			GlobalFree(m_hDevNames);                     // Free old copies.
		if (pd.m_pd.hDevMode)
			m_hDevMode  = CopyHandle(pd.m_pd.hDevMode);  // Save new copies.
		if (pd.m_pd.hDevNames)
			m_hDevNames = CopyHandle(pd.m_pd.hDevNames); // Save new copies.
	}
	// Regardless of whether the user clicked OK or CANCEL,
	// we need to ALWAYS do a GlobalFree of CMyPrintDialog's
	// m_pd.hDevMode and m_pd.hDevNames upon return from
	// DoModal in order to prevent a resource leak.
	GlobalFree(pd.m_pd.hDevMode);   // Because DoModal was called,
	GlobalFree(pd.m_pd.hDevNames);  // we need to free these.
	return DevmodePrinterName();
}

//
// This code is from a microsoft sample:
// HOWTO: Implementing an Application-Defined Default Printer 
// Article ID: Q193103 
//
HANDLE CPrinterSettings::CopyHandle(HANDLE h)
{
	// Return a handle to a copy of the data
	// that the passed handle was for.
	if (!h) return NULL;
	DWORD   dwLen = GlobalSize(h);
	HANDLE hCopy = GlobalAlloc(GHND, dwLen);
	if (hCopy) {
		BYTE* lpCopy = (BYTE*)GlobalLock(hCopy);
		BYTE* lp     = (BYTE*)GlobalLock(h);
		CopyMemory(lpCopy,lp,dwLen);
		GlobalUnlock(hCopy);
		GlobalUnlock(h);
	}
	return hCopy;
}

//
// retrieve our printername
//
CString CPrinterSettings::DevmodePrinterName()
{
	if (!m_hDevMode)
	{
		m_strPrinterName = theApp.GetMessageString(IDS_PRINTER_NOT_SET); // PRINTERNAME_UNDEFINED;
	} 
	else
	{
		DEVMODE* pDevMode = (DEVMODE*)GlobalLock(m_hDevMode);
		m_strPrinterName = (LPCTSTR) (pDevMode->dmDeviceName);
		GlobalUnlock(m_hDevMode);
	}
	return m_strPrinterName;
}


void CPrinterSettings::CopyDefaultMfcPrinter()
{
	PRINTDLG pd;
	// Get MFC's printer
	if (AfxGetApp()->GetPrinterDeviceDefaults(&pd) ) {
		// Make a copy
		m_hDevNames = CopyHandle(pd.hDevNames);
		m_hDevMode = CopyHandle(pd.hDevMode);
		DevmodePrinterName();
	}
}

BOOL CPrinterSettings::SetThisPrinter()
{
	PRINTDLG pd;
	// Save MFC's printer and select ours instead.
	if (AfxGetApp()->GetPrinterDeviceDefaults(&pd)) {
		m_hSaveDevNames = pd.hDevNames;
		m_hSaveDevMode = pd.hDevMode;
		AfxGetApp()->SelectPrinter(m_hDevNames,m_hDevMode,FALSE);

		return (TRUE);
	}

	return (FALSE);

}

void CPrinterSettings::RestorePrinter()
{
	// Restore previous MFC printer if possible
	if (m_hSaveDevNames && m_hSaveDevMode)
		AfxGetApp()->SelectPrinter(m_hSaveDevNames,m_hSaveDevMode,FALSE);
	m_hSaveDevNames = NULL;
	m_hSaveDevMode = NULL;
}

//
// Save our settings to file
//
int CPrinterSettings::Save(LPCTSTR strFileName)
{
	SetFileName(strFileName);
	ASSERT( !m_strfilename.IsEmpty());
	// ASSERT( !m_strdirname.IsEmpty());
	ASSERT(m_hDevMode);
	ASSERT(m_hDevNames);
	DWORD dret = NO_ERROR;
	// create special dir for printer settings
	/*
	BOOL ret = CreateDirectory(m_strdirname,NULL);
	if (!ret) {
		dret = GetLastError();
		if (dret != ERROR_ALREADY_EXISTS) {
			TRACE("error at CreateDirectory: %d\n",dret);
			return dret;
		}
	}
	*/
	// ask for the length of current directoryname 
	// (e.g. "f:\code guru\sample\" )
	dret = GetCurrentDirectory(0,NULL);
	if (dret > 0 ) {
		CString od;
		// just big enough
		LPTSTR pst = od.GetBuffer(dret);
		// to hold the string
		dret = GetCurrentDirectory(dret,pst);
		// change to our directory
		// SetCurrentDirectory(m_strdirname);
		// create a new empty file
		HANDLE hDatei = 
			CreateFile(
				m_strfilename,
				GENERIC_READ,
				0,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);

		if (INVALID_HANDLE_VALUE != hDatei) {
			SetFileAttributes (m_strfilename, FILE_ATTRIBUTE_NORMAL);
		}

		CloseHandle (hDatei);

		hDatei = 
			CreateFile(
				m_strfilename,
				GENERIC_READ | GENERIC_WRITE,
				0,
				NULL,
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL);

		if (INVALID_HANDLE_VALUE == hDatei) {
			TRACE("error at CreateFile: %d\n",GetLastError());
			return GetLastError();
		}
		//
		// Devmode
		//
		DWORD   dwLen = GlobalSize(m_hDevMode);
		BYTE* lp     = (BYTE*)GlobalLock(m_hDevMode);
		// leading length
		VERIFY(WriteFile(hDatei,&dwLen,sizeof(dwLen),&dret,NULL));
		// structure data
		VERIFY(WriteFile(hDatei,lp,dwLen,&dret,NULL));
		if (dret != dwLen) {
			TRACE("printersettings: problem writing DevMode %d <-> %d\n",dwLen,dret);
		}
		GlobalUnlock(m_hDevMode);
		//
		// Devnames
		//
		dwLen = GlobalSize(m_hDevNames);
		lp     = (BYTE*)GlobalLock(m_hDevNames);
		// leading length
		VERIFY(WriteFile(hDatei,&dwLen,sizeof(dwLen),&dret,NULL));
		// structure data
		VERIFY(WriteFile(hDatei,lp,dwLen,&dret,NULL));
		if (dret != dwLen) {
			TRACE("printersettings: problem writing DevNames %d <-> %d\n",dwLen,dret);
		}
		GlobalUnlock(m_hDevNames);

		CloseHandle(hDatei);
		// restore to previous directory
		SetCurrentDirectory(pst);
		od.ReleaseBuffer();
	}
	return NO_ERROR;
}

//
// load our settings from file
//
int CPrinterSettings::Load(LPCTSTR strFileName)
{
	SetFileName(strFileName);
	ASSERT( !m_strfilename.IsEmpty());
	// ASSERT( !m_strdirname.IsEmpty());
	if (m_hDevMode) GlobalFree(m_hDevMode);
	if (m_hDevNames) GlobalFree(m_hDevNames);
	m_hDevMode = m_hDevNames = NULL;
	DWORD dret = NO_ERROR;
	
	// use a special directory for our printer settings
	/*
	BOOL ret = CreateDirectory(m_strdirname,NULL);
	if (!ret) {
		dret = GetLastError();
		if (dret != ERROR_ALREADY_EXISTS) {
			TRACE("error at CreateDirectory: %d\n",dret);
			return dret;
		}
	}
	*/
	// where are we
	dret = GetCurrentDirectory(0,NULL);
	if (dret > 0 ) {
		LPTSTR pst = new TCHAR[dret+1];
		dret = GetCurrentDirectory(dret,pst);
		// and where will we go
		// SetCurrentDirectory(m_strdirname);
		// open file, ensure it is present
		HANDLE hDatei = 
			CreateFile(
				m_strfilename,
				GENERIC_READ,
				0,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);
		// if the file could not be found
		// use MFC's fefault printer
		if (INVALID_HANDLE_VALUE == hDatei) {
			dret = GetLastError();
			CopyDefaultMfcPrinter();
			SetCurrentDirectory(pst);
			delete pst;
			DevmodePrinterName();
			return dret;
		}

		//
		// DEVMODE
		//
		// 
		DWORD   dwLen = 0;
		// read leading length
		VERIFY(ReadFile(hDatei,&dwLen,sizeof(dwLen),&dret,NULL));
		// if desired do more testing for proper size here
		ASSERT(dwLen < 0xFFFF);
		ASSERT(dwLen > 0);
		m_hDevMode = GlobalAlloc(GHND, dwLen);
		if (NULL == m_hDevMode) {
			dret = GetLastError();
			TRACE("error at GlobalAlloc m_hDevMode: %d\n",dret);
			CloseHandle(hDatei);
			return dret;
		}
		BYTE* lpCopy = (BYTE*)GlobalLock(m_hDevMode);
		// read the whole caboodle
		VERIFY(ReadFile(hDatei,lpCopy,dwLen,&dret,NULL));
		/*if (bHorizontal)
		{
			DEVMODE* pDevMode = (DEVMODE*)lpCopy;
			pDevMode->dmOrientation = DMORIENT_LANDSCAPE;
		}*/
		GlobalUnlock(m_hDevMode);
		//
		// DEVNAMES
		//
		// read leading length
		VERIFY(ReadFile(hDatei,&dwLen,sizeof(dwLen),&dret,NULL));
		// is it believable
		ASSERT(dwLen < 0xFFFF);
		ASSERT(dwLen > 0);
		m_hDevNames = GlobalAlloc(GHND, dwLen);
		if (NULL == m_hDevNames) {
			dret = GetLastError();
			TRACE("error at GlobalAlloc m_hDevNames: %d\n",dret);
			CloseHandle(hDatei);
			return dret;
		}
		lpCopy = (BYTE*)GlobalLock(m_hDevNames);
		// read DevNames structure
		VERIFY(ReadFile(hDatei,lpCopy,dwLen,&dret,NULL));
		GlobalUnlock(m_hDevNames);
		CloseHandle(hDatei);
		// restore former dir
		SetCurrentDirectory(pst);
		delete pst;
	}

	DevmodePrinterName();
	return NO_ERROR;
}


int CPrinterSettings::CopyDevmode(DEVMODE *pDest,const DEVMODE *pSrc) const
{
	int nBytes = min( pDest->dmSize + pDest->dmDriverExtra, pSrc->dmSize + pSrc->dmDriverExtra);
	CopyMemory(pDest,pSrc,nBytes);
	return nBytes;
}

HDC MyCreateDC(HGLOBAL hDevNames, HGLOBAL hDevMode)
{
	if (hDevNames == NULL)
		return NULL;

	LPDEVNAMES lpDevNames = (LPDEVNAMES)::GlobalLock(hDevNames);
	LPDEVMODE  lpDevMode = (hDevMode != NULL) ?
						(LPDEVMODE)::GlobalLock(hDevMode) : NULL;

	if (lpDevNames == NULL)
		return NULL;

	HDC hDC = ::CreateDC((LPCTSTR)lpDevNames + lpDevNames->wDriverOffset,
					  (LPCTSTR)lpDevNames + lpDevNames->wDeviceOffset,
					  (LPCTSTR)lpDevNames + lpDevNames->wOutputOffset,
					  lpDevMode);

	::GlobalUnlock(hDevNames);
	if (hDevMode != NULL)
		::GlobalUnlock(hDevMode);
	return hDC;
}


HDC CPrinterSettings::CreatePrinterDC()
{
	return ::MyCreateDC(m_hDevNames, m_hDevMode);
}

// Sandro // 04/01/2011 //
LPDEVMODE CPrinterSettings::GetDevModePointer()
{
	if (m_hDevMode == NULL)
		return NULL;

	// Davide - Aggiunto CopyHandle //
	return (LPDEVMODE)GlobalLock(CopyHandle(m_hDevMode));
}

// Sandro // 04/01/2011 //
LPDEVNAMES CPrinterSettings::GetDevNamesPointer()
{
	if (m_hDevNames == NULL)
		return NULL;

	// Davide - Aggiunto CopyHandle //
	return (LPDEVNAMES)GlobalLock(CopyHandle(m_hDevNames));
}

// Sandro // 04/01/2011 //
HGLOBAL CPrinterSettings::GetDevModeHandle()
{
	if (m_hDevMode == NULL)
		return NULL;

	return CopyHandle(m_hDevMode);
}

// Sandro // 04/01/2011 //
HGLOBAL CPrinterSettings::GetDevNamesHandle()
{
	if (m_hDevNames == NULL)
		return NULL;

	return CopyHandle(m_hDevNames);
}