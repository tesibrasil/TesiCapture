#include "stdafx.h"
#include "Endox.h"
#include "AcqFromDiskDermoxSceltaMultiplaDlg.h"

#include <io.h>
#include <math.h>
#include <algorithm>

#include "DLL_Imaging\h\ImgArchive.h"

#include "MisureConfig\SerializerDLL.h"

#include "Calib.h"
#include "CalibList.h"
#include "Common.h"
#include "EditStringDlg.h"
#include "EsamiView.h"
#include "LogFileWriter.h"
#include "MaximizeDlg.h"
#include "MmSystem.h"
#include "PuntiPazienteSet.h"
#include "PuntiSediSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define RAGGIO_PUNTO_1 3
#define RAGGIO_PUNTO_2 6

IMPLEMENT_DYNAMIC(CAcqFromDiskDermoxSceltaMultiplaDlg, CEndoxResizableDlg)

CAcqFromDiskDermoxSceltaMultiplaDlg::CAcqFromDiskDermoxSceltaMultiplaDlg(CWnd* pParent, CEsamiView* pEsamiView, CStringList *photoFileList, BOOL bDeleteAfterAcquisition)
: CEndoxResizableDlg(CAcqFromDiskDermoxSceltaMultiplaDlg::IDD, pParent)
{
	ASSERT(pEsamiView != NULL);

	m_pEsamiView = pEsamiView;
	m_pEsamiView->m_pImgManager->SetCurrentIDs(m_pEsamiView->m_pEsamiSet->m_lPaziente, m_pEsamiView->m_pEsamiSet->m_lContatore);

	m_rectFrameLive = CRect(0, 0, 0, 0);
	m_rectFrameSagoma = CRect(0, 0, 0, 0);
	m_rectPaintSagoma = CRect(0, 0, 0, 0);
	m_rectFrameZoom = CRect(0, 0, 0, 0);

	m_iCurSelPointIndex = -1;
	m_iCurSelPointX = -1;
	m_iCurSelPointY = -1;

	m_lIDSede = 0;

	m_sCalib = "";

	m_photoFileList = photoFileList;
	m_bDeleteAfterAcquisition = bDeleteAfterAcquisition;

	// --- //

	m_bFillingList = TRUE;
}

CAcqFromDiskDermoxSceltaMultiplaDlg::~CAcqFromDiskDermoxSceltaMultiplaDlg()
{		
	POSITION pos = m_listImagesTemp.GetHeadPosition();
	while (pos != NULL)
	{
		IMG imgTemp = m_listImagesTemp.GetAt(pos);

		DeleteObject(imgTemp.hBmp);
		DeleteObject(imgTemp.hBmpSelected);

		m_listImagesTemp.GetNext(pos);
	}
}

BEGIN_MESSAGE_MAP(CAcqFromDiskDermoxSceltaMultiplaDlg, CEndoxResizableDlg)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()

	ON_MESSAGE(EPM_IMAGESEL_NOTIFY, OnImageSel)
	ON_MESSAGE(EPM_THUMB_DBLCLICK, OnNMDblClkListImages)

	ON_BN_CLICKED(IDC_BTN_PHOTO, OnBnClickedBtnPhoto)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBnClickedBtnExit)

	ON_CBN_SELCHANGE(IDC_COMBO_CALIB, OnCbnSelchangeComboCalib)

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_POINTS, OnLvnItemchangedListPoints)	
	
	ON_BN_CLICKED(IDC_BTN_DELETE, &CAcqFromDiskDermoxSceltaMultiplaDlg::OnBnClickedBtnDelete)
	ON_BN_CLICKED(IDC_BTN_SEL_DESEL_ALL, &CAcqFromDiskDermoxSceltaMultiplaDlg::OnBnClickedBtnSelDeselAll)
END_MESSAGE_MAP()

void CAcqFromDiskDermoxSceltaMultiplaDlg::DoDataExchange(CDataExchange* pDX)
{
	CEndoxResizableDlg::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_PHOTO, m_ctrlBtnPhoto);
	DDX_Control(pDX, IDC_BTN_DELETE, m_ctrlBtnDelete);

	DDX_Control(pDX, IDC_BTN_EXIT, m_ctrlBtnExit);

	DDX_Control(pDX, IDC_COMBO_CALIB, m_ctrlComboCalib);

	DDX_Control(pDX, IDC_LIST_POINTS, m_ctrlListPoints);
	DDX_Control(pDX, IDC_LIST_IMAGES, m_ctrlListImages);
	DDX_Control(pDX, IDC_LIST_IMAGES_TEMP, m_ctrlListImagesTemp);

	DDX_Control(pDX, IDC_STATIC_SAGOMA, m_ctrlFrameSagoma);	
	DDX_Control(pDX, IDC_STATIC_ZOOM, m_ctrlFrameZoom);
	DDX_Control(pDX, IDC_STATIC_SEDE1, m_ctrlFrameSede);

	DDX_Control(pDX, IDC_STATIC_SEDE2, m_ctrlStaticSede);
	DDX_Control(pDX, IDC_STATIC_FRAME_CALIB, m_ctrlStaticFrameCalib);
}

void CAcqFromDiskDermoxSceltaMultiplaDlg::OnCancel()
{
}

