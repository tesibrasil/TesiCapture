#ifndef _VISTAGRUPPISET_H_
	#define _VISTAGRUPPISET_H_

#include "BaseSet.h"

class CVistaGruppiSet : public CBaseSet
{

	DECLARE_DYNAMIC(CVistaGruppiSet)

public:

	CVistaGruppiSet();

	CString m_sNome;
	long m_lPermessi;
	long m_lUO;

	long GetUO(CString sNome);

private:

	virtual void DoFieldExchange(CFieldExchange* pFX);
	virtual CString GetDefaultSQL();
	virtual void SetEmpty();

	virtual CBaseSet* CreateNew() { return NULL; } // � una vista //
	virtual void CopyFields(CBaseSet* pOriginalSet, BOOL bCopyAll) { ; } // � una vista //
	virtual void SaveLog(CBaseSet* pOldValues, CBaseSet* pNewValues, const CString &sCommento) { ; } // � una vista //
	virtual BOOL DeleteRecordset(const CString &strCommento) { ASSERT(FALSE); return FALSE; } // � una vista //

};

#endif /* _VISTAGRUPPISET_H_ */