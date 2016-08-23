#include "StdAfx.h"

namespace DuiLib {

CControlUI::CControlUI() : 
m_pManager(NULL), 
m_pParent(NULL), 
m_bUpdateNeeded(true),
m_bMenuUsed(false),
m_bVisible(true), 
m_bInternVisible(true),
m_bFocused(false),
m_bEnabled(true),
m_bMouseEnabled(true),
m_bKeyboardEnabled(true),
m_bFloat(false),
m_bSetPos(false),
m_chShortcut('\0'),
m_pTag(NULL),
m_dwBackColor(0),
m_dwBackColor2(0),
m_dwBackColor3(0),
m_dwBorderColor(0),
m_dwFocusBorderColor(0),
m_bColorHSL(false),
m_nBorderSize(0),
m_nBorderStyle(PS_SOLID),
m_nTooltipWidth(300)
{
	m_bEnabledEffect = false;
	m_tCurEffects.m_bEnableEffect = false;
	m_tCurEffects.m_iZoom = -1;
	m_tCurEffects.m_dFillingBK = 0xffffffff;
	m_tCurEffects.m_iOffectX = 0;
	m_tCurEffects.m_iOffectY = 0;
	m_tCurEffects.m_iAlpha = -255;
	m_tCurEffects.m_fRotation = 0.0;
	m_tCurEffects.m_iNeedTimer = 350;

	memcpy(&m_tMouseInEffects, &m_tCurEffects, sizeof(TEffectAge));
	memcpy(&m_tMouseOutEffects, &m_tCurEffects, sizeof(TEffectAge));
	memcpy(&m_tMouseClickEffects, &m_tCurEffects, sizeof(TEffectAge));

    m_cXY.cx = m_cXY.cy = 0;
    m_cxyFixed.cx = m_cxyFixed.cy = 0;
    m_cxyMin.cx = m_cxyMin.cy = 0;
    m_cxyMax.cx = m_cxyMax.cy = 9999;
    m_cxyBorderRound.cx = m_cxyBorderRound.cy = 0;

    ::ZeroMemory(&m_rcPadding, sizeof(RECT));
    ::ZeroMemory(&m_rcItem, sizeof(RECT));
    ::ZeroMemory(&m_rcPaint, sizeof(RECT));
	::ZeroMemory(&m_rcBorderSize,sizeof(RECT));
    ::ZeroMemory(&m_tRelativePos, sizeof(TRelativePosUI));
}

CControlUI::~CControlUI()
{
    if( OnDestroy ) OnDestroy(this);
    if( m_pManager != NULL ) m_pManager->ReapObjects(this);
}

CDuiString CControlUI::GetName() const
{
    return m_sName;
}

void CControlUI::SetName(LPCTSTR pstrName)
{
    m_sName = pstrName;
}

LPVOID CControlUI::GetInterface(LPCTSTR pstrName)
{
    if( _tcscmp(pstrName, DUI_CTR_CONTROL) == 0 ) return this;
    return NULL;
}

LPCTSTR CControlUI::GetClass() const
{
    return _T("ControlUI");
}

UINT CControlUI::GetControlFlags() const
{
    return 0;
}

bool CControlUI::Activate()
{
    if( !IsVisible() ) return false;
    if( !IsEnabled() ) return false;
    return true;
}

CPaintManagerUI* CControlUI::GetManager() const
{
    return m_pManager;
}

void CControlUI::SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit)
{
    m_pManager = pManager;
    m_pParent = pParent;
    if( bInit && m_pParent ) Init();
}

CControlUI* CControlUI::GetParent() const
{
    return m_pParent;
}

CDuiString CControlUI::GetText() const
{
    return m_sText;
}

void CControlUI::SetText(LPCTSTR pstrText)
{
    if( m_sText == pstrText ) return;

    m_sText = pstrText;
    Invalidate();
}

DWORD CControlUI::GetBkColor() const
{
    return m_dwBackColor;
}

void CControlUI::SetBkColor(DWORD dwBackColor)
{
    if( m_dwBackColor == dwBackColor ) return;

    m_dwBackColor = dwBackColor;
    Invalidate();
}

DWORD CControlUI::GetBkColor2() const
{
    return m_dwBackColor2;
}

void CControlUI::SetBkColor2(DWORD dwBackColor)
{
    if( m_dwBackColor2 == dwBackColor ) return;

    m_dwBackColor2 = dwBackColor;
    Invalidate();
}

DWORD CControlUI::GetBkColor3() const
{
    return m_dwBackColor3;
}

void CControlUI::SetBkColor3(DWORD dwBackColor)
{
    if( m_dwBackColor3 == dwBackColor ) return;

    m_dwBackColor3 = dwBackColor;
    Invalidate();
}

LPCTSTR CControlUI::GetBkImage()
{
    return m_sBkImage;
}

void CControlUI::SetBkImage(LPCTSTR pStrImage)
{
    if( m_sBkImage == pStrImage ) return;

    m_sBkImage = pStrImage;
    Invalidate();
}

DWORD CControlUI::GetBorderColor() const
{
    return m_dwBorderColor;
}

void CControlUI::SetBorderColor(DWORD dwBorderColor)
{
    if( m_dwBorderColor == dwBorderColor ) return;

    m_dwBorderColor = dwBorderColor;
    Invalidate();
}

DWORD CControlUI::GetFocusBorderColor() const
{
    return m_dwFocusBorderColor;
}

void CControlUI::SetFocusBorderColor(DWORD dwBorderColor)
{
    if( m_dwFocusBorderColor == dwBorderColor ) return;

    m_dwFocusBorderColor = dwBorderColor;
    Invalidate();
}

bool CControlUI::IsColorHSL() const
{
    return m_bColorHSL;
}

void CControlUI::SetColorHSL(bool bColorHSL)
{
    if( m_bColorHSL == bColorHSL ) return;

    m_bColorHSL = bColorHSL;
    Invalidate();
}

int CControlUI::GetBorderSize() const
{
    return m_nBorderSize;
}

void CControlUI::SetBorderSize(int nSize)
{
    if( m_nBorderSize == nSize ) return;

    m_nBorderSize = nSize;
    Invalidate();
}

void CControlUI::SetBorderSize( RECT rc )
{
	m_rcBorderSize = rc;
	Invalidate();
}

SIZE CControlUI::GetBorderRound() const
{
    return m_cxyBorderRound;
}

void CControlUI::SetBorderRound(SIZE cxyRound)
{
    m_cxyBorderRound = cxyRound;
    Invalidate();
}

bool CControlUI::DrawImage(HDC hDC, LPCTSTR pStrImage, LPCTSTR pStrModify)
{
    return CRenderEngine::DrawImageString(hDC, m_pManager, m_rcItem, m_rcPaint, pStrImage, pStrModify);
}

const RECT& CControlUI::GetPos() const
{
    return m_rcItem;
}

