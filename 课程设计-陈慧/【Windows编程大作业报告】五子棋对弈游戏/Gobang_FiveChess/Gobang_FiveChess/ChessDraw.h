#pragma once

//画棋局
class CChessDraw
{
public:
	CChessDraw(void);
	~CChessDraw(void);

	void    InitBoard(CRect	rect, UINT	uiRows, UINT	uiCols, UINT	uiBoardWidth, UINT	uiPieceRadius);

	void	SetDC(CDC*	pDC);
	void	DrawBackground();							            // 画背景
	void	DrawBoard();										    // 画棋盘
	BOOL	DrawPiece(CPoint	pt,	BOOL	bBlack);				// 画棋子		
	BOOL	DrawPiece(UINT	uiX,	UINT	uiY,	BOOL	bBlack);// 画棋子		
	void	DrawPieceCur(CPoint	pt);								// 给当前的棋子加上标记
	void	DrawPieceCur(UINT	uiX,	UINT	uiY);				// 给当前的棋子加上标记

	CRect	GetRectBoard();	// 获取棋盘区域	
	BOOL	GetCoordinateWithPoint(CPoint&	pt,	UINT*	puiX = NULL,	UINT*	puiY = NULL);	// 已知pt，获取棋盘坐标点(puiX,	puiY)	
	BOOL	GetPointWithCoordinate(CPoint&	pt,	UINT	uiX ,	UINT	uiY);                   // 已知棋盘坐标点(puiX,	puiY)，获取pt

//	void	    SetColorBK(COLORREF crBegin, COLORREF crEnd, BOOL bVertical = FALSE);		    // 设置背景颜色
//	void	    SetColorBoard(COLORREF cr);		// 设置棋盘颜色
//	void	    SetColorPiece(COLORREF cr);		// 设置棋子颜色

private:
	CDC*		m_pDC;					// 绘图DC，用于函数对应用程序窗口的客户区进行绘图
	CRect		m_rcBK;					// 背景区域	
	CRect		m_rcBoard;				// 棋盘区域	
	//一个CRect包含用于定位矩形的左上角和右下角点的成员变量。

	UINT		m_uiBoardCols;			// 棋盘的列数		// 五子棋行、列一样多，其他棋(如象棋)可能不一样
	UINT		m_uiBoardRows;		    // 棋盘的行数
	UINT		m_uiPieceRadius;		// 棋子半径
	UINT		m_uiBoardWidth;		    // 每一格棋盘的边长(正方形格子)

	COLORREF	m_crBKBegin;			// 渐变色起始端
	COLORREF	m_crBKEnd;				// 结尾端
	COLORREF	m_crBoard;				// 棋盘线条的颜色

	void		DrawSpecialPoints();	// 画指定的点，如五子棋中的天元等5个点	这里每种棋的规则都不一样，因此需单独修改
};

