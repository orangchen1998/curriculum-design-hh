#include "stdafx.h"
#include "FaceFunc.h"

//»­Æå×Ó
void FillGradientEx(CDC      *pDC,
                                CRect    rcDraw,
                                COLORREF clrStart,
                                COLORREF clrFinish,
                                BOOL     bHorz)
{
    if ( pDC == NULL )
    {
        return;
    }

    int nShift = 8;
    int nSteps = 1 << nShift;

    for (int i = 0; i < nSteps; i++)
    {
        BYTE   bR = (BYTE) ((GetRValue(clrStart) * (nSteps - i) + GetRValue(clrFinish) * i) >> nShift);
        BYTE   bG = (BYTE) ((GetGValue(clrStart) * (nSteps - i) + GetGValue(clrFinish) * i) >> nShift);
        BYTE   bB = (BYTE) ((GetBValue(clrStart) * (nSteps - i) + GetBValue(clrFinish) * i) >> nShift);
        CBrush br (RGB(bR, bG, bB));
        CRect  r2 = rcDraw;

        if ( bHorz )
        {
            r2.bottom = rcDraw.bottom - ((i * rcDraw.Height()) >> nShift);
            r2.top    = rcDraw.bottom - (((i + 1) * rcDraw.Height()) >> nShift);
            if ( r2.Height() > 0 )
            {
                pDC->FillRect(r2, &br);
            }
        }
        else
        {
            r2.left = rcDraw.left + ((i * rcDraw.Width()) >> nShift);
            r2.right = rcDraw.left + (((i + 1) * rcDraw.Width()) >> nShift);
            if ( r2.Width() > 0 )
            {
                pDC->FillRect(r2, &br);
            }
        }
    }
}

void HorizontalGradient(CDC	*pDC, LPRECT lpRect, COLORREF clrStart, COLORREF clrEnd)
{
    if ( pDC == NULL )
    {
        return;
    }

	// Gradient params
	int width = lpRect->right - lpRect->left - 1;

	// Draw gradient
	double percent;
	unsigned char red, green, blue;
	COLORREF color;
	RECT rect;
	for (int i=0; i<width-1; i++)
	{
		// Gradient color percent
		percent = 1 - (double)i / (double)(width-2);

		// Gradient color
		red = (unsigned char)(GetRValue(clrStart)*percent) + (unsigned char)(GetRValue(clrEnd)*(1-percent));
		green = (unsigned char)(GetGValue(clrStart)*percent) + (unsigned char)(GetGValue(clrEnd)*(1-percent));
		blue = (unsigned char)(GetBValue(clrStart)*percent) + (unsigned char)(GetBValue(clrEnd)*(1-percent));

		color = RGB(red, green, blue);

		// Gradient
		rect.left = lpRect->left + i + 1;
		rect.top = lpRect->top + 1;
		rect.right = rect.left + 1;
		rect.bottom = lpRect->bottom - 1;

        CBrush br (color);
		pDC->FillRect(&rect, &br);
	}
}

void GradientFillRect(CDC	*pDC, CRect &rcFill, COLORREF crLeftTop, COLORREF crRightBottom, BOOL bVertical,BOOL bTran)
{
    ULONG nMode = GRADIENT_FILL_RECT_H;
    if (bVertical)
        nMode = GRADIENT_FILL_RECT_V;

    COLOR16 r = (COLOR16) ((crLeftTop & 0x000000FF)<<8);
    COLOR16 g = (COLOR16) (crLeftTop & 0x0000FF00);
    COLOR16 b = (COLOR16) ((crLeftTop & 0x00FF0000)>>8);

    TRIVERTEX        vert[2] ;
    GRADIENT_RECT    gRect;
    vert [0] .x      = rcFill.left;
    vert [0] .y      = rcFill.top;
    vert [0] .Red    = r;
    vert [0] .Green  = g;
    vert [0] .Blue   = b;
    vert [0] .Alpha  = 0x0000;

    r = (COLOR16) ((crRightBottom & 0x000000FF)<<8);
    g = (COLOR16) (crRightBottom & 0x0000FF00);
    b = (COLOR16) ((crRightBottom & 0x00FF0000)>>8);

    vert [1] .x      = rcFill.right;
    vert [1] .y      = rcFill.bottom;
    vert [1] .Red    = r;
    vert [1] .Green  = g;
    vert [1] .Blue   = b;
	vert [1] .Alpha  = 0x0000;
//  vert [1] .Alpha  = 0x0100;

    gRect.UpperLeft  = 0;
    gRect.LowerRight = 1;
    
    pDC->GradientFill(vert, 2, &gRect,1, nMode);

	//if(bTran)
	//{
		BLENDFUNCTION bfGra;
		bfGra.BlendOp=AC_SRC_OVER;
		bfGra.BlendFlags=0;
		bfGra.AlphaFormat=AC_SRC_ALPHA;		//0 »ò AC_SRC_ALPHA; // The alpha info is encode for each pixel.
		bfGra.SourceConstantAlpha=50;	//0xFF; // The src constant blend will be fully opaque
		pDC->AlphaBlend(rcFill.left,rcFill.top,rcFill.Width(),rcFill.Height(),pDC,0,0,rcFill.Width(),rcFill.Height(),bfGra);

	//}
	//else
	//{
	//	pDC->BitBlt(rcFill.left,rcFill.top,rcFill.Width(),rcFill.Height(),pDC,0,0,SRCCOPY);
	//}
}
