
// MFCApplication1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include "tchart1.h"
#include "CSeries.h"
#include "ACom.h"

nodelist list;
static HANDLE comThread;
static DWORD WINAPI comThreadFun(LPVOID IpParameter);
int convert(std::string strr, unsigned char * buffer);

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
unsigned char *buffer=new unsigned char[8];
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 对话框



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMFCApplication1Dlg::IDD, pParent)
	//, m_chart(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TCHART1, m_chart);
	DDX_Control(pDX, IDC_LIST2, mlist);

	LONG lStyle;
	lStyle = GetWindowLong(mlist.m_hWnd, GWL_STYLE);
	lStyle &= ~LVS_TYPEMASK;
	lStyle |= LVS_REPORT;
	SetWindowLong(mlist.m_hWnd, GWL_STYLE, lStyle);

	DWORD dwStyle = mlist.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_CHECKBOXES;
	mlist.SetExtendedStyle(dwStyle);

	mlist.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	mlist.InsertColumn(0, L"节点号", LVCFMT_LEFT, 100);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMFCApplication1Dlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT1, &CMFCApplication1Dlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &CMFCApplication1Dlg::OnLvnItemchangedList2)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST2, &CMFCApplication1Dlg::OnLvnColumnclickList2)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, &CMFCApplication1Dlg::OnNMClickList2)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1Dlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 消息处理程序

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCApplication1Dlg::PaintCoordinateSystem(
	int x_min,    // x轴起始座标
	int x_step, // 显示间隔
	int y_min, // y轴起始座标
	int y_step)// 显示间隔
{
	CPen pen(1, 5, RGB(0x7A, 0x7A, 0x7A));
	CClientDC *pDC = new CClientDC(this);
	//设置映射模式，单位为0.1mm，引时窗口坐标系以客户区左上角为原点，
	//X轴的方向为从左往右，Y轴的方向为从下往上
	pDC->SetMapMode(MM_LOMETRIC);
	pDC->SelectObject(&pen);
	//获得客户区域
	CRect rect;
	GetClientRect(&rect);
	//将客户坐标转换为逻辑坐标，
	pDC->DPtoLP(&rect);
	//在逻辑坐标下，意图将视口原点所置的位置
	CSize org(200, -rect.bottom - 200);
	//将该位置转换为设置坐标，方便移动视口坐标原点
	pDC->LPtoDP(&org);
	//移动视口坐标原点，整个坐标尺在可见区域
	pDC->SetViewportOrg(org.cx, org.cy);
	//在逻辑坐标系下画X轴，起点为逻辑坐标系下的原点，终点为自定的
	pDC->MoveTo(0, 0);
	//X轴的长度为客户区的宽度减去5厘米（一个象素单位为0.1mm),此时X轴终点距右边框的距离将为3cm（视口原点已经移
	//动到距左边框2cm处）
	pDC->LineTo(rect.right * 2 / 3, 0);


	pDC->MoveTo(rect.right * 2 / 3, 0);
	pDC->LineTo(rect.right * 2 / 3 - 50, 20);
	pDC->MoveTo(rect.right * 2 / 3, 0);
	pDC->LineTo(rect.right * 2 / 3 - 50, -20);
	pDC->TextOut(rect.right * 2 / 3, 0, L"X");


	//在逻辑坐标系下画Y轴，起点为逻辑坐标系下的原点，终点为自定的
	pDC->MoveTo(0, 0);
	//Y轴的长度为客户区的高度减去5厘米（一个象素单位为0.1mm),此时Y轴终点距上边框的距离将为3cm（视口原点已经移
	//动到距下边框2cm处）
	pDC->LineTo(0, -rect.bottom - 500);
	//以下代码用来画Y轴的终点箭头
	pDC->MoveTo(-20, -rect.bottom - 540);
	pDC->LineTo(0, -rect.bottom - 500);
	pDC->MoveTo(20, -rect.bottom - 540);
	pDC->LineTo(0, -rect.bottom - 500);
	pDC->TextOut(0, -rect.bottom - 400, L"Y");
	//在画标尺时，刻度的最大单位为cm，最小单位为mm，坐标轴上只画有整数个cm段，并在相应的位置标明cm刻度
	//计算X轴可以画出多少个cm刻度
	int num = (rect.right + 1000) / 200;
	//利用循环画X轴上的刻度，i表示第多少个mm刻度，总共为num*10个毫米刻度
	int k = x_min;
	for (int i = 0; i <= num * 10; i++)
	{

		//刻度能为5整除，此时的可能值为：5，10，15……
		if (i % 5 == 0)
		{


			//刻度为10的倍数时，画出刻度标记，它的长度为4mm,并在刻度的下方标明刻度值，单位为cm
			if ((i % 20 == 0) && (k%x_step == 0)) // 间隔为step,默认为20
			{
				pDC->MoveTo(i * 10, 0);
				pDC->LineTo(i * 10, 40);
				CString str;
				str.Format(L"%d", k);  // 显示刻度
				pDC->TextOut(i * 10 - 10, -10, str);
				k += x_step;
			}
			else//刻度为5，15等时，画出刻度标记，它的长度为2mm
			{
				pDC->MoveTo(i * 10, 0);
				pDC->LineTo(i * 10, 20);
			}

		}
	}


	//计算Y轴可以画出多少个cm刻度
	num = (-rect.bottom - 500) / 100;
	k = y_step;
	//相用画X轴上的坐标相同的方法来绘制Y轴坐标，此时注意绘画点的改变
	for (int i = 1; i <= num * 10; i++)
	{
		if (i % 5 == 0)
		{
			if ((i % 20 == 0) && (k%y_step == 0))
			{
				pDC->MoveTo(0, i * 10);
				pDC->LineTo(40, i * 10);
				CString str;
				str.Format(L"%d", k);
				pDC->TextOut(-45, i * 10 + 10, str);
				k += y_step;
			}
			else
			{
				pDC->MoveTo(0, i * 10);
				pDC->LineTo(20, i * 10);
			}
		}
	}


	delete pDC;
}

