#ifndef _CENTRIDICOSTOSET_H_
	#define _CENTRIDICOSTOSET_H_

#include "BaseSet.h"

class CCentriDiCostoSet : public CBaseSet
{
	DECLARE_DYNAMIC(CCentriDiCostoSet)

public:

	CCentriDiCostoSet();

	long	m_lContatore;
	CString	m_sDescrizione;
	long m_lUO;
	BOOL    m_bEliminato;

	//
	CString GetFieldText(long lContatore);

	// ridefinisco DeleteRecordset (in realt� cambia flag eliminato) //
	BOOL DeleteRecordset(const CString &strCommento);

	// mi serve per escludere i record di altre UO e quelli segnati come cancellati //
	CString SetBaseFilter(const CString &strFilter);

	// ridefinisco UpdateRecordset per salvare in automatico la UO per tutti i nuovi record //
	BOOL UpdateRecordset(const CString &strCommento);

private:

	virtual void DoFieldExchange(CFieldExchange* pFX);
	virtual CString GetDefaultSQL();
	virtual void SetEmpty();

	virtual CBaseSet* CreateNew();
	virtual void CopyFields(CBaseSet* pOriginalSet, BOOL bCopyAll);
	virtual void SaveLog(CBaseSet* pOldValues, CBaseSet* pNewValues, const CString &sCommento);

};

#endif /* _MSTORGANOSET_H_ */