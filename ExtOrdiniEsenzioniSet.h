#ifndef _EXTORDINIESENZIONI_H_
	#define _EXTORDINIESENZIONI_H_

class CExtOrdiniEsenzioniSet : public CBaseSet
{
	DECLARE_DYNAMIC(CExtOrdiniEsenzioniSet)

public:

	CExtOrdiniEsenzioniSet();
	~CExtOrdiniEsenzioniSet();

	long m_lID;
	long m_lIDRichiesta;
	CString m_sCodice;
	CString m_sDescrizione;
	CString m_sDataScadenza;
		
protected:

	void DoFieldExchange(CFieldExchange* pFX);
	CString GetDefaultSQL();

private:

	CBaseSet* CreateNew() { return NULL; } // � una vista //
	void CopyFields(CBaseSet* pOriginalSet, BOOL bCopyAll) { ; } // � una vista //
	void SaveLog(CBaseSet* pOldValues, CBaseSet* pNewValues, const CString &sCommento) { ; } // � una vista //

	BOOL DeleteRecordset(const CString &strCommento) { ASSERT(FALSE); return FALSE; } // � una vista //

};

#endif /* _EXTORDINIESENZIONI_H_ */