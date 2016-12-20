// PR100Setting.cpp : 实现文件
//

#include "stdafx.h"
#include "FST.h"
#include "PR100Setting.h"
#include "afxdialogex.h"
#include <windows.h>
using namespace std;
// PR100Setting 对话框

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


// PR100Setting 消息处理程序


void PR100Setting::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CDialogEx::OnOK();
}
