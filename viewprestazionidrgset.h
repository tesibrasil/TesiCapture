#ifndef _VIEWPRESTAZIONIDRGSET_H_
	#define _VIEWPRESTAZIONIDRGSET_H_

#include "BaseSet.h"

class CViewPrestazioniDrgSet : public CBaseSet
{

	DECLARE_DYNAMIC(CViewPrestazioniDrgSet)

public:

	CViewPrestazioniDrgSet();

	long m_lId;
	long m_lData;
	long m_lQuantita;
	CString m_sCodNazionale;
	CString m_sCodRegionale;
	CString m_sDescrizione;
	float m_fCostoTotale;
	CString m_sSedeEsame;
	long m_lIdSedeEsame;

private:

	virtual void DoFieldExchange(CFieldExchange* pFX);
	virtual CString GetDefaultSQL();
	virtual void SetEmpty();

	virtual CBaseSet* CreateNew() { return NULL; } // � una vista //
	virtual void CopyFields(CBaseSet* pOriginalSet, BOOL bCopyAll) { ; } // � una vista //
	virtual void SaveLog(CBaseSet* pOldValues, CBaseSet* pNewValues, const CString &sCommento) { ; } // � una vista //
	virtual BOOL DeleteRecordset(const CString &strCommento) { ASSERT(FALSE); return FALSE; } // � una vista //

};

#endif /* _VIEWPRESTAZIONIDRGSET_H_ */