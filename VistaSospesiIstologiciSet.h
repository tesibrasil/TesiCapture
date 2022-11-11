#ifndef _VISTASOSPESIISTOLOGICISET_H_
	#define _VISTASOSPESIISTOLOGICISET_H_

#include "BaseSet.h"

class CVistaSospesiIstologiciSet : public CBaseSet
{

	DECLARE_DYNAMIC(CVistaSospesiIstologiciSet)

public:

	CVistaSospesiIstologiciSet();

	long m_lIDPaziente;
	long m_lIDEsame;
	CString m_sCognome;
	CString m_sNome;
	long m_lNatoIlCustom;
	long m_lData;
	CString m_sDescrizioneEsame;
	long m_lMedico;
	CString m_sCognomeMedico;
	CString m_sNomeMedico;

private:

	virtual void DoFieldExchange(CFieldExchange* pFX);
	virtual CString GetDefaultSQL();
	virtual void SetEmpty();

	virtual CBaseSet* CreateNew() { return NULL; } // � una vista //
	virtual void CopyFields(CBaseSet* pOriginalSet, BOOL bCopyAll) { ; } // � una vista //
	virtual void SaveLog(CBaseSet* pOldValues, CBaseSet* pNewValues, const CString &sCommento) { ; } // � una vista //
	virtual BOOL DeleteRecordset(const CString &strCommento) { ASSERT(FALSE); return FALSE; } // � una vista //

};

#endif /* _VISTASOSPESIISTOLOGICISET_H_ */