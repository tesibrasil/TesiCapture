#ifndef _JOINESAMISET_H_
	#define _JOINESAMISET_H_

#include "BaseSet.h"

class CJoinEsamiSet : public CBaseSet
{

	DECLARE_DYNAMIC(CJoinEsamiSet)

public:

	CJoinEsamiSet();

	//virtual void Open(int nOpenType, LPCTSTR lpszSQL, int nOptions);

	long m_lPaziente;
	long m_lEsame;
	long m_lData;

	void SetFromWhere(const CString &strFrom, const CString &strWhere);
	void SetSelect(const CString &strSelect);

	virtual CString GetDefaultSQL();

private:

	virtual void DoFieldExchange(CFieldExchange* pFX);
	virtual void SetEmpty();

	virtual void CopyFields(CBaseSet* pOriginalSet, BOOL bCopyAll) { ; } // � una vista //
	virtual void SaveLog(CBaseSet* pOldValues, CBaseSet* pNewValues, const CString &sCommento) { ; } // � una vista //
	virtual CBaseSet* CreateNew() { return NULL; } // � una vista //
	virtual BOOL DeleteRecordset(const CString &strCommento) { ASSERT(FALSE); return FALSE; }

	// le metto private perch� devo chiamare SetFromWhere e non queste //
	CString SetBaseFilter(const CString &strFilter) { ASSERT(FALSE); return ""; }
	CString SetOpenFilter(const CString &strFilter) { ASSERT(FALSE); return ""; }

	CString m_sFrom;
	CString m_sSelect;

};

#endif /* _JOINESAMISET_H_ */