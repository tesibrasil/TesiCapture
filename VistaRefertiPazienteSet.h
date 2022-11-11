#ifndef _VISTAREFERTIPAZIENTESET_H_
	#define _VISTAREFERTIPAZIENTESET_H_

#include "BaseSet.h"

class CVistaRefertiPazienteSet : public CBaseSet
{

	DECLARE_DYNAMIC(CVistaRefertiPazienteSet)

public:

	CVistaRefertiPazienteSet();

	// long m_lIDPaziente;
	long m_lIDEsame;
	CString m_sTipoEsame;
	long m_lData;
	CString m_sReferto;

private:

	virtual void DoFieldExchange(CFieldExchange* pFX);
	virtual CString GetDefaultSQL();
	virtual void SetEmpty();

	virtual CBaseSet* CreateNew() { return NULL; } // � una vista //
	virtual void CopyFields(CBaseSet* pOriginalSet, BOOL bCopyAll) { ; } // � una vista //
	virtual void SaveLog(CBaseSet* pOldValues, CBaseSet* pNewValues, const CString &sCommento) { ; } // � una vista //
	virtual BOOL DeleteRecordset(const CString &strCommento) { ASSERT(FALSE); return FALSE; } // � una vista //

};

#endif /* _VISTAREFERTIPAZIENTESET_H_ */