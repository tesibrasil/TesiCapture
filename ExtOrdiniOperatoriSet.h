#ifndef _EXTORDINIOPERATORISET_H_
#define _EXTORDINIOPERATORISET_H_

class CExtOrdiniOperatoriSet : public CBaseSet
{
	DECLARE_DYNAMIC(CExtOrdiniOperatoriSet)

public:

	CExtOrdiniOperatoriSet();
	~CExtOrdiniOperatoriSet();

	long m_lID;
	long m_lIDRichiesta;
	CString m_sCodice;

protected:

	void DoFieldExchange(CFieldExchange* pFX);
	CString GetDefaultSQL();

private:

	CBaseSet* CreateNew() { return NULL; } // � una vista //
	void CopyFields(CBaseSet* pOriginalSet, BOOL bCopyAll) { ; } // � una vista //
	void SaveLog(CBaseSet* pOldValues, CBaseSet* pNewValues, const CString &sCommento) { ; } // � una vista //

	BOOL DeleteRecordset(const CString &strCommento) { ASSERT(FALSE); return FALSE; } // � una vista //
	CString SetBaseFilter(const CString &strFilter);
};

#endif /* _EXTORDINIOPERATORISET_H_ */