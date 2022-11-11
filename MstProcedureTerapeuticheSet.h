#ifndef _MSTPROCEDURETERAPEUTICHESET_H_
	#define _MSTPROCEDURETERAPEUTICHESET_H_

#include "BaseSet.h"

class CMstProcedureTerapeuticheSet : public CBaseSet
{

	DECLARE_DYNAMIC(CMstProcedureTerapeuticheSet)

public:

	CMstProcedureTerapeuticheSet();

	long	m_lContatore;
	long	m_lTipoEsame;
	long	m_lPadre;
	long	m_lLivello;
	CString	m_sDescrizione;
	BOOL    m_bEliminato;

	// mi serve per escludere i record segnati come cancellati
	CString SetBaseFilter(const CString &strFilter);

	// ridefinisco DeleteRecordset (in realt� cambia flag eliminato
	BOOL DeleteRecordset(const CString &strCommento);

private:

	virtual void DoFieldExchange(CFieldExchange* pFX);
	virtual CString GetDefaultSQL();
	virtual void SetEmpty();

	virtual CBaseSet* CreateNew();
	virtual void CopyFields(CBaseSet* pOriginalSet, BOOL bCopyAll);
	virtual void SaveLog(CBaseSet* pOldValues, CBaseSet* pNewValues, const CString &sCommento);

};

#endif /* _MSTPROCEDURETERAPEUTICHESET_H_ */