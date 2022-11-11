#ifndef _VISTAESAMIPERWLSET_H_
	#define _VISTAESAMIPERWLSET_H_

#include "BaseSet.h"

class CVistaEsamiPerWLSet : public CBaseSet
{

	DECLARE_DYNAMIC(CVistaEsamiPerWLSet)

public:

	CVistaEsamiPerWLSet();

	long m_lContatore;
	CString m_sDescrizione;
	CString m_sDataEsame;
	CString m_sMedico;
	CString m_sSalaEsame;
	BOOL	m_bSuperLocked;

private:

	virtual void DoFieldExchange(CFieldExchange* pFX);
	virtual CString GetDefaultSQL();
	virtual void SetEmpty();

	virtual CBaseSet* CreateNew() { return NULL; } // � una vista //
	virtual void CopyFields(CBaseSet* pOriginalSet, BOOL bCopyAll) { ; } // � una vista //
	virtual void SaveLog(CBaseSet* pOldValues, CBaseSet* pNewValues, const CString &sCommento) { ; } // � una vista //
	virtual BOOL DeleteRecordset(const CString &strCommento) { ASSERT(FALSE); return FALSE; } // � una vista //

};

#endif /* _VISTAESAMIPERWLSET_H_ */