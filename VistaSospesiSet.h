#ifndef _VISTASOSPESISET_H_
	#define _VISTASOSPESISET_H_

#include "BaseSet.h"

class CVistaSospesiSet : public CBaseSet
{

	DECLARE_DYNAMIC(CVistaSospesiSet)

public:

	CVistaSospesiSet();

	long m_lIDPaziente;
	CString m_sCognome;
	CString m_sNome;
	long m_lNatoIlCustom;
	long m_lIDEsame;
	CString m_sTipoEsame;
	long m_lData;
	long m_lDataScadenzaSospensione;
	CString m_sMotivo;
	CString m_sMotivoSospensione;

private:

	virtual void DoFieldExchange(CFieldExchange* pFX);
	virtual CString GetDefaultSQL();
	virtual void SetEmpty();

	virtual CBaseSet* CreateNew() { return NULL; } // � una vista //
	virtual void CopyFields(CBaseSet* pOriginalSet, BOOL bCopyAll) { ; } // � una vista //
	virtual void SaveLog(CBaseSet* pOldValues, CBaseSet* pNewValues, const CString &sCommento) { ; } // � una vista //
	virtual BOOL DeleteRecordset(const CString &strCommento) { ASSERT(FALSE); return FALSE; } // � una vista //

};

#endif /* _VISTASOSPESISET_H_ */