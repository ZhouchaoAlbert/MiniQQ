#include "stdafx.h"
#include <windowsx.h>		//zjg 20160223 因为和Cef的CefDOMNode一些函数定义冲突，从UILib.h移入
#include "UIEditComb.h"

namespace DuiLib
{

class CEditCombWnd : public CWindowWnd
{
public:
	CEditCombWnd();
	~CEditCombWnd();

	void Init(CEditCombUI* pOwner);
	RECT CalPos();

	LPCTSTR GetWindowClassName() const;
	LPCTSTR GetSuperClassName() const;
	void OnFinalMessage(HWND hWnd);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEditChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	BOOL ShowBalloonTip(LPCTSTR lpszText);
protected:
	CEditCombUI* m_pOwner;
	HBRUSH m_hBkBrush;
	bool m_bInit;
};


CEditCombWnd::CEditCombWnd() : m_pOwner(NULL), m_hBkBrush(NULL), m_bInit(false)
{
}

CEditCombWnd::~CEditCombWnd()
{
	if (m_pOwner) m_pOwner->m_sBalloonTip = _T("");
}

void CEditCombWnd::Init(CEditCombUI* pOwner)
{
	m_pOwner = pOwner;
	RECT rcPos = CalPos();
	UINT uStyle = 0;
	if (m_pOwner->GetManager()->IsBackgroundTransparent())
	{
		uStyle = WS_POPUP | ES_AUTOHSCROLL | WS_VISIBLE;
		RECT rcWnd = { 0 };
		::GetWindowRect(m_pOwner->GetManager()->GetPaintWindow(), &rcWnd);
		rcPos.left += rcWnd.left;
		rcPos.right += rcWnd.left;
		rcPos.top += rcWnd.top;
		rcPos.bottom += rcWnd.top;
	}
	else
	{
		uStyle = WS_CHILD | ES_AUTOHSCROLL;
	}
	Create(m_pOwner->GetManager()->GetPaintWindow(), NULL, uStyle, 0, rcPos);
	HFONT hFont = NULL;
	int iFontIndex = m_pOwner->GetFont();
	if (iFontIndex != -1)
		hFont = m_pOwner->GetManager()->GetFont(iFontIndex);
	if (hFont == NULL)
		hFont = m_pOwner->GetManager()->GetDefaultFontInfo()->hFont;

	SetWindowFont(m_hWnd, hFont, TRUE);
	Edit_LimitText(m_hWnd, m_pOwner->GetMaxChar());
	Edit_SetText(m_hWnd, m_pOwner->GetText());
	Edit_SetModify(m_hWnd, FALSE);
	SendMessage(EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(0, 0));
	Edit_Enable(m_hWnd, m_pOwner->IsEnabled() == true);

	//Styls
	LONG styleValue = ::GetWindowLong(m_hWnd, GWL_STYLE);
	styleValue |= pOwner->GetWindowStyls();
	::SetWindowLong(GetHWND(), GWL_STYLE, styleValue);
	::ShowWindow(m_hWnd, SW_SHOWNOACTIVATE);
	::SetFocus(m_hWnd);

	//
	CDuiString sBalloonTip = m_pOwner->m_sBalloonTip;
	if (!sBalloonTip.IsEmpty()) ShowBalloonTip(sBalloonTip);
	m_bInit = true;
}

BOOL CEditCombWnd::ShowBalloonTip(LPCTSTR lpszText)
{
	ASSERT(::IsWindow(m_hWnd));
	EDITBALLOONTIP bt;
	::ZeroMemory(&bt, sizeof(EDITBALLOONTIP));

	bt.cbStruct = sizeof(EDITBALLOONTIP);
	bt.pszTitle = NULL;
	bt.pszText = lpszText;
	bt.ttiIcon = TTI_NONE;

	return Edit_ShowBalloonTip(m_hWnd, &bt); // EM_SHOWBALLOONTIP
}

RECT CEditCombWnd::CalPos()
{
	CDuiRect rcPos = m_pOwner->GetPos();
	RECT rcInset = m_pOwner->GetTextPadding();
	rcPos.left += rcInset.left;
	rcPos.top += rcInset.top;
	rcPos.right -= rcInset.right;
	rcPos.right -= m_pOwner->GetArrowWidth();
	rcPos.bottom -= rcInset.bottom;
	LONG lEditHeight = m_pOwner->GetManager()->GetFontInfo(m_pOwner->GetFont())->tm.tmHeight;
	if (lEditHeight < rcPos.GetHeight()) {
		rcPos.top += (rcPos.GetHeight() - lEditHeight) / 2;
		rcPos.bottom = rcPos.top + lEditHeight;
	}
	return rcPos;
}

LPCTSTR CEditCombWnd::GetWindowClassName() const
{
	return _T("EditCombWnd");
}

LPCTSTR CEditCombWnd::GetSuperClassName() const
{
	return WC_EDIT;
}

void CEditCombWnd::OnFinalMessage(HWND /*hWnd*/)
{
	m_pOwner->Invalidate();
	// Clear reference and die
	if (m_hBkBrush != NULL) ::DeleteObject(m_hBkBrush);
	m_pOwner->m_pWindow = NULL;
	delete this;
}

LRESULT CEditCombWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	if (uMsg == WM_KILLFOCUS) lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
	else if (uMsg == OCM_COMMAND) {
		if (GET_WM_COMMAND_CMD(wParam, lParam) == EN_CHANGE) lRes = OnEditChanged(uMsg, wParam, lParam, bHandled);
		else if (GET_WM_COMMAND_CMD(wParam, lParam) == EN_UPDATE) {
			RECT rcClient;
			::GetClientRect(m_hWnd, &rcClient);
			::InvalidateRect(m_hWnd, &rcClient, FALSE);
		}
	}
	else if (uMsg == WM_KEYDOWN && TCHAR(wParam) == VK_RETURN){

		m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_RETURN);

	}
	else if (uMsg == OCM__BASE + WM_CTLCOLOREDIT || uMsg == OCM__BASE + WM_CTLCOLORSTATIC) {
		if (m_pOwner->GetNativeEditBkColor() == 0xFFFFFFFF) return NULL;
		::SetBkMode((HDC)wParam, TRANSPARENT);

		DWORD dwTextColor;
		if (m_pOwner->GetNativeEditTextColor() != 0x000000)
			dwTextColor = m_pOwner->GetNativeEditTextColor();
		else
			dwTextColor = m_pOwner->GetTextColor();

		::SetTextColor((HDC)wParam, RGB(GetBValue(dwTextColor), GetGValue(dwTextColor), GetRValue(dwTextColor)));
		if (m_hBkBrush == NULL) {
			DWORD clrColor = m_pOwner->GetNativeEditBkColor();
			m_hBkBrush = ::CreateSolidBrush(RGB(GetBValue(clrColor), GetGValue(clrColor), GetRValue(clrColor)));
		}
		return (LRESULT)m_hBkBrush;
	}
	else bHandled = FALSE;

	if (!bHandled) return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return lRes;
}

