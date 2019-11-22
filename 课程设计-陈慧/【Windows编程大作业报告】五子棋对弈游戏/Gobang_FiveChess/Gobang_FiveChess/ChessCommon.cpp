#include "StdAfx.h"
#include "ChessCommon.h"


// 位置(uiCol,   uiRow)   颜色为emChessColor 的个数
// emChessColor     棋子的颜色
// bResetCount      是否清零计数
// iColorCoun       相同颜色的数目
// bOver            是否停止计数
// piBlankCount     空位的状况
// emChess          整个棋局的信息
void     SameColorAdder(UINT	uiCol,	UINT	uiRow,	enumChessColor   emChessColor,    BOOL    bResetCount ,int &iColorCount,   BOOL&    bOver,   
                        const enumChessColor emChess[][ROWS],  int* piBlankCount )
{
	// 当m_iPositionPiece越界时，VS不会让其崩溃，而是跳过，所以不处理越界也可正确执行
	// 但安全的做法必须处理越界
    if(PtInBroad(uiCol, uiRow))       // 边界判断
    {
        if(emChessColor	== emChess[uiCol][uiRow])
        {
            iColorCount++;
        }
        else   
        {
            if(bResetCount)
            {
                iColorCount = 0;
            }
            else
            {
                bOver   = TRUE;
            }
              
            // 空位计数
            if(piBlankCount)
            {
                if(0 == emChess[uiCol][uiRow])
                {
                    if((0 == (*piBlankCount))  ||  bOver)
                    {
                        (*piBlankCount)++;
                    }
                }
                else if(! bOver)
                {
                    (*piBlankCount)=0;
                }
            }
        }
    }  
}

// 位置(uiCol,   uiRow)   各个方向上相同颜色的棋子数及空位状况
// emChessColor     棋子的颜色
// iSameColor       各个方向上相同颜色的棋子数
// piCreaseDirect   各个方向的iCrease增量
// bIgnoreBlank     是否忽略空格
// piBlankFlag      各个方向上空位的状况
// emChess          整个棋局的信息
void     GetSameColor(UINT	uiCol,	UINT	uiRow,	enumChessColor   emChessColor,    int   *  iSameColor,  
                      const enumChessColor emChess[][ROWS] ,    int*    piCreaseDirect,  BOOL   bIgnoreBlank ,    int*    piBlankFlag)
{    	     
    int	iColTemp,	iRowTemp,	iCrease;
    BOOL    bBreak  =FALSE;

    memset(iSameColor,	 0,	 sizeof(iSameColor) * MAXCREASE);
    if(piCreaseDirect)
    {
        memset(piCreaseDirect,0, sizeof(piCreaseDirect) * MAXCREASE);
    }
    if(piBlankFlag)
    {
        memset(piBlankFlag,	  0, sizeof(piBlankFlag)    * MAXCREASE);
    }

    for(int iDirect = 0;    iDirect < 4;    iDirect++,  bBreak  =FALSE)
    {          
        int iBlankCount = 0;//  0 两边都无空位     1只有一边有空位    2两边都有空位

        for(iCrease = -MAXCREASE;	! bBreak  &&  (iCrease <= MAXCREASE);	iCrease++)
        {
            if(bIgnoreBlank    &&  ! iCrease)    // 因为AI中原位置为空，所以不要计数
            {
                continue;   //  或者  iCrease++;
            }
            switch(iDirect)
            {
            case    0:      // 判断横向
                iColTemp     = uiCol + iCrease;
                iRowTemp   = uiRow;
                break;
            case    1:		// 判断竖向
                iColTemp     = uiCol;
                iRowTemp   = uiRow + iCrease;
                break;
            case    2:		// 判断反斜向【\】
                iColTemp     = uiCol + iCrease;
                iRowTemp   = uiRow + iCrease;
                break;
            case    3:      // 判断斜向【/】
                iColTemp     = uiCol + iCrease;
                iRowTemp   = uiRow - iCrease;
                break;
            default:
                break;
            }    
            if(PtInBroad(iColTemp, iRowTemp)) 
            {           
                SameColorAdder(iColTemp, iRowTemp,   emChessColor,   iCrease <= 0,   iSameColor[iDirect], bBreak,    emChess, &iBlankCount);     
            }
            else    if(iCrease > 0)
            {
                iCrease+=1;
                break;
            }
        } //for iCrease 
      
        if(piCreaseDirect)
        {
            piCreaseDirect [iDirect] = iCrease   - 2;
        }

        if(piBlankFlag)
        {
            piBlankFlag[iDirect]     = iBlankCount;
        }
    } //for  iDirect < 4 
}

BOOL    PtInBroad(UINT	uiCol,	UINT	uiRow)
{
    if((uiCol >= 0)	&&	(uiCol < COLUMNS)	&&	(uiRow >= 0)	&&	(uiRow < ROWS))	
    {
        return	TRUE;
    }
    return	FALSE;
}