void CControlUI::SetPos(RECT rc)
{
    if( rc.right < rc.left ) rc.right = rc.left;
    if( rc.bottom < rc.top ) rc.bottom = rc.top;

    CDuiRect invalidateRc = m_rcItem;
    if( ::IsRectEmpty(&invalidateRc) ) invalidateRc = rc;

    m_rcItem = rc;
    if( m_pManager == NULL ) return;

    if( !m_bSetPos ) {
        m_bSetPos = true;
        if( OnSize ) OnSize(this);
        m_bSetPos = false;
    }
    
    if( m_bFloat ) {
        CControlUI* pParent = GetParent();
        if( pParent != NULL ) {
            RECT rcParentPos = pParent->GetPos();
            if( m_cXY.cx >= 0 ) m_cXY.cx = m_rcItem.left - rcParentPos.left;
            else m_cXY.cx = m_rcItem.right - rcParentPos.right;
            if( m_cXY.cy >= 0 ) m_cXY.cy = m_rcItem.top - rcParentPos.top;
            else m_cXY.cy = m_rcItem.bottom - rcParentPos.bottom;
            m_cxyFixed.cx = m_rcItem.right - m_rcItem.left;
            m_cxyFixed.cy = m_rcItem.bottom - m_rcItem.top;
        }
    }

    m_bUpdateNeeded = false;
    invalidateRc.Join(m_rcItem);

    CControlUI* pParent = this;
    RECT rcTemp;
    RECT rcParent;
    while( pParent = pParent->GetParent() )
    {
        rcTemp = invalidateRc;
        rcParent = pParent->GetPos();
        if( !::IntersectRect(&invalidateRc, &rcTemp, &rcParent) ) 
        {
            return;
        }
    }
    m_pManager->Invalidate(invalidateRc);
}

int CControlUI::GetWidth() const
{
    return m_rcItem.right - m_rcItem.left;
}

int CControlUI::GetHeight() const
{
    return m_rcItem.bottom - m_rcItem.top;
}

int CControlUI::GetX() const
{
    return m_rcItem.left;
}

int CControlUI::GetY() const
{
    return m_rcItem.top;
}

RECT CControlUI::GetPadding() const
{
    return m_rcPadding;
}

void CControlUI::SetPadding(RECT rcPadding)
{
    m_rcPadding = rcPadding;
    NeedParentUpdate();
}

SIZE CControlUI::GetFixedXY() const
{
    return m_cXY;
}

void CControlUI::SetFixedXY(SIZE szXY)
{
    m_cXY.cx = szXY.cx;
    m_cXY.cy = szXY.cy;
    if( !m_bFloat ) NeedParentUpdate();
    else NeedUpdate();
}

int CControlUI::GetFixedWidth() const
{
    return m_cxyFixed.cx;
}

void CControlUI::SetFixedWidth(int cx)
{
    if( cx < 0 ) return; 
    m_cxyFixed.cx = cx;
    if( !m_bFloat ) NeedParentUpdate();
    else NeedUpdate();
}

int CControlUI::GetFixedHeight() const
{
    return m_cxyFixed.cy;
}

void CControlUI::SetFixedHeight(int cy)
{
    if( cy < 0 ) return; 
    m_cxyFixed.cy = cy;
    if( !m_bFloat ) NeedParentUpdate();
    else NeedUpdate();
}

int CControlUI::GetMinWidth() const
{
    return m_cxyMin.cx;
}

void CControlUI::SetMinWidth(int cx)
{
    if( m_cxyMin.cx == cx ) return;

    if( cx < 0 ) return; 
    m_cxyMin.cx = cx;
    if( !m_bFloat ) NeedParentUpdate();
    else NeedUpdate();
}

int CControlUI::GetMaxWidth() const
{
    return m_cxyMax.cx;
}

void CControlUI::SetMaxWidth(int cx)
{
    if( m_cxyMax.cx == cx ) return;

    if( cx < 0 ) return; 
    m_cxyMax.cx = cx;
    if( !m_bFloat ) NeedParentUpdate();
    else NeedUpdate();
}

int CControlUI::GetMinHeight() const
{
    return m_cxyMin.cy;
}

void CControlUI::SetMinHeight(int cy)
{
    if( m_cxyMin.cy == cy ) return;

    if( cy < 0 ) return; 
    m_cxyMin.cy = cy;
    if( !m_bFloat ) NeedParentUpdate();
    else NeedUpdate();
}

int CControlUI::GetMaxHeight() const
{
    return m_cxyMax.cy;
}

void CControlUI::SetMaxHeight(int cy)
{
    if( m_cxyMax.cy == cy ) return;

    if( cy < 0 ) return; 
    m_cxyMax.cy = cy;
    if( !m_bFloat ) NeedParentUpdate();
    else NeedUpdate();
}

void CControlUI::SetRelativePos(SIZE szMove,SIZE szZoom)
{
    m_tRelativePos.bRelative = TRUE;
    m_tRelativePos.nMoveXPercent = szMove.cx;
    m_tRelativePos.nMoveYPercent = szMove.cy;
    m_tRelativePos.nZoomXPercent = szZoom.cx;
    m_tRelativePos.nZoomYPercent = szZoom.cy;
}

void CControlUI::SetRelativeParentSize(SIZE sz)
{
    m_tRelativePos.szParent = sz;
}

TRelativePosUI CControlUI::GetRelativePos() const
{
    return m_tRelativePos;
}

bool CControlUI::IsRelativePos() const
{
    return m_tRelativePos.bRelative;
}

CDuiString CControlUI::GetToolTip() const
{
    return m_sToolTip;
}

void CControlUI::SetToolTip(LPCTSTR pstrText)
{
	CDuiString strTemp(pstrText);
	strTemp.Replace(_T("<n>"),_T("\r\n"));
	m_sToolTip=strTemp;
}

void CControlUI::SetToolTipWidth( int nWidth )
{
	m_nTooltipWidth=nWidth;
}

int CControlUI::GetToolTipWidth( void )
{
	return m_nTooltipWidth;
}

TCHAR CControlUI::GetShortcut() const
{
    return m_chShortcut;
}

void CControlUI::SetShortcut(TCHAR ch)
{
    m_chShortcut = ch;
}

bool CControlUI::IsContextMenuUsed() const
{
    return m_bMenuUsed;
}

void CControlUI::SetContextMenuUsed(bool bMenuUsed)
{
    m_bMenuUsed = bMenuUsed;
}

const CDuiString& CControlUI::GetUserData()
{
    return m_sUserData;
}

void CControlUI::SetUserData(LPCTSTR pstrText)
{
    m_sUserData = pstrText;
}

UINT_PTR CControlUI::GetTag() const
{
    return m_pTag;
}

void CControlUI::SetTag(UINT_PTR pTag)
{
    m_pTag = pTag;
}

bool CControlUI::IsVisible() const
{

    return m_bVisible && m_bInternVisible;
}

void CControlUI::SetVisible(bool bVisible)
{
    if( m_bVisible == bVisible ) return;

    bool v = IsVisible();
    m_bVisible = bVisible;
    if( m_bFocused ) m_bFocused = false;
	if (!bVisible && m_pManager && m_pManager->GetFocus() == this) {
		m_pManager->SetFocus(NULL) ;
	}
    if( IsVisible() != v ) {
        NeedParentUpdate();
    }
}