BOOL CAcqFromDiskDermoxSceltaMultiplaDlg::OnInitDialog()
{
	CEndoxResizableDlg::OnInitDialog();

	BeginWaitCursor();

	BOOL bOk = FALSE;

	char szPathImm[MAX_PATH];
	char szPathMin[MAX_PATH];
	if (CImgArchive(&theApp.m_dbEndox).GetArchivePrimary(szPathImm, szPathMin, MAX_PATH))
	{
		CString fileSagomeName = "sagome.reg";

		CString sFileSagome;
		sFileSagome.Format("%s\\Organ\\sagome.org", szPathImm);

		CString sFileSedi;
		sFileSedi.Format("%s\\Organ\\%s", szPathImm, fileSagomeName);

		if (_access(sFileSagome, 00) == 0)
		{
			if (_access(sFileSedi, 00) == 0)
			{
				HBITMAP hBit;

				// sagome //
				if (m_pEsamiView->m_pImgManager->LoadLocalImageEx(sFileSagome, &hBit) > 0)
					if (hBit != INVALID_HANDLE_VALUE)
						bOk = m_dibSagome.Attach(CDib::BitmapToDIB(hBit, NULL), TRUE);
				DeleteObject(hBit);

				// sedi //
				if (m_pEsamiView->m_pImgManager->LoadLocalImageEx(sFileSedi, &hBit) > 0)
					if (hBit != INVALID_HANDLE_VALUE)
						bOk = m_dibSedi.Attach(CDib::BitmapToDIB(hBit, NULL), TRUE);
				DeleteObject(hBit);
			}
			else
			{
				CString strError;
				strError.Format(theApp.GetMessageString(IDS_FILE_SAGOME_NON_TROVATO), sFileSedi);
				theApp.AfxMessageBoxEndo(strError);
			}
		}
		else
		{
			CString strError;
			strError.Format(theApp.GetMessageString(IDS_FILE_SAGOME_NON_TROVATO), sFileSagome);
			theApp.AfxMessageBoxEndo(strError);
		}		
	}
	else
	{
		ASSERT(FALSE);
	}

	if (bOk)
	{
		AddAnchor(IDC_BTN_EXIT, CSize(50, 100), CSize(100, 100));
		AddAnchor(IDC_BTN_DELETE, CSize(50, 100), CSize(50, 100));
		AddAnchor(IDC_BTN_SEL_DESEL_ALL, CSize(50, 100), CSize(50, 100));
		AddAnchor(IDC_BTN_PHOTO, CSize(50, 100), CSize(100, 100));

		AddAnchor(IDC_COMBO_CALIB, CSize(50, 100), CSize(100, 100));

		AddAnchor(IDC_LIST_POINTS, CSize(0, 0), CSize(50, 0));
		AddAnchor(IDC_LIST_IMAGES, CSize(0, 100), CSize(50, 100));
		AddAnchor(IDC_LIST_IMAGES_TEMP, CSize(50, 0), CSize(100, 100));

		AddAnchor(IDC_STATIC_SAGOMA, CSize(0, 0), CSize(50, 100));		
		AddAnchor(IDC_STATIC_ZOOM, CSize(0, 100), CSize(0, 100));
		AddAnchor(IDC_STATIC_SEDE1, CSize(0, 100), CSize(0, 100));

		AddAnchor(IDC_STATIC_SEDE2, CSize(0, 100), CSize(0, 100));
		AddAnchor(IDC_STATIC_FRAME_CALIB, CSize(75, 100), CSize(100, 100));

		//

		m_ctrlStaticSede.SetFont(&theApp.m_fontBold);
		m_ctrlBtnPhoto.SetFont(&theApp.m_fontBold);
		m_ctrlBtnExit.SetFont(&theApp.m_fontBold);
		m_ctrlStaticFrameCalib.SetFont(&theApp.m_fontBold);
		m_ctrlComboCalib.SetFont(&theApp.m_fontBold);

		//

		//SystemParametersInfo(SPI_GETWORKAREA, 0, &rectDesk, 0);
		SetWindowPos(NULL, theApp.m_rectDesk.left, theApp.m_rectDesk.top, theApp.m_rectDesk.right - theApp.m_rectDesk.left, theApp.m_rectDesk.bottom - theApp.m_rectDesk.top, SWP_NOOWNERZORDER); // non mettere n� prima n� dopo, va bene qui //

		//

		m_ctrlListPoints.SetExtendedStyle(m_ctrlListPoints.GetExtendedStyle() | LVS_EX_BORDERSELECT | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_LABELTIP);
		m_ctrlListPoints.InsertColumn(0, "", LVCFMT_LEFT, 0); // empty //
		m_ctrlListPoints.InsertColumn(1, "", LVCFMT_RIGHT, 0); // numero //
		m_ctrlListPoints.InsertColumn(2, theApp.GetMessageString(IDS_SEDEANATOMICA), LVCFMT_LEFT, 0); // sede //
		m_ctrlListPoints.InsertColumn(3, theApp.GetMessageString(IDS_PATTERN), LVCFMT_LEFT, 0); // pattern //
		m_ctrlListPoints.InsertColumn(4, theApp.GetMessageString(IDS_ASPORTAZIONE), LVCFMT_LEFT, 0); // asportazione //
		m_ctrlListPoints.InsertColumn(5, "", LVCFMT_LEFT, 0); // empty //

		//

		m_listImages.Create(HSIZE_PAL_4, VSIZE_PAL_4, ILC_COLOR32, 0, 1);
		m_ctrlListImages.SetExtendedStyle(m_ctrlListImages.GetExtendedStyle() | LVS_EX_BORDERSELECT);
		m_ctrlListImages.SetImageList(&m_listImages, LVSIL_NORMAL);
		m_ctrlListImages.SetIconSpacing(HSIZE_PAL_4 + 5, VSIZE_PAL_4 + 5);

		//

		int nIndex = 0;
		POSITION pos1 = theApp.m_pListCalib->GetFirstCalibPos();
		while (pos1 != NULL)
		{
			CCalib* pCalib = theApp.m_pListCalib->GetNextCalib(pos1);

			m_ctrlComboCalib.InsertString(nIndex, pCalib->m_strName);
			m_ctrlComboCalib.SetItemDataPtr(nIndex, pCalib);

			nIndex++;
		}

		//
		LoadSettings();

		//		
		//riempio lista delle immagini acquisite
		//

		int i = 0;
		POSITION pos = m_photoFileList->GetHeadPosition();
		while (pos)
		{
			CString imgPath = m_photoFileList->GetNext(pos);
			HBITMAP hBmp;
			HBITMAP hBmpSel;
			if (m_pEsamiView->m_pImgManager->LoadLocalImageEx(imgPath, &hBmp, &hBmpSel, HSIZE_PAL_4, VSIZE_PAL_4) > 0)
			{
				IMG imgTemp;

				imgTemp.lID = 0;
				imgTemp.type = _image_;
				imgTemp.hBmp = hBmp;
				imgTemp.hBmpSelected = hBmpSel;
				strcpy_s(imgTemp.szDescription, imgPath);

				m_listImagesTemp.AddTail(imgTemp);
			}						

			i++;
		}

		m_ctrlListImagesTemp.SetThumbs(&m_listImagesTemp);
		m_ctrlListImagesTemp.SetMessageDest(this, NULL);
								

		//
		CString strFilter;
		strFilter.Format("IDPaziente=%li", m_pEsamiView->m_pPazientiSet->m_lContatore);

		CPuntiPazienteSet setPunti;
		setPunti.SetOpenFilter(strFilter);
		if (setPunti.OpenRecordset("CAcqFromDiskDermoxSceltaMultiplaDlg::OnInitDialog"))
		{
			while (!setPunti.IsEOF())
			{
				tagPuntiDermox tagTemp;
				tagTemp.lIDPunto = setPunti.m_lID;
				tagTemp.ptPunto.x = setPunti.m_lX;
				tagTemp.ptPunto.y = setPunti.m_lY;
				tagTemp.lNumeroDermox = setPunti.m_lNumeroDermox;
				sprintf_s(tagTemp.szSede, 255, "%s", setPunti.m_sSede);
				sprintf_s(tagTemp.szPattern, 255, "%s", GetTextPattern(setPunti.m_lPattern));
				tagTemp.bAsportazione = setPunti.m_bAsportazione;

				m_listPunti.AddTail(tagTemp);

				setPunti.MoveNext();
			}

			setPunti.CloseRecordset("CAcqFromDiskDermoxSceltaMultiplaDlg::OnInitDialog");
		}

		//
		CPuntiSediSet setSedi;
		if (setSedi.OpenRecordset("CAcqFromDiskDermoxSceltaMultiplaDlg::OnInitDialog"))
		{
			while (!setSedi.IsEOF())
			{
				tagSede tagTemp;

				tagTemp.m_lID = setSedi.m_lID;
				tagTemp.m_sSede = setSedi.m_sSede;
				tagTemp.m_sSede.MakeUpper();

				m_listSedi.AddTail(tagTemp);

				setSedi.MoveNext();
			}

			setSedi.CloseRecordset("CAcqFromDiskDermoxSceltaMultiplaDlg::OnInitDialog");
		}

		//
		m_ctrlListPoints.SetRedraw(FALSE);
		m_ctrlListPoints.DeleteAllItems();

		int nItem = 0;
		POSITION pos2 = m_listPunti.GetHeadPosition();
		while (pos2 != NULL)
		{
			CString strTemp;
			tagPuntiDermox tagTemp = m_listPunti.GetAt(pos2);

			m_ctrlListPoints.InsertItem(nItem, "");
			m_ctrlListPoints.SetItemData(nItem, (DWORD)tagTemp.lIDPunto);

			strTemp.Format("%li", tagTemp.lNumeroDermox);
			m_ctrlListPoints.SetItemText(nItem, 1, strTemp);

			m_ctrlListPoints.SetItemText(nItem, 2, tagTemp.szSede);

			m_ctrlListPoints.SetItemText(nItem, 3, tagTemp.szPattern);

			m_ctrlListPoints.SetItemText(nItem, 4, tagTemp.bAsportazione ? theApp.GetMessageString(IDS_YES) : theApp.GetMessageString(IDS_NO));

			nItem++;
			m_listPunti.GetNext(pos2);
		}

		m_ctrlListPoints.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
		m_ctrlListPoints.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
		m_ctrlListPoints.SetColumnWidth(3, LVSCW_AUTOSIZE_USEHEADER);
		m_ctrlListPoints.SetColumnWidth(4, LVSCW_AUTOSIZE_USEHEADER);
		m_ctrlListPoints.SetRedraw(TRUE);

		m_bFillingList = FALSE;

		if (m_iCurSelPointIndex >= 0)
		{
			m_ctrlListPoints.SetItemState(m_iCurSelPointIndex, LVIS_SELECTED, LVIS_SELECTED);
			m_ctrlListPoints.EnsureVisible(m_iCurSelPointIndex, FALSE);
		}

		//
		theApp.LocalizeDialog(this, CAcqFromDiskDermoxSceltaMultiplaDlg::IDD, "AcqFromDiskDermoxSceltaMultiplaDlg");

		//
		m_ctrlBtnPhoto.SetWindowText(theApp.GetMessageString(IDS_ACQUISIZIONE_IMMAGINE3));
	}
	else
	{
		m_dibImageFile.ReleaseContents();
		m_dibImageFile.Detach();

		m_dibSagome.ReleaseContents();
		m_dibSagome.Detach();

		m_dibSedi.ReleaseContents();
		m_dibSedi.Detach();

		EndDialog(IDCANCEL);
	}

	EndWaitCursor();

	return TRUE;
}

