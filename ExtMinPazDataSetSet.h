#ifndef _EXTMINPAZDATASETSET_H_
	#define _EXTMINPAZDATASETSET_H_

#include "BaseSet.h"

class CExtMinPazDataSetSet : public CBaseSet
{

	DECLARE_DYNAMIC(CExtMinPazDataSetSet)

public:

	CExtMinPazDataSetSet();

	BOOL	m_bCognome,
			m_bCognome2,
			m_bNome,
			m_bDataNascita,
			m_bIstatNascita,
			m_bIndirizzoResidenza,
			m_bIstatResidenza,
			m_bCapResidenza,
			m_bTelefono1,
			m_bTelefono2,
			m_bCellulare1,
			m_bCellulare2,
			m_bCodiceFiscale,
			m_bCodiceSanitario,
			m_bSesso;
	
private:

	virtual void DoFieldExchange(CFieldExchange* pFX);
	virtual CString GetDefaultSQL();
	virtual void SetEmpty();

	virtual CBaseSet* CreateNew() { return NULL; } // � una vista //
	virtual void CopyFields(CBaseSet* pOriginalSet, BOOL bCopyAll) { ; } // � una vista //
	virtual void SaveLog(CBaseSet* pOldValues, CBaseSet* pNewValues, const CString &sCommento) { ; } // � una vista //
	virtual BOOL DeleteRecordset(const CString &strCommento) { ASSERT(FALSE); return FALSE; } // � una vista //

};

#endif /* _EXTMINPAZDATASETSET_H_ */