void CControlUI::SetInternVisible(bool bVisible)
{
    m_bInternVisible = bVisible;
	if (!bVisible && m_pManager && m_pManager->GetFocus() == this) {
		m_pManager->SetFocus(NULL) ;
	}
}

bool CControlUI::IsEnabled() const
{
    return m_bEnabled;
}

void CControlUI::SetEnabled(bool bEnabled)
{
    if( m_bEnabled == bEnabled ) return;

    m_bEnabled = bEnabled;
    Invalidate();
}

bool CControlUI::IsMouseEnabled() const
{
    return m_bMouseEnabled;
}

void CControlUI::SetMouseEnabled(bool bEnabled)
{
    m_bMouseEnabled = bEnabled;
}

bool CControlUI::IsKeyboardEnabled() const
{
	return m_bKeyboardEnabled ;
}
void CControlUI::SetKeyboardEnabled(bool bEnabled)
{
	m_bKeyboardEnabled = bEnabled ; 
}

bool CControlUI::IsFocused() const
{
    return m_bFocused;
}

void CControlUI::SetFocus()
{
    if( m_pManager != NULL ) m_pManager->SetFocus(this);
}

bool CControlUI::IsFloat() const
{
    return m_bFloat;
}

void CControlUI::SetFloat(bool bFloat)
{
    if( m_bFloat == bFloat ) return;

    m_bFloat = bFloat;
    NeedParentUpdate();
}

CControlUI* CControlUI::FindControl(FINDCONTROLPROC Proc, LPVOID pData, UINT uFlags)
{
    if( (uFlags & UIFIND_VISIBLE) != 0 && !IsVisible() ) return NULL;
    if( (uFlags & UIFIND_ENABLED) != 0 && !IsEnabled() ) return NULL;
	if( (uFlags & UIFIND_HITTEST) != 0 && (!m_bMouseEnabled || !::PtInRect(&m_rcItem, * static_cast<LPPOINT>(pData))) ) return NULL;
    return Proc(this, pData);
}

void CControlUI::Invalidate()
{
    if( !IsVisible() ) return;

    RECT invalidateRc = m_rcItem;

    CControlUI* pParent = this;
    RECT rcTemp;
    RECT rcParent;
    while( pParent = pParent->GetParent() )
    {
        rcTemp = invalidateRc;
        rcParent = pParent->GetPos();
        if( !::IntersectRect(&invalidateRc, &rcTemp, &rcParent) ) 
        {
            return;
        }
    }

    if( m_pManager != NULL ) m_pManager->Invalidate(invalidateRc);
}

bool CControlUI::IsUpdateNeeded() const
{
    return m_bUpdateNeeded;
}

void CControlUI::NeedUpdate()
{
    if( !IsVisible() ) return;
    m_bUpdateNeeded = true;
    Invalidate();

    if( m_pManager != NULL ) m_pManager->NeedUpdate();
}

void CControlUI::NeedParentUpdate()
{
    if( GetParent() ) {
        GetParent()->NeedUpdate();
        GetParent()->Invalidate();
    }
    else {
        NeedUpdate();
    }

    if( m_pManager != NULL ) m_pManager->NeedUpdate();
}

DWORD CControlUI::GetAdjustColor(DWORD dwColor)
{
    if( !m_bColorHSL ) return dwColor;
    short H, S, L;
    CPaintManagerUI::GetHSL(&H, &S, &L);
    return CRenderEngine::AdjustColor(dwColor, H, S, L);
}

void CControlUI::Init()
{
    DoInit();
    if( OnInit ) OnInit(this);
}

void CControlUI::DoInit()
{

}

void CControlUI::Event(TEventUI& event)
{
    if( OnEvent(&event) ) DoEvent(event);
}

void CControlUI::DoEvent(TEventUI& event)
{
    if( event.Type == UIEVENT_SETCURSOR )
    {
        ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
        return;
    }
    if( event.Type == UIEVENT_SETFOCUS ) 
    {
        m_bFocused = true;
        Invalidate();
        return;
    }
    if( event.Type == UIEVENT_KILLFOCUS ) 
    {
        m_bFocused = false;
        Invalidate();
        return;
    }
    if( event.Type == UIEVENT_TIMER )
    {
        m_pManager->SendNotify(this, DUI_MSGTYPE_TIMER, event.wParam, event.lParam);
        return;
    }
    if( event.Type == UIEVENT_CONTEXTMENU )
    {
        if( IsContextMenuUsed() ) {
            m_pManager->SendNotify(this, DUI_MSGTYPE_MENU, event.wParam, event.lParam);
            return;
        }
    }

    if( m_pParent != NULL ) m_pParent->DoEvent(event);
}


void CControlUI::SetVirtualWnd(LPCTSTR pstrValue)
{
	m_sVirtualWnd = pstrValue;
	m_pManager->UsedVirtualWnd(true);
}

CDuiString CControlUI::GetVirtualWnd() const
{
	CDuiString str;
	if( !m_sVirtualWnd.IsEmpty() ){
		str = m_sVirtualWnd;
	}
	else{
		CControlUI* pParent = GetParent();
		if( pParent != NULL){
			str = pParent->GetVirtualWnd();
		}
		else{
			str = _T("");
		}
	}
	return str;
}

void CControlUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    if( _tcscmp(pstrName, _T("pos")) == 0 ) {
        RECT rcPos = { 0 };
        LPTSTR pstr = NULL;
        rcPos.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
        rcPos.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
        rcPos.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
        rcPos.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
        SIZE szXY = {rcPos.left >= 0 ? rcPos.left : rcPos.right, rcPos.top >= 0 ? rcPos.top : rcPos.bottom};
        SetFixedXY(szXY);
        SetFixedWidth(rcPos.right - rcPos.left);
        SetFixedHeight(rcPos.bottom - rcPos.top);
    }
    else if( _tcscmp(pstrName, _T("relativepos")) == 0 ) {
        SIZE szMove,szZoom;
        LPTSTR pstr = NULL;
        szMove.cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
        szMove.cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
        szZoom.cx = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
        szZoom.cy = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr); 
        SetRelativePos(szMove,szZoom);
    }
    else if( _tcscmp(pstrName, _T("padding")) == 0 ) {
        RECT rcPadding = { 0 };
        LPTSTR pstr = NULL;
        rcPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
        rcPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);    
        rcPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);    
        rcPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);    
        SetPadding(rcPadding);
    }
    else if( _tcscmp(pstrName, _T("bkcolor")) == 0 || _tcscmp(pstrName, _T("bkcolor1")) == 0 ) {
        while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
        if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
        LPTSTR pstr = NULL;
        DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
        SetBkColor(clrColor);
    }
    else if( _tcscmp(pstrName, _T("bkcolor2")) == 0 ) {
        while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
        if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
        LPTSTR pstr = NULL;
        DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
        SetBkColor2(clrColor);
    }
    else if( _tcscmp(pstrName, _T("bkcolor3")) == 0 ) {
        while( *pstrValue > _T('\0') && *pstrValue <= _T(' ') ) pstrValue = ::CharNext(pstrValue);
        if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
        LPTSTR pstr = NULL;
        DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
        SetBkColor3(clrColor);
    }
    else if( _tcscmp(pstrName, _T("bordercolor")) == 0 ) {
        if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
        LPTSTR pstr = NULL;
        DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
        SetBorderColor(clrColor);
    }
    else if( _tcscmp(pstrName, _T("focusbordercolor")) == 0 ) {
        if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
        LPTSTR pstr = NULL;
        DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
        SetFocusBorderColor(clrColor);
    }
    else if( _tcscmp(pstrName, _T("colorhsl")) == 0 ) SetColorHSL(_tcscmp(pstrValue, _T("true")) == 0);
	else if( _tcscmp(pstrName, _T("bordersize")) == 0 ) {
		CDuiString nValue = pstrValue;
		if(nValue.Find(',') < 0)
		{
			SetBorderSize(_ttoi(pstrValue));
			RECT rcPadding = {0};
			SetBorderSize(rcPadding);
		}
		else
		{
			RECT rcPadding = { 0 };
			LPTSTR pstr = NULL;
			rcPadding.left = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);
			rcPadding.top = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);
			rcPadding.right = _tcstol(pstr + 1, &pstr, 10);  ASSERT(pstr);
			rcPadding.bottom = _tcstol(pstr + 1, &pstr, 10); ASSERT(pstr);
			SetBorderSize(rcPadding);
		}
	}
	else if( _tcscmp(pstrName, _T("leftbordersize")) == 0 ) SetLeftBorderSize(_ttoi(pstrValue));
	else if( _tcscmp(pstrName, _T("topbordersize")) == 0 ) SetTopBorderSize(_ttoi(pstrValue));
	else if( _tcscmp(pstrName, _T("rightbordersize")) == 0 ) SetRightBorderSize(_ttoi(pstrValue));
	else if( _tcscmp(pstrName, _T("bottombordersize")) == 0 ) SetBottomBorderSize(_ttoi(pstrValue));
	else if( _tcscmp(pstrName, _T("borderstyle")) == 0 ) SetBorderStyle(_ttoi(pstrValue));
    else if( _tcscmp(pstrName, _T("borderround")) == 0 ) {
        SIZE cxyRound = { 0 };
        LPTSTR pstr = NULL;
        cxyRound.cx = _tcstol(pstrValue, &pstr, 10);  ASSERT(pstr);    
        cxyRound.cy = _tcstol(pstr + 1, &pstr, 10);    ASSERT(pstr);     
        SetBorderRound(cxyRound);
    }
    else if( _tcscmp(pstrName, _T("bkimage")) == 0 ) SetBkImage(pstrValue);
    else if( _tcscmp(pstrName, _T("width")) == 0 ) SetFixedWidth(_ttoi(pstrValue));
    else if( _tcscmp(pstrName, _T("height")) == 0 ) SetFixedHeight(_ttoi(pstrValue));
    else if( _tcscmp(pstrName, _T("minwidth")) == 0 ) SetMinWidth(_ttoi(pstrValue));
    else if( _tcscmp(pstrName, _T("minheight")) == 0 ) SetMinHeight(_ttoi(pstrValue));
    else if( _tcscmp(pstrName, _T("maxwidth")) == 0 ) SetMaxWidth(_ttoi(pstrValue));
    else if( _tcscmp(pstrName, _T("maxheight")) == 0 ) SetMaxHeight(_ttoi(pstrValue));
    else if( _tcscmp(pstrName, _T("name")) == 0 ) SetName(pstrValue);
    else if( _tcscmp(pstrName, _T("text")) == 0 ) SetText(pstrValue);
    else if( _tcscmp(pstrName, _T("tooltip")) == 0 ) SetToolTip(pstrValue);
    else if( _tcscmp(pstrName, _T("userdata")) == 0 ) SetUserData(pstrValue);
    else if( _tcscmp(pstrName, _T("enabled")) == 0 ) SetEnabled(_tcscmp(pstrValue, _T("true")) == 0);
    else if( _tcscmp(pstrName, _T("mouse")) == 0 ) SetMouseEnabled(_tcscmp(pstrValue, _T("true")) == 0);
	else if( _tcscmp(pstrName, _T("keyboard")) == 0 ) SetKeyboardEnabled(_tcscmp(pstrValue, _T("true")) == 0);
    else if( _tcscmp(pstrName, _T("visible")) == 0 ) SetVisible(_tcscmp(pstrValue, _T("true")) == 0);
    else if( _tcscmp(pstrName, _T("float")) == 0 ) SetFloat(_tcscmp(pstrValue, _T("true")) == 0);
    else if( _tcscmp(pstrName, _T("shortcut")) == 0 ) SetShortcut(pstrValue[0]);
    else if( _tcscmp(pstrName, _T("menu")) == 0 ) SetContextMenuUsed(_tcscmp(pstrValue, _T("true")) == 0);
	else if (_tcscmp(pstrName, _T("animeffects")) == 0) SetAnimEffects(_tcscmp(pstrValue, _T("true")) == 0);
	else if (_tcscmp(pstrName, _T("adveffects")) == 0) SetEffectsStyle(pstrValue, &m_tCurEffects);
	else if (_tcscmp(pstrName, _T("easyeffects")) == 0) AnyEasyEffectsPorfiles(pstrValue, &m_tCurEffects);
	else if (_tcscmp(pstrName, _T("mouseineffects")) == 0) SetEffectsStyle(pstrValue, &m_tMouseInEffects);
	else if (_tcscmp(pstrName, _T("mouseouteffects")) == 0) SetEffectsStyle(pstrValue, &m_tMouseOutEffects);
	else if (_tcscmp(pstrName, _T("mouseclickeffects")) == 0) SetEffectsStyle(pstrValue, &m_tMouseClickEffects);
	else if (_tcscmp(pstrName, _T("effectstyle")) == 0) SetEffectsStyle(pstrValue, &m_tCurEffects);
	else if (_tcscmp(pstrName, _T("mouseinstyle")) == 0) SetEffectsStyle(pstrValue, &m_tMouseInEffects);
	else if (_tcscmp(pstrName, _T("mouseoutstyle")) == 0) SetEffectsStyle(pstrValue, &m_tMouseOutEffects);
	else if (_tcscmp(pstrName, _T("mouseclickstyle")) == 0) SetEffectsStyle(pstrValue, &m_tMouseClickEffects);
	else if( _tcscmp(pstrName, _T("virtualwnd")) == 0 ) SetVirtualWnd(pstrValue);
}

CControlUI* CControlUI::ApplyAttributeList(LPCTSTR pstrList)
{
    CDuiString sItem;
    CDuiString sValue;
    while( *pstrList != _T('\0') ) {
        sItem.Empty();
        sValue.Empty();
        while( *pstrList != _T('\0') && *pstrList != _T('=') ) {
            LPTSTR pstrTemp = ::CharNext(pstrList);
            while( pstrList < pstrTemp) {
                sItem += *pstrList++;
            }
        }
        ASSERT( *pstrList == _T('=') );
        if( *pstrList++ != _T('=') ) return this;
        ASSERT( *pstrList == _T('\"') );
        if( *pstrList++ != _T('\"') ) return this;
        while( *pstrList != _T('\0') && *pstrList != _T('\"') ) {
            LPTSTR pstrTemp = ::CharNext(pstrList);
            while( pstrList < pstrTemp) {
                sValue += *pstrList++;
            }
        }
        ASSERT( *pstrList == _T('\"') );
        if( *pstrList++ != _T('\"') ) return this;
        SetAttribute(sItem, sValue);
        if( *pstrList++ != _T(' ') ) return this;
    }
    return this;
}

