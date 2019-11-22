#include "StdAfx.h"
#include "ChessAI.h"


// SCORE_BLACK[i][j] 
// 表示连续j+1个黑棋(且两边共i个空位)的评分
// i = 0表示两边都被堵住    i = 1表示有一边没被堵住  i = 2 表示两边都有空位
const   double SCORE_WHITE[][5]   =   {   {0,  0, 0,	0,	10000000},   {1,     25,	 500,	 2500,   10000000},  {5,    100,	 2500,   50000,   	10000000} };
const   double SCORE_BLACK[][5]   =   {   {0,  0, 0,	0,	-1000000},   {-5,   -100,	-500,   -10000,	-1000000 },  {-25,  -500,	-10000,	-250000,	-1000000} };

// 这里假设i=0的分数最低，i>0时，子越多越好，
// 如：[1][3]>[2][2]  即死四>活三  ，因为再下一棋，死四即成五连
// 死三>活二  ，因为己方再下一棋，可以使死三成死四,或者活二成活三，就变成了死四>活三 ；敌方再下一棋，可以使死四变成死棋(两边都被堵)，或者活三变成死三
// 为了不使分数重叠，[i+1][j]    >= 4*[i][j] +1  ，[i][j+1]    >= 4*[i+1][j] +1   这里采取向最高位取整，即9取整为10,     81取整为100
// 黑方的负分要更大，这样多一个黑方会大大拉低总分，就使白棋去堵黑棋     这里黑棋的负分为白棋的10倍,    当然，五子相连时白方分数要大，不然总是去堵黑方，自己不赢

/*
白方活三>黑方死四
所以当白方有活二，黑方有死三时，应该填活三位置，
这样即使黑方填成了死四，那么也只需要一步即可拦住
*/


void    GetMaxValuePos(POINT &pt,  const  int iPieceScore[][ROWS])
{     
    pt.x   = 0;
    pt.y  = 0;
    for(int i=0;  i<ROWS;  i++)
    {
        for(int j=0;  j<ROWS;  j++)
        {
            if(iPieceScore[pt.x][pt.y]<iPieceScore[i][j]) 
            {
                pt.x=i;
                pt.y=j;
            }
        }
    }
}

void    GetMaxValuePosEx(POINT &pt ,  const  int iPieceScore[][COLUMNS][ROWS])
{  
    POINT   ptBlack;

    GetMaxValuePos(ptBlack,  iPieceScore[0]);     // 获取黑方最好的位置
    GetMaxValuePos(pt,       iPieceScore[1]);     // 获取白方最好的位置

    if(iPieceScore[0][ptBlack.x][ptBlack.y] >=iPieceScore[1][pt.x][pt.y]) 
    {
        pt  = ptBlack;
    }    
}

long double    GetScore(UINT uiCol,    UINT  uiRow,  const  enumChessColor emChess[][ROWS],  BOOL   bIgnoreBlank   = FALSE)   
{    
    int iSameColor[MAXCREASE];
    int iBlankFlag[MAXCREASE]   = {0};                         

    GetSameColor(uiCol, uiRow,  emChess[uiCol][uiRow],   iSameColor,    emChess, NULL,    bIgnoreBlank,   iBlankFlag);
     
    long double dbScore    = 0;
    for(int m = 0;  m < MAXCREASE;	m++)
    {
        int i   = iBlankFlag[m];
        int j   = iSameColor[m] - 1;
        
        if(WHITE  == emChess[uiCol][uiRow])
        {
            dbScore  += SCORE_WHITE[i][j];     
        }
        else
        {
            dbScore  += SCORE_BLACK[i][j];     
        }
    }
    
    return  dbScore;
}

long double ScoreEvaluate(const  enumChessColor emChess[][ROWS],  BOOL   bIgnoreBlank   = FALSE)
{
    // 目前棋局的评分  白方为机器  ，所以白方评分为正，黑方为负
    // 评所有棋子加起来的分
    long double dbScoreSum   = 0;
    for(int i=0;  i<ROWS;  i++)
    {
        for(int j=0;  j<ROWS;  j++)
        {
            if(NONE !=  emChess[i][j])   
            {                   
                dbScoreSum   += GetScore(i,j, emChess,    bIgnoreBlank);
            }
        }
    }

    return  dbScoreSum;
}

