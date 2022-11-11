#ifndef _FARMACIUNIONSET_H_
	#define _FARMACIUNIONSET_H_

#include "BaseSet.h"

class CFarmaciUnionSet : public CBaseSet
{
	DECLARE_DYNAMIC(CFarmaciUnionSet)

public:

	CFarmaciUnionSet();

	long m_lID;
	CString m_sDescrizione; 
	CString m_sDescrizioneMedia; 
	CString m_sDescrizioneMassima; 

	BOOL OpenRecordset(const CString &strCommento);

private:

	virtual void DoFieldExchange(CFieldExchange* pFX);
	virtual CString GetDefaultSQL();
	virtual void SetEmpty();

	virtual CBaseSet* CreateNew() { return NULL; } // � una vista //
	virtual void CopyFields(CBaseSet* pOriginalSet, BOOL bCopyAll) { ; } // � una vista //
	virtual void SaveLog(CBaseSet* pOldValues, CBaseSet* pNewValues, const CString &sCommento) { ; } // � una vista //
	virtual BOOL DeleteRecordset(const CString &strCommento) { ASSERT(FALSE); return FALSE; } // � una vista //
};

#endif /* _FARMACIUNIONSET_H_ */