LRESULT CEditCombWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	PostMessage(WM_CLOSE);
	return lRes;
}

LRESULT CEditCombWnd::OnEditChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if (!m_bInit) return 0;
	if (m_pOwner == NULL) return 0;
	// Copy text back
	int cchLen = ::GetWindowTextLength(m_hWnd) + 1;
	LPTSTR pstr = static_cast<LPTSTR>(_alloca(cchLen * sizeof(TCHAR)));
	ASSERT(pstr);
	if (pstr == NULL) return 0;
	::GetWindowText(m_hWnd, pstr, cchLen);
	m_pOwner->m_sText = pstr;
	m_pOwner->GetManager()->SendNotify(m_pOwner, DUI_MSGTYPE_TEXTCHANGED);
	return 0;
}


/////////////////////////////////////////////////////////////////////////////////////
//
//
CEditCombUI::CEditCombUI() : m_pWindow(NULL), m_uMaxChar(255), m_uButtonState(0),
m_dwEditbkColor(0xFFFFFFFF), m_dwEditTextColor(0x00000000), m_iWindowStyls(0), m_dwTipValueColor(0xFFBAC0C5)
{
	SetTextPadding(CDuiRect(4, 3, 4, 3));
	SetBkColor(0xFFFFFFFF);
	m_nArrowWidth = 0;
	m_bDropState = false;
}

