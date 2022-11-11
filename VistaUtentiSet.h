#ifndef _VISTAUTENTISET_H_
	#define _VISTAUTENTISET_H_

#include "BaseSet.h"

class CVistaUtentiSet : public CBaseSet
{

	DECLARE_DYNAMIC(CVistaUtentiSet)

public:

	CVistaUtentiSet();

	long m_lId;
	CString m_sUsername;

private:

	virtual void DoFieldExchange(CFieldExchange* pFX);
	virtual CString GetDefaultSQL();
	virtual void SetEmpty();

	virtual CBaseSet* CreateNew() { return NULL; } // � una vista //
	virtual void CopyFields(CBaseSet* pOriginalSet, BOOL bCopyAll) { ; } // � una vista //
	virtual void SaveLog(CBaseSet* pOldValues, CBaseSet* pNewValues, const CString &sCommento) { ; } // � una vista //
	virtual BOOL DeleteRecordset(const CString &strCommento) { ASSERT(FALSE); return FALSE; } // � una vista //
};

#endif /* _VISTAUTENTILIBERISET_H_ */