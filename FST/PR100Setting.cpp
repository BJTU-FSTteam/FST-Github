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
{

	curAT = 0;
}

PR100Setting::~PR100Setting()
{
}

void PR100Setting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PR100Setting, CDialogEx)
	ON_BN_CLICKED(IDOK, &PR100Setting::OnBnClickedOk)
END_MESSAGE_MAP()


// PR100Setting ��Ϣ�������


void PR100Setting::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	CDialogEx::OnOK();
}
