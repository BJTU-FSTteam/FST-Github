// PR100Setting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FST.h"
#include "PR100Setting.h"
#include "afxdialogex.h"
#include <windows.h>
using namespace std;
// PR100Setting �Ի���

IMPLEMENT_DYNAMIC(PR100Setting, CDialogEx)

PR100Setting::PR100Setting(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PR100_DIALOG, pParent)
	, m_AT(_T(" 0 dB"))
	, m_strFrequency(_T(" 15 kHz"))
	, m_FR(_T(" 467.925"))
	, m_LD(_T(" 1 ms"))
	, m_LU(_T(" dBuV"))
	, m_AD(_T(" FM"))
{

	curAT = 0;
}

PR100Setting::~PR100Setting()
{
}

void PR100Setting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AT_COMBO, m_ctrlAT);
	DDX_CBString(pDX, IDC_AT_COMBO, m_AT);
// 	DDX_Control(pDX, IDC_COMBO_Frequency, m_ctrlAT);
	DDX_Control(pDX, IDC_COMBO_Frequency, m_cbFrequency);
	//  DDX_CBString(pDX, IDC_COMBO_Frequency, m_strFrequency);
	DDX_CBString(pDX, IDC_FR_COMBO, m_FR);
	DDX_Text(pDX, IDC_LD_EDIT, m_LD);
	DDX_Text(pDX, IDC_LU_EDIT, m_LU);
	DDX_Text(pDX, IDC_AD_EDIT, m_AD);
	DDX_CBString(pDX, IDC_COMBO_Frequency, m_strFrequency);
}


BEGIN_MESSAGE_MAP(PR100Setting, CDialogEx)
	ON_BN_CLICKED(IDOK, &PR100Setting::OnBnClickedOk)
//	ON_CBN_SELCHANGE(IDC_COMBO_Frequency, &PR100Setting::OnCbnSelchangeComboFrequency)
	ON_EN_CHANGE(IDC_LU_EDIT, &PR100Setting::OnEnChangeLuEdit)
	ON_EN_CHANGE(IDC_LD_EDIT, &PR100Setting::OnEnChangeLdEdit)
	ON_EN_CHANGE(IDC_AD_EDIT, &PR100Setting::OnEnChangeAdEdit)
	ON_CBN_SELCHANGE(IDC_AT_COMBO, &PR100Setting::OnCbnSelchangeAtCombo)
	ON_CBN_SELCHANGE(IDC_FR_COMBO, &PR100Setting::OnCbnSelchangeFrCombo)
	ON_BN_CLICKED(IDCANCEL, &PR100Setting::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_Frequency, &PR100Setting::OnCbnSelchangeAtCombo)
END_MESSAGE_MAP()


// PR100Setting ��Ϣ�������


void PR100Setting::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CDialogEx::OnOK();
}


//void PR100Setting::OnCbnSelchangeCombo1()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//}


//void PR100Setting::OnCbnSelchangeComboFrequency()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	curAT = m_ctrlAT.GetCurSel() + 1;
//
//}


void PR100Setting::OnEnChangeLuEdit()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void PR100Setting::OnEnChangeLdEdit()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void PR100Setting::OnEnChangeAdEdit()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void PR100Setting::OnCbnSelchangeAtCombo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	curAT = m_ctrlAT.GetCurSel() + 1;
}


void PR100Setting::OnCbnSelchangeFrCombo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void PR100Setting::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


BOOL PR100Setting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CString dirName;

	dirName = _T("C:\\��ǿ����\\");

	CString strTemp, strTempA, strDisp;
	//	TCHAR szPath[MAX_PATH] = {"c:\\��ǿ����\\*.TXT"} ; 

	::GetPrivateProfileString(_T("Receiver_PARAMETERS"), _T("Frequency"), m_FR,
		strTemp.GetBuffer(20), 20, dirName + "fsmconfig.ini");
	m_FR = strTemp;
	strDisp.Format(_T("%s"), strTemp);
	//	MessageBox(strDisp);

	m_cbFrequency.InsertString(-1, _T("  9 kHz"));
	m_cbFrequency.InsertString(-1, _T(" 15 kHz"));
	m_cbFrequency.InsertString(-1, _T(" 30 kHz"));
	m_cbFrequency.InsertString(-1, _T(" 150 kHz"));
	m_cbFrequency.SetCurSel(3);

	UpdateData(FALSE);

	curAT = m_ctrlAT.GetCurSel() + 1;

	m_ctrlAT.SetCurSel(0);

	return TRUE;
}


void PR100Setting::OnSelchangeComboFrequency()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
