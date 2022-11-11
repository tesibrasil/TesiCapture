#ifndef _VISTASNOMEDSET_H_
	#define _VISTASNOMEDSET_H_

#include "BaseSet.h"

class CVistaSnomedSet : public CBaseSet
{

	DECLARE_DYNAMIC(CVistaSnomedSet)

public:

	CVistaSnomedSet();

	CString m_sCodice;
	CString m_sDescrizione;

private:

	virtual void DoFieldExchange(CFieldExchange* pFX);
	virtual CString GetDefaultSQL();
	virtual void SetEmpty();

	virtual CBaseSet* CreateNew() { return NULL; } // � una vista //
	virtual void CopyFields(CBaseSet* pOriginalSet, BOOL bCopyAll) { ; } // � una vista //
	virtual void SaveLog(CBaseSet* pOldValues, CBaseSet* pNewValues, const CString &sCommento) { ; } // � una vista //
	virtual BOOL DeleteRecordset(const CString &strCommento) { ASSERT(FALSE); return FALSE; } // � una vista //

};

#endif /* _VISTASNOMEDSET_H_ */