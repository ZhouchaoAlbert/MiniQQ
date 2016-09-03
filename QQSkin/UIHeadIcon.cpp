#include "stdafx.h"
#include <atlimage.h>
#include "UIHeadIcon.h"

namespace DuiLib{

CHeadIconUI::CHeadIconUI()
{
    m_nCircleBorderWidth = 0;
}

CHeadIconUI::~CHeadIconUI()
{
}

LPCTSTR CHeadIconUI::GetClass() const
{
    return _T("HeadIconUI");
}

LPVOID CHeadIconUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, _T("HeadIconUI")) == 0) return static_cast<CHeadIconUI*>(this);
    return __super::GetInterface(pstrName);
}

void CHeadIconUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    if (_tcscmp(pstrName, _T("circleborderwidth")) == 0) SetCircleBorderWidth(_ttoi(pstrValue));
    else __super::SetAttribute(pstrName, pstrValue);
}

void CHeadIconUI::PaintBkImage(HDC hDC)
{
    __super::PaintBkImage(hDC);

	UINT32 iW = m_rcItem.right - m_rcItem.left;
	UINT32 iH = m_rcItem.bottom - m_rcItem.top;
	UINT32 nWidth	= iW - 2 * m_nCircleBorderWidth;
	UINT32 nHeight  = iH - 2 * m_nCircleBorderWidth;

	HBITMAP hBmp = ::CreateCompatibleBitmap(hDC, nWidth, nHeight);
	HDC hMemDC = CreateCompatibleDC(hDC);
	HBITMAP hOriBmp = (HBITMAP)SelectObject(hMemDC, hBmp);
	RECT rcBmp = { 0, 0, nWidth, nHeight };
	::SetBkColor(hMemDC, RGB(255, 255, 255));
	::ExtTextOut(hMemDC, 0, 0, ETO_OPAQUE, &rcBmp, NULL, 0, NULL);
	CRenderEngine::DrawImageString(hMemDC, GetManager(), rcBmp, rcBmp, m_strUserIcon);
	Gdiplus::Bitmap bitmap(hBmp, NULL);
	SelectObject(hMemDC, hOriBmp);
	DeleteDC(hMemDC);

	//创建圆形图片
	Gdiplus::Bitmap bmpRound(nWidth, nHeight);
	Gdiplus::Graphics g2(&bmpRound);
	Gdiplus::RectF	rcDest((Gdiplus::REAL)0, (Gdiplus::REAL)0, (Gdiplus::REAL)nWidth, (Gdiplus::REAL)nHeight);
	Gdiplus::TextureBrush brush(&bitmap, rcDest);
	g2.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	g2.FillEllipse(&brush, 0, 0, nWidth - 1, nHeight - 1);

	DeleteObject(hBmp);

	//把圆形图片贴到窗口上
	Gdiplus::Graphics	g(hDC);
	UINT32 nX = m_rcItem.left + m_nCircleBorderWidth;
	UINT32 nY = m_rcItem.top + m_nCircleBorderWidth;
#if 0
	Gdiplus::CachedBitmap cachedBitmap(&bmpRound, &g);
	g.DrawCachedBitmap(&cachedBitmap, nX, nY);
#else
	g.DrawImage(&bmpRound, Gdiplus::Rect(nX, nY, nWidth, nHeight));
#endif
}

void CHeadIconUI::SetCircleBorderWidth(UINT nWidth)
{
    m_nCircleBorderWidth = nWidth;
    Invalidate();
}

void CHeadIconUI::SetHeadIcon(LPCTSTR szIcon)
{
	m_strUserIcon = szIcon;
    Invalidate();
}

}
