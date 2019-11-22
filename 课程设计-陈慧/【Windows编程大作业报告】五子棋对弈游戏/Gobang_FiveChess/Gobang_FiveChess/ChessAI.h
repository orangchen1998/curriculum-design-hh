#pragma once
#include "ChessCommon.h"

// 以下AI下棋速度逐渐减慢，CPU占用逐渐增高，智力也逐渐增高
// AIFoolish         虽然智力最低，但其占用的内存和CPU是最低的，
// AIPrimary         占用内存低，占用CPU也低
// AIMiddle          内存和CPU占用居中
// AIHigh            占用内存低，占用CPU高

// 这里默认机器为白方
BOOL    AIFoolish (POINT&	 ptPosWhite,	POINT ptPosBlack,	enumChessColor   emChessColor,  const  enumChessColor emChess[][ROWS]);    //AIBlock   // 阻碍法AI： 	尽力阻碍对手成5连	智力较低      
BOOL    AIPrimary (POINT &pt ,   enumChessColor emChess[][ROWS]);                // 初级AI      
BOOL    AIMiddle  (POINT &pt ,   const  enumChessColor emChess[][ROWS]);         // 中级AI     
BOOL    AIHigh    (POINT &pt ,   enumChessColor emChess[][ROWS]);                // 高级AI      博弈树极大极小alpha-beta剪枝
