#ifndef _EXTORDINIALLEGATI_H_
#define _EXTORDINIALLEGATI_H_

class CExtOrdiniAllegatiSet : public CBaseSet
{
	DECLARE_DYNAMIC(CExtOrdiniAllegatiSet)

public:

	CExtOrdiniAllegatiSet();
	~CExtOrdiniAllegatiSet();

	long m_lID;
	long m_lIDRichiesta;
	CString m_sPath;
	CString m_sDescrizione;

protected:

	void DoFieldExchange(CFieldExchange* pFX);
	CString GetDefaultSQL();

private:

	CBaseSet* CreateNew() { return NULL; } // � una vista //
	void CopyFields(CBaseSet* pOriginalSet, BOOL bCopyAll) { ; } // � una vista //
	void SaveLog(CBaseSet* pOldValues, CBaseSet* pNewValues, const CString &sCommento) { ; } // � una vista //

	BOOL DeleteRecordset(const CString &strCommento) { ASSERT(FALSE); return FALSE; } // � una vista //

};

#endif /* _EXTORDINIALLEGATI_H_ */