void CAcqFromDiskDermoxSceltaMultiplaDlg::OnOK()
{	
}

void CAcqFromDiskDermoxSceltaMultiplaDlg::OnBnClickedBtnExit()
{
	if (m_ctrlListImagesTemp.GetItemCount() > 0)
	{
		if (theApp.AfxMessageBoxEndo(theApp.GetMessageString(IDS_DERMOXACQ_EXIT_WARNING), MB_YESNO) == IDNO)
			return;
	}

	CResizableDialog::OnCancel();
}

void CAcqFromDiskDermoxSceltaMultiplaDlg::OnBnClickedBtnPhoto()
{
	if (m_iCurSelPointIndex < 0)
	{
		if (theApp.AfxMessageBoxEndo(theApp.GetMessageString(IDS_DERMOXACQ_PHOTO_WARNING), MB_YESNO) == IDNO)
			return;
	}
	else if (theApp.AfxMessageBoxEndo(theApp.GetMessageString(IDS_DERMOXACQ_PHOTO_CONFIRM), MB_YESNO) == IDNO)
		return;
	
	//
	BeginWaitCursor();	

	// importo il tutto //
	long lImmaginiImportate = ImportaImmagini();

	// aggiorno la finestra delle immagini //
	if (lImmaginiImportate > 0)
		CaricaImmaginiPunto();

	if (m_ctrlListImagesTemp.GetItemCount() == 0)
		OnBnClickedBtnExit();

	//Refresh dei punti
	m_ctrlListImagesTemp.DeselectAll();
	m_iCurSelPointIndex = 0;
	POSITION pos = m_ctrlListPoints.GetFirstSelectedItemPosition();

	while (pos)
	{
		int index = m_ctrlListPoints.GetNextSelectedItem(pos);
		m_ctrlListPoints.SetItemState(index, 0, LVIS_SELECTED);
	}

	RefreshPoints();

	//
	EndWaitCursor();

	//
	//CResizableDialog::OnOK();
	
}

void CAcqFromDiskDermoxSceltaMultiplaDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	point.x++;
	point.y++;
	if (m_rectPaintSagoma.PtInRect(point))
	{
		point = ScreenToImage(point);
		SelezionaPunto(point);
	}	
}

void CAcqFromDiskDermoxSceltaMultiplaDlg::OnLvnItemchangedListPoints(NMHDR *pNMHDR, LRESULT *pResult)
{
	RefreshPoints();
}

