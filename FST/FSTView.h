
// FSTView.h : CFSTView 类的接口
//

#pragma once


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
	BOOL InitPr100();
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

};

#ifndef _DEBUG  // FSTView.cpp 中的调试版本
inline CFSTDoc* CFSTView::GetDocument() const
   { return reinterpret_cast<CFSTDoc*>(m_pDocument); }
#endif

