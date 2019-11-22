#include "StdAfx.h"
#include "ChessDraw.h"
#include "FaceFunc.h"

CChessDraw::CChessDraw()
{
	m_pDC	= NULL;
	m_rcBK.SetRectEmpty();// 背景区域
	m_rcBoard.SetRectEmpty();// 棋盘区域	

	m_crBKBegin		= RGB(255,232,166);// 渐变色起始端
	m_crBKEnd		= RGB(255,220,200);// 结尾端
	m_crBoard		= RGB(0,0,0);// 棋盘线条的颜色

	m_uiPieceRadius	= 0;// 棋子半径
	m_uiBoardRows	= 0;// 棋盘的行数
	m_uiBoardCols	= 0;// 棋盘的列数
	m_uiBoardWidth	= 0; // 每一格棋盘的边长(正方形格子)
}

CChessDraw::~CChessDraw()
{
}

//初始化棋盘
void	CChessDraw::InitBoard(CRect	rect, UINT	uiRows, UINT	uiCols, UINT	uiBoardWidth, UINT	uiPieceRadius)
{
	m_rcBK	= rect;
	m_uiBoardRows	= uiRows;
	m_uiBoardCols	= uiCols;
	m_uiBoardWidth	= uiBoardWidth;
	m_uiPieceRadius	= uiPieceRadius;

	// 计算棋盘区域
	m_rcBoard	= m_rcBK;
	m_rcBoard.DeflateRect(CSize(20,20));	// 背景区域的左上点偏移(20,20)再画棋盘
	m_rcBoard.right		= m_rcBoard.left + m_uiBoardWidth * (m_uiBoardCols - 1);//右边界位置随左边界和列数和每一格棋盘的边长决定
	m_rcBoard.bottom	= m_rcBoard.top  + m_uiBoardWidth * (m_uiBoardRows - 1);//下边界位置随上边界和行数和每一格棋盘的边长决定
}

CRect	CChessDraw::GetRectBoard()// 获取棋盘区域	
{
	return	m_rcBoard;
}
	
// 获取绘图DC
void	CChessDraw::SetDC(CDC*	pDC)
{
    m_pDC	= pDC;
}

// 画背景
void CChessDraw::DrawBackground()
{
    if(! m_pDC)//m_pDC=0,返回；！=0才画
    {
        return;
    }

	// 将指定区域（棋盘背景区域）填充成渐变颜色
    GradientFillRect(m_pDC,	m_rcBK,	m_crBKBegin,	m_crBKEnd,	TRUE);
}

 // 画棋盘
void CChessDraw::DrawBoard()
{
	if(! m_pDC)//m_pDC=0,返回；！=0才画
	{
		return;
	}
	// 画横线  CRect类型m_rcBoard表示棋盘区域	
	UINT	ptTop	= m_rcBoard.top;

	for(UINT i	= 0;	i < m_uiBoardRows;	i ++)//用行数控制，由上至下
	{
		m_pDC->MoveTo(m_rcBoard.left, ptTop);
		m_pDC->LineTo(m_rcBoard.right,	ptTop);
		ptTop	+= m_uiBoardWidth;
	}

	// 画竖线
	UINT	ptLeft	= m_rcBoard.left;
	for(UINT i	= 0;	i < m_uiBoardCols;	i ++)//用列数控制，由左到右
	{
		m_pDC->MoveTo(ptLeft, m_rcBoard.top);
		m_pDC->LineTo(ptLeft,	m_rcBoard.bottom);
		ptLeft	+= m_uiBoardWidth;
	}

	// 五子棋中的天元等5个点
	DrawSpecialPoints();
}

