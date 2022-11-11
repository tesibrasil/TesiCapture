#ifndef _CAMPIOBBLIGATORIPAZIENTESET_H_
	#define _CAMPIOBBLIGATORIPAZIENTESET_H_

#include "BaseSet.h"

class CCampiObbligatoriPazienteSet : public CBaseSet
{

	DECLARE_DYNAMIC(CCampiObbligatoriPazienteSet)

public:

	CCampiObbligatoriPazienteSet();

	long m_lID;
	CString m_sDescrizione;
	long m_lIDStatic;
	long m_lIDEdit;
	long m_lUO;
	BOOL m_bEliminato;

	// ridefinisco DeleteRecordset (in realt� cambia flag eliminato) //
	BOOL DeleteRecordset(const CString &strCommento);

	// mi serve per escludere i record di altre UO e quelli segnati come cancellati //
	CString SetBaseFilter(const CString &strFilter);

	// ridefinisco UpdateRecordset per salvare in automatico la UO per tutti i nuovi record //
	BOOL UpdateRecordset(const CString &strCommento);

	//
	BOOL IsObbligatorioEdit(long lIDEdit);
	BOOL IsObbligatorioStatic(long lIDStatic);
	CString GetDescrizione(long lIDEdit);

private:

	virtual void DoFieldExchange(CFieldExchange* pFX);
	virtual CString GetDefaultSQL();
	virtual void SetEmpty();

	virtual CBaseSet* CreateNew();
	virtual void CopyFields(CBaseSet* pOriginalSet, BOOL bCopyAll);
	virtual void SaveLog(CBaseSet* pOldValues, CBaseSet* pNewValues, const CString &sCommento);

};

#endif /* _CAMPIOBBLIGATORIPAZIENTESET_H_ */