// 为提高速度，记录每一次搜索的估值结果，若有则从记录中搜索，若无，则添加到记录中  // 即置换表
// AlphaBeta(3, -INFINITE,  INFINITE,   pt)     在pt点搜索3层

// 白棋记分为正，黑棋为负
// 所以轮到白棋时，找总分最大的点
// 轮到黑棋时，找总分最小的点

// alphaBetaMax总是找评分最大的，所以适合白方  假设5子相连时，若白色赢，则为10000，若黑色赢则-10000
// 那么alphaBetaMax(-10000,   10000,  3); 返回的便是10000      若无五子相连，则返回尽可能大的值
// alphaBetaMin则返回尽可能小的值
//

long double AlphaBetaMin( long double dbAlphaMin, long double dbBetaMax, int iDepthleft ,  enumChessColor emChess[][ROWS],   POINT& ptBest) ;
long double AlphaBetaMax( long double dbAlphaMin, long double dbBetaMax, int iDepthleft ,  enumChessColor emChess[][ROWS],   POINT& ptBest) 
{
    ptBest  = CPoint(-1, -1);       // 如果函数调用完后，ptBest还是CPoint(-1, -1)，说明棋子下满了
    if ( iDepthleft == 0 ) 
    {
        return ScoreEvaluate(emChess); 
    }
    CPoint  ptTmp;      // 做为一个参数传递，不要关心

    for(int i=0;  i<ROWS;  i++)
    {
        for(int j=0;  j<ROWS;  j++)
        {
            if(NONE ==  emChess[i][j])
            {
                if(ptBest.x  == -1)   
                {
                    ptBest  = CPoint(i, j); // 如果是第一次进入
                }

                emChess[i][j] = WHITE ;     // 走一步棋

                long double   dbScore = AlphaBetaMin( dbAlphaMin, dbBetaMax, iDepthleft - 1,   emChess,   ptTmp);   // 找出Min走棋后能找到的最大分数
                             
                emChess[i][j] = NONE;       // 撤销上面走的那一步棋

                if( dbScore >= dbBetaMax )      
                {
                    ptBest  = CPoint(i, j);
                    return dbBetaMax;       // dbBetaMax-cutoff
//                    return dbScore;  
                }
                if( dbScore > dbAlphaMin )
                {
                    ptBest  = CPoint(i, j);
                    dbAlphaMin = dbScore;   // dbAlphaMin acts like max in MiniMax
                }
            }
        }
    }
    return dbAlphaMin;    //Max
}

long double AlphaBetaMin( long double dbAlphaMin, long double dbBetaMax, int iDepthleft ,  enumChessColor emChess[][ROWS],   POINT& ptBest) 
{
    ptBest  = CPoint(-1, -1);
    if ( iDepthleft == 0 ) 
    {
        //  当AlphaBetaMax()中    iDepthleft为2时，加不加负号都差不多
        // 当iDepthleft为3时，加负号能很快剪枝得出结果，但结果并不正确，所以这里不加负号
//     return -ScoreEvaluate(emChess);       
        return ScoreEvaluate(emChess);         
    }

    for(int i=0;  i<ROWS;  i++)
    {
        for(int j=0;  j<ROWS;  j++)
        {
            if(NONE ==  emChess[i][j])
            {
                emChess[i][j] = BLACK ;     // 走一步棋

                long double   dbScore = AlphaBetaMax( dbAlphaMin, dbBetaMax, iDepthleft - 1,   emChess,   ptBest); // 找出Max走棋后能找到的最小分数

                emChess[i][j] = NONE;       // 撤销上面走的那一步棋
               
                if( dbScore <= dbAlphaMin )  
                {   
                    ptBest  = CPoint(i, j);
                    return dbAlphaMin;      // dbAlphaMin-cutoff
   //                 return dbScore;          
                }
                if( dbScore < dbBetaMax )
                {
                    ptBest  = CPoint(i, j);
                    dbBetaMax = dbScore;    // dbBetaMax acts like min in MiniMax
                }
            }
        }
    }
    return dbBetaMax; // Min
}




// ------------------------------------------------------------ AI  Function ------------------------------------------------------------

