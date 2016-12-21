
// FSTView.cpp : CFSTView 类的实现
//
#pragma once
#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "FST.h"
#endif

#include "FSTDoc.h"
#include "FSTView.h"
#include "PR100Setting.h"
#include "MainFrm.h"
#include "conio.h"

#include "math.h"
#include <iostream>
using namespace std;
//extern CStatusBar  m_wndStatusBar;
//extern HICON  train;
extern CFSTApp theApp;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CFSTView.cpp
/*********************此处定义全局常量并添加注释********************/   //edit by zwbai 161221
#define nDrawRangeXMin 65
#define nDrawRangeXMax 965
#define nPix500M 30				//本版程序中每30个像素点表示500米
#define nKMDisplayNum 29		//每次画14km:((840Pix/60)*28)
const int nPix100m = 6;

/*********************以上定义全局常量并添加注释********************/

/*********************此处定义全局变量并添加注释********************/   //edit by zwbai 161221
int Pr100ProcFlag = 0;			//Pr100接收线程工作控制变量  
float db_value = 0.0;			//场强补偿值
double	unit;					//里程计算相关变量
int		pulse = 45;			//200/4=50	//360/4=90;	//80/2=40;          //pulses each circle
int     pulseDivision = 2;	//2;        //80 /2 =40

int		pulse100M = 100;
int		pulse95 = 5;
int		pulse90 = 10;
int		pulse50 = 50;

/*********************以上定义全局变量并添加注释********************/

IMPLEMENT_DYNCREATE(CFSTView, CView)

BEGIN_MESSAGE_MAP(CFSTView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_PR100_SETTING, &CFSTView::OnPr100Setting)
END_MESSAGE_MAP()

// CFSTView 构造/析构

CFSTView::CFSTView()//添加未完成add by zwbai 161221
{
	// TODO: 在此处添加构造代码

	//  Pr100ProcFlag = 0;
	firstTime = 1;

	/********************此处为里程计算相关变量初始化*********************///add by bzw  161110 start
	PI = atan(1.0)*4.0;
	//unit = atof(m_diameter)*PI*0.001 / pulse;		//meter//目前先注释掉，之后要恢复
	pulse100M = (int)(100.0 / unit + 0.5);
	pulse95 = (int)(5.0 / unit + 0.5);
	pulse90 = (int)(10.0 / unit + 0.5);
	pulse50 = (int)(50.0 / unit + 0.5);

	/********************此处为Pr100接收线程相关变量初始化*********************///add by bzw  161110 start
	InitPR100flag = FALSE;
	stopPR100 = false;
	starRecordLevel = FALSE;
	WorkAreaFlag = false;   //FALSE可读,TRUE 可写
	SampleAreaFlag = true;  //FALSE表示可读,TRUE 可写
	pWorkArea = 0;
	pSampleArea = 0;
	/*****************************************///add by bzw  161110 end

}

CFSTView::~CFSTView()
{
}

BOOL CFSTView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CFSTView 绘制

