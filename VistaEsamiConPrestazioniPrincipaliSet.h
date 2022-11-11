#ifndef _VISTAESAMICONPRESTAZIONIPRINCIPALISET_H_
	#define _VISTAESAMICONPRESTAZIONIPRINCIPALISET_H_

#include "BaseSet.h"

class CVistaEsamiConPrestazioniPrincipaliSet : public CBaseSet
{

	DECLARE_DYNAMIC(CVistaEsamiConPrestazioniPrincipaliSet)

public:

	CVistaEsamiConPrestazioniPrincipaliSet();

	BOOL TrovaEsame(long lIDEsame);

private:

	long m_lEsame;

	//

	virtual void DoFieldExchange(CFieldExchange* pFX);
	virtual CString GetDefaultSQL();
	virtual void SetEmpty();

	virtual CBaseSet* CreateNew() { return NULL; } // � una vista //
	virtual void CopyFields(CBaseSet* pOriginalSet, BOOL bCopyAll) { ; } // � una vista //
	virtual void SaveLog(CBaseSet* pOldValues, CBaseSet* pNewValues, const CString &sCommento) { ; } // � una vista //
	virtual BOOL DeleteRecordset(const CString &strCommento) { ASSERT(FALSE); return FALSE; } // � una vista //

};

#endif /* _VISTAESAMICONPRESTAZIONIPRINCIPALISET_H_ */