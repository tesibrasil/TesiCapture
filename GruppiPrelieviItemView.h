#ifndef _GRUPPIPRELIEVIITEMVIEW_H_
	#define _GRUPPIPRELIEVIITEMVIEW_H_

#include "BaseSet.h"

class CGruppiPrelieviItemView : public CBaseSet
{

	DECLARE_DYNAMIC(CGruppiPrelieviItemView)

public:

	CGruppiPrelieviItemView();

	long m_lID;
	long m_lIDGruppo;
	CString m_sTipo;
	CString m_sCodice;
	CString m_sIntervento;
	long m_lPrelievi;
	CString m_sDescrizione;

private:

	virtual void DoFieldExchange(CFieldExchange* pFX);
	virtual CString GetDefaultSQL();
	virtual void SetEmpty();

	virtual CBaseSet* CreateNew() { return NULL; } // � una vista //
	virtual void CopyFields(CBaseSet* pOriginalSet, BOOL bCopyAll) { ; } // � una vista //
	virtual void SaveLog(CBaseSet* pOldValues, CBaseSet* pNewValues, const CString &sCommento) { ; } // � una vista //
	virtual BOOL DeleteRecordset(const CString &strCommento) { ASSERT(FALSE); return FALSE; } // � una vista //

};

#endif /* _GRUPPIPRELIEVIITEMVIEW_H_ */