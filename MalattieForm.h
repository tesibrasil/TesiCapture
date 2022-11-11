#ifndef _MALATTIEFORM_H_
	#define _MALATTIEFORM_H_

#include "ExtEdit.h"
#include "MaskEdit.h"
#include "SubForm.h"

class CMalattieForm : public CSubForm
{

public:

	CMalattieForm(CWnd* pParent, CEsamiView* pEsamiView);
	~CMalattieForm();

	enum { IDD = IDD_SUBFORM_MALATTIE };

	static void CreateFormEx(CWnd* pParent, CWnd* pParentFrame, CEsamiView* pEsamiView, long lColore, long lIDFaseBlocco);

protected:

	void DoDataExchange(CDataExchange* pDX);
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	BOOL OnInitDialog();

	LRESULT OnFormAnnullaEsame(WPARAM wParam, LPARAM lParam);
	LRESULT OnFormClone(WPARAM wParam, LPARAM lParam);
	LRESULT OnFormLoad(WPARAM wParam, LPARAM lParam);
	LRESULT OnFormReadOnly(WPARAM wParam, LPARAM lParam);
	LRESULT OnFormReset(WPARAM wParam, LPARAM lParam);
	LRESULT OnFormSave(WPARAM wParam, LPARAM lParam);
	LRESULT OnFormSearchNew(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:

	CExtEdit m_ctrlMalattie[3];
	CMaskEdit m_ctrlData[3];
	CStaticLocalize m_ctrlStatic[3];

	STRUCT_MALATTIE	m_ctrLine[MAX_OBJ_X_FORM];

	long GetContenuto();
	BOOL CampoObbligatorioValido();
	virtual void ModificaForm();

	virtual long GetCounterSetOfLine(int nCounter);
	virtual BOOL IsLineDirty(int nCounter);
	virtual BOOL IsValidSet (int nCounter);
	virtual void SetLineDirty(int nCounter, BOOL bSetDirty);
	virtual void UpdateDataLine(CBaseSet* pBaseSet, int nCounter, BOOL bToSet);

	afx_msg void OnChangeMalattie1();
	afx_msg void OnChangeMalattie2();
	afx_msg void OnChangeMalattie3();
	afx_msg void OnChangeClasse1();
	afx_msg void OnChangeClasse2();
	afx_msg void OnChangeClasse3();
	afx_msg void OnChangeEpisodi1();
	afx_msg void OnChangeEpisodi2();
	afx_msg void OnChangeEpisodi3();

	CString GetStringEpisodi(long lEpisodi);

};

#endif /* _MALATTIEFORM_H_ */