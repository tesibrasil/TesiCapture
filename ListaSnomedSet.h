#ifndef _LISTASNOMEDSET_H_
	#define _LISTASNOMEDSET_H_

#include "BaseSet.h"

class CListaSnomedSet : public CBaseSet  
{

	DECLARE_DYNAMIC(CListaSnomedSet)

public:

	CListaSnomedSet();
	virtual ~CListaSnomedSet();

	long	m_lIDPaziente;
	long	m_lIDEsame;
	long	m_lData;
	CString m_sCodici;
	CString m_sDescrizioni;

private:

	virtual void DoFieldExchange(CFieldExchange* pFX);
	virtual CString GetDefaultSQL();
	virtual void SetEmpty();

	virtual CBaseSet* CreateNew() { return NULL; } // � una vista //
	virtual void CopyFields(CBaseSet* pOriginalSet, BOOL bCopyAll) { ; } // � una vista //
	virtual void SaveLog(CBaseSet* pOldValues, CBaseSet* pNewValues, const CString &sCommento) { ; } // � una vista //
	virtual BOOL DeleteRecordset(const CString &strCommento) { ASSERT(FALSE); return FALSE; } // � una vista //

};

#endif /* _LISTASNOMEDSET_H_*/