#ifndef _FARMACIDISTINCTSET_H_
	#define _FARMACIDISTINCTSET_H_

#include "BaseSet.h"

class CFarmaciDistinctSet : public CBaseSet
{
	DECLARE_DYNAMIC(CFarmaciDistinctSet)

public:

	CFarmaciDistinctSet();

	CString m_sNome;
	CString m_sPrincipioAttivo; 

	BOOL OpenRecordset(const CString &strCommento);

private:

	virtual void DoFieldExchange(CFieldExchange* pFX);
	virtual CString GetDefaultSQL();
	virtual void SetEmpty();

	virtual CBaseSet* CreateNew() { return NULL; } // � una vista //
	virtual void CopyFields(CBaseSet* pOriginalSet, BOOL bCopyAll) { ; } // � una vista //
	virtual void SaveLog(CBaseSet* pOldValues, CBaseSet* pNewValues, const CString &sCommento) { ; } // � una vista //
	virtual BOOL DeleteRecordset(const CString &strCommento) { ASSERT(FALSE); return FALSE; } // � una vista //
};

#endif /* _FARMACIDISTINCTSET_H_ */