void CFSTView::OnDraw(CDC* pDC)
{
	CFSTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	CRect rcClip;
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor(RGB(255, 255, 255));

	if (firstTime == 1)
	{
		// add by shao 2011.03.03
		//EnumChildWindows(this->m_hWnd, EnumChildProc, CtrlHide);
		//m_picture.ShowWindow(SW_SHOW);
		GetClientRect(&rcClip);
		pDC->FillSolidRect(rcClip.left, rcClip.top, rcClip.right, rcClip.bottom/*-140*/, 0xFFFFFF);
		bSecondTime = true;
		// add end by shao

		CPen myPen3(PS_DOT, 1, RGB(0xE0, 0xE0, 0xFF));
		pDC->SelectObject(&myPen3);

		for (int i = 0; i < 128; i++)
		{
			pDC->MoveTo(0, i * 4);
			pDC->LineTo(/*1023*/rcClip.Width(), i * 4);
		}
		for (int i = 1; i < 256; i++)
		{
			pDC->MoveTo(i * 4, 0);
			pDC->LineTo(i * 4, /*508*/rcClip.Height());
		}


		CFont myFont1, myFont2, *pOldFont;
		myFont1.CreateFont(90, 30, 0, 0, FW_HEAVY, 0, 0, 0, DEFAULT_CHARSET,
			OUT_DEVICE_PRECIS, VARIABLE_PITCH | FF_ROMAN, PROOF_QUALITY, 0, _T("隶书"));
		pOldFont = pDC->SelectObject(&myFont1);

		pDC->SetTextColor(RGB(0x00, 0x60, 0x40));
		for (int i = 1; i < 6; i++)
			pDC->TextOut(150 - i, 160 - i, _T("无线列调场强自动测试系统"));
		pDC->SetTextColor(RGB(0x00, 0xC0, 0xA0));
		pDC->TextOut(150, 160, _T("无线列调场强自动测试系统"));
		pDC->SelectObject(pOldFont);

		GetParent()->SetWindowText(_T("              无  线  列  调  场  强  自  动  测  试  系  统"));

		myFont2.CreateFont(50, 16, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET,
			OUT_RASTER_PRECIS, (VARIABLE_PITCH | FF_ROMAN) & 0xFFFD, PROOF_QUALITY, 0, _T("隶书"));

		pOldFont = pDC->SelectObject(&myFont2);


		pDC->SetTextColor(RGB(0x00, 0x80, 0xFF));
		pDC->TextOut(200, 350, _T("研制单位：北京交通大学电子信息工程学院"));
		pDC->TextOut(410, 435, _T("2O16年11月"));
		pDC->SelectObject(pOldFont);

		//ReadLineName("综合库.txt");
	}
	//此处先注释第二次进入主界面的相关代码add by zwbai 161221
// 	else /*if((offset>0) && (readStatus==1))*/
// 	{
// 		// add by zgliu 2011.03.03
// 		if (bSecondTime)
// 		{
// 			CRect rcDrawRange;
// 			GetDlgItem(IDC_STATIC_RANGE)->GetWindowRect(&rcDrawRange);
// 			ScreenToClient(&rcDrawRange);
// 			EnumChildWindows(this->m_hWnd, EnumChildProc, CtrlShow);
// 			//			m_picture.ShowWindow(FALSE);
// 			GetDlgItem(IDC_INQUIRY_LIST)->ShowWindow(FALSE);
// 			GetClientRect(&rcClip);
// 			pDC->FillSolidRect(rcClip.left, rcClip.top, rcClip.right, rcClip.bottom - rcDrawRange.Height()/*140*/, 0xFFFFFF);
// 			bSecondTime = false;
// 			Invalidate(TRUE);
// 		}
// 		// add end by zgliu 
// 
// 		InitScreen();
// 
// 		CPen	myPen11(PS_SOLID, 2, RGB(0x00, 0x00, 0xC0));
// 		CPen	myPen0(PS_SOLID, 1, RGB(0xFF, 0x00, 0xFF));
// 
// 		int	jj = AD_num100 - sectionNum;
// 
// 		pDC->SelectObject(&myPen11);
// 		pDC->MoveTo(nDrawRangeXMin, 410 - (int)AD_value1[jj] * 4);
// 		for (int ii = 1; ii <= sectionNum; ii++)
// 		{
// 			pDC->LineTo(nDrawRangeXMin + (ii*(nPix100m)), 410 - (int)AD_value1[jj + ii - 1] * 4);
// 		}
// 
// 		// add by zgliu 2011.04.15
// 		if (1 == readStatus)
// 		{
// 			// 			CPen  myPenTrainPos(PS_SOLID, 1, RGB(0xFF, 0x00, 0x00));   //draw line for current position of the train
// 			// 			CPen* pOldPen;
// 			// 			int   nOldDrawMode;
// 			// 			nOldDrawMode = pDC->SetROP2(R2_XORPEN);
// 			// 			pDC->SetBkMode(TRANSPARENT);
// 			// 			pOldPen = pDC->SelectObject(&myPenTrainPos);
// 			// 			pDC->MoveTo(nDrawRangeXMin + sectionNum*nPix100m, 50);
// 			// 			pDC->LineTo(nDrawRangeXMin + sectionNum*nPix100m, 458);
// 			// 			pDC->SelectObject(pOldPen);
// 			// 			pDC->SetROP2(nOldDrawMode);
// 			// 		    pDC->SetBkMode(OPAQUE);
// 		}
// 		// add end by zgliu 2011.04.15
// 
// 		//////////////////////////////////////////		  
// 		CFont	myFont, *pOldFont;
// 		myFont.CreateFont(18, 6, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
// 			OUT_DEVICE_PRECIS, VARIABLE_PITCH | FF_ROMAN, PROOF_QUALITY, 0, "ROMAN");
// 		pOldFont = pDC->SelectObject(&myFont);
// 
// 
// 		pDC->SetTextColor(RGB(0xFF, 0x00, 0xFF));
// 		pDC->TextOut(nDrawRangeXMin - 5 - 15, 460 + 10, startKM);
// 		pDC->TextOut(nDrawRangeXMin - 5 + offset, 460 + 10, nextKM);
// 		if (_T("") != currentName)
// 		{
// 			pDC->TextOut(nDrawRangeXMin - 5 - 15, 480 + 10, currentName + _T(" (") + currentCode + _T(")"));
// 		}
// 		else
// 		{
// 			pDC->TextOut(nDrawRangeXMin - 5 - 15, 480 + 10, _T(""));
// 		}
// 		pDC->TextOut(nDrawRangeXMin - 5 + offset, 480 + 10, nextName + _T(" (") + nextCode + _T(")"));
// 
// 		DisplayOthers(pDC, startKM, nextKM);
// 
// 		// add by zgliu 2011.04.13 
// 		// 每1KM显示一个刻度值
// 		CFont myKMFont;
// 		myKMFont.CreateFont(15, 5, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
// 			OUT_DEVICE_PRECIS, VARIABLE_PITCH | FF_ROMAN, PROOF_QUALITY, 0, "ROMAN");
// 		pOldFont = pDC->SelectObject(&myKMFont);
// 		const int nPix1KM = nPix500M * 2;
// 		const int nDeltaKM = offset / nPix1KM;
// 		CString strTempKM;
// 		for (int i = 1; i <= nDeltaKM; ++i)
// 		{
// 			if (i <= (nKMDisplayNum + 1) / 2)
// 			{
// 				if (-1 != m_updown.Find(_T("增加")))
// 				{
// 					strTempKM.Format(_T("%0.2f"), atof(startKM) + i);
// 				}
// 				else if (-1 != m_updown.Find(_T("减少")))
// 				{
// 					strTempKM.Format(_T("%0.2f"), atof(startKM) - i);
// 				}
// 				pDC->SetTextColor(RGB(0x00, 0x00, 0x00));
// 				pDC->TextOut(nDrawRangeXMin - 8 + nPix1KM*i, 455, strTempKM);
// 			}
// 		}
// 		pDC->SelectObject(pOldFont);
// 		// add end by zgliu 
// 
// 
// 		pDC->SelectObject(&myPen0);
// 		pDC->MoveTo(nDrawRangeXMin + offset / 2, 445);
// 		pDC->LineTo(nDrawRangeXMin + offset / 2, 450);
// 		pDC->MoveTo(nDrawRangeXMin + offset, 50);
// 		pDC->LineTo(nDrawRangeXMin + offset, 450);
// 
// 		pDC->SelectObject(pOldFont);
// 	}
}