void CMFCApplication1Dlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	//PaintCoordinateSystem(0, 1, 0, 1);
	OnOK();
}

int CMFCApplication1Dlg::convert(std::string strr, unsigned char * buffer){
	stringstream sstr;      
	sstr.clear();                
	sstr << strr;//将s的值放到stringstream      
	sstr>>buffer;//将stringstream中的值导出到ch
	return 0;
}

void CMFCApplication1Dlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	//获得EDIT 
	//CEdit* pBoxOne; 
	//pBoxOne = (CEdit*) GetDlgItem(IDC_EDIT1); 
	//赋值 
	//pBoxOne-> SetWindowText( _T("FOO ") ); 
	//取值 
	//CString str;
	//pBoxOne-> GetWindowText(str); 

	//MessageBox(str,_T("程序运行结果"),MB_OK);
	//str.ReleaseBuffer();

	// TODO:  在此添加控件通知处理程序代码
}


void CMFCApplication1Dlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	mlist.DeleteAllItems();
	mlist.InsertItem(0, L"1005");
	mlist.InsertItem(0, L"1000");
	mlist.InsertItem(0, L"1001");
	mlist.InsertItem(0, L"1002");
	mlist.InsertItem(0, L"1003");
	CSeries lineSeries = (CSeries)m_chart.Series(1);
	lineSeries.Clear();
	for (int i = 0; i < 100; i=i+10)
	{
		lineSeries.AddXY((double)i, rand(), NULL, NULL);
	}
}


void CMFCApplication1Dlg::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}


void CMFCApplication1Dlg::OnLvnColumnclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}


void CMFCApplication1Dlg::OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	POSITION pos = mlist.GetFirstSelectedItemPosition();
	CString str;
	if (pos == NULL)
		MessageBox(L"No items were selected!\n");
	else
	{
		while (pos)
		{
			int nItem = -1;
			nItem = mlist.GetNextSelectedItem(pos);

			if (nItem >= 0 && nItem < mlist.GetItemCount())
			{

				str = mlist.GetItemText(nItem, 0);
				MessageBox(str);
				/*edit1->SetWindowTextW(str);
				str = m_list.GetItemText(nItem, 1);
				edit2->SetWindowTextW(str);
				str = m_list.GetItemText(nItem, 2);
				edit3->SetWindowTextW(str);
				str = m_list.GetItemText(nItem, 3);
				edit4->SetWindowTextW(str);
				str = m_list.GetItemText(nItem, 4);
				edit5->SetWindowTextW(str);
				str = m_list.GetItemText(nItem, 5);
				edit6->SetWindowTextW(str);
				str = m_list.GetItemText(nItem, 6);*/
				//edit7->SetWindowTextW(str);
				//USES_CONVERSION;
				//if (!strcmp(W2A(str), W2A(L"Friend")))
				//	CComb->SetWindowTextW(L"Friend");
				//else if (!strcmp(W2A(str), W2A(L"Family")))
				//	CComb->SetWindowTextW(L"Family");
				//else if (!strcmp(W2A(str), W2A(L"Coworker")))
				//	CComb->SetWindowTextW(L"Coworker");
				//else
				//	CComb->SetWindowTextW(L"Stranger");
			}

			//MessageBox(L"Item %d was selected!n");
			// you could do your own processing on nItem here
		}
	}
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}

DWORD WINAPI comThreadFun(LPVOID IpParameter){
	string strr = "ok";
	convert(strr, buffer);
	Data data;
	//WriteData(A_hCom, buffer, sizeof(buffer));
	while (1){

		data.nlist = 0;
		list.add(data);
		
		//数据通信
	}
	CloseComm(A_hCom);
	return 0;
}

void CMFCApplication1Dlg::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT1);
	CString speedcom;
	edit1->GetWindowTextW(speedcom);
	if (!OpenComm("COM3")){
		MessageBox(L"串口打开失败", 0);
		printf("串口打开失败");
		//return;
		//MessageBox(L"串口打开失败", L"test com");
	}
	else{
		MessageBox(L"串口打开成功");
		//MessageBox(L"串口打开成功", L"test");
	}

	comThread = CreateThread(NULL, 0, comThreadFun, (LPVOID)NULL, 0, NULL);

}
int convert(std::string strr, unsigned char * buffer){
	stringstream sstr;
	sstr.clear();
	sstr << strr;//将s的值放到stringstream      
	sstr >> buffer;//将stringstream中的值导出到ch
	return 0;
}