void CAcqFromDiskDermoxSceltaMultiplaDlg::RefreshPoints()
{
	if (!m_bFillingList)
	{
		BeginWaitCursor();

		POSITION pos1 = m_ctrlListPoints.GetFirstSelectedItemPosition();
		if (pos1 != NULL)
		{
			m_iCurSelPointIndex = m_ctrlListPoints.GetNextSelectedItem(pos1);

			POSITION pos2 = m_listPunti.FindIndex(m_iCurSelPointIndex);
			m_iCurSelPointX = m_listPunti.GetAt(pos2).ptPunto.x;
			m_iCurSelPointY = m_listPunti.GetAt(pos2).ptPunto.y;

			InvalidateRect(m_rectFrameSagoma, FALSE);
			InvalidateRect(m_rectFrameZoom, FALSE);
		}
		else
		{
			m_iCurSelPointIndex = -1;
			m_iCurSelPointX = -1;
			m_iCurSelPointY = -1;

			m_lIDSede = 0;

			InvalidateRect(m_rectFrameSagoma, FALSE);
			InvalidateRect(m_rectFrameZoom, FALSE);
		}

		CaricaDatiPunto();
		CaricaImmaginiPunto();

		EndWaitCursor();
	}
}

void CAcqFromDiskDermoxSceltaMultiplaDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CEndoxResizableDlg::OnMouseMove(nFlags, point);

	// calcolo l'area del rect di zoom //
	InvalidateRect(m_rectFrameZoom, FALSE);
}

LRESULT CAcqFromDiskDermoxSceltaMultiplaDlg::OnNMDblClkListImages(WPARAM wParam, LPARAM lParam)
{
	int i = (int)wParam;
	if (POSITION pos = m_listImagesTemp.FindIndex(i))
	{
		IMG img = m_listImagesTemp.GetAt(pos);

		CDib dibTemp;
		HBITMAP hBitNor;
		if (m_pEsamiView->m_pImgManager->LoadLocalImageEx(img.szDescription, &hBitNor) > 0)
		{
			if (hBitNor != NULL)
			{
				dibTemp.Attach(CDib::BitmapToDIB(hBitNor, NULL), TRUE);

				if (dibTemp.IsValidDib())
				{
					CMaximizeDlg dlg(this, &dibTemp);
					dlg.DoModal();

					dibTemp.ReleaseContents();
				}
			}

			DeleteObject(hBitNor);
		}
	}

	return (LRESULT)1;
}