// CFSTView 打印

BOOL CFSTView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CFSTView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CFSTView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CFSTView 诊断

#ifdef _DEBUG
void CFSTView::AssertValid() const
{
	CView::AssertValid();
}

void CFSTView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFSTDoc* CFSTView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFSTDoc)));
	return (CFSTDoc*)m_pDocument;
}
#endif //_DEBUG


// CFSTView 消息处理程序


void CFSTView::OnPr100Setting()
{
	// TODO: 在此添加命令处理程序代码
	PR100Setting m_PR100SetDlg;
	if (IDOK == m_PR100SetDlg.DoModal())
	{

		switch (m_PR100SetDlg.curAT)
		{
		case 0: Antenna = 0; break;
		case 1: Antenna = 3; break;
		case 2: Antenna = 10; break;
		case 3: Antenna = 20; break;
		case 4: Antenna = 30; break;
		case 5: Antenna = 40; break;
		case 6: Antenna = 50; break;
		default: break;

		}
		BeginWaitCursor();
// 		m_wndStatusBar.SetPaneText(1, _T("正在设置设备参数，请稍等...   "), TRUE);
		Pr100freq = m_PR100SetDlg.m_FR;
		if (TRUE == InitPr100())//PR100初始化
		{
			InitPR100flag = TRUE;
			MessageBox(_T("     设置场强仪成功！"));
			wchar_t buffer[150];
			wprintf(buffer, "场强仪参数 Band:15kHz LU:dBuV LD:5ms AD:FM AT:%ddB Frq:%sMHz", Antenna, Pr100freq);
			//以下设置状态栏宽度
// 			CClientDC dc(this);
// 			CSize sz = dc.GetTextExtent(buffer);
// 			m_wndStatusBar.SetPaneInfo(1, 0, 0, sz.cx);
// 			m_wndStatusBar.SetPaneText(1, buffer, TRUE);
		}
		EndWaitCursor();
	}
}