// 当黑方有三连及以上时便阻碍，否则增加自己的
// ptPosBlack	        黑方当前下子的位置
// ptPosWhite           返回自己应下子的位置
// emEnemyChessColor	敌方的标志	WHITE,  BLACK
BOOL    AIFoolish (POINT&	 ptPosWhite,	POINT ptPosBlack,	enumChessColor   emEnemyChessColor,  const  enumChessColor emChess[][ROWS])
{
    int     piCreaseDirect[MAXCREASE] = {0};
    int	    iSameColor[MAXCREASE];
	UINT	uiCol = ptPosBlack.x,	uiRow = ptPosBlack.y;	

    GetSameColor(uiCol, uiRow,  emEnemyChessColor,   iSameColor,   emChess,  piCreaseDirect);

    int iSameMaxValue  = iSameColor[0],    iSameMaxIndex  = 0;
    for(int m = 1;	    m < 4;		m++)
    {
        if(iSameColor[m] > iSameMaxValue)
        {
            iSameMaxValue   = iSameColor[m];
            iSameMaxIndex   = m;
        }
    }

    if(iSameColor[iSameMaxIndex] > 0)
    {
        switch(iSameMaxIndex)
        {
        case	0:
            ptPosWhite.y	= ptPosBlack.y;
            ptPosWhite.x	= ptPosBlack.x	+ piCreaseDirect[iSameMaxIndex] - iSameColor[iSameMaxIndex];	    // 堵左方

            if(! PtInBroad(ptPosWhite.x,  ptPosWhite.y)   ||	(NONE != emChess[ptPosWhite.x][ptPosWhite.y]))    // 如果左方已有子          
            {
                ptPosWhite.x	= ptPosBlack.x	+ piCreaseDirect[iSameMaxIndex]	+ 1;	    // 堵右方                 
            }	
            break;
        case	1:
            ptPosWhite.x	= ptPosBlack.x;
            ptPosWhite.y	= ptPosBlack.y	+ piCreaseDirect[iSameMaxIndex] - iSameColor[iSameMaxIndex];	    // 堵上方
	
            if(! PtInBroad(ptPosWhite.x,  ptPosWhite.y)    ||	(NONE != emChess[ptPosWhite.x][ptPosWhite.y]))                  
            {
                ptPosWhite.y	= ptPosBlack.y	+ piCreaseDirect[iSameMaxIndex]	+ 1;	    // 堵下方
            }		
            break;
        case	2:
           ptPosWhite.x	= ptPosBlack.x	+ piCreaseDirect[iSameMaxIndex] - iSameColor[iSameMaxIndex];
            ptPosWhite.y= ptPosBlack.y	+ piCreaseDirect[iSameMaxIndex] - iSameColor[iSameMaxIndex];	    // 堵左上方

            if(! PtInBroad(ptPosWhite.x,  ptPosWhite.y)   ||	(NONE != emChess[ptPosWhite.x][ptPosWhite.y]))                  
            {
                ptPosWhite.x	= ptPosBlack.x	+ piCreaseDirect[iSameMaxIndex]	+ 1;
                ptPosWhite.y	= ptPosBlack.y	+ piCreaseDirect[iSameMaxIndex]	+ 1;		// 堵右下方
            }		
            break;
        case	3:
            ptPosWhite.x	= ptPosBlack.x	+ piCreaseDirect[iSameMaxIndex] - iSameColor[iSameMaxIndex];
            ptPosWhite.y	= ptPosBlack.y	- piCreaseDirect[iSameMaxIndex] + iSameColor[iSameMaxIndex];		// 堵左下方

            if(! PtInBroad(ptPosWhite.x,  ptPosWhite.y)  ||	(NONE != emChess[ptPosWhite.x][ptPosWhite.y]))                  
            {
                ptPosWhite.x	= ptPosBlack.x	+ piCreaseDirect[iSameMaxIndex]	+ 1;
                ptPosWhite.y	= ptPosBlack.y	- piCreaseDirect[iSameMaxIndex]	- 1;	    // 堵右上方
            }		
            break;
        default:
            break;
        }

        // 还要判断这点是否为空，如果不为空，继续找其他点
        if(PtInBroad(ptPosWhite.x,  ptPosWhite.y)  &&	(NONE == emChess[ptPosWhite.x][ptPosWhite.y]))
        {
            return	TRUE;
        }
    }   // if > 0

    // 如果没找到合适位置，则随意找一个
    for(int	m = 0;	m < COLUMNS;	m++)
    {
        for(int	n = 0;	n < ROWS;	n++)
            if(NONE == emChess[m][n])
            {
                ptPosWhite.x	= m;
                ptPosWhite.y	= n;	
                return	TRUE;
            }
    }

	return	FALSE;		// 说明棋子满了
}

