#ifndef _ERRORIESTERNISET_H_
	#define _ERRORIESTERNISET_H_

#include "BaseSet.h"

class CErroriEsterniSet : public CBaseSet
{

	DECLARE_DYNAMIC(CErroriEsterniSet)

public:

	CErroriEsterniSet();

	long m_lIDServizio;
	BOOL m_bErrore;
	CString m_sMessaggio;

private:

	virtual void DoFieldExchange(CFieldExchange* pFX);
	virtual CString GetDefaultSQL();
	virtual void SetEmpty();

	virtual CBaseSet* CreateNew() { return NULL; } // � una vista //
	virtual void CopyFields(CBaseSet* pOriginalSet, BOOL bCopyAll) { ; } // � una vista //
	virtual void SaveLog(CBaseSet* pOldValues, CBaseSet* pNewValues, const CString &sCommento) { ; } // � una vista //
	virtual BOOL DeleteRecordset(const CString &strCommento) { ASSERT(FALSE); return FALSE; } // � una vista //

};

#endif /* _ERRORIESTERNISET_H_ */