SIZE CControlUI::EstimateSize(SIZE szAvailable)
{
    return m_cxyFixed;
}

void CControlUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
    if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) ) return;

    // ����ѭ�򣺱�����ɫ->����ͼ->״̬ͼ->�ı�->�߿�
    if( m_cxyBorderRound.cx > 0 || m_cxyBorderRound.cy > 0 ) {
        CRenderClip roundClip;
        CRenderClip::GenerateRoundClip(hDC, m_rcPaint,  m_rcItem, m_cxyBorderRound.cx, m_cxyBorderRound.cy, roundClip);
        PaintBkColor(hDC);
        PaintBkImage(hDC);
        PaintStatusImage(hDC);
        PaintText(hDC);
        PaintBorder(hDC);
    }
    else {
        PaintBkColor(hDC);
        PaintBkImage(hDC);
        PaintStatusImage(hDC);
        PaintText(hDC);
        PaintBorder(hDC);
    }
}

void CControlUI::PaintBkColor(HDC hDC)
{
    if( m_dwBackColor != 0 ) {
        if( m_dwBackColor2 != 0 ) {
            if( m_dwBackColor3 != 0 ) {
                RECT rc = m_rcItem;
                rc.bottom = (rc.bottom + rc.top) / 2;
                CRenderEngine::DrawGradient(hDC, rc, GetAdjustColor(m_dwBackColor), GetAdjustColor(m_dwBackColor2), true, 8);
                rc.top = rc.bottom;
                rc.bottom = m_rcItem.bottom;
                CRenderEngine::DrawGradient(hDC, rc, GetAdjustColor(m_dwBackColor2), GetAdjustColor(m_dwBackColor3), true, 8);
            }
            else 
                CRenderEngine::DrawGradient(hDC, m_rcItem, GetAdjustColor(m_dwBackColor), GetAdjustColor(m_dwBackColor2), true, 16);
        }
        else if( m_dwBackColor >= 0xFF000000 ) CRenderEngine::DrawColor(hDC, m_rcPaint, GetAdjustColor(m_dwBackColor));
        else CRenderEngine::DrawColor(hDC, m_rcItem, GetAdjustColor(m_dwBackColor));
    }
}

void CControlUI::PaintBkImage(HDC hDC)
{
    if( m_sBkImage.IsEmpty() ) return;
    if( !DrawImage(hDC, (LPCTSTR)m_sBkImage) ) m_sBkImage.Empty();
}

void CControlUI::PaintStatusImage(HDC hDC)
{
    return;
}

void CControlUI::PaintText(HDC hDC)
{
    return;
}

void CControlUI::PaintBorder(HDC hDC)
{
	if(m_dwBorderColor != 0 || m_dwFocusBorderColor != 0)
	{
		if(m_nBorderSize > 0 && ( m_cxyBorderRound.cx > 0 || m_cxyBorderRound.cy > 0 ))//��Բ�Ǳ߿�
		{
			if (IsFocused() && m_dwFocusBorderColor != 0)
				CRenderEngine::DrawRoundRect(hDC, m_rcItem, m_nBorderSize, m_cxyBorderRound.cx, m_cxyBorderRound.cy, GetAdjustColor(m_dwFocusBorderColor));
			else
				CRenderEngine::DrawRoundRect(hDC, m_rcItem, m_nBorderSize, m_cxyBorderRound.cx, m_cxyBorderRound.cy, GetAdjustColor(m_dwBorderColor));
		}
		else
		{
			if (IsFocused() && m_dwFocusBorderColor != 0 && m_nBorderSize > 0)
				CRenderEngine::DrawRect(hDC, m_rcItem, m_nBorderSize, GetAdjustColor(m_dwFocusBorderColor));
			else if(m_rcBorderSize.left > 0 || m_rcBorderSize.top > 0 || m_rcBorderSize.right > 0 || m_rcBorderSize.bottom > 0)
			{
				RECT rcBorder;

				if(m_rcBorderSize.left > 0){
					rcBorder		= m_rcItem;
					rcBorder.right	= m_rcItem.left;
					CRenderEngine::DrawLine(hDC,rcBorder,m_rcBorderSize.left,GetAdjustColor(m_dwBorderColor),m_nBorderStyle);
				}
				if(m_rcBorderSize.top > 0){
					rcBorder		= m_rcItem;
					rcBorder.bottom	= m_rcItem.top;
					CRenderEngine::DrawLine(hDC,rcBorder,m_rcBorderSize.top,GetAdjustColor(m_dwBorderColor),m_nBorderStyle);
				}
				if(m_rcBorderSize.right > 0){
					rcBorder		= m_rcItem;
					rcBorder.left	= m_rcItem.right;
					CRenderEngine::DrawLine(hDC,rcBorder,m_rcBorderSize.right,GetAdjustColor(m_dwBorderColor),m_nBorderStyle);
				}
				if(m_rcBorderSize.bottom > 0){
					rcBorder		= m_rcItem;
					rcBorder.top	= m_rcItem.bottom;
					CRenderEngine::DrawLine(hDC,rcBorder,m_rcBorderSize.bottom,GetAdjustColor(m_dwBorderColor),m_nBorderStyle);
				}
			}
			else if(m_nBorderSize > 0)
				CRenderEngine::DrawRect(hDC, m_rcItem, m_nBorderSize, GetAdjustColor(m_dwBorderColor));
		}
	}
}

void CControlUI::DoPostPaint(HDC hDC, const RECT& rcPaint)
{
    return;
}

//************************************
// Method:    SetAnimEffects
// FullName:  CControlUI::SetAnimEffects
// Access:    virtual public 
// Returns:   void
// Qualifier:
// Parameter: bool bEnableEffect
// Note:	  
//************************************
void CControlUI::SetAnimEffects(bool bEnableEffect)
{
	try
	{
		m_bEnabledEffect = bEnableEffect;
	}
	catch (...)
	{
		throw "CControlUI::SetAnimEffects";
	}
}

//************************************
// Method:    GetAnimEffects
// FullName:  CControlUI::GetAnimEffects
// Access:    virtual public 
// Returns:   bool
// Qualifier: const
// Note:	  
//************************************
bool CControlUI::GetAnimEffects() const
{
	try
	{
		return m_bEnabledEffect;
	}
	catch (...)
	{
		throw "CControlUI::GetAnimEffects";
	}
}
//************************************
// Method:    SetEffectsZoom
// FullName:  CControlUI::SetEffectsZoom
// Access:    virtual public 
// Returns:   void
// Qualifier:
// Parameter: int iZoom
// Note:	  
//************************************
void CControlUI::SetEffectsZoom(int iZoom)
{
	try
	{
		m_tCurEffects.m_iZoom = iZoom;
	}
	catch (...)
	{
		throw "CControlUI::SetEffectsZoom";
	}
}

