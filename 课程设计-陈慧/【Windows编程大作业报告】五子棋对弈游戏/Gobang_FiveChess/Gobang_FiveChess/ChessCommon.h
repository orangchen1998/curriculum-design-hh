#pragma once


//  公用头文件   目前只被ChessAI.h   和   Chess.h 共用
//	此项目默认为15*15的五子棋,  
//  注意: 
//  人机对战时，默认人为黑方，机器为白方，     而五子棋规定黑棋先行，即人先走棋


const	int	    MAXCREASE	= 4;    // 此项目为五子棋，所以最大数目为5-1
const	UINT	COLUMNS	    = 15;   // 棋盘的列数
const	UINT	ROWS		= 15;   // 棋盘的行数
enum	enumChessColor  {NONE,	WHITE,	BLACK};		// NONE	无棋子		WHITE 白方棋子	    BLACK 黑方棋子


// 相同颜色计数的加法器
void    SameColorAdder  (UINT	uiCol,	UINT	uiRow,	enumChessColor   emChessColor,    BOOL    bResetCount, int& iColorCount,   BOOL&    bOver,   
                          const enumChessColor iChess[][ROWS],     int* piBlankCount    = NULL );
  
// 在某点处各方向相同颜色的棋子数
void    GetSameColor    (UINT	uiCol,	UINT	uiRow,	enumChessColor   emChessColor,    int*  iSameColor,   
                          const enumChessColor iChess[][ROWS],    int*  piCreaseDirect = NULL,    BOOL   bIgnoreBlank   = FALSE,  int*    piBlankFlag = NULL);

// 某点是否在棋盘中
BOOL    PtInBroad(UINT	uiCol,	UINT	uiRow);