void CAcqFromDiskDermoxSceltaMultiplaDlg::OnPaint()
{
	CPaintDC dc(this);

	///////////////////
	// SAGOMA GRANDE //
	///////////////////

	// disegno la dib //
	m_rectPaintSagoma = m_dibSagome.PaintDIBSagome(&dc, m_rectFrameSagoma);
	// TRACE("   SAGOME IN L%04li R%04li T%04li B%04li\n", m_rectSagoma.left, m_rectSagoma.right, m_rectSagoma.top, m_rectSagoma.bottom);

	/////////////////////////
	// PUNTI SAGOMA GRANDE //
	/////////////////////////

	CPen penSel(PS_SOLID, 1, RGB_RED);
	CBrush brushSel(RGB_RED);
	CPen penNormal(PS_SOLID, 1, RGB_BLUE);
	CBrush brushNormal(RGB_BLUE);
	CPen penAsportaz(PS_SOLID, 1, RGB_BLACK);
	CBrush brushAsportaz(RGB_BLACK);
	COLORREF crOldColor;

	dc.SetBkMode(TRANSPARENT);

	int i = 0;
	POSITION pos = m_listPunti.GetHeadPosition();
	while (pos != NULL)
	{
		CPen* pOldPen;
		CBrush* pOldBrush;
		if (i == m_iCurSelPointIndex)
		{
			pOldPen = dc.SelectObject(&penSel);
			pOldBrush = dc.SelectObject(&brushSel);
			crOldColor = dc.SetTextColor(RGB_RED);
		}
		else
		{
			if (m_listPunti.GetAt(pos).bAsportazione)
			{
				pOldPen = dc.SelectObject(&penAsportaz);
				pOldBrush = dc.SelectObject(&brushAsportaz);
				crOldColor = dc.SetTextColor(RGB_BLACK);
			}
			else
			{
				pOldPen = dc.SelectObject(&penNormal);
				pOldBrush = dc.SelectObject(&brushNormal);
				crOldColor = dc.SetTextColor(RGB_BLUE);
			}
		}

		CPoint ptToPaint = m_listPunti.GetAt(pos).ptPunto;
		ptToPaint = ImageToScreen(ptToPaint);

		CRect rectTemp(ptToPaint.x - RAGGIO_PUNTO_1, ptToPaint.y - RAGGIO_PUNTO_1, ptToPaint.x + RAGGIO_PUNTO_1, ptToPaint.y + RAGGIO_PUNTO_1);

		dc.Ellipse(rectTemp);

		if (theApp.m_bNumeriSuImmagineSagoma)
		{
			CString sTemp;
			sTemp.Format("%li", m_listPunti.GetAt(pos).lNumeroDermox);

			CFont* pOldFont = dc.SelectObject(&theApp.m_fontBold);
			int nOldBkMode = dc.SetBkMode(OPAQUE);
			dc.TextOut(ptToPaint.x + 4, ptToPaint.y - 9, sTemp);
			dc.SetBkMode(nOldBkMode);
			dc.SelectObject(pOldFont);
		}

		dc.SelectObject(pOldPen);
		dc.SelectObject(pOldBrush);
		dc.SetTextColor(crOldColor);

		m_listPunti.GetNext(pos);
		i++;
	}

	/////////////////////////
	// FINESTRELLA DI ZOOM //
	/////////////////////////

	// disegno	nella finestra di zoom //
	RECT rectPaintZoom = { 0, 0, 0, 0 };
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(&ptMouse);
	ptMouse.x++;
	ptMouse.y++;
	if (m_rectPaintSagoma.PtInRect(ptMouse))
	{
		// se sono sulla sagoma faccio lo zoom intorno al cursore //

		// TRACE2("   CURSOR IN    X%04li       Y%04li\n", ptMouse.x, ptMouse.y);
		POINT ptToZoom;
		ptToZoom.x = ptMouse.x - m_rectPaintSagoma.left;
		ptToZoom.x = ptToZoom.x * m_dibSagome.Width() / m_rectPaintSagoma.Width();
		ptToZoom.y = ptMouse.y - m_rectPaintSagoma.top;
		ptToZoom.y = ptToZoom.y * m_dibSagome.Height() / m_rectPaintSagoma.Height();
		ptToZoom.y = m_dibSagome.Height() - ptToZoom.y;
		// TRACE2("   CURSOR IN   (X%04li)     (Y%04li)\n", ptToZoom.x, ptToZoom.y);

		rectPaintZoom = m_dibSagome.PaintDIBSagomeZoom(&dc, m_rectFrameZoom, ptToZoom);
		LONG lTeeemp = rectPaintZoom.top;
		rectPaintZoom.top = m_dibSagome.Height() - rectPaintZoom.bottom;
		rectPaintZoom.bottom = m_dibSagome.Height() - lTeeemp;
		// TRACE("    ZOOM  IN L%04li R%04li T%04li B%04li\n\n", rectPaintZoom.left, rectPaintZoom.right, rectPaintZoom.top, rectPaintZoom.bottom);

		///////////////////
		// TESTO REGIONE //
		///////////////////

		BYTE bytePixel;
		m_dibSedi.GetPixel(ptToZoom.x, ptToZoom.y, &bytePixel);
		//TRACE("   GETPIX IN X%04li %04li) --> %li\n", ptToZoom.x, ptToZoom.y, (int)bytePixel);

		if ((long)bytePixel != m_lIDSede)
		{
			CString strSede = "";
			m_lIDSede = (long)bytePixel;

			if ((m_lIDSede > 0) && (m_lIDSede < 255))
			{
				POSITION pos = m_listSedi.GetHeadPosition();

				while (pos != NULL)
				{
					tagSede tagTemp = m_listSedi.GetNext(pos);

					if (tagTemp.m_lID == m_lIDSede)
					{
						strSede = tagTemp.m_sSede;
						break;
					}
				}
			}

			m_ctrlStaticSede.SetWindowText(strSede);
		}
	}
	else
	{
		if ((m_iCurSelPointIndex >= 0) && (m_iCurSelPointX >= 0) && (m_iCurSelPointY >= 0))
		{
			// se sono fuori dalla sagoma ma c'� un punto selezionato ci faccio lo zoom attorno //

			POINT ptToZoom;
			ptToZoom.x = m_iCurSelPointX;
			ptToZoom.y = m_iCurSelPointY;
			ptToZoom.y = m_dibSagome.Height() - ptToZoom.y;

			rectPaintZoom = m_dibSagome.PaintDIBSagomeZoom(&dc, m_rectFrameZoom, ptToZoom);
			LONG lTeeemp = rectPaintZoom.top;
			rectPaintZoom.top = m_dibSagome.Height() - rectPaintZoom.bottom;
			rectPaintZoom.bottom = m_dibSagome.Height() - lTeeemp;
			// TRACE("    ZOOM  IN L%04li R%04li T%04li B%04li\n\n", rectPaintZoom.left, rectPaintZoom.right, rectPaintZoom.top, rectPaintZoom.bottom);

			///////////////////
			// TESTO REGIONE //
			///////////////////

			BYTE bytePixel;
			m_dibSedi.GetPixel(ptToZoom.x, ptToZoom.y, &bytePixel);
			// TRACE3("   GETPIX IN X%04li %04li) --> \n", ptToZoom.x, ptToZoom.y);

			if ((long)bytePixel != m_lIDSede)
			{
				CString strSede = "";
				m_lIDSede = (long)bytePixel;

				if ((m_lIDSede > 0) && (m_lIDSede < 255))
				{
					POSITION pos = m_listSedi.GetHeadPosition();

					while (pos != NULL)
					{
						tagSede tagTemp = m_listSedi.GetNext(pos);

						if (tagTemp.m_lID == m_lIDSede)
						{
							strSede = tagTemp.m_sSede;
							break;
						}
					}
				}

				m_ctrlStaticSede.SetWindowText(strSede);
			}
		}
		else
		{
			// se sono fuori dalla sagoma e non c'� un punto selezionato svuoto la finestra di zoom //

			CBrush brushNull;
			brushNull.CreateSolidBrush(RGB_BLACK);
			dc.FillRect(m_rectFrameZoom, &brushNull);
			brushNull.DeleteObject();

			///////////////////
			// TESTO REGIONE //
			///////////////////

			m_lIDSede = 0;
			m_ctrlStaticSede.SetWindowText("");
		}
	}

	///////////////////////////////
	// PUNTI FINESTRELLA DI ZOOM //
	///////////////////////////////

	dc.SetBkMode(TRANSPARENT);

	i = 0;
	pos = m_listPunti.GetHeadPosition();
	while (pos != NULL)
	{
		CPoint ptToPaint = m_listPunti.GetAt(pos).ptPunto;
		if (CRect(rectPaintZoom).PtInRect(ptToPaint))
		{
			CPen* pOldPen;
			CBrush* pOldBrush;
			if (i == m_iCurSelPointIndex)
			{
				pOldPen = dc.SelectObject(&penSel);
				pOldBrush = dc.SelectObject(&brushSel);
				crOldColor = dc.SetTextColor(RGB_RED);
			}
			else
			{
				if (m_listPunti.GetAt(pos).bAsportazione)
				{
					pOldPen = dc.SelectObject(&penAsportaz);
					pOldBrush = dc.SelectObject(&brushAsportaz);
					crOldColor = dc.SetTextColor(RGB_BLACK);
				}
				else
				{
					pOldPen = dc.SelectObject(&penNormal);
					pOldBrush = dc.SelectObject(&brushNormal);
					crOldColor = dc.SetTextColor(RGB_BLUE);
				}
			}

			ptToPaint = ImageToScreenZoom(ptToPaint, rectPaintZoom);

			CRect rectTemp(ptToPaint.x - RAGGIO_PUNTO_2, ptToPaint.y - RAGGIO_PUNTO_2, ptToPaint.x + RAGGIO_PUNTO_2, ptToPaint.y + RAGGIO_PUNTO_2);

			dc.Ellipse(rectTemp);

			if (theApp.m_bNumeriSuImmagineSagoma)
			{
				CString sTemp;
				sTemp.Format("%li", m_listPunti.GetAt(pos).lNumeroDermox);

				CFont* pOldFont = dc.SelectObject(&theApp.m_fontBigBig);
				int nOldBkMode = dc.SetBkMode(TRANSPARENT);
				dc.TextOut(ptToPaint.x + 6, ptToPaint.y - 13, sTemp);
				dc.SetBkMode(nOldBkMode);
				dc.SelectObject(pOldFont);
			}

			dc.SelectObject(pOldPen);
			dc.SelectObject(pOldBrush);
			dc.SetTextColor(crOldColor);
		}

		m_listPunti.GetNext(pos);
		i++;
	}
	penSel.DeleteObject();
	brushSel.DeleteObject();
	penNormal.DeleteObject();
	brushNormal.DeleteObject();
}

