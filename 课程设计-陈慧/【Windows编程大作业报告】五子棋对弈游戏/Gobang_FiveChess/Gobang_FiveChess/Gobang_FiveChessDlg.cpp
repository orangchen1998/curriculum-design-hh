
// Gobang_FiveChessDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Gobang_FiveChess.h"
#include "Gobang_FiveChessDlg.h"
#include "afxdialogex.h"

#include "DialogMore.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CGobang_FiveChessDlg 对话框
BEGIN_EASYSIZE_MAP(CGobang_FiveChessDlg)
	EASYSIZE(IDC_BUTTON_GAME_START,	ES_KEEPSIZE,	ES_BORDER,
        ES_BORDER,ES_KEEPSIZE,0)
	EASYSIZE(IDC_BUTTON_REGRET,	ES_KEEPSIZE,	ES_BORDER,
        ES_BORDER,ES_KEEPSIZE,0)
	EASYSIZE(IDC_BUTTON_MORE,	ES_KEEPSIZE,	ES_BORDER,
        ES_BORDER,ES_KEEPSIZE,0)


END_EASYSIZE_MAP

CGobang_FiveChessDlg::CGobang_FiveChessDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGobang_FiveChessDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
   
CGobang_FiveChessDlg::~CGobang_FiveChessDlg()
{   
}

void CGobang_FiveChessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);//与对应的变量交换对话框数据
}

BEGIN_MESSAGE_MAP(CGobang_FiveChessDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_GAME_START, &CGobang_FiveChessDlg::OnBnClickedButtonGameStart)
	ON_BN_CLICKED(IDC_BUTTON_REGRET, &CGobang_FiveChessDlg::OnBnClickedButtonRegret)
	ON_BN_CLICKED(IDC_BUTTON_MORE, &CGobang_FiveChessDlg::OnBnClickedButtonMore)
    ON_WM_SIZE()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CGobang_FiveChessDlg 消息处理程序

BOOL CGobang_FiveChessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CRect   rcClient;

	GetClientRect(&rcClient);
	m_chess.Init(rcClient);

    INIT_EASYSIZE;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

//系统响应消息映射
void CGobang_FiveChessDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGobang_FiveChessDlg::OnPaint()
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
		CDialogEx::OnPaint();

        CRect   rcBtnStart, rcBtnRegret, rcBtnMore;
        CRgn    rgnChildWnd,    rgnBtnStart,    rgnBtnRegret,    rgnBtnMore;			
		CDC*	pDC	= GetDC();	

        // 获取按钮在父窗口的位置
        GetDlgItem(IDC_BUTTON_GAME_START)->GetWindowRect(rcBtnStart);
        GetDlgItem(IDC_BUTTON_REGRET)->GetWindowRect(rcBtnRegret);
        GetDlgItem(IDC_BUTTON_MORE)->GetWindowRect(rcBtnMore);
        ScreenToClient(rcBtnStart);//把屏幕上指定点的屏幕坐标转换成用户坐标
        ScreenToClient(rcBtnRegret);
        ScreenToClient(rcBtnMore);

        // 剪切按钮的位置，创建的剪裁区域大小和控件一致
        rgnBtnStart.CreateRectRgnIndirect(rcBtnStart);
        rgnBtnRegret.CreateRectRgnIndirect(rcBtnRegret);   
        rgnBtnMore.CreateRectRgnIndirect(rcBtnMore);   
        rgnChildWnd.CreateRectRgn(0, 0, 0, 0);
        rgnChildWnd.CombineRgn(&rgnBtnStart, &rgnBtnRegret, RGN_OR);
        rgnChildWnd.CombineRgn(&rgnChildWnd, &rgnBtnMore, RGN_OR);
        pDC->SelectClipRgn(&rgnChildWnd, RGN_DIFF);

        // 绘棋局
		m_chess.Draw(pDC);

        // 善后
        pDC->SelectClipRgn(NULL);
        rgnBtnStart.DeleteObject();
        rgnBtnRegret.DeleteObject();
        rgnBtnMore.DeleteObject();
        rgnChildWnd.DeleteObject();
		ReleaseDC(pDC);
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGobang_FiveChessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGobang_FiveChessDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_chess.SetPiecePos(point);
	Invalidate();

    switch(m_chess.GetWinFlag())
    {
    case    WHITE_WIN:
        AfxMessageBox(_T("白旗赢"));
        break;
    case    BLACK_WIN:
        AfxMessageBox(_T("黑旗赢"));
        break;
    case    PEACE:
        AfxMessageBox(_T("平局"));
        break;
    default:
        break;
    }

	CDialogEx::OnLButtonUp(nFlags, point);
}


BOOL CGobang_FiveChessDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

//	return CDialogEx::OnEraseBkgnd(pDC);
	return TRUE;
}

//“开始游戏”按钮触发
void CGobang_FiveChessDlg::OnBnClickedButtonGameStart()
{
	// TODO: 在此添加控件通知处理程序代码
    m_chess.NewGame();// 新游戏
    Invalidate();
}

//“悔棋”按钮触发
void CGobang_FiveChessDlg::OnBnClickedButtonRegret()
{
	// TODO: 在此添加控件通知处理程序代码
    if(! m_chess.Regret())
    {
        AfxMessageBox(_T("再悔棋就不理你了~"));
    }
    Invalidate();
}

//“模式设置”按钮触发
void CGobang_FiveChessDlg::OnBnClickedButtonMore()
{
    CDialogMore* dlgMore    = new   CDialogMore;

	dlgMore->Create(IDD_DIALOG_MORE);   	
    dlgMore->ShowWindow(SW_SHOW);       // 非模态
// dlgMore->DoModal();

    dlgMore->SetChess(&m_chess);

	// TODO: 在此添加控件通知处理程序代码
}


//缩放后界面棋盘大小设置
void CGobang_FiveChessDlg::OnSize(UINT nType, int cx, int cy)
{       
    CDialogEx::OnSize(nType, cx, cy);   

    CRect	rcClient;
	GetClientRect(&rcClient);
	//获取窗口客户区的大小。注意一下：窗口的客户区为窗口中除标题栏、菜单栏之外的地方

	m_chess.Init(rcClient);

    UPDATE_EASYSIZE;
    Invalidate();
    // TODO: 在此处添加消息处理程序代码
}

//设置鼠标在键盘上形式为“+”字光标
void CGobang_FiveChessDlg::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    CRect   rcBoard = m_chess.GetRectBoard();
	// 获取棋盘区域	

    if(rcBoard.PtInRect(point))//判断点是否在CRect中
    {
        HCURSOR hCursor = AfxGetApp()->LoadStandardCursor(IDC_CROSS); //IDC_CROSS 十字光标
        SetCursor(hCursor);	
    }

    CDialogEx::OnMouseMove(nFlags, point);
}
