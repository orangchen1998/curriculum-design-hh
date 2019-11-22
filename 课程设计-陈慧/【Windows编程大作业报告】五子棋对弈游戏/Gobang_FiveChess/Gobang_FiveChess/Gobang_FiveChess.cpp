
// Gobang_FiveChess.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "Gobang_FiveChess.h"
#include "Gobang_FiveChessDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGobang_FiveChessApp

BEGIN_MESSAGE_MAP(CGobang_FiveChessApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CGobang_FiveChessApp 构造

CGobang_FiveChessApp::CGobang_FiveChessApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CGobang_FiveChessApp 对象

CGobang_FiveChessApp theApp;


// CGobang_FiveChessApp 初始化

BOOL CGobang_FiveChessApp::InitInstance()
{

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;


	//设置MFC程序的注册表访问键,并把读写 ini 文件的成员函数映射到读写注册表
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CGobang_FiveChessDlg dlg;
	m_pMainWnd = &dlg;//主窗口变量
	INT_PTR nResponse = dlg.DoModal();
	//在主界面上使用Dialog时，DoModal()设置对话框模态
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