LPCTSTR CEditCombUI::GetClass() const
{
	return _T("EditCombUI");
}

LPVOID CEditCombUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, _T("EditCombUI")) == 0) return static_cast<CEditCombUI*>(this);
	return CLabelUI::GetInterface(pstrName);
}

UINT CEditCombUI::GetControlFlags() const
{
	if (!IsEnabled()) return CControlUI::GetControlFlags();

	return UIFLAG_SETCURSOR | UIFLAG_TABSTOP;
}

void CEditCombUI::DoEvent(TEventUI& event)
{
	if (!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND) {
		if (m_pParent != NULL) m_pParent->DoEvent(event);
		else CLabelUI::DoEvent(event);
		return;
	}

	if (event.Type == UIEVENT_SETCURSOR && IsEnabled())
	{
		POINT pt = { 0 };
		CDuiRect rcItem = m_rcItem;
		GetCursorPos(&pt);
		rcItem.left = rcItem.right - m_nArrowWidth;
		::ScreenToClient(m_pManager->GetPaintWindow(), &pt);
		if (!::PtInRect(&rcItem, pt))
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
		else
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
		return;
	}
	if (event.Type == UIEVENT_WINDOWSIZE)
	{
		if (m_pWindow != NULL) m_pManager->SetFocusNeeded(this);
	}
	if (event.Type == UIEVENT_SCROLLWHEEL)
	{
		if (m_pWindow != NULL) return;
	}
	if (event.Type == UIEVENT_SETFOCUS && IsEnabled())
	{
		POINT pt = { 0 };
		CDuiRect rcItem = m_rcItem;
		GetCursorPos(&pt);
		rcItem.left = rcItem.right - m_nArrowWidth;
		::ScreenToClient(m_pManager->GetPaintWindow(), &pt);
		if (!::PtInRect(&rcItem, pt))
		{
			if (m_pWindow) return;
			m_pWindow = new CEditCombWnd();
			ASSERT(m_pWindow);
			m_pWindow->Init(this);
		}
		Invalidate();
	}
	if (event.Type == UIEVENT_KILLFOCUS && IsEnabled())
	{
		Invalidate();
	}
	if (event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK || event.Type == UIEVENT_RBUTTONDOWN)
	{
		if (IsEnabled()) {
			GetManager()->ReleaseCapture();
			CDuiRect rcItem = m_rcItem;
			rcItem.left = rcItem.right - m_nArrowWidth;
			if (::PtInRect(&rcItem, event.ptMouse))
			{
				m_pManager->SendNotify(this, DUI_MSGTYPE_DROPDOWN);
				return;
			}
			if (IsFocused() && m_pWindow == NULL)
			{
				m_pWindow = new CEditCombWnd();
				ASSERT(m_pWindow);
				m_pWindow->Init(this);

				if (PtInRect(&m_rcItem, event.ptMouse))
				{
					int nSize = GetWindowTextLength(*m_pWindow);
					if (nSize == 0)
						nSize = 1;

					Edit_SetSel(*m_pWindow, 0, nSize);
				}
			}
			else if (m_pWindow != NULL)
			{
#if 1
				int nSize = GetWindowTextLength(*m_pWindow);
				if (nSize == 0)
					nSize = 1;

				Edit_SetSel(*m_pWindow, 0, nSize);
#else
				POINT pt = event.ptMouse;
				pt.x -= m_rcItem.left + m_rcTextPadding.left;
				pt.y -= m_rcItem.top + m_rcTextPadding.top;
				::SendMessage(*m_pWindow, WM_LBUTTONDOWN, event.wParam, MAKELPARAM(pt.x, pt.y));
#endif
			}
		}
		return;
	}
	if (event.Type == UIEVENT_MOUSEMOVE)
	{
		return;
	}
	if (event.Type == UIEVENT_BUTTONUP)
	{
		return;
	}
	if (event.Type == UIEVENT_CONTEXTMENU)
	{
		return;
	}
	if (event.Type == UIEVENT_MOUSEENTER)
	{
		if (IsEnabled()) {
			m_uButtonState |= UISTATE_HOT;
			Invalidate();
		}
		return;
	}
	if (event.Type == UIEVENT_MOUSELEAVE)
	{
		if (IsEnabled()) {
			m_uButtonState &= ~UISTATE_HOT;
			Invalidate();
		}
		return;
	}
	CLabelUI::DoEvent(event);
}

