#ifndef _EXTORDINICAMPIAGGIUNTIVI_H_
	#define _EXTORDINICAMPIAGGIUNTIVI_H_

class CExtOrdiniCampiAggiuntiviSet : public CBaseSet
{
	DECLARE_DYNAMIC(CExtOrdiniCampiAggiuntiviSet)

public:

	CExtOrdiniCampiAggiuntiviSet();
	~CExtOrdiniCampiAggiuntiviSet();

	long m_lID;
	long m_lIDRichiesta;
	CString m_sTabella;
	CString m_sCampo;
	CString m_sValore;
		
protected:

	void DoFieldExchange(CFieldExchange* pFX);
	CString GetDefaultSQL();

private:

	CBaseSet* CreateNew() { return NULL; } // � una vista //
	void CopyFields(CBaseSet* pOriginalSet, BOOL bCopyAll) { ; } // � una vista //
	void SaveLog(CBaseSet* pOldValues, CBaseSet* pNewValues, const CString &sCommento) { ; } // � una vista //

	BOOL DeleteRecordset(const CString &strCommento) { ASSERT(FALSE); return FALSE; } // � una vista //

};

#endif /* _EXTORDINICAMPIAGGIUNTIVI_H_ */