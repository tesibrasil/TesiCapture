#ifndef _VISTAINPUTMESSAGEWARNINGSET_H_
#define _VISTAINPUTMESSAGEWARNINGSET_H_

#include "EsamiView.h"

class CVistaInputMessageWarningSet : public CBaseSet
{
	DECLARE_DYNAMIC(CVistaInputMessageWarningSet)

public:

	CVistaInputMessageWarningSet();
	~CVistaInputMessageWarningSet();

	CString	m_sMessageId,
		m_sAppSender,
		m_sStoreTime,
		m_sEncodedMessage,
		m_sWarningMessage;

	long m_lId;

protected:

	void DoFieldExchange(CFieldExchange* pFX);
	CString GetDefaultSQL();

private:

	CBaseSet* CreateNew() { return NULL; } // � una vista //
	void CopyFields(CBaseSet* pOriginalSet, BOOL bCopyAll) { ; } // � una vista //
	void SaveLog(CBaseSet* pOldValues, CBaseSet* pNewValues, const CString &sCommento) { ; } // � una vista //

	BOOL DeleteRecordset(const CString &strCommento) { ASSERT(FALSE); return FALSE; } // � una vista //

};

#endif /* _VISTAINPUTMESSAGEWARNINGSET_H_ */