void CEditCombUI::SetEnabled(bool bEnable)
{
	CControlUI::SetEnabled(bEnable);
	if (!IsEnabled()) {
		m_uButtonState = 0;
	}
}

void CEditCombUI::SetText(LPCTSTR pstrText)
{
	m_sText = pstrText;
	if (m_pWindow != NULL) Edit_SetText(*m_pWindow, m_sText);
	Invalidate();
}

void CEditCombUI::SetMaxChar(UINT uMax)
{
	m_uMaxChar = uMax;
	if (m_pWindow != NULL) Edit_LimitText(*m_pWindow, m_uMaxChar);
}

UINT CEditCombUI::GetMaxChar()
{
	return m_uMaxChar;
}

void CEditCombUI::SetNumberOnly(bool bNumberOnly)
{
	if (bNumberOnly)
	{
		m_iWindowStyls |= ES_NUMBER;
	}
	else
	{
		m_iWindowStyls &= ~ES_NUMBER;
	}
}

bool CEditCombUI::IsNumberOnly() const
{
	return m_iWindowStyls&ES_NUMBER ? true : false;
}

int CEditCombUI::GetWindowStyls() const
{
	return m_iWindowStyls;
}

LPCTSTR CEditCombUI::GetNormalImage()
{
	return m_sNormalImage;
}

void CEditCombUI::SetNormalImage(LPCTSTR pStrImage)
{
	m_sNormalImage = pStrImage;
	Invalidate();
}

LPCTSTR CEditCombUI::GetHotImage()
{
	return m_sHotImage;
}

void CEditCombUI::SetHotImage(LPCTSTR pStrImage)
{
	m_sHotImage = pStrImage;
	Invalidate();
}

LPCTSTR CEditCombUI::GetFocusedImage()
{
	return m_sFocusedImage;
}

void CEditCombUI::SetFocusedImage(LPCTSTR pStrImage)
{
	m_sFocusedImage = pStrImage;
	Invalidate();
}

LPCTSTR CEditCombUI::GetDisabledImage()
{
	return m_sDisabledImage;
}

void CEditCombUI::SetDisabledImage(LPCTSTR pStrImage)
{
	m_sDisabledImage = pStrImage;
	Invalidate();
}

LPCTSTR CEditCombUI::GetForeImage()
{
	return m_sForeImage;
}

void CEditCombUI::SetForeImage(LPCTSTR pStrImage)
{
	m_sForeImage = pStrImage;
	Invalidate();
}

void CEditCombUI::SetNativeEditBkColor(DWORD dwBkColor)
{
	m_dwEditbkColor = dwBkColor;
}

DWORD CEditCombUI::GetNativeEditBkColor() const
{
	return m_dwEditbkColor;
}

