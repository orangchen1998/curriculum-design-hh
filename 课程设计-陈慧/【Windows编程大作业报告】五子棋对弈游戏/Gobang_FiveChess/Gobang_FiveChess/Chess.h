#pragma once
#include "ChessDraw.h"
#include "ChessAI.h"
#include "ChessCommon.h"



enum	enumWinFlag	{FIGHTING,	    WHITE_WIN,	   BLACK_WIN,	    PEACE};		
// FIGHTING	 对局中		WHITE_WIN	白方赢		 BLACK_WIN	黑方赢		PEACE 平局
enum	enumAIDepth	{AI_FOOLISH,	AI_PRIMARY,    AI_MIDDLE,	    AI_HIGH};
// AI_FOOLISH 弱智      AI_PRIMARY 初级          AI_MIDDLE 中级         AI_HIGH  高级
enum	enumVSMode	
{PERSON_VS_PERSON,	           
PERSON_VS_MACHINE,	   
MACHINE_VS_MACHINE};		// 人人对战、人机对战、机器对战

typedef struct
{
    int      iPieceNum;              // 记录的棋子个数
    CPoint   ptWhite;                // 最近一次白棋的位置
    CPoint   ptBlack;                // 最近一次黑棋的位置
    CPoint   ptLastCurPoint;         // 最近一次的棋子焦点位置
}STC_REGRET;    // 记录最近一次的下棋位置，便于悔棋操作


class CChess
{
public:
	CChess(void);
	~CChess(void);

	void	    Init(CRect	rect);
	void	    Draw(CDC*	pDC);				    // 画棋局
	void	    SetPiecePos(CPoint	ptCurrent);		// 设置当前棋子的位置    下棋函数

	void	    NewGame();		    // 新游戏
	BOOL	    Regret();			// 悔棋
	void	    SetVSMode(enumVSMode	emVSMode);	// 设置对战模式	enumVSMode
	void	    SetAIDepth(int	emAIDepth);	        // 设置AI智能程度
	enumWinFlag	GetWinFlag();		                // 获取输赢状况
	CRect	    GetRectBoard();	    // 获取棋盘区域	

private:
	enumChessColor	m_iPositionPiece[COLUMNS][ROWS];// 棋子的信息
	CChessDraw      m_chessdraw;			        // 棋局绘制类
	CRect			m_rcBoard;				        // 棋盘区域	
	CPoint			m_ptCurrent;			        // 棋子当前位置
	enumWinFlag	    m_emWin;			            // 输赢状况
	enumVSMode	    m_emVSMode;		                // 对战模式
    STC_REGRET      m_stcLastPos;                   // 记录最近一次的下棋位置
    BOOL            m_bTurnBlack;                   // 是否轮到黑方下棋
    int	            m_emAIDepth;		            // AI智能程度   为方便外部调用SetAIDepth()，所以设为了int类型

	BOOL   IsWin(UINT	uiCol,	UINT	uiRow,	enumChessColor   emChessColor);				              // 在第uiCol列第uiRow行，判断是否结束，若结束则返回TRUE
    BOOL   GetBestPosByAI(UINT&	uiCol,	UINT&	uiRow,	enumChessColor   emEnemyChessColor  = BLACK);     // 获取最好的下棋位置，机器都为白色
};

