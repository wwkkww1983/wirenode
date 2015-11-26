
// MFCApplication1Dlg.cpp : ʵ���ļ�
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
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMFCApplication1Dlg �Ի���



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
	mlist.InsertColumn(0, L"�ڵ��", LVCFMT_LEFT, 100);
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


// CMFCApplication1Dlg ��Ϣ�������

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCApplication1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCApplication1Dlg::PaintCoordinateSystem(
	int x_min,    // x����ʼ����
	int x_step, // ��ʾ���
	int y_min, // y����ʼ����
	int y_step)// ��ʾ���
{
	CPen pen(1, 5, RGB(0x7A, 0x7A, 0x7A));
	CClientDC *pDC = new CClientDC(this);
	//����ӳ��ģʽ����λΪ0.1mm����ʱ��������ϵ�Կͻ������Ͻ�Ϊԭ�㣬
	//X��ķ���Ϊ�������ң�Y��ķ���Ϊ��������
	pDC->SetMapMode(MM_LOMETRIC);
	pDC->SelectObject(&pen);
	//��ÿͻ�����
	CRect rect;
	GetClientRect(&rect);
	//���ͻ�����ת��Ϊ�߼����꣬
	pDC->DPtoLP(&rect);
	//���߼������£���ͼ���ӿ�ԭ�����õ�λ��
	CSize org(200, -rect.bottom - 200);
	//����λ��ת��Ϊ�������꣬�����ƶ��ӿ�����ԭ��
	pDC->LPtoDP(&org);
	//�ƶ��ӿ�����ԭ�㣬����������ڿɼ�����
	pDC->SetViewportOrg(org.cx, org.cy);
	//���߼�����ϵ�»�X�ᣬ���Ϊ�߼�����ϵ�µ�ԭ�㣬�յ�Ϊ�Զ���
	pDC->MoveTo(0, 0);
	//X��ĳ���Ϊ�ͻ����Ŀ�ȼ�ȥ5���ף�һ�����ص�λΪ0.1mm),��ʱX���յ���ұ߿�ľ��뽫Ϊ3cm���ӿ�ԭ���Ѿ���
	//��������߿�2cm����
	pDC->LineTo(rect.right * 2 / 3, 0);


	pDC->MoveTo(rect.right * 2 / 3, 0);
	pDC->LineTo(rect.right * 2 / 3 - 50, 20);
	pDC->MoveTo(rect.right * 2 / 3, 0);
	pDC->LineTo(rect.right * 2 / 3 - 50, -20);
	pDC->TextOut(rect.right * 2 / 3, 0, L"X");


	//���߼�����ϵ�»�Y�ᣬ���Ϊ�߼�����ϵ�µ�ԭ�㣬�յ�Ϊ�Զ���
	pDC->MoveTo(0, 0);
	//Y��ĳ���Ϊ�ͻ����ĸ߶ȼ�ȥ5���ף�һ�����ص�λΪ0.1mm),��ʱY���յ���ϱ߿�ľ��뽫Ϊ3cm���ӿ�ԭ���Ѿ���
	//�������±߿�2cm����
	pDC->LineTo(0, -rect.bottom - 500);
	//���´���������Y����յ��ͷ
	pDC->MoveTo(-20, -rect.bottom - 540);
	pDC->LineTo(0, -rect.bottom - 500);
	pDC->MoveTo(20, -rect.bottom - 540);
	pDC->LineTo(0, -rect.bottom - 500);
	pDC->TextOut(0, -rect.bottom - 400, L"Y");
	//�ڻ����ʱ���̶ȵ����λΪcm����С��λΪmm����������ֻ����������cm�Σ�������Ӧ��λ�ñ���cm�̶�
	//����X����Ի������ٸ�cm�̶�
	int num = (rect.right + 1000) / 200;
	//����ѭ����X���ϵĿ̶ȣ�i��ʾ�ڶ��ٸ�mm�̶ȣ��ܹ�Ϊnum*10�����׿̶�
	int k = x_min;
	for (int i = 0; i <= num * 10; i++)
	{

		//�̶���Ϊ5��������ʱ�Ŀ���ֵΪ��5��10��15����
		if (i % 5 == 0)
		{


			//�̶�Ϊ10�ı���ʱ�������̶ȱ�ǣ����ĳ���Ϊ4mm,���ڿ̶ȵ��·������̶�ֵ����λΪcm
			if ((i % 20 == 0) && (k%x_step == 0)) // ���Ϊstep,Ĭ��Ϊ20
			{
				pDC->MoveTo(i * 10, 0);
				pDC->LineTo(i * 10, 40);
				CString str;
				str.Format(L"%d", k);  // ��ʾ�̶�
				pDC->TextOut(i * 10 - 10, -10, str);
				k += x_step;
			}
			else//�̶�Ϊ5��15��ʱ�������̶ȱ�ǣ����ĳ���Ϊ2mm
			{
				pDC->MoveTo(i * 10, 0);
				pDC->LineTo(i * 10, 20);
			}

		}
	}


	//����Y����Ի������ٸ�cm�̶�
	num = (-rect.bottom - 500) / 100;
	k = y_step;
	//���û�X���ϵ�������ͬ�ķ���������Y�����꣬��ʱע��滭��ĸı�
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//PaintCoordinateSystem(0, 1, 0, 1);
	OnOK();
}

int CMFCApplication1Dlg::convert(std::string strr, unsigned char * buffer){
	stringstream sstr;      
	sstr.clear();                
	sstr << strr;//��s��ֵ�ŵ�stringstream      
	sstr>>buffer;//��stringstream�е�ֵ������ch
	return 0;
}

void CMFCApplication1Dlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
	//���EDIT 
	//CEdit* pBoxOne; 
	//pBoxOne = (CEdit*) GetDlgItem(IDC_EDIT1); 
	//��ֵ 
	//pBoxOne-> SetWindowText( _T("FOO ") ); 
	//ȡֵ 
	//CString str;
	//pBoxOne-> GetWindowText(str); 

	//MessageBox(str,_T("�������н��"),MB_OK);
	//str.ReleaseBuffer();

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CMFCApplication1Dlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CMFCApplication1Dlg::OnLvnColumnclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
		
		//����ͨ��
	}
	CloseComm(A_hCom);
	return 0;
}

void CMFCApplication1Dlg::OnBnClickedButton2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CEdit* edit1 = (CEdit*)GetDlgItem(IDC_EDIT1);
	CString speedcom;
	edit1->GetWindowTextW(speedcom);
	if (!OpenComm("COM3")){
		MessageBox(L"���ڴ�ʧ��", 0);
		printf("���ڴ�ʧ��");
		//return;
		//MessageBox(L"���ڴ�ʧ��", L"test com");
	}
	else{
		MessageBox(L"���ڴ򿪳ɹ�");
		//MessageBox(L"���ڴ򿪳ɹ�", L"test");
	}

	comThread = CreateThread(NULL, 0, comThreadFun, (LPVOID)NULL, 0, NULL);

}
int convert(std::string strr, unsigned char * buffer){
	stringstream sstr;
	sstr.clear();
	sstr << strr;//��s��ֵ�ŵ�stringstream      
	sstr >> buffer;//��stringstream�е�ֵ������ch
	return 0;
}