void CEditCombUI::SetNativeEditTextColor(LPCTSTR pStrColor)
{
	if (*pStrColor == _T('#')) pStrColor = ::CharNext(pStrColor);
	LPTSTR pstr = NULL;
	DWORD clrColor = _tcstoul(pStrColor, &pstr, 16);

	m_dwEditTextColor = clrColor;
}

DWORD CEditCombUI::GetNativeEditTextColor() const
{
	return m_dwEditTextColor;
}

void CEditCombUI::SetSel(long nStartChar, long nEndChar)
{
	if (m_pWindow != NULL) Edit_SetSel(*m_pWindow, nStartChar, nEndChar);
}

void CEditCombUI::SetSel(DWORD dwSelection)
{
	if (m_pWindow != NULL) Edit_SetSel(*m_pWindow, LOWORD(dwSelection), HIWORD(dwSelection));
}

DWORD CEditCombUI::GetSel()
{
	if (m_pWindow != NULL) return Edit_GetSel(*m_pWindow);
	return 0;
}

void CEditCombUI::SetSelAll()
{
	SetSel(0, -1);
}

void CEditCombUI::SetReplaceSel(LPCTSTR lpszReplace)
{
	if (m_pWindow != NULL) Edit_ReplaceSel(*m_pWindow, lpszReplace);
}

void CEditCombUI::SetTipValue(LPCTSTR pStrTipValue)
{
	m_sTipValue = pStrTipValue;
}

LPCTSTR CEditCombUI::GetTipValue()
{
	return m_sTipValue.GetData();
}

void CEditCombUI::SetTipValueColor(LPCTSTR pStrColor)
{
	if (*pStrColor == _T('#')) pStrColor = ::CharNext(pStrColor);
	LPTSTR pstr = NULL;
	DWORD clrColor = _tcstoul(pStrColor, &pstr, 16);

	m_dwTipValueColor = clrColor;
}

DWORD CEditCombUI::GetTipValueColor()
{
	return m_dwTipValueColor;
}

