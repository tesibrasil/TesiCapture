#ifndef _RICERCAINDICAZIONISET_H_
	#define _RICERCAINDICAZIONISET_H_

#include "BaseSet.h"

class CRicercaIndicazioniSet : public CBaseSet
{

	DECLARE_DYNAMIC(CRicercaIndicazioniSet)

public:

	CRicercaIndicazioniSet();

	CString	m_sDescr1;
	CString	m_sDescr2;

private:

	virtual void DoFieldExchange(CFieldExchange* pFX);
	virtual CString GetDefaultSQL();
	virtual void SetEmpty();

	virtual CBaseSet* CreateNew() { return NULL; } // � una vista //
	virtual void CopyFields(CBaseSet* pOriginalSet, BOOL bCopyAll) { ; } // � una vista //
	virtual void SaveLog(CBaseSet* pOldValues, CBaseSet* pNewValues, const CString &sCommento) { ; } // � una vista //
	virtual BOOL DeleteRecordset(const CString &strCommento) { ASSERT(FALSE); return FALSE; } // � una vista //

};

#endif /* _RICERCAINDICAZIONISET_H_ */