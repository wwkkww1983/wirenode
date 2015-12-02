
// MFCApplication1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include "tchart1.h"
#include "CSeries.h"
#include "ACom.h"

#define bufferSize 35
Data mdata[4];

NL list;
static HANDLE comThread;
static DWORD WINAPI comThreadFun(LPVOID IpParameter);
int convert(std::string strr, unsigned char * buffer);

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
unsigned char *buffer=new unsigned char[8];
Data data;
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
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCApplication1Dlg::OnBnClickedButton3)
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
	mlist.InsertColumn(0, L"节点", LVCFMT_LEFT, 100);
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

void CMFCApplication1Dlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
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

	// TODO:  在此添加控件通知处理程序代码
}
//创建线程还是点击更新再计算数据点。

void CMFCApplication1Dlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	mlist.DeleteAllItems();
	mlist.InsertItem(0, L"1000");
	mlist.InsertItem(1, L"1001");
	mlist.InsertItem(2, L"1002");
	mlist.InsertItem(3, L"1003");
	RePainter();
	
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
	CEdit *edit1 = (CEdit*)GetDlgItem(IDC_EDIT5);
	CEdit *edit2 = (CEdit*)GetDlgItem(IDC_EDIT3);
	CEdit *edit3 = (CEdit*)GetDlgItem(IDC_EDIT4);
	CEdit *edit4 = (CEdit*)GetDlgItem(IDC_EDIT2);
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
				edit1->SetWindowTextW(str);//double -> CString
				str.Format(_T("%f"), mdata[nItem].ex);
				edit2->SetWindowTextW(str);
				str.Format(_T("%f"), mdata[nItem].ey);
				edit3->SetWindowTextW(str);
				str.Format(_T("%f"), mdata[nItem].rssi);
				edit4->SetWindowTextW(str);
			}
		}
	}
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}

//串口数据解析
DWORD WINAPI comThreadFun(LPVOID IpParameter){
	string strr = "ok";
	convert(strr, buffer);
	list.delall();
	//WriteData(A_hCom, buffer, sizeof(buffer));
	while (1){
		ReadData(A_hCom, buffer, 1);
		if (buffer[0] == 'R'){
			ReadData(A_hCom, buffer, bufferSize);
			if (buffer[0] == '$'){
				if (buffer[5] == '1'){
					mdata[0].nlist = buffer[5]-0x30;
					//解析纬度7-10 12-16
					mdata[0].ey = ((buffer[7] - 0x30) * 10 + buffer[8] - 0x30) + \
						(buffer[9] - 0x30) / 6.0 + (buffer[10] - 0x30) / 60.0 + (buffer[12] - 0x30) / 600.0 + \
						(buffer[13] - 0x30) / 6000.0 + (buffer[14] - 0x30) / 60000.0 + (buffer[15] - 0x30) / 600000.0+(buffer[16]-0x30)/6000000.0;
					//解析经度18-22 24-28
					mdata[0].ex = ((buffer[18]-0x30)*100+(buffer[19] - 0x30) * 10 + buffer[20] - 0x30) +\
						(buffer[21] - 0x30) / 6.0 + (buffer[22] - 0x30) / 60.0 + (buffer[24] - 0x30) / 600.0 +\
						(buffer[25] - 0x30) / 6000.0 + (buffer[26] - 0x30) / 60000.0 + (buffer[27] - 0x30) / 600000.0 + (buffer[28] - 0x30) / 6000000.0;
					//解析rssi30 32 33 34;
					mdata[0].rssi = (buffer[30] - 0x30) * 10 + (buffer[31] - 0x30) +\
						(buffer[33] - 0x30) / 10.0 + (buffer[34] - 0x30) / 100.0;
					//会不会溢出？？？
				}
				if (buffer[5] == '2'){
					mdata[1].nlist = buffer[5] - 0x30;
					//解析
					mdata[1].ey = ((buffer[7] - 0x30) * 10 + buffer[8] - 0x30) + \
						(buffer[9] - 0x30) / 6.0 + (buffer[10] - 0x30) / 60.0 + (buffer[12] - 0x30) / 600.0 + \
						(buffer[13] - 0x30) / 6000.0 + (buffer[14] - 0x30) / 60000.0 + (buffer[15] - 0x30) / 600000.0 + (buffer[16] - 0x30) / 6000000.0;
					//解析经度18-22 24-28
					mdata[1].ex = ((buffer[18] - 0x30) * 100 + (buffer[19] - 0x30) * 10 + buffer[20] - 0x30) + \
						(buffer[21] - 0x30) / 6.0 + (buffer[22] - 0x30) / 60.0 + (buffer[24] - 0x30) / 600.0 + \
						(buffer[25] - 0x30) / 6000.0 + (buffer[26] - 0x30) / 60000.0 + (buffer[27] - 0x30) / 600000.0 + (buffer[28] - 0x30) / 6000000.0;
					//解析rssi30 32 33 34;
					mdata[1].rssi = (buffer[30] - 0x30) * 10 + (buffer[31] - 0x30) + \
						(buffer[33] - 0x30) / 10.0 + (buffer[34] - 0x30) / 100.0;
				}
				if (buffer[5] == '3'){
					mdata[2].nlist = buffer[5] - 0x30;
					//解析
					mdata[2].ey = ((buffer[7] - 0x30) * 10 + buffer[8] - 0x30) + \
						(buffer[9] - 0x30) / 6.0 + (buffer[10] - 0x30) / 60.0 + (buffer[12] - 0x30) / 600.0 + \
						(buffer[13] - 0x30) / 6000.0 + (buffer[14] - 0x30) / 60000.0 + (buffer[15] - 0x30) / 600000.0 + (buffer[16] - 0x30) / 6000000.0;
					//解析经度18-22 24-28
					mdata[2].ex = ((buffer[18] - 0x30) * 100 + (buffer[19] - 0x30) * 10 + buffer[20] - 0x30) + \
						(buffer[21] - 0x30) / 6.0 + (buffer[22] - 0x30) / 60.0 + (buffer[24] - 0x30) / 600.0 + \
						(buffer[25] - 0x30) / 6000.0 + (buffer[26] - 0x30) / 60000.0 + (buffer[27] - 0x30) / 600000.0 + (buffer[28] - 0x30) / 6000000.0;
					//解析rssi30 32 33 34;
					mdata[2].rssi = (buffer[30] - 0x30) * 10 + (buffer[31] - 0x30) + \
						(buffer[33] - 0x30) / 10.0 + (buffer[34] - 0x30) / 100.0;
				}
				//链表操作
				/*data.ey= (buffer[5] - 0x30) * 10 + buffer[6] - 0x30;
				data.ex = (buffer[16] - 0x30) * 100 + (buffer[17] - 0x30) * 10 + buffer[18] - 0x30;
				data.nlist = buffer[28]-0x30;
				list.del(data);
				list.add(data);*/
				
			}
		}
		//数据通信
	}
	CloseComm(A_hCom);
	return 0;
}

