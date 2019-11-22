// DialogMore.cpp : 实现文件
//

#include "stdafx.h"
#include "Gobang_FiveChess.h"
#include "DialogMore.h"
#include "afxdialogex.h"


// CDialogMore 对话框

IMPLEMENT_DYNAMIC(CDialogMore, CDialogEx)//确定运行时对象属于哪一个类

CDialogMore::CDialogMore(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogMore::IDD, pParent)
{
    m_pChess    = NULL;
}

CDialogMore::~CDialogMore()
{
}

void CDialogMore::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_AI, m_comboxAI);
    DDX_Control(pDX, IDC_COMBO_VS_MODE, m_comboxVSMode);
}

//宏开始消息映射，为每个消息处理函数加入一个入口，
BEGIN_MESSAGE_MAP(CDialogMore, CDialogEx)
    ON_BN_CLICKED(IDOK, &CDialogMore::OnBnClickedOk)//用户单击按钮时,向父对象发送消息
END_MESSAGE_MAP()


// CDialogMore 消息处理程序,,
void CDialogMore::OnBnClickedOk()
{
    // TODO: 在此添加控件通知处理程序代码
    if(m_pChess)
    {
        m_pChess->SetAIDepth(m_comboxAI.GetCurSel());//先选择机器AI等级

        switch(m_comboxVSMode.GetCurSel())
        {
        case    0:
            m_pChess->SetVSMode(PERSON_VS_MACHINE);//人机对战
            break;
        case    1:
            m_pChess->SetVSMode(PERSON_VS_PERSON);//人人对战
            break;

        default:
            break;
        }       
    }
   
    CDialogEx::OnOK();//确认键开始
}


BOOL CDialogMore::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化       
    m_comboxAI.SetCurSel(2);              // 中级
    m_comboxVSMode.SetCurSel(0);    // 人机
    return TRUE;  // return TRUE unless you set the focus to a control
    // 异常: OCX 属性页应返回 FALSE
}

//// 传入棋类指针
void    CDialogMore::SetChess(CChess  *pChess)
{
    m_pChess    = pChess;
}