BOOL CFSTView::InitPr100()
{
	if (TRUE == InitPR100flag)
	{
		closesocket(nSocketUdp);
	}
	struct sockaddr_in addr;
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	int maxV = 2;
	int minV = 0;

	wVersionRequested = MAKEWORD(maxV, minV);
	//WSASYSNOTREADY 指出网络通信依赖的网络子系统还没有准备好.
	//WSAVERNOTSUPPORTED 所需的Windows Sockets API的版本未由特定的Windows Sockets实现提供.
	//WSAEINVAL 应用程序指出的Windows Sockets版本不被该DLL支持
	err = WSAStartup(wVersionRequested, &wsaData);//Windows套接字异步启动命令
	if (err != 0)
	{
		return FALSE;
	}
	//16进制数最低（最右边）字节的内容 || 16进制数最高（最左边）字节的内容
	if (LOBYTE(wsaData.wVersion) != maxV || HIBYTE(wsaData.wVersion) != minV)
	{
		WSACleanup();
		return FALSE;
	}

	//为了检查PC端网络设置是否正确，获取本地机器IP地址
	CString strClient;
	char *ip;
	char name[155];
	PHOSTENT hostinfo;
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) == 0)
	{
		if (gethostname(name, sizeof(name)) == 0)
		{
			if ((hostinfo = gethostbyname(name)) != NULL)
			{
				ip = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
				strClient.Format(_T("%s"), ip);
			}
		}
		WSACleanup();
	}
	//得到本地机器IP地址 结束
	if (strClient.Mid(0, 11) != _T("172.17.75.2")&&strClient.Mid(0, 9) != _T("127.0.0.1"))//判断PC本地IP是否为172.17.75.2
	{
		MessageBox(_T("请将本地IP设置为172.17.75.2,子网掩码设为255.255.255.0,默认网关设置为172.17.75.1"));
		return FALSE;
	}
	if (strClient.Mid(0, 9) == _T("127.0.0.1"))
	{
		MessageBox(_T("请打开场强仪！"));
		return FALSE;
	}
	else
	{
		nSocketTcp = socket(AF_INET, SOCK_STREAM, 0);
		if (nSocketTcp != -1)
		{
			memset(&addr, 0, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_addr.s_addr = inet_addr("172.17.75.1");
			addr.sin_port = htons(5555);

			/*			err=connect(nSocketTcp,(struct sockaddr *)&addr,sizeof(addr));*/
			if (SOCKET_ERROR == connect(nSocketTcp, (struct sockaddr *)&addr, sizeof(addr)))
			{
				MessageBox(_T("连接场强仪失败"));
				return FALSE;
			}
			else
			{
				int i = 1;
				setsockopt(nSocketTcp, IPPROTO_TCP, TCP_NODELAY, (char*)&i, sizeof(i));
				//复位PR100
				send(nSocketTcp, "*RST\n", strlen("*RST\n"), 0);
				Sleep(100);
				//按键锁
				send(nSocketTcp, "SYST:KLOCK ON\n", strlen("SYST:KLOCK ON\n"), 0);
				Sleep(50);
				//设置系统音量为0
				send(nSocketTcp, "SYST:AUD:VOL 0\n", strlen("SYST:AUD:VOL 0\n"), 0);
				Sleep(50);
				//设置频率模式MSC
				send(nSocketTcp, "FREQ:MODE MSC\n", strlen("FREQ:MODE MSC\n"), 0);
				Sleep(50);
				//设置MSC 数运行
				send(nSocketTcp, "MSC:COUN INF\n", strlen("MSC:COUN INF\n"), 0);
				Sleep(50);
				//同步时间
				send(nSocketTcp, "MSC:DWEL 0\n", strlen("MSC:DWEL 0\n"), 0);
				Sleep(50);
				//收听时间
				send(nSocketTcp, "MSC:HOLD:TIME 0\n", strlen("MSC:HOLD:TIME 0\n"), 0);
				Sleep(50);
				//扫描模式
				send(nSocketTcp, "MSC:LIST STAR 0\n", strlen("MSC:LIST STAR 0\n"), 0);
				Sleep(50);
				send(nSocketTcp, "MSC:LIST STOP 0\n", strlen("MSC:LIST STOP 0\n"), 0);
				Sleep(50);
				//Clear memory lists
				send(nSocketTcp, "MEM:CLE 0\n", strlen("MEM:CLE 0\n"), 0);
				Sleep(50);
				char sendbuff[40];
				sprintf_s(sendbuff, "MEM:CONT 0,%sE6,0,FM,15kHz,6,0,0,0,0,1\n", Pr100freq);
				Sleep(50);
				send(nSocketTcp, sendbuff, strlen(sendbuff), 0);
				Sleep(50);
				sprintf_s(sendbuff, "MEM:LABEL 0,\"%s MHz, BW 15kHz\"\n", Pr100freq);
				send(nSocketTcp, sendbuff, strlen(sendbuff), 0);
				sprintf_s(sendbuff, "FREQ %sE6\n", Pr100freq);
				//设置频率
				send(nSocketTcp, sendbuff, strlen(sendbuff), 0);
				Sleep(50);
				//设置带宽15KHz
				send(nSocketTcp, "BAND 15kHz\n", strlen("BAND 15kHz\n"), 0);
				Sleep(50);
				//设置检查方式为Max Peak
				send(nSocketTcp, "SENS:DET PEAK\n", strlen("SENS:DET PEAK\n"), 0);


				Sleep(50);
				//设置中频显示范围100dbuv
				send(nSocketTcp, "DISP:IFP:LEV:RANG 100\n", strlen("DISP:IFP:LEV:RANG 100\n"), 0);
				Sleep(50);
				//设置中频显示最大值为90dbuv
				send(nSocketTcp, "DISP:IFP:LEV:REF 90\n", strlen("DISP:IFP:LEV:REF 90\n"), 0);
				Sleep(50);
				send(nSocketTcp, "DISP:WIND \"Spectrum\"\n", strlen("DISP:WIND \"Spectrum\"\n"), 0);

				Sleep(50);
				//设置测量模式为周期测量
				send(nSocketTcp, "MEAS:MODE PER\n", strlen("MEAS:MODE PER\n"), 0);
				Sleep(50);
				//设置测量周期为0.5ms
				send(nSocketTcp, "MEAS:TIME 0.5 ms\n", strlen("MEAS:TIME 0.5 ms\n"), 0);
				Sleep(50);
				//删除所有的TRAC UdpPath
				send(nSocketTcp, "TRAC:UDP:DEL ALL\n", strlen("TRAC:UDP:DEL ALL\n"), 0);
				Sleep(50);
				//设置UdpPath,将电平测量值发送到IP地址为172.17.75.2的PC机的19000端口
				send(nSocketTcp, "TRAC:UDP:DEF:FLAG:ON \"172.17.75.2\",19000,\"VOLT:AC\"\n", \
					strlen("TRAC:UDP:DEF:FLAG:ON \"172.17.75.2\",19000,\"VOLT:AC\"\n"), 0);
				Sleep(50);
				//
				send(nSocketTcp, "TRAC:UDP:DEF:TAG:ON \"172.17.75.2\",19000,MSC\n", \
					strlen("TRAC:UDP:DEF:TAG:ON \"172.17.75.2\",19000,MSC\n"), 0);
				Sleep(50);
				send(nSocketTcp, "INIT:CONM\n", strlen("INIT:CONM\n"), 0);
				closesocket(nSocketTcp);


				//UDP初始化
				nSocketUdp = socket(AF_INET, SOCK_DGRAM, 0);
				if (INVALID_SOCKET == nSocketUdp)
				{
					MessageBox(_T("套接字创建失败！"));
					return FALSE;
				}
				struct sockaddr_in addrSock;
				addrSock.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
				addrSock.sin_family = AF_INET;
				addrSock.sin_port = htons(19000); //此端口与命令15、16中的端口19000保持一致
				int retval;
				retval = bind(nSocketUdp, (SOCKADDR*)&addrSock, sizeof(SOCKADDR));
				if (SOCKET_ERROR == retval)
				{
					closesocket(nSocketUdp);
					MessageBox(_T("绑定端口失败！"));
					return FALSE;
				}
				else
				{
					return TRUE;
				}
			}
		}
	}

	return TRUE;
}


