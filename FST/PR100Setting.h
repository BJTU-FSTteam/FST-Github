//进行PR100参数设置


// PR100Setting 对话框
 #pragma once
 #include "afxwin.h"
class PR100Setting : public CDialogEx
{
	DECLARE_DYNAMIC(PR100Setting)

public:
	PR100Setting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PR100Setting();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PR100_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	int curAT;
	enum { IDD = IDD_PR100_DIALOG };
	CComboBox m_ctrlAT;
	CString m_AT;
	CComboBox m_cbFrequency;
	CString m_strFrequency;
	CString m_FR;
	CString m_LD;
	CString m_LU;
	CString m_AD;

	afx_msg void OnEnChangeLuEdit();
	afx_msg void OnEnChangeLdEdit();
	afx_msg void OnEnChangeAdEdit();
	afx_msg void OnCbnSelchangeAtCombo();
	afx_msg void OnCbnSelchangeFrCombo();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboFrequency();
};
