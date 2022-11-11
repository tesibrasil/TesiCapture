#ifndef _TBLOGALBUMIMMAGINISET_H_
	#define _TBLOGALBUMIMMAGINISET_H_

#include "BaseSet.h"

class CTbLogAlbumImmaginiSet : public CBaseSet
{
	DECLARE_DYNAMIC(CTbLogAlbumImmaginiSet)

public:

	CTbLogAlbumImmaginiSet();

	long m_lID;
	long m_lIDPaziente;
	long m_lIDEsame;
	long m_lIDImmagine;
	CString m_sDataOra;
	CString m_sUtente;
	CString m_sStazione;
	long m_lOperazione;

	//

	BOOL HasRigheDopoDataOra(long lIDEsame, CString sDataOra);

private:

	virtual void DoFieldExchange(CFieldExchange* pFX);
	virtual CString GetDefaultSQL();
	virtual void SetEmpty();

	virtual CBaseSet* CreateNew() { return NULL; } // � una vista //
	virtual void CopyFields(CBaseSet* pOriginalSet, BOOL bCopyAll) { ; } // � una vista //
	virtual void SaveLog(CBaseSet* pOldValues, CBaseSet* pNewValues, const CString &sCommento) { ; } // � una vista //

	virtual BOOL DeleteRecordset(const CString &strCommento) { ASSERT(FALSE); return FALSE; } // � una vista //

};

#endif /* _TBLOGALBUMIMMAGINISET_H_ */