//PR100 场强值接收线程
DWORD WINAPI CFSTView::RecvProc(LPVOID lpParameter)
{
	SOCKET sock = ((RECVPARAM*)lpParameter)->sock;
	HWND hwnd = ((RECVPARAM*)lpParameter)->hwnd;
	CFSTView *pView = ((RECVPARAM*)lpParameter)->m_pView;
	char cBuffer[33]; //接收数据缓冲区
	memset(cBuffer, 0, 33);
	int len = 0;
	pView->nCountLevel = 0;
	//	MSCpacket *pt;	
	CString str, strtemp, strdisplay = _T("");
	while (TRUE)
	{
		if (0 == Pr100ProcFlag || true == pView->stopPR100)
		{
			return 0;
		}
		//str.Format("%d,%d",pView->WorkAreaFlag,pView->SampleAreaFlag);
		//pView->SetDlgItemText(IDC_EDIT3,str);
		len = recv(sock, cBuffer, sizeof(cBuffer), 0);//接收数据
		pView->nLevel[pView->nCountLevel] = cBuffer[len - 4];
		pView->nLevel[pView->nCountLevel + 1] = cBuffer[len - 3];

		//***  显示原始场强值 调试用   *****//
		float Val;
		Val = ((short)(((pView->nLevel[pView->nCountLevel] << 8) & 0xff00) | (pView->nLevel[pView->nCountLevel + 1]) & 0xff));
		Val = (float)Val / 10;
		//Pr100输出场强值为short型，是真实场强值的10倍

// 		CString str1;
// 		str1.Format("%2.1f", Val);
// 		pView->SetDlgItemText(IDC_EDIT20, str1);
// 		//*** 显示原始场强值 调试用 end   *****//

		pView->nCountLevel += 2;

		if (true == pView->SampleAreaFlag)
		{
			//写SampleArea
			//	if(TRUE==pView->starRecordLevel)//写场强值操作
			//	{
			for (int i = 0; i<pView->nCountLevel; i += 2)
			{
				pView->SampleArea[pView->pSampleArea] = 0xff;
				/*					for(int j=0;j<6;j++)
				{
				//pView->SampleArea[pView->pSampleArea+j+1]=pView->TaxData[j];  //0-5帧  写入TAX箱里程值
				pView->SampleArea[pView->pSampleArea+j+1]=0x00;
				}
				*/
				pView->SampleArea[pView->pSampleArea + 7] = pView->nLevel[i];
				pView->SampleArea[pView->pSampleArea + 8] = pView->nLevel[i + 1];       //6-7帧  写入场强值
				for (int k = 0; k<7; k++)
				{
					pView->SampleArea[pView->pSampleArea + k + 9] = 0x00;               //8-15帧 写入0x00
				}
				/*************************************************************/
				//pView->SetDlgItemText(IDC_EDIT7,"Sample TAX+LEVEL");
				//str.Format("FF %02x %02x %02x %02x %02x %02x %02x %02x",pView->TaxData[0],pView->TaxData[1],\
										pView->TaxData[2],pView->TaxData[3],pView->TaxData[4],pView->TaxData[5],pView->nLevel[i],\
						pView->nLevel[i+1]);
//pView->SetDlgItemText(IDC_EDIT_TAX,str);  //显示用，可删除
/*************************************************************/
				pView->pSampleArea += 16;

			}

			pView->nCountLevel = 0;
			memset(pView->nLevel, 0, 10000);
			//	}
			// 			if (TRUE==pView->GpsRecordStart)//写GPS数据操作
			// 			{
			//  			pView->GpsRecordStart=false;
			// 				for(int i=0;i<pView->GpsmessageLen;i++)
			// 				{
			// 					pView->SampleArea[pView->pSampleArea+i]=pView->Gpsmessage.GetAt(i);
			// 				}
			// 				/*************************************************************/
			// 				pView->SetDlgItemText(IDC_EDIT6,"GPS");
			// 				pView->SetDlgItemText(IDC_EDIT_GPS,pView->Gpsmessage);  //显示用，可删除
			// 				/*************************************************************/
			// 
			// 				pView->pSampleArea+=pView->GpsmessageLen;
			// 
			// 				/*************************************************************/
			// 				pView->SetDlgItemText(IDC_EDIT4,"当前SamArea\r\n字节数");
			// 				str.Format("%d",pView->pSampleArea);                            //显示用，可删除
			// 				pView->SetDlgItemText(IDC_EDIT_SAMBYTENUMC,str);
			// 				/*************************************************************/
			// 			}
		}
		if (true == pView->WorkAreaFlag)
		{
			//写WorkArea
			//if(TRUE==pView->starRecordLevel)//写场强值
			//{
			for (int i = 0; i<pView->nCountLevel; i += 2)
			{
				pView->WorkArea[pView->pWorkArea] = 0xff;                          //帧头 0xff
																				   /*					for(int j=0;j<6;j++)
																				   {
																				   pView->WorkArea[pView->pWorkArea+j+1]=pView->TaxData[j];     //1-6帧  写入TAX箱里程值
																				   }
																				   */
				pView->WorkArea[pView->pWorkArea + 7] = pView->nLevel[i];
				pView->WorkArea[pView->pWorkArea + 8] = pView->nLevel[i + 1];          //7-8帧  写入场强值
				for (int k = 0; k<7; k++)
				{
					pView->WorkArea[pView->pWorkArea + k + 9] = 0x00;                  //9-16帧 写入0x00
				}
				pView->pWorkArea += 16;
				/*************************************************************/
				//pView->SetDlgItemText(IDC_EDIT7,"Work TAX+LEVEL");
				//str.Format("FF %02x %02x %02x %02x %02x %02x %02x %02x",pView->TaxData[0],pView->TaxData[1],\
										pView->TaxData[2],pView->TaxData[3],pView->TaxData[4],pView->TaxData[5],pView->nLevel[i],\
						pView->nLevel[i+1]);
//pView->SetDlgItemText(IDC_EDIT_TAX,str); //显示用，可删除
/*************************************************************/

			}

			pView->nCountLevel = 0;
			memset(pView->nLevel, 0, 10000);

			//}
			// 			if (TRUE==pView->GpsRecordStart)//写GPS数据操作
			// 			{
			//  				pView->GpsRecordStart=false;
			// 				for(int i=0;i<pView->GpsmessageLen;i++)
			// 				{
			// 					pView->WorkArea[pView->pWorkArea+i]=pView->Gpsmessage.GetAt(i);
			// 				}
			// 
			// 				/*************************************************************/
			// 				pView->SetDlgItemText(IDC_EDIT6,"GPS");
			// 				pView->SetDlgItemText(IDC_EDIT_GPS,pView->Gpsmessage);  //显示用，可删除
			// 				/*************************************************************/
			// 		
			// 				pView->pWorkArea+=pView->GpsmessageLen;
			// 
			// 				/*************************************************************/
			// 				pView->SetDlgItemText(IDC_EDIT5,"当前WorkArea\r\n字节数");
			// 				str.Format("%d",pView->pWorkArea);                            //显示用，可删除
			// 				pView->SetDlgItemText(IDC_EDIT_WORKBYTENUMC,str);
			// 				/*************************************************************/
			// 			}
		}
		/*if(pView->pSampleArea>ShareBufferSize || pView->pWorkArea>ShareBufferSize)
		{
		AfxMessageBox("内存溢出");
		return 0;
		}*/
		if (pView->pSampleArea>ShareBufferSize)
		{
			AfxMessageBox(_T("内存溢出 pView->pSampleArea") + pView->nCountLevel);
			//	cout << pView->nCountLevel << endl;
			return 0;
		}
		else if (pView->pWorkArea>ShareBufferSize)
		{
			AfxMessageBox(_T("内存溢出 pView->pWorkArea") + pView->nCountLevel);
			//	cout << pView->nCountLevel << endl;
			return 0;
		}
	}
	return 0;

}