void CAcqFromDiskDermoxSceltaMultiplaDlg::OnSize(UINT nType, int cx, int cy)
{
	CEndoxResizableDlg::OnSize(nType, cx, cy);

	if (IsWindow(m_ctrlFrameSagoma))
	{
		m_ctrlFrameSagoma.GetWindowRect(&m_rectFrameSagoma);
		ScreenToClient(&m_rectFrameSagoma);
	}

	if (IsWindow(m_ctrlFrameZoom))
	{
		m_ctrlFrameZoom.GetWindowRect(&m_rectFrameZoom);
		ScreenToClient(&m_rectFrameZoom);
	}
}

CPoint CAcqFromDiskDermoxSceltaMultiplaDlg::ImageToScreen(CPoint ptPoint)
{
	CPoint ptReturn;

	ptReturn.x = ptPoint.x * m_rectPaintSagoma.Width() / m_dibSagome.Width();
	ptReturn.x = ptReturn.x + m_rectPaintSagoma.left;
	ptReturn.y = ptPoint.y * m_rectPaintSagoma.Height() / m_dibSagome.Height();
	ptReturn.y = ptReturn.y + m_rectPaintSagoma.top;

	return ptReturn;
}

CPoint CAcqFromDiskDermoxSceltaMultiplaDlg::ImageToScreenZoom(CPoint ptPoint, CRect rectImageZoom)
{
	CPoint ptReturn;

	ptReturn.x = ((ptPoint.x - rectImageZoom.left) * m_rectFrameZoom.Width() / rectImageZoom.Width()) + m_rectFrameZoom.left;
	ptReturn.y = ((ptPoint.y - rectImageZoom.top) * m_rectFrameZoom.Height() / rectImageZoom.Height()) + m_rectFrameZoom.top;

	return ptReturn;
}

CPoint CAcqFromDiskDermoxSceltaMultiplaDlg::ScreenToImage(CPoint ptPoint)
{
	CPoint ptReturn;

	ptReturn.x = ptPoint.x - m_rectPaintSagoma.left;
	ptReturn.x = ptReturn.x * m_dibSagome.Width() / m_rectPaintSagoma.Width();
	ptReturn.y = ptPoint.y - m_rectPaintSagoma.top;
	ptReturn.y = ptReturn.y * m_dibSagome.Height() / m_rectPaintSagoma.Height();

	return ptReturn;
}

int CAcqFromDiskDermoxSceltaMultiplaDlg::AggiungiPunto(CString sSede, CPoint ptPoint)
{
	int nReturn = -1;

	CPuntiPazienteSet setTemp;
	setTemp.SetOpenFilter("ID=0");
	if (setTemp.OpenRecordset("CAcqFromDiskDermoxSceltaMultiplaDlg::AggiungiPunto"))
	{
		if (setTemp.AddNewRecordset("CAcqFromDiskDermoxSceltaMultiplaDlg::AggiungiPunto"))
		{
			setTemp.m_lIDPaziente = m_pEsamiView->m_pPazientiSet->m_lContatore;
			setTemp.m_sSede = sSede;
			setTemp.m_lX = ptPoint.x;
			setTemp.m_lY = ptPoint.y;

			if (setTemp.UpdateRecordset("CAcqFromDiskDermoxSceltaMultiplaDlg::AggiungiPunto"))
			{
				CString strFilter;
				strFilter.Format("ID=%li", setTemp.GetLastAdd());

				setTemp.CloseRecordset("CAcqFromDiskDermoxSceltaMultiplaDlg::AggiungiPunto");
				setTemp.SetOpenFilter(strFilter);
				if (setTemp.OpenRecordset("CAcqFromDiskDermoxSceltaMultiplaDlg::AggiungiPunto"))
				{
					if (!setTemp.IsEOF())
					{
						tagPuntiDermox puntoTemp;
						puntoTemp.lIDPunto = setTemp.m_lID;
						puntoTemp.ptPunto = ptPoint;
						puntoTemp.lNumeroDermox = setTemp.m_lNumeroDermox;
						sprintf_s(puntoTemp.szSede, 255, "%s", sSede);
						sprintf_s(puntoTemp.szPattern, 255, "%s", "");
						puntoTemp.bAsportazione = setTemp.m_bAsportazione;

						m_listPunti.AddTail(puntoTemp);

						nReturn = m_listPunti.GetCount() - 1;
					}
				}
			}
		}

		setTemp.CloseRecordset("CAcqFromDiskDermoxSceltaMultiplaDlg::AggiungiPunto");
	}

	return nReturn;
}

void CAcqFromDiskDermoxSceltaMultiplaDlg::CaricaDatiPunto()
{
	// ID del punto //
	long lIDPunto = 0;
	CString strSede = "";

	//
	if (m_iCurSelPointIndex >= 0)
	{
		tagPuntiDermox tagTemp = m_listPunti.GetAt(m_listPunti.FindIndex(m_iCurSelPointIndex));
		lIDPunto = tagTemp.lIDPunto;
		strSede = tagTemp.szSede;
	}

	//
	CString strButton = theApp.GetMessageString(IDS_ACQUISIZIONE_IMMAGINE3);
	if (lIDPunto > 0)
	{
		if (strSede.IsEmpty())
			strButton.Format(theApp.GetMessageString(IDS_ACQUISIZIONE_IMMAGINE1));
		else
			strButton.Format(theApp.GetMessageString(IDS_ACQUISIZIONE_IMMAGINE2), strSede);
	}
	m_ctrlBtnPhoto.SetWindowText(strButton);
}