void CEditCombUI::SetPos(RECT rc)
{
	CControlUI::SetPos(rc);
	if (m_pWindow != NULL) {
		RECT rcPos = m_pWindow->CalPos();
		::SetWindowPos(m_pWindow->GetHWND(), NULL, rcPos.left, rcPos.top, rcPos.right - rcPos.left,
			rcPos.bottom - rcPos.top, SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

void CEditCombUI::SetVisible(bool bVisible)
{
	if (m_bVisible == bVisible) return; //增加是否已经设置的判断，已设置则不再处理设置的逻辑。

	CControlUI::SetVisible(bVisible);
	if (!IsVisible() && m_pWindow != NULL) m_pManager->SetFocus(NULL);
}

void CEditCombUI::SetInternVisible(bool bVisible)
{
	if (!IsVisible() && m_pWindow != NULL) m_pManager->SetFocus(NULL);
}

SIZE CEditCombUI::EstimateSize(SIZE szAvailable)
{
	if (m_cxyFixed.cy == 0) return CSize(m_cxyFixed.cx, m_pManager->GetFontInfo(GetFont())->tm.tmHeight + 6);
	return CControlUI::EstimateSize(szAvailable);
}

void CEditCombUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if (_tcscmp(pstrName, _T("numberonly")) == 0) SetNumberOnly(_tcscmp(pstrValue, _T("true")) == 0);
	else if (_tcscmp(pstrName, _T("maxchar")) == 0) SetMaxChar(_ttoi(pstrValue));
	else if (_tcscmp(pstrName, _T("normalimage")) == 0) SetNormalImage(pstrValue);
	else if (_tcscmp(pstrName, _T("hotimage")) == 0) SetHotImage(pstrValue);
	else if (_tcscmp(pstrName, _T("focusedimage")) == 0) SetFocusedImage(pstrValue);
	else if (_tcscmp(pstrName, _T("disabledimage")) == 0) SetDisabledImage(pstrValue);
	else if (_tcscmp(pstrName, _T("foreimage")) == 0) SetForeImage(pstrValue);
	else if (_tcscmp(pstrName, _T("tipvalue")) == 0) SetTipValue(pstrValue);
	else if (_tcscmp(pstrName, _T("tipvaluecolor")) == 0) SetTipValueColor(pstrValue);
	else if (_tcscmp(pstrName, _T("nativetextcolor")) == 0) SetNativeEditTextColor(pstrValue);
	else if (_tcscmp(pstrName, _T("nativebkcolor")) == 0) {
		if (*pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetNativeEditBkColor(clrColor);
	}
	else if (_tcscmp(pstrName, _T("arrowwidth")) == 0) SetArrowWidth(_ttoi(pstrValue));
	else if (_tcscmp(pstrName, _T("normalarrow")) == 0) SetNormalArrow(pstrValue);
	else if (_tcscmp(pstrName, _T("hotarrow")) == 0) SetHotArrow(pstrValue);
	else if (_tcscmp(pstrName, _T("pushedarrow")) == 0) SetPushedArrow(pstrValue);
	else if (_tcscmp(pstrName, _T("disabledarrow")) == 0) SetDisabledArrow(pstrValue);
	else if (_tcscmp(pstrName, _T("droparrow")) == 0) SetDropArrow(pstrValue);
	else if (_tcscmp(pstrName, _T("dropstate")) == 0) SetDropState(_tcscmp(pstrValue, _T("true")) == 0);
	else CLabelUI::SetAttribute(pstrName, pstrValue);
}

void CEditCombUI::PaintStatusImage(HDC hDC)
{
	if (IsFocused()) m_uButtonState |= UISTATE_FOCUSED;
	else m_uButtonState &= ~UISTATE_FOCUSED;
	if (!IsEnabled()) m_uButtonState |= UISTATE_DISABLED;
	else m_uButtonState &= ~UISTATE_DISABLED;

	if ((m_uButtonState & UISTATE_DISABLED) != 0) {
		if (!m_sDisabledImage.IsEmpty()) {
			if (!DrawImage(hDC, (LPCTSTR)m_sDisabledImage)) m_sDisabledImage.Empty();
			else goto Label_ForeImage;
		}
	}
	else if ((m_uButtonState & UISTATE_FOCUSED) != 0) {
		if (!m_sFocusedImage.IsEmpty()) {
			if (!DrawImage(hDC, (LPCTSTR)m_sFocusedImage)) m_sFocusedImage.Empty();
			else goto Label_ForeImage;
		}
	}
	else if ((m_uButtonState & UISTATE_HOT) != 0) {
		if (!m_sHotImage.IsEmpty()) {
			if (!DrawImage(hDC, (LPCTSTR)m_sHotImage)) m_sHotImage.Empty();
			else goto Label_ForeImage;
		}
	}

	if (!m_sNormalImage.IsEmpty()) {
		if (!DrawImage(hDC, (LPCTSTR)m_sNormalImage)) m_sNormalImage.Empty();
		else goto Label_ForeImage;
	}

Label_ForeImage:
	if (!m_sForeImage.IsEmpty()) {
		if (!DrawImage(hDC, (LPCTSTR)m_sForeImage)) m_sForeImage.Empty();
	}
	PaintStatusArrowImage(hDC);
}

void CEditCombUI::PaintText(HDC hDC)
{
	DWORD mCurTextColor = m_dwTextColor;

	if (m_dwTextColor == 0) mCurTextColor = m_dwTextColor = m_pManager->GetDefaultFontColor();
	if (m_dwDisabledTextColor == 0) m_dwDisabledTextColor = m_pManager->GetDefaultDisabledColor();

	CDuiString sText;
	if (GetText() == m_sTipValue || GetText() == _T(""))
	{
		mCurTextColor = m_dwTipValueColor;
		sText = m_sTipValue;
	}
	else
	{
		sText = m_sText;
	}

	RECT rc = m_rcItem;
	rc.left += m_rcTextPadding.left;
	rc.right -= m_rcTextPadding.right;
	rc.top += m_rcTextPadding.top;
	rc.bottom -= m_rcTextPadding.bottom;
	if (IsEnabled()) {
		CRenderEngine::DrawText(hDC, m_pManager, rc, sText, mCurTextColor, \
			m_iFont, DT_SINGLELINE | m_uTextStyle);
	}
	else {
		CRenderEngine::DrawText(hDC, m_pManager, rc, sText, m_dwDisabledTextColor, \
			m_iFont, DT_SINGLELINE | m_uTextStyle);
	}
}

void CEditCombUI::SetArrowWidth(int nArrowWidth)
{
	m_nArrowWidth = nArrowWidth;
}

int CEditCombUI::GetArrowWidth()
{
	return m_nArrowWidth;
}

void CEditCombUI::SetNormalArrow(LPCTSTR pStrImage)
{
	m_sNormalArrow = pStrImage;
	Invalidate();
}

void CEditCombUI::SetHotArrow(LPCTSTR pStrImage)
{
	m_sHotArrow = pStrImage;
	Invalidate();
}

void CEditCombUI::SetPushedArrow(LPCTSTR pStrImage)
{
	m_sPushedArrow = pStrImage;
	Invalidate();
}

void CEditCombUI::SetDisabledArrow(LPCTSTR pStrImage)
{
	m_sDisabledArrow = pStrImage;
	Invalidate();
}

void CEditCombUI::SetDropArrow(LPCTSTR pStrImage)
{
	m_sDropArrow = pStrImage;
	Invalidate();
}

void CEditCombUI::PaintStatusArrowImage(HDC hDC)
{
	CDuiString sModify;
	CSize size(m_rcItem);

    int nArrowHeight = m_nArrowWidth;
    if (size.cy < nArrowHeight) nArrowHeight = size.cy;
    int nY = (size.cy - nArrowHeight) / 2;
    sModify.SmallFormat(_T("dest='%d,%d,%d,%d'"), size.cx - nArrowHeight, nY, size.cx, nY + nArrowHeight);
	
	if (m_bDropState){
		if (!m_sDropArrow.IsEmpty()) {
			if (!DrawImage(hDC, (LPCTSTR)m_sDropArrow, (LPCTSTR)sModify)) m_sDropArrow.Empty();
			else return;
		}
	}
	if ((m_uButtonState & UISTATE_DISABLED) != 0) {
		if (!m_sDisabledArrow.IsEmpty()) {
			if (!DrawImage(hDC, (LPCTSTR)m_sDisabledArrow, (LPCTSTR)sModify)) m_sDisabledArrow.Empty();
			else return;
		}
	}
	else if ((m_uButtonState & UISTATE_HOT) != 0) {
		if (!m_sHotArrow.IsEmpty()) {
			if (!DrawImage(hDC, (LPCTSTR)m_sHotArrow, (LPCTSTR)sModify)) m_sHotArrow.Empty();
			else return;
		}
	}
	else if ((m_uButtonState & UISTATE_PUSHED) != 0) {
		if (!m_sPushedArrow.IsEmpty()) {
			if (!DrawImage(hDC, (LPCTSTR)m_sPushedArrow, (LPCTSTR)sModify)) m_sPushedArrow.Empty();
			else return;
		}
	}
	if (!m_sNormalArrow.IsEmpty()) {
		if (!DrawImage(hDC, (LPCTSTR)m_sNormalArrow, (LPCTSTR)sModify)) m_sNormalArrow.Empty();
		else return;
	}
}

void CEditCombUI::ShowBalloonTip(LPCTSTR lpszText)
{
	m_sBalloonTip = lpszText;
}

void CEditCombUI::SetDropState(bool bState)
{
	m_bDropState = bState;
	Invalidate();
}

}