void CFSTView::ReadData(unsigned char inData[], unsigned char sharebuffer[], int sizeofBuf)
{
	if (sizeofBuf >= 1024)
	{
		int i;
		int j;
		for (i = 0; i < 1024; i += 16)
		{
			if (0xff == inData[i])
			{
				inData[i + 7] = sharebuffer[i + 7];  //高位场强值数据  测试时场强值赋为常数 /0/
				inData[i + 8] = sharebuffer[i + 8];    //低位场强值数据  /200/

				for (j = 9; j < 16; j++)
				{
					inData[i + j] = 0x00;
				}
			}
		}
	}
	if (sizeofBuf < 1024)
	{
		int i;
		int k = 0;
		for (i = 0; i < sizeofBuf; i += 16)
		{
			if (0xff == inData[i])
			{
				inData[i + 7] = sharebuffer[i + 7];  //高位场强值数据  测试时场强值赋为常数 /0/
				inData[i + 8] = sharebuffer[i + 8];    //低位场强值数据  /200/
				for (int j = 9; j < 16; j++)
				{
					inData[i + j] = 0x00;
				}

			}
		}
		for (k = sizeofBuf; k < 1024; k++)
		{

			inData[k] = 0x00;
		}
	}
}



BOOL CFSTView::EnumChildProc(HWND hwndChild, LPARAM lParam)
{
	if (CtrlHide == lParam)
	{
		::ShowWindow(hwndChild, SW_HIDE);
	}
	else if (CtrlShow == lParam)
	{
		::ShowWindow(hwndChild, SW_SHOW);
	}
	return TRUE;
}