void CAcqFromDiskDermoxSceltaMultiplaDlg::CaricaImmaginiPunto()
{
	// ID del punto //
	long lIDPunto = 0;
	if (m_iCurSelPointIndex >= 0)
		lIDPunto = m_listPunti.GetAt(m_listPunti.FindIndex(m_iCurSelPointIndex)).lIDPunto;

	if (lIDPunto > 0)
	{
		// lista delle immagini di quel punto //
		CImgList listImg;
		int nImageCount = m_pEsamiView->m_pImgManager->GetImgListByPoint(m_pEsamiView->m_pEsamiSet->m_lPaziente, lIDPunto, &listImg);

		// reimposto la lista di immagini ed il relativo controllo //
		m_listImages.SetImageCount(nImageCount);
		m_ctrlListImages.DeleteAllItems();

		// riempio di nuovo tutto //
		for (int i = 0; i < nImageCount; i++)
		{
			IMG img = listImg.GetAt(listImg.FindIndex(i));

			m_listImages.Replace(i, CBitmap::FromHandle(img.hBmp), RGB(0, 0, 0));
			m_ctrlListImages.InsertItem(i, CEsamiSet().GetDataEsame(img.lIDEsame), i);
			m_ctrlListImages.SetItemData(i, (DWORD)img.lID);

			DeleteObject(img.hBmp);
			DeleteObject(img.hBmpSelected);
		}
	}
	else
	{
		m_listImages.SetImageCount(0);
		m_ctrlListImages.DeleteAllItems();
	}
}

