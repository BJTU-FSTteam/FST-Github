//����PR100��������


// PR100Setting �Ի���

class PR100Setting : public CDialogEx
{
	DECLARE_DYNAMIC(PR100Setting)

public:
	PR100Setting(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~PR100Setting();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PR100_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	int curAT;
};