//************************************
// Method:    GetEffectsZoom
// FullName:  CControlUI::GetEffectsZoom
// Access:    virtual public 
// Returns:   int
// Qualifier: const
// Note:	  
//************************************
int CControlUI::GetEffectsZoom() const
{
	try
	{
		return m_tCurEffects.m_iZoom;
	}
	catch (...)
	{
		throw "CControlUI::GetEffectsZoom";
	}
}

//************************************
// Method:    SetEffectsFillingBK
// FullName:  CControlUI::SetEffectsFillingBK
// Access:    virtual public 
// Returns:   void
// Qualifier:
// Parameter: DWORD dFillingBK
// Note:	  
//************************************
void CControlUI::SetEffectsFillingBK(DWORD dFillingBK)
{
	try
	{
		m_tCurEffects.m_dFillingBK = dFillingBK;
	}
	catch (...)
	{
		throw "CControlUI::SetEffectsFillingBK";
	}
}

//************************************
// Method:    GetEffectsFillingBK
// FullName:  CControlUI::GetEffectsFillingBK
// Access:    virtual public 
// Returns:   DWORD
// Qualifier: const
// Note:	  
//************************************
DWORD CControlUI::GetEffectsFillingBK() const
{
	try
	{
		return m_tCurEffects.m_dFillingBK;
	}
	catch (...)
	{
		throw "CControlUI::GetEffectsFillingBK";
	}
}

//************************************
// Method:    SetEffectsOffectX
// FullName:  CControlUI::SetEffectsOffectX
// Access:    virtual public 
// Returns:   void
// Qualifier:
// Parameter: int iOffectX
// Note:	  
//************************************
void CControlUI::SetEffectsOffectX(int iOffectX)
{
	try
	{
		m_tCurEffects.m_iOffectX = iOffectX;
	}
	catch (...)
	{
		throw "CControlUI::SetEffectsOffectX";
	}
}

//************************************
// Method:    GetEffectsOffectX
// FullName:  CControlUI::GetEffectsOffectX
// Access:    virtual public 
// Returns:   int
// Qualifier: const
// Note:	  
//************************************
int CControlUI::GetEffectsOffectX() const
{
	try
	{
		return m_tCurEffects.m_iOffectX;
	}
	catch (...)
	{
		throw "CControlUI::GetEffectsOffectX";
	}
}

//************************************
// Method:    SetEffectsOffectY
// FullName:  CControlUI::SetEffectsOffectY
// Access:    virtual public 
// Returns:   void
// Qualifier:
// Parameter: int iOffectY
// Note:	  
//************************************
void CControlUI::SetEffectsOffectY(int iOffectY)
{
	try
	{
		m_tCurEffects.m_iOffectY = iOffectY;
	}
	catch (...)
	{
		throw "CControlUI::SetEffectsOffectY";
	}
}

//************************************
// Method:    GetEffectsOffectY
// FullName:  CControlUI::GetEffectsOffectY
// Access:    virtual public 
// Returns:   int
// Qualifier: const
// Note:	  
//************************************
int CControlUI::GetEffectsOffectY() const
{
	try
	{
		return m_tCurEffects.m_iOffectY;
	}
	catch (...)
	{
		throw "CControlUI::GetEffectsOffectY";
	}
}

//************************************
// Method:    SetEffectsAlpha
// FullName:  CControlUI::SetEffectsAlpha
// Access:    virtual public 
// Returns:   void
// Qualifier:
// Parameter: int iAlpha
// Note:	  
//************************************
void CControlUI::SetEffectsAlpha(int iAlpha)
{
	try
	{
		m_tCurEffects.m_iAlpha = iAlpha;
	}
	catch (...)
	{
		throw "CControlUI::SetEffectsAlpha";
	}
}

//************************************
// Method:    GetEffectsAlpha
// FullName:  CControlUI::GetEffectsAlpha
// Access:    virtual public 
// Returns:   int
// Qualifier: const
// Note:	  
//************************************
int CControlUI::GetEffectsAlpha() const
{
	try
	{
		return m_tCurEffects.m_iAlpha;
	}
	catch (...)
	{
		throw "CControlUI::GetEffectsAlpha";
	}
}

//************************************
// Method:    SetEffectsRotation
// FullName:  CControlUI::SetEffectsRotation
// Access:    virtual public 
// Returns:   void
// Qualifier:
// Parameter: float fRotation
// Note:	  
//************************************
void CControlUI::SetEffectsRotation(float fRotation)
{
	try
	{
		m_tCurEffects.m_fRotation = fRotation;
	}
	catch (...)
	{
		throw "CControlUI::SetEffectsRotation";
	}
}

//************************************
// Method:    GetEffectRotation
// FullName:  CControlUI::GetEffectsRotation
// Access:    virtual public 
// Returns:   float
// Qualifier: 
// Note:	  
//************************************
float CControlUI::GetEffectsRotation()
{
	try
	{
		return m_tCurEffects.m_fRotation;
	}
	catch (...)
	{
		throw "CControlUI::GetEffectsRotation";
	}
}

//************************************
// Method:    SetEffectsNeedTimer
// FullName:  CControlUI::SetEffectsNeedTimer
// Access:    virtual public 
// Returns:   void
// Qualifier:
// Parameter: int iNeedTimer
// Note:	  
//************************************
void CControlUI::SetEffectsNeedTimer(int iNeedTimer)
{
	try
	{
		m_tCurEffects.m_iNeedTimer = iNeedTimer;
	}
	catch (...)
	{
		throw "CControlUI::SetEffectsNeedTimer";
	}
}

//************************************
// Method:    GetEffectsNeedTimer
// FullName:  CControlUI::GetEffectsNeedTimer
// Access:    virtual public 
// Returns:   int
// Qualifier:
// Note:	  
//************************************
int CControlUI::GetEffectsNeedTimer()
{
	try
	{
		return m_tCurEffects.m_iNeedTimer;
	}
	catch (...)
	{
		throw "CControlUI::GetEffectsNeedTimer";
	}
}


//************************************
// Method:    GetCurEffects
// FullName:  CControlUI::GetCurEffects
// Access:    virtual public 
// Returns:   TEffectAge*
// Qualifier:
// Note:	  
//************************************
TEffectAge* CControlUI::GetCurEffects()
{
	try
	{
		return &m_tCurEffects;
	}
	catch (...)
	{
		throw "CControlUI::GetCurEffects";
	}
}

//************************************
// Method:    GetMouseInEffect
// FullName:  CControlUI::GetMouseInEffect
// Access:    virtual public 
// Returns:   TEffectAge*
// Qualifier:
// Note:	  
//************************************
TEffectAge* CControlUI::GetMouseInEffect()
{
	try
	{
		return &m_tMouseInEffects;
	}
	catch (...)
	{
		throw "CControlUI::GetMouseInEffect";
	}
}

//************************************
// Method:    GetMouseOutEffect
// FullName:  CControlUI::GetMouseOutEffect
// Access:    virtual public 
// Returns:   TEffectAge*
// Qualifier:
// Note:	  
//************************************
TEffectAge* CControlUI::GetMouseOutEffect()
{
	try
	{
		return &m_tMouseOutEffects;
	}
	catch (...)
	{
		throw "CControlUI::GetMouseOutEffect";
	}
}