long CAcqFromDiskDermoxSceltaMultiplaDlg::ImportaImmagini()
{
	long lReturn;

	CThumbIDList* selThumbIDList = m_ctrlListImagesTemp.GetSelectedThumb();
	POSITION pos = selThumbIDList->GetHeadPosition();
	int *selectedIndexList = new int[selThumbIDList->GetCount()];

	int index = -1;
	while (pos)
	{
		index++;
		selectedIndexList[index] = selThumbIDList->GetNext(pos);

		//Importo l'immagine se esiste
		CString imgPath = m_ctrlListImagesTemp.GetDescription(selectedIndexList[index]);
		
		if (_access(imgPath, 0) == 0)
		{
			CStringList listPath;

			// controllo che la dimensione del file sia maggiore di 0 (capita per certi filmati e si blocca tutto) //
			HANDLE hFile = CreateFile(imgPath, STANDARD_RIGHTS_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if ((hFile != NULL) && (hFile != INVALID_HANDLE_VALUE))
			{
				LARGE_INTEGER llTemp;
				if (GetFileSizeEx(hFile, &llTemp))
				{
					if (llTemp.QuadPart > 0)
						listPath.AddHead(imgPath);
				}
			}
			CloseHandle(hFile);

			// creo il relativo file della calibrazione //
			int nIndex = m_ctrlComboCalib.GetCurSel();
			if (nIndex >= 0)
			{
				CCalib* pCalib = (CCalib*)m_ctrlComboCalib.GetItemDataPtr(nIndex);

				if (pCalib != NULL)
					SetCalib(imgPath, pCalib);
			}

			BOOL bExportToPacs = FALSE;
			// In modalit� 2 devo avviare il trasferimento automatico su PACS
			if (theApp.m_lDcmStoreMode == 2)
				bExportToPacs = TRUE;

			// ID del punto //
			long lIDPunto = 0;
			if (m_iCurSelPointIndex >= 0)
				lIDPunto = m_listPunti.GetAt(m_listPunti.FindIndex(m_iCurSelPointIndex)).lIDPunto;


			CImgIDList listIDImageImported;
			m_pEsamiView->m_pImgManager->SetCurrentIDs(m_pEsamiView->m_pEsamiSet->m_lPaziente, m_pEsamiView->m_pEsamiSet->m_lContatore);

			DWORD timeForLog = timeGetTime(); // Sandro 16/07/2014 //
			if (m_pEsamiView->m_pImgManager->ImportImgList(theApp.m_sUtenteLoggato, theApp.m_sNomeComputer, theApp.m_sNomeImpianto, lIDPunto, &listPath, FORM_IMM, FORM_MINI, &listIDImageImported, bExportToPacs, FALSE, FALSE) > 0)
			{
				long tempReturn = 0;

				if (listIDImageImported.GetCount() > 0)
				{
					tempReturn = listIDImageImported.GetHead();					
				}

				lReturn = listIDImageImported.GetCount();

				// importo anche il file .MIS associato //
				if (tempReturn > 0)
				{
					CString strLastFile = m_pEsamiView->m_pImgManager->GetFullPath(tempReturn);

					CString strOrigMis = imgPath.Left(imgPath.GetLength() - 4) + ".MIS";
					CString strDestMis = strLastFile.Left(strLastFile.GetLength() - 4) + ".MIS";

					MoveFile(strOrigMis, strDestMis);
				}
				// --- //

			}
			if (theApp.m_bSaveCompleteLog) // Sandro 16/07/2014 //
				CLogFileWriter::Get()->WriteLogLine0_ImmStore(timeGetTime() - timeForLog, m_pEsamiView->m_pEsamiSet->m_lPaziente, m_pEsamiView->m_pEsamiSet->m_lContatore, listIDImageImported.GetCount(), "CAcqFromDiskDermoxSceltaMultiplaDlg::ImportaImmagini");

			listPath.RemoveAll();

		}
	}

	
	//Elimino le immagini importate
	std::sort(selectedIndexList, selectedIndexList + index + 1); //Devo riordinare l'array altrimenti rischio di non cancellare tutto

	for (int i = index; i >= 0; i--)
	{
		POSITION pos = m_listImagesTemp.FindIndex(selectedIndexList[i]);

		if (pos)
		{
			IMG imgTemp = m_listImagesTemp.GetAt(pos);

			DeleteObject(imgTemp.hBmp);
			DeleteObject(imgTemp.hBmpSelected);

			m_listImagesTemp.RemoveAt(pos);

		}		

	}

	m_ctrlListImagesTemp.SetThumbs(&m_listImagesTemp);

	delete(selectedIndexList);	

	return lReturn;
}

void CAcqFromDiskDermoxSceltaMultiplaDlg::SelezionaPunto(CPoint ptPoint)
{
	BeginWaitCursor();

	int nIndex = TrovaPunto(ptPoint);
	if (nIndex >= 0)
	{
		if (nIndex != m_iCurSelPointIndex)
		{
			m_iCurSelPointIndex = nIndex; // il punto esiste gi�, mi basta soltanto selezionarlo //
			m_iCurSelPointX = ptPoint.x;
			m_iCurSelPointY = ptPoint.y;

			InvalidateRect(m_rectFrameSagoma, FALSE);
			InvalidateRect(m_rectFrameZoom, FALSE);

			//
			for (int i = 0; i < m_ctrlListPoints.GetItemCount(); i++)
			{
				if (i == nIndex)
				{
					m_ctrlListPoints.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
					m_ctrlListPoints.EnsureVisible(i, FALSE);
				}
				else
				{
					m_ctrlListPoints.SetItemState(i, 0, LVIS_SELECTED);
				}
			}
			// OnLvnItemchangedListPoints(NULL, NULL);
		}
	}
	else
	{
		CString strSede;
		m_ctrlStaticSede.GetWindowText(strSede);
		strSede.Trim();

		CString strMessage;
		if (strSede.IsEmpty())
			strMessage.Format(theApp.GetMessageString(IDS_QUESTION_ADD_PUNTO1));
		else
			strMessage.Format(theApp.GetMessageString(IDS_QUESTION_ADD_PUNTO2), strSede);

		if (theApp.AfxMessageBoxEndo(strMessage, MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			int nTemp = AggiungiPunto(strSede, ptPoint); // il punto non esiste, lo aggiungo //

			// se tutto � andato bene seleziono il nuovo punto aggiunto //
			if (nTemp >= 0)
			{
				m_iCurSelPointIndex = nTemp;
				m_iCurSelPointX = ptPoint.x;
				m_iCurSelPointY = ptPoint.y;

				InvalidateRect(m_rectFrameSagoma, FALSE);
				InvalidateRect(m_rectFrameZoom, FALSE);

				// --- //

				int nItem = m_ctrlListPoints.GetItemCount();

				CString strTemp;
				tagPuntiDermox tagTemp = m_listPunti.GetTail();

				m_ctrlListPoints.InsertItem(nItem, "");
				m_ctrlListPoints.SetItemData(nItem, (DWORD)tagTemp.lIDPunto);

				strTemp.Format("%li", tagTemp.lNumeroDermox);
				m_ctrlListPoints.SetItemText(nItem, 1, strTemp);

				m_ctrlListPoints.SetItemText(nItem, 2, tagTemp.szSede);

				m_ctrlListPoints.SetItemText(nItem, 3, tagTemp.szPattern);

				m_ctrlListPoints.SetItemText(nItem, 4, tagTemp.bAsportazione ? theApp.GetMessageString(IDS_YES) : theApp.GetMessageString(IDS_NO));

				m_ctrlListPoints.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
				m_ctrlListPoints.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
				m_ctrlListPoints.SetColumnWidth(3, LVSCW_AUTOSIZE_USEHEADER);
				m_ctrlListPoints.SetColumnWidth(4, LVSCW_AUTOSIZE_USEHEADER);

				//
				for (int i = 0; i < m_ctrlListPoints.GetItemCount(); i++)
				{
					if (i == nItem)
					{
						m_ctrlListPoints.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
						m_ctrlListPoints.EnsureVisible(i, FALSE);
					}
					else
					{
						m_ctrlListPoints.SetItemState(i, 0, LVIS_SELECTED);
					}
				}
				// OnLvnItemchangedListPoints(NULL, NULL);
			}
			else
			{
				theApp.AfxMessageBoxEndo(theApp.GetMessageString(IDS_ERROR_ADD_PUNTO), MB_ICONSTOP);
			}
		}
	}

	// CaricaDatiPunto();
	// CaricaImmaginiPunto();

	EndWaitCursor();
}

int CAcqFromDiskDermoxSceltaMultiplaDlg::TrovaPunto(CPoint& ptPoint)
{
	int nIndex = 0;

	POSITION pos = m_listPunti.GetHeadPosition();
	while (pos != NULL)
	{
		CPoint ptTemp = m_listPunti.GetAt(pos).ptPunto;

		if (sqrt(pow((double)(ptPoint.x - ptTemp.x), 2) + pow((double)(ptPoint.y - ptTemp.y), 2)) <= RAGGIO_PUNTO_1)
		{
			ptPoint = ptTemp;
			return nIndex;
		}

		m_listPunti.GetNext(pos);
		nIndex++;
	}

	return -1;
}

LRESULT CAcqFromDiskDermoxSceltaMultiplaDlg::OnImageSel(WPARAM wParam, LPARAM lParam)
{
	m_ctrlBtnDelete.EnableWindow((int)lParam > 0);
	m_ctrlBtnPhoto.EnableWindow((int)lParam > 0);

	return (LRESULT)1;
}


void CAcqFromDiskDermoxSceltaMultiplaDlg::OnBnClickedBtnDelete()
{
	if (theApp.AfxMessageBoxEndo(theApp.GetMessageString(IDS_DERMOXACQ_DELETE_WARNING), MB_YESNO) == IDYES)
	{
		CThumbIDList* selThumbIDList = m_ctrlListImagesTemp.GetSelectedThumb();
		POSITION pos = selThumbIDList->GetHeadPosition();

		int *selectedIndexList = new int[selThumbIDList->GetCount()];

		int index = -1;
		while (pos)
		{
			index++;
			selectedIndexList[index] = selThumbIDList->GetNext(pos);
		}


		//Elimino le immagini
		std::sort(selectedIndexList, selectedIndexList + index + 1); //Devo riordinare l'array altrimenti rischio di non cancellare tutto

		for (int i = index; i >= 0; i--)
		{
			POSITION pos = m_listImagesTemp.FindIndex(selectedIndexList[i]);

			if (pos)
			{
				IMG imgTemp = m_listImagesTemp.GetAt(pos);

				DeleteObject(imgTemp.hBmp);
				DeleteObject(imgTemp.hBmpSelected);

				m_listImagesTemp.RemoveAt(pos);

			}			
		}

		m_ctrlListImagesTemp.SetThumbs(&m_listImagesTemp);

		delete(selectedIndexList);
	}
}

void CAcqFromDiskDermoxSceltaMultiplaDlg::OnCbnSelchangeComboCalib()
{
	CString sSection = "SorgenteDermox10";

	int nTemp = m_ctrlComboCalib.GetCurSel();
	if (nTemp >= 0)
	{
		CString sTemp;
		m_ctrlComboCalib.GetLBText(nTemp, sTemp);
		WritePrivateProfileString(sSection, "Calib", sTemp, theApp.m_sFileConfig);
	}
}

void CAcqFromDiskDermoxSceltaMultiplaDlg::LoadSettings()
{
	CString sSection = "SorgenteDermox10";

	m_sCalib = GetPrivateProfileString(sSection, "Calib", "", theApp.m_sFileConfig);
	m_ctrlComboCalib.SetCurSel(m_ctrlComboCalib.FindStringExact(-1, m_sCalib));
}

void CAcqFromDiskDermoxSceltaMultiplaDlg::OnBnClickedBtnSelDeselAll()
{
	if (m_ctrlListImagesTemp.GetItemCount() > m_ctrlListImagesTemp.GetSelectedThumb()->GetCount())
		m_ctrlListImagesTemp.SelectAll();
	else
		m_ctrlListImagesTemp.DeselectAll();
}
