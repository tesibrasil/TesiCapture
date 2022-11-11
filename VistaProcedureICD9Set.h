#ifndef _VISTAPROCEDUREICD9SET_H_
	#define _VISTAPROCEDUREICD9SET_H_

#include "BaseSet.h"

class CVistaProcedureICD9Set : public CBaseSet
{

	DECLARE_DYNAMIC(CVistaProcedureICD9Set)

public:

	CVistaProcedureICD9Set();

	long m_lID;
	CString m_sDescrizione;
	CString m_sCodificaInterna;
	CString m_sCodificaICD9Interni;
	CString m_sCodificaICD9Esterni;
	long m_lUO;
	long m_lIDTipoEsame;

	// mi serve per escludere i record di altre UO //
	CString SetBaseFilter(const CString &strFilter);

private:

	virtual void DoFieldExchange(CFieldExchange* pFX);
	virtual CString GetDefaultSQL();
	virtual void SetEmpty();

	virtual CBaseSet* CreateNew() { return NULL; } // � una vista //
	virtual void CopyFields(CBaseSet* pOriginalSet, BOOL bCopyAll) { ; } // � una vista //
	virtual void SaveLog(CBaseSet* pOldValues, CBaseSet* pNewValues, const CString &sCommento) { ; } // � una vista //
	virtual BOOL DeleteRecordset(const CString &strCommento) { ASSERT(FALSE); return FALSE; } // � una vista //

};

#endif /* _VISTAPROCEDUREICD9SET_H_ */