// 画棋子	
BOOL CChessDraw::DrawPiece(CPoint	pt,	BOOL	bBlack)//CPoint类操纵CPoint和POINT结构的成员函数
{
	//已知pt，获取棋盘坐标点(puiX,	puiY)
	if(! m_pDC	|| ! GetCoordinateWithPoint(pt))
	{
		return	FALSE;
	}

	//CRect	rc定义矩形的左上角和右下角点的成员变量。
	CRect	rc(pt - CSize(m_uiPieceRadius,	m_uiPieceRadius),	pt + CSize(m_uiPieceRadius,	m_uiPieceRadius));
	CRgn	rgn;

	//CRgn封装了一个Windows图形设备接口(GDI)区域。这一区域是某一窗口中的一个椭圆或多边形区域
	
	//创建一内切于特定矩形的椭圆区域
	rgn.CreateEllipticRgnIndirect(rc);

	//选择一个区域作为指定设备环境的当前剪切区域,即选取该椭圆区域
	m_pDC->SelectClipRgn(&rgn, RGN_AND);

	if(bBlack)//bBlack=1
	{
		FillGradientEx(m_pDC,	rc,		RGB(140,130,120),	RGB(0,0,0),	FALSE);             // 画黑棋
		//	GradientFillRect(m_pDC,	rc,		RGB(0,0,255),	RGB(180,180,180),	TRUE);		// 此函数有点问题(以这行代码运行后，可以看出来)
	}
	else
	{
		FillGradientEx(m_pDC,	rc,		RGB(255,255,255),	RGB(180,200,180),	FALSE);     // 画白棋
	}

	m_pDC->SelectClipRgn(NULL);
	return	TRUE;
}
	
BOOL	CChessDraw::DrawPiece(UINT	uiX,	UINT	uiY,	BOOL	bBlack)
{
	CPoint	pt;
	// 已知棋盘坐标点(puiX,	puiY)，获取pt
	if(! m_pDC	|| ! GetPointWithCoordinate(pt,	 uiX, uiY))
	{
		return	FALSE;
	}

	CRect	rc(pt - CSize(m_uiPieceRadius,	m_uiPieceRadius),	pt + CSize(m_uiPieceRadius,	m_uiPieceRadius));
	CRgn	rgn;

	rgn.CreateEllipticRgnIndirect(rc);
	m_pDC->SelectClipRgn(&rgn, RGN_AND);

	if(bBlack)
	{
		FillGradientEx(m_pDC,	rc,		RGB(140,130,120),	RGB(0,0,0),	FALSE);
	}
	else
	{
		FillGradientEx(m_pDC,	rc,		RGB(255,255,255),	RGB(180,200,180),	FALSE);
	}

	m_pDC->SelectClipRgn(NULL);
	return	TRUE;
}

// 给当前的棋子加上标记
void CChessDraw::DrawPieceCur(CPoint	pt)
{
	if(! m_pDC	|| ! GetCoordinateWithPoint(pt))
	{
		return;
	}
	//CPen类是画笔的对象，用来在DC上完成绘制线条的任务。
	CPen	penNew(PS_DASH, 1, RGB(255,0,0));
	CPen*	penOld	= m_pDC->SelectObject(&penNew);

//	int	iSize	= 5;
	int	iSize	= m_uiPieceRadius/3;
	// 画横线
	m_pDC->MoveTo(pt.x	-	iSize, pt.y);
	m_pDC->LineTo(pt.x	+	iSize, pt.y);
	// 画竖线
	m_pDC->MoveTo(pt.x,	pt.y	-	iSize);
	m_pDC->LineTo(pt.x,	pt.y	+	iSize);

	m_pDC->SelectObject(penOld);
}

void		CChessDraw::DrawPieceCur(UINT	uiX,	UINT	uiY)
{
	CPoint	pt;
	if(! m_pDC	|| ! GetPointWithCoordinate(pt,	 uiX, uiY))
	{
		return	;
	}

	CPen	penNew(PS_DASH, 1, RGB(255,0,0));
	CPen*	penOld	= m_pDC->SelectObject(&penNew);

	int	iSize	= m_uiPieceRadius/2 - 1;
	// 画横线
	m_pDC->MoveTo(pt.x	-	iSize, pt.y);
	m_pDC->LineTo(pt.x	+	iSize, pt.y);
	// 画竖线
	m_pDC->MoveTo(pt.x,	pt.y	-	iSize);
	m_pDC->LineTo(pt.x,	pt.y	+	iSize);

	m_pDC->SelectObject(penOld);
}