int CControlUI::GetLeftBorderSize() const
{
	return m_rcBorderSize.left;
}

void CControlUI::SetLeftBorderSize( int nSize )
{
	m_rcBorderSize.left = nSize;
	Invalidate();
}

int CControlUI::GetTopBorderSize() const
{
	return m_rcBorderSize.top;
}

void CControlUI::SetTopBorderSize( int nSize )
{
	m_rcBorderSize.top = nSize;
	Invalidate();
}

int CControlUI::GetRightBorderSize() const
{
	return m_rcBorderSize.right;
}

void CControlUI::SetRightBorderSize( int nSize )
{
	m_rcBorderSize.right = nSize;
	Invalidate();
}

int CControlUI::GetBottomBorderSize() const
{
	return m_rcBorderSize.bottom;
}

void CControlUI::SetBottomBorderSize( int nSize )
{
	m_rcBorderSize.bottom = nSize;
	Invalidate();
}

int CControlUI::GetBorderStyle() const
{
	return m_nBorderStyle;
}

void CControlUI::SetBorderStyle( int nStyle )
{
	m_nBorderStyle = nStyle;
	Invalidate();
}

//************************************
// Method:    GetClickInEffect
// FullName:  CControlUI::GetClickInEffect
// Access:    virtual public 
// Returns:   TEffectAge*
// Qualifier:
// Note:	  
//************************************
TEffectAge* CControlUI::GetClickInEffect()
{
	try
	{
		return &m_tMouseClickEffects;
	}
	catch (...)
	{
		throw "CControlUI::GetClickInEffect";
	}
}

//************************************
// Method:    SetEffectsStyle
// FullName:  CControlUI::SetEffectsStyle
// Access:    virtual public 
// Returns:   void
// Qualifier:
// Parameter: LPCTSTR pstrEffectStyle
// Parameter: TEffectAge * pTEffectAge
// Note:	  
//************************************
void CControlUI::SetEffectsStyle(LPCTSTR pstrEffectStyle, TEffectAge* pTEffectAge /*= NULL*/)
{
	try
	{
		if (pstrEffectStyle)
		{
			AnyEffectsAdvProfiles(pstrEffectStyle, pTEffectAge);
			AnyEasyEffectsPorfiles(pstrEffectStyle, pTEffectAge);
		}
	}
	catch (...)
	{
		throw "CControlUI::SetEffectsStyle";
	}
}

//************************************
// Method:    AnyEffectsAdvProfiles
// FullName:  UiLib::CControlUI::AnyEffectsAdvProfiles
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: LPCTSTR pstrEffects
// Parameter: TEffectAge * pTEffectAge
//************************************
void CControlUI::AnyEffectsAdvProfiles(LPCTSTR pstrEffects, TEffectAge* pTEffectAge /*= NULL*/)
{
	try
	{
		CDuiString sItem;
		CDuiString sValue;
		LPTSTR pstr = NULL;

		TEffectAge* pcTEffectAge = pTEffectAge ? pTEffectAge : &m_tCurEffects;

		while (*pstrEffects != _T('\0')) {
			sItem.Empty();
			sValue.Empty();
			while (*pstrEffects > _T('\0') && *pstrEffects <= _T(' ')) pstrEffects = ::CharNext(pstrEffects);
			while (*pstrEffects != _T('\0') && *pstrEffects != _T('=') && *pstrEffects > _T(' ')) {
				LPTSTR pstrTemp = ::CharNext(pstrEffects);
				while (pstrEffects < pstrTemp) {
					sItem += *pstrEffects++;
				}
			}
			while (*pstrEffects > _T('\0') && *pstrEffects <= _T(' ')) pstrEffects = ::CharNext(pstrEffects);
			if (*pstrEffects++ != _T('=')) break;
			while (*pstrEffects > _T('\0') && *pstrEffects <= _T(' ')) pstrEffects = ::CharNext(pstrEffects);
			if (*pstrEffects++ != _T('\'')) break;
			while (*pstrEffects != _T('\0') && *pstrEffects != _T('\'')) {
				LPTSTR pstrTemp = ::CharNext(pstrEffects);
				while (pstrEffects < pstrTemp) {
					sValue += *pstrEffects++;
				}
			}
			if (*pstrEffects++ != _T('\'')) break;
			if (!sValue.IsEmpty()) {
				if (sItem == _T("zoom")) pcTEffectAge->m_iZoom = (_ttoi(sValue.GetData()));
				else if (sItem == _T("fillingbk")){
					if (sValue == _T("none"))
						sValue == _T("#ffffffff");

					if (*sValue.GetData() == _T('#'))
						sValue = ::CharNext(sValue.GetData());

					pcTEffectAge->m_dFillingBK = (_tcstoul(sValue.GetData(), &pstr, 16));
					ASSERT(pstr);
				}
				else if (sItem == _T("offsetx")) pcTEffectAge->m_iOffectX = (_ttoi(sValue.GetData()));
				else if (sItem == _T("offsety")) pcTEffectAge->m_iOffectY = (_ttoi(sValue.GetData()));
				else if (sItem == _T("alpha")) pcTEffectAge->m_iAlpha = (_ttoi(sValue.GetData()));
				else if (sItem == _T("rotation")) pcTEffectAge->m_fRotation = (float)(_tstof(sValue.GetData()));
				else if (sItem == _T("needtimer")) pcTEffectAge->m_iNeedTimer = (_ttoi(sValue.GetData()));
			}
			if (*pstrEffects++ != _T(' ')) break;
		}
		pcTEffectAge->m_bEnableEffect = true;
	}
	catch (...)
	{
		throw "CControlUI::AnyEffectsAdvProfiles";
	}
}

