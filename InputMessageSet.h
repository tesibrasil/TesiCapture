#ifndef _INPUTMESSAGESET_H_
#define _INPUTMESSAGESET_H_

#include "EsamiView.h"

class CInputMessageSet : public CBaseSet
{
	DECLARE_DYNAMIC(CInputMessageSet)

public:

	CInputMessageSet();
	~CInputMessageSet();

	long	m_lId;

	CString	m_sStoreTime,
		m_sMessageId,
		m_sAppSender,
		m_sEncodedMessage,
		m_sSynchronousMessage,
		m_sErrorMessage;

	BOOL	m_bErrored;
	

protected:

	void DoFieldExchange(CFieldExchange* pFX);
	CString GetDefaultSQL();

private:

	CBaseSet* CreateNew() { return NULL; } // � una vista //
	void CopyFields(CBaseSet* pOriginalSet, BOOL bCopyAll) { ; } // � una vista //
	void SaveLog(CBaseSet* pOldValues, CBaseSet* pNewValues, const CString &sCommento) { ; } // � una vista //

	BOOL DeleteRecordset(const CString &strCommento) { ASSERT(FALSE); return FALSE; } // � una vista //

};

#endif /* _INPUTMESSAGESET_H_ */