//// 五子棋中的天元等5个点
void CChessDraw::DrawSpecialPoints()
{
	if(! m_pDC)
	{
		return;
	}
	CRect	    rcTmp;
	CRect	    rc	= m_rcBoard;// 棋盘区域	
	COLORREF	rgb = RGB(0,0,0);		
	UINT	    uiSpecialRadius = m_uiPieceRadius/2 - 1;	// 天元等点的半径(默认为正方形，此值表示边长的一半)
	CSize	    szRadius(uiSpecialRadius,	uiSpecialRadius);
	const	int	SPACE	= 3 * m_uiBoardWidth;		        // 偏移

	rc.DeflateRect(CSize(SPACE,	SPACE));//通过朝CRect的中心移动边以缩小CRect

	rcTmp	= CRect(rc.TopLeft(),	rc.TopLeft());//指向新区域的左上角
	rcTmp.InflateRect(szRadius);//增大或减小指定矩形的宽和高
	m_pDC->FillSolidRect(rcTmp,		rgb);

	rcTmp	= CRect(rc.BottomRight(),	rc.BottomRight());
	rcTmp.InflateRect(szRadius);
	m_pDC->FillSolidRect(rcTmp,		rgb);//指定填充矩形使用的颜色。

	rcTmp	= CRect(CPoint(rc.right, rc.top),	CPoint(rc.right, rc.top));
	rcTmp.InflateRect(szRadius);
	m_pDC->FillSolidRect(rcTmp,		rgb);

	rcTmp	= CRect(CPoint(rc.left, rc.bottom),	CPoint(rc.left, rc.bottom));
	rcTmp.InflateRect(szRadius);
	m_pDC->FillSolidRect(rcTmp,		rgb);

	rcTmp	= CRect(rc.CenterPoint(),	rc.CenterPoint());
	rcTmp.InflateRect(szRadius);
	m_pDC->FillSolidRect(rcTmp,		rgb);
}

// 已知棋盘坐标点(puiX,	puiY)，获取pt
BOOL	CChessDraw::GetCoordinateWithPoint(CPoint&	pt,	UINT*	puiX,	UINT*	puiY)
{
	// 对坐标点进行取整，使其刚好在线条交叉处
	CRect	rcTmp	= m_rcBoard;

	rcTmp.InflateRect(CSize(m_uiBoardWidth/2, m_uiBoardWidth/2));
	if(! rcTmp.PtInRect(pt))
	{
		return	FALSE;
	}

	//UINT	uiPosX	= (pt.x - m_rcBoard.left + m_uiBoardWidth/2)/m_uiBoardWidth;			// 在第uiPosX个交叉点
	//UINT	uiPosY	= (pt.y - m_rcBoard.top + m_uiBoardWidth/2)/m_uiBoardWidth;
	UINT	uiPosX	= (pt.x - rcTmp.left)/m_uiBoardWidth;			// 在第uiPosX个交叉点
	UINT	uiPosY	= (pt.y - rcTmp.top)/m_uiBoardWidth;

	pt.x	= m_rcBoard.left+ m_uiBoardWidth * uiPosX;
	pt.y	= m_rcBoard.top	+ m_uiBoardWidth * uiPosY;

	if(puiX)
	{
		*puiX	= uiPosX;
	}
	if(puiY)
	{
		*puiY	= uiPosY;
	}

	return	TRUE;
}
	
// 已知pt，获取棋盘坐标点(puiX,	puiY)	
BOOL	CChessDraw::GetPointWithCoordinate(CPoint&	pt,	UINT	uiX ,	UINT	uiY)
{
	if((uiX	 < 0)	||	(uiX >= m_uiBoardCols)	|| (uiY	 < 0)	||	(uiY >= m_uiBoardRows)	)
	{
		return	FALSE;
	}
	pt.x	= m_rcBoard.left+ m_uiBoardWidth * uiX;
	pt.y	= m_rcBoard.top	+ m_uiBoardWidth * uiY;

	return	TRUE;
}