//************************************
// Method:    AnyEasyEffectsPorfiles
// FullName:  UiLib::CControlUI::AnyEasyEffectsPorfiles
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: LPCTSTR pstrEffects
// Parameter: TEffectAge * pTEffectAge
//************************************
void CControlUI::AnyEasyEffectsPorfiles(LPCTSTR pstrEffects, TEffectAge* pTEffectAge /*= NULL*/)
{
	try
	{
		CDuiString sItem;
		CDuiString sValue;
		CDuiString sAnim;
		LPTSTR pstr = NULL;

		TEffectAge* pcTEffectAge = pTEffectAge ? pTEffectAge : &m_tCurEffects;

		while (*pstrEffects != _T('\0')) {
			sItem.Empty();
			sValue.Empty();
			while (*pstrEffects > _T('\0') && *pstrEffects <= _T(' ')) pstrEffects = ::CharNext(pstrEffects);
			while (*pstrEffects != _T('\0') && *pstrEffects != _T('=') && *pstrEffects > _T(' ')) {
				LPTSTR pstrTemp = ::CharNext(pstrEffects);
				while (pstrEffects < pstrTemp) {
					sItem += *pstrEffects++;
				}
			}
			while (*pstrEffects > _T('\0') && *pstrEffects <= _T(' ')) pstrEffects = ::CharNext(pstrEffects);
			if (*pstrEffects++ != _T('=')) break;
			while (*pstrEffects > _T('\0') && *pstrEffects <= _T(' ')) pstrEffects = ::CharNext(pstrEffects);
			if (*pstrEffects++ != _T('\'')) break;
			while (*pstrEffects != _T('\0') && *pstrEffects != _T('\'')) {
				LPTSTR pstrTemp = ::CharNext(pstrEffects);
				while (pstrEffects < pstrTemp) {
					sValue += *pstrEffects++;
				}
			}
			if (*pstrEffects++ != _T('\'')) break;
			if (!sValue.IsEmpty()) {
				if (sItem == _T("anim")){
					sAnim = sValue;

					if (sValue == _T("zoom+")){
						if (pcTEffectAge->m_iZoom > 0)	pcTEffectAge->m_iZoom = (pcTEffectAge->m_iZoom - pcTEffectAge->m_iZoom * 2);
						if (pcTEffectAge->m_iZoom == 0) pcTEffectAge->m_iZoom = -1;
						pcTEffectAge->m_iAlpha = -255;
						pcTEffectAge->m_fRotation = 0.0;
					}
					else if (sValue == _T("zoom-")){
						if (pcTEffectAge->m_iZoom < 0)	pcTEffectAge->m_iZoom = (pcTEffectAge->m_iZoom - pcTEffectAge->m_iZoom * 2);
						if (pcTEffectAge->m_iZoom == 0) pcTEffectAge->m_iZoom = 1;
						pcTEffectAge->m_iAlpha = 255;
						pcTEffectAge->m_fRotation = 0.0;
					}
					else if (sValue == _T("left2right")){
						if (pcTEffectAge->m_iOffectX > 0)	pcTEffectAge->m_iOffectX = (pcTEffectAge->m_iOffectX - pcTEffectAge->m_iOffectX * 2);
						pcTEffectAge->m_iAlpha = 255;
						pcTEffectAge->m_iZoom = 0;
						pcTEffectAge->m_iOffectY = 0;
						pcTEffectAge->m_fRotation = 0.0;
					}
					else if (sValue == _T("right2left")){
						if (pcTEffectAge->m_iOffectX < 0)	pcTEffectAge->m_iOffectX = (pcTEffectAge->m_iOffectX - pcTEffectAge->m_iOffectX * 2);
						pcTEffectAge->m_iAlpha = 255;
						pcTEffectAge->m_iZoom = 0;
						pcTEffectAge->m_iOffectY = 0;
						pcTEffectAge->m_fRotation = 0.0;
					}
					else if (sValue == _T("top2bottom")){
						if (pcTEffectAge->m_iOffectY > 0)	pcTEffectAge->m_iOffectY = (pcTEffectAge->m_iOffectY - pcTEffectAge->m_iOffectY * 2);
						pcTEffectAge->m_iAlpha = 255;
						pcTEffectAge->m_iZoom = 0;
						pcTEffectAge->m_iOffectX = 0;
						pcTEffectAge->m_fRotation = 0.0;
					}
					else if (sValue == _T("bottom2top")){
						if (pcTEffectAge->m_iOffectY < 0)	pcTEffectAge->m_iOffectY = (pcTEffectAge->m_iOffectY - pcTEffectAge->m_iOffectY * 2);
						pcTEffectAge->m_iAlpha = 255;
						pcTEffectAge->m_iZoom = 0;
						pcTEffectAge->m_iOffectX = 0;
						pcTEffectAge->m_fRotation = 0.0;
					}
				}
				else if (sItem == _T("offset")){
					if (sAnim == _T("zoom+") || sAnim == _T("zoom-")){
						pcTEffectAge->m_iOffectX = 0;
						pcTEffectAge->m_iOffectY = 0;
					}
					else if (sAnim == _T("left2right") || sAnim == _T("right2left")){
						pcTEffectAge->m_iOffectX = _ttoi(sValue.GetData());
						pcTEffectAge->m_iOffectY = 0;

						if (sAnim == _T("left2right"))
						if (pcTEffectAge->m_iOffectX > 0)	pcTEffectAge->m_iOffectX = (pcTEffectAge->m_iOffectX - pcTEffectAge->m_iOffectX * 2);
						else if (sAnim == _T("right2left"))
						if (pcTEffectAge->m_iOffectX < 0)	pcTEffectAge->m_iOffectX = (pcTEffectAge->m_iOffectX - pcTEffectAge->m_iOffectX * 2);

					}
					else if (sAnim == _T("top2bottom") || sAnim == _T("bottom2top")){
						pcTEffectAge->m_iOffectX = 0;
						pcTEffectAge->m_iOffectY = _ttoi(sValue.GetData());

						if (sAnim == _T("top2bottom"))
						if (pcTEffectAge->m_iOffectY > 0)	pcTEffectAge->m_iOffectY = (pcTEffectAge->m_iOffectY - pcTEffectAge->m_iOffectY * 2);
						if (sAnim == _T("bottom2top"))
						if (pcTEffectAge->m_iOffectY < 0)	pcTEffectAge->m_iOffectY = (pcTEffectAge->m_iOffectY - pcTEffectAge->m_iOffectY * 2);
					}
				}
				else if (sItem == _T("needtimer")) pcTEffectAge->m_iNeedTimer = (_ttoi(sValue.GetData()));
				else if (sItem == _T("fillingbk")){
					if (sValue == _T("none"))
						sValue == _T("#ffffffff");

					if (*sValue.GetData() == _T('#'))
						sValue = ::CharNext(sValue.GetData());

					pcTEffectAge->m_dFillingBK = _tcstoul(sValue.GetData(), &pstr, 16);
					ASSERT(pstr);
				}
			}
			if (*pstrEffects++ != _T(' ')) break;
		}
		pcTEffectAge->m_bEnableEffect = true;
	}
	catch (...)
	{
		throw "CControlUI::AnyEasyEffectsPorfiles";
	}
}

//************************************
// Method:    TriggerEffects
// FullName:  CControlUI::TriggerEffects
// Access:    virtual public 
// Returns:   void
// Qualifier:
// Parameter: TEffectAge * pTEffectAge
// Note:	  
//************************************
void CControlUI::TriggerEffects(TEffectAge* pTEffectAge /*= NULL*/)
{
	try
	{
		try
		{
			TEffectAge* pcTEffect = pTEffectAge ? pTEffectAge : &m_tCurEffects;

			if (GetManager() && m_bEnabledEffect && pcTEffect->m_bEnableEffect)
				GetManager()->AddAnimationJob(CDxAnimationUI(UIANIMTYPE_FLAT, 0, pcTEffect->m_iNeedTimer, pcTEffect->m_dFillingBK, pcTEffect->m_dFillingBK, GetPos(), pcTEffect->m_iOffectX, pcTEffect->m_iOffectY, pcTEffect->m_iZoom, pcTEffect->m_iAlpha, (float)pcTEffect->m_fRotation));
		}
		catch (...)
		{
			throw "CControlUI::TriggerEffects";
		}
	}
	catch (...)
	{
		throw "CControlUI::TriggerEffects";
	}
}

} // namespace DuiLib
