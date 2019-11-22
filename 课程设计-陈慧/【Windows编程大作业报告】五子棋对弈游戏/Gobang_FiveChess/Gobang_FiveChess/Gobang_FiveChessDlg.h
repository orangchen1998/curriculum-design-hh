
// Gobang_FiveChessDlg.h : 头文件
//

#pragma once
#include "Chess.h"

// CGobang_FiveChessDlg 对话框
class CGobang_FiveChessDlg : public CDialogEx
{
    DECLARE_EASYSIZE
// 构造
public:
	CGobang_FiveChessDlg(CWnd* pParent = NULL);	// 标准构造函数
    CGobang_FiveChessDlg::~CGobang_FiveChessDlg();

// 对话框数据
	enum { IDD = IDD_GOBANG_FIVECHESS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	CChess		m_chess;	// 棋类
// 实现
protected:
	HICON m_hIcon;//图标句柄

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	//afx_msg向系统声明：有消息映射到函数实现体
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButtonGameStart();
	afx_msg void OnBnClickedButtonRegret();
	afx_msg void OnBnClickedButtonMore();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
