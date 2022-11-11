#ifndef _VISTAOUTPUTMESSAGESET_H_
#define _VISTAOUTPUTMESSAGESET_H_

#include "EsamiView.h"

class CVistaOutputMessageSet : public CBaseSet
{
	DECLARE_DYNAMIC(CVistaOutputMessageSet)

public:

	CVistaOutputMessageSet();
	~CVistaOutputMessageSet();

	CString	m_sMessageId,
		m_sAppReceiver,
		m_sStoreTime,
		m_sRawMessage,
		m_sResponseMessage,
		m_sErrorMessage,
		m_sEndoxTable,
		m_sTableKey,
		m_sFlagName;		

	long m_lKeyValue,
		m_lFlagValue;

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

#endif /* _VISTAOUTPUTMESSAGESET_H_ */