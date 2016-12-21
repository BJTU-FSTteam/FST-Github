
// FSTView.h : CFSTView 类的接口
//

#pragma once
#include "PR100Setting.h"
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <string>
#include <fstream>
using namespace std;
#include <afxtempl.h>

/*********************此处定义常量并添加注释********************/   //edit by zwbai 161221

typedef CList<CRect, CRect> CListRect;
const unsigned int nBUFSIZE = 128;     //64组数据，每组占两字节
#define tagQueryEspi 5       //查询ESPI场强值的定时器ID
#define tagATCtimeCntDn 6    //控发时间倒计时
#define tagAutoSave 7        //场强文件自动存盘定时器
#define MSG_COMDATARECV WM_USER+26
#define ShareBufferSize 10000000		//场强接收缓存区size


/*********************以上定义常变量并添加注释********************/



class CFSTView : public CView
{
protected: // 仅从序列化创建
	CFSTView();
	DECLARE_DYNCREATE(CFSTView)

// 特性
public:
	CFSTDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CFSTView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPr100Setting();
	BOOL InitPr100();

// 下面的变量均在线程函数或其它类中被访问，故为public类型,主要用于场强接收线程以及与主线程传递数据使用
/*****************************************/
//add by bzw 161220 start
	struct RECVPARAM //用于向线程传递参数
	{
		SOCKET sock;
		HWND hwnd;
		CFSTView* m_pView;
	};
	BOOL InitPR100flag; // FALSE:没有初始化场强仪  TRUE: 场强仪完成初始化
	bool stopPR100;   //true停止 false不停止 
	CString Pr100freq;
	unsigned char nLevel[10000];
	int nCountLevel;
	SOCKET  nSocketUdp;
	SOCKET  nSocketTcp;
	bool WorkAreaFlag;   //FALSE 可读,TRUE 可写
	bool SampleAreaFlag;  //FALSE 可读,TRUE 可写
	int pWorkArea;
	int pSampleArea;
	unsigned char WorkArea[ShareBufferSize];
	unsigned char SampleArea[ShareBufferSize];
	UINT Antenna;
	static DWORD WINAPI RecvProc(LPVOID lpParameter);
	void ReadData(unsigned char inData[], unsigned char sharebuffer[], int sizeofBuf);
	BOOL starRecordLevel;
	unsigned char TaxData[6];
	//add by bzw 161220 end
	/*****************************************/
public:
	BOOL EnumChildProc(HWND hwndChild, LPARAM lParam);

private:
	int firstTime;
	bool bSecondTime;
	double PI;
	enum
	{
		CtrlShow = 0,
		CtrlHide,
		CtrlMove
	};

public:
	afx_msg void OnSavedataButton();
};

#ifndef _DEBUG  // FSTView.cpp 中的调试版本
inline CFSTDoc* CFSTView::GetDocument() const
   { return reinterpret_cast<CFSTDoc*>(m_pDocument); }
#endif