// 不预测
// 对棋局进行评分，找到分值最大的位置    
BOOL    AIPrimary (POINT &pt ,  enumChessColor emChess[][ROWS])
{
    AlphaBetaMax(-1e10, 1e11, 1, emChess, pt);

    if(pt.x  == -1)   
    {
        return  FALSE;  // 棋子下满了
    }
    return  TRUE;
}

// 预测一步
// 对棋局进行评分，找到分值最大的位置    分值设置和AIPrimary不一样，评分会影响机器的棋力
BOOL  AIMiddle(POINT &pt ,  const  enumChessColor emChess[][ROWS])
{   
    BOOL    bIsBoardFull    = TRUE;
    const   int PLAYER_NUM  = 2;
    int iPieceScore[PLAYER_NUM][COLUMNS][ROWS]={0};     // 记录棋型分数
    int iArraySameColor[MAXCREASE];
    int iBlankFlag[MAXCREASE];                                   

    // 评分可以影响智力，    如把下面的第一组换到最后一组，则智力降低很多
    const   int SCORE_BLACK[][4]   =   {   {0, 0,	0,	50000} ,   {2,   10,    110,	    50000},         {21, 81,	2000,	50000}     };
    const   int SCORE_WHITE[][4]   =   {   {0, 0,	0,	10000}  ,  {1,   9,	    100,	    10000},         {20, 80,	400	,	10000}     };

    int i,j,    iPlayer, iScore=0;   
    for(iPlayer=0;  iPlayer<PLAYER_NUM;     iPlayer++)
    {
        for(i=0;  i<COLUMNS;  i++)
            for(j=0;  j<ROWS;  j++)
            { 
                if(emChess[i][j]==0)     // AI里只计算空位，非空位分数都为0
                {
                    GetSameColor(i, j,  iPlayer   ?   BLACK   :   WHITE,   iArraySameColor,    emChess, NULL, TRUE,   iBlankFlag);

                    for(int iDirect = 0;    iDirect < MAXCREASE;    iDirect++)
                    {                                            
                        for(int iBlank = 2;  iBlank >= 0;  iBlank--)
                            if(iBlankFlag[iDirect]    ==  iBlank)                 //  在   iDirect方向上两边的空位数
                            {
                                for(int iSameColor = 4;  iSameColor > 0;  iSameColor--)
                                    if(iArraySameColor[iDirect] ==iSameColor)     //  在   iDirect方向上相同颜色的棋子数           
                                    {
                                        if(iPlayer)    //记录黑棋分数
                                        {
                                            iScore+=SCORE_BLACK[iBlank][iSameColor-1];      
                                        }
                                        else           //记录白棋分数    
                                        {
                                            iScore+=SCORE_WHITE[iBlank][iSameColor-1];
                                        }
                                    }
                            }
                    }// for iDirect<4

                    iPieceScore[iPlayer][i][j]  = iScore;           // 因为iPosPiece之后不再用，所以用于给黑、白方记录棋型分数
                    iScore=0;                                     
                    bIsBoardFull    = FALSE;
                }//  if(emChess[i][j]==0)  
            }// for j<ROWS
    }
    GetMaxValuePosEx(pt, iPieceScore);
    
    if(bIsBoardFull)
    {
        return  FALSE; // 棋子下满了
    }
    return  TRUE;
}
    
// 预测2步棋型
BOOL    AIHigh    (POINT &pt ,  enumChessColor emChess[][ROWS])
{
//    AlphaBetaMax(-1e9, 1e10, 2, emChess, pt);
//    AlphaBetaMax(-1e11, INFINITE, 2, emChess, pt);
    AlphaBetaMax(-10000000, 100000000, 2, emChess, pt);
    
    if(pt.x  == -1)   
    {
        return  FALSE;  // 棋子下满了
    }
    return  TRUE;
}