void CMFCApplication1Dlg::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	USES_CONVERSION;
	CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT1);
	CString speedcom;
	edit1->GetWindowTextW(speedcom);
	if (!OpenComm(W2A(speedcom))){
		MessageBox(L"串口打开失败", 0);
		printf("串口打开失败");
		return;
	}
	else{
		MessageBox(L"串口打开成功");
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
BEGIN_EVENTSINK_MAP(CMFCApplication1Dlg, CDialog)
	ON_EVENT(CMFCApplication1Dlg, IDC_TCHART1, 4, CMFCApplication1Dlg::OnClickAxisTchart1, VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CMFCApplication1Dlg, IDC_TCHART1, 7, CMFCApplication1Dlg::OnClickSeriesTchart1, VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CMFCApplication1Dlg, IDC_TCHART1, 3, CMFCApplication1Dlg::OnClickTchart1, VTS_NONE)
	ON_EVENT(CMFCApplication1Dlg, IDC_TCHART1, 31, CMFCApplication1Dlg::OnTimerTchart1, VTS_NONE)
END_EVENTSINK_MAP()


void CMFCApplication1Dlg::OnClickAxisTchart1(long Axis, long Button, long Shift, long X, long Y)
{
	// TODO:  在此处添加消息处理程序代码
}


void CMFCApplication1Dlg::OnClickSeriesTchart1(long SeriesIndex, long ValueIndex, long Button, long Shift, long X, long Y)
{
	// TODO:  在此处添加消息处理程序代码
}


void CMFCApplication1Dlg::OnClickTchart1()
{
	// TODO:  在此处添加消息处理程序代码
}


void CMFCApplication1Dlg::OnTimerTchart1()
{
	// TODO:  在此处添加消息处理程序代码
}

void CMFCApplication1Dlg::RePainter(){
	CSeries lineSeries = (CSeries)m_chart.Series(1);
	lineSeries.Clear();
	if (mdata[0].ex == 0){
		MessageBox(L"链表中的数据空");
		return;
	}
	lineSeries.AddXY((double)mdata[0].ex, (double)mdata[0].ey, L"1001", NULL);
	lineSeries.AddXY((double)mdata[1].ex, (double)mdata[1].ey, L"1002", NULL);
	lineSeries.AddXY((double)mdata[2].ex, (double)mdata[2].ey, L"1003", NULL);
	Computer(mdata);
	lineSeries.AddXY((double)mdata[3].ex, (double)mdata[3].ey, L"移动节点", NULL);
}

int CMFCApplication1Dlg::Computer(Data *data){
	//计算的自节点的坐标；
	double x[3];
	x[0] = data[0].ex;
	x[1] = data[1].ex;
	x[2] = data[2].ex;
	double y[3];
	y[0] = data[0].ey;
	y[1] = data[1].ey;
	y[2] = data[2].ey;
	double d[3];
	d[0] = data[0].rssi;
	d[1] = data[1].rssi;
	d[2] = data[2].rssi;
	double k = 1000000.0 / ((x[0] - x[2])*(y[1] - y[2]) - (y[0] - y[2])*(x[1] - x[2]));
	mdata[3].ex = (x[0] * x[0] - x[2] * x[2] + y[0] * y[0] - y[2] * y[2] + d[2] * d[2]- d[0] * d[0])*(y[1] - y[2]) + \
		(y[2] - y[0])*(x[0] * x[0] - x[2] * x[2]+ y[1] * y[1] - y[2] * y[2] + d[2] * d[2] - d[1] * d[1]);
	mdata[3].ex = mdata[3].ex / k;
	mdata[3].ey = (x[0] * x[0] - x[2] * x[2] + y[0] * y[0] - y[2] * y[2] + d[2] * d[2] - d[0] * d[0])*(x[2] - x[1]) + \
		(x[0] - x[2])*(x[0] * x[0] - x[2] * x[2] + y[1] * y[1] - y[2] * y[2] + d[2] * d[2] - d[1] * d[1]);
	mdata[3].ey = mdata[3].ey / k;
	return 0;
}

void CMFCApplication1Dlg::OnBnClickedButton3()
{
	CloseComm(A_hCom);
	CloseHandle(comThread);
	// TODO:  在此添加控件通知处理程序代码
}

CMFCApplication1Dlg::~CMFCApplication1Dlg(){
	CloseHandle(comThread);
}