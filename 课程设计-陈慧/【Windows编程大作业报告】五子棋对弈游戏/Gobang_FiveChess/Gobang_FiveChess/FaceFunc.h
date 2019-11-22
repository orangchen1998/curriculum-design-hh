#pragma once

// 将指定区域填充成渐变颜色	    //下面三种方法，GradientFillRect最快
void FillGradientEx(CDC *pDC, CRect rect, COLORREF colorStart, COLORREF colorFinish, BOOL bHorz = TRUE);
void HorizontalGradient(CDC	*pDC, LPRECT lpRect, COLORREF clrStart,  COLORREF clrEnd);
void GradientFillRect(CDC	*pDC, CRect &rcFill, COLORREF crLeftTop, COLORREF crRightBottom, BOOL bVertical = FALSE,	BOOL bTran = FALSE);	//bTran是否透明
