
// FSTView.h : CFSTView ��Ľӿ�
//

#pragma once


class CFSTView : public CView
{
protected: // �������л�����
	CFSTView();
	DECLARE_DYNCREATE(CFSTView)

// ����
public:
	CFSTDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CFSTView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPr100Setting();
	BOOL InitPr100();

// ����ı��������̺߳������������б����ʣ���Ϊpublic����,��Ҫ���ڳ�ǿ�����߳��Լ������̴߳�������ʹ��
/*****************************************/
//add by bzw 161220 start
	struct RECVPARAM //�������̴߳��ݲ���
	{
		SOCKET sock;
		HWND hwnd;
		CFSTView* m_pView;
	};
	BOOL InitPR100flag; // FALSE:û�г�ʼ����ǿ��  TRUE: ��ǿ����ɳ�ʼ��
	bool stopPR100;   //trueֹͣ false��ֹͣ 
	CString Pr100freq;
	BOOL InitPr100();
	unsigned char nLevel[10000];
	int nCountLevel;
	SOCKET  nSocketUdp;
	SOCKET  nSocketTcp;
	bool WorkAreaFlag;   //FALSE �ɶ�,TRUE ��д
	bool SampleAreaFlag;  //FALSE �ɶ�,TRUE ��д
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

#ifndef _DEBUG  // FSTView.cpp �еĵ��԰汾
inline CFSTDoc* CFSTView::GetDocument() const
   { return reinterpret_cast<CFSTDoc*>(m_pDocument); }
#endif

