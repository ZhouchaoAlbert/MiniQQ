#ifndef __UIEDIT_COMB_H__
#define __UIEDIT_COMB_H__

namespace DuiLib
{

class CEditCombWnd;

class  CEditCombUI : public CLabelUI
{
	friend class CEditCombWnd;
public:
	CEditCombUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	UINT GetControlFlags() const;

	void SetEnabled(bool bEnable = true);
	void SetText(LPCTSTR pstrText);
	void SetMaxChar(UINT uMax);
	UINT GetMaxChar();
	void SetNumberOnly(bool bNumberOnly);
	bool IsNumberOnly() const;
	int GetWindowStyls() const;

	LPCTSTR GetNormalImage();
	void SetNormalImage(LPCTSTR pStrImage);
	LPCTSTR GetHotImage();
	void SetHotImage(LPCTSTR pStrImage);
	LPCTSTR GetFocusedImage();
	void SetFocusedImage(LPCTSTR pStrImage);
	LPCTSTR GetDisabledImage();
	void SetDisabledImage(LPCTSTR pStrImage);
	LPCTSTR GetForeImage();
	void SetForeImage(LPCTSTR pStrImage);
	void SetNativeEditBkColor(DWORD dwBkColor);
	DWORD GetNativeEditBkColor() const;
	void SetNativeEditTextColor(LPCTSTR pStrColor);
	DWORD GetNativeEditTextColor() const;

	void SetSel(long nStartChar, long nEndChar);
	void SetSel(DWORD dwSelection);
	DWORD GetSel();
	void SetSelAll();
	void SetReplaceSel(LPCTSTR lpszReplace);

	void SetTipValue(LPCTSTR pStrTipValue);
	LPCTSTR GetTipValue();
	void SetTipValueColor(LPCTSTR pStrColor);
	DWORD GetTipValueColor();

	void SetPos(RECT rc);
	void SetVisible(bool bVisible = true);
	void SetInternVisible(bool bVisible = true);
	SIZE EstimateSize(SIZE szAvailable);
	void DoEvent(TEventUI& event);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	void PaintStatusImage(HDC hDC);
	void PaintText(HDC hDC);

	void SetArrowWidth(int nArrowWidth);
	int GetArrowWidth();
	void SetNormalArrow(LPCTSTR pStrImage);
	void SetHotArrow(LPCTSTR pStrImage);
	void SetPushedArrow(LPCTSTR pStrImage);
	void SetDisabledArrow(LPCTSTR pStrImage);
	void SetDropArrow(LPCTSTR pStrImage);
	void PaintStatusArrowImage(HDC hDC);
	void ShowBalloonTip(LPCTSTR lpszText);
	void SetDropState(bool bState);
protected:
	CEditCombWnd* m_pWindow;
	UINT m_uMaxChar;
	UINT m_uButtonState;
	CDuiString m_sNormalImage;
	CDuiString m_sHotImage;
	CDuiString m_sFocusedImage;
	CDuiString m_sDisabledImage;
	CDuiString m_sForeImage;
	CDuiString m_sTipValue;
	DWORD m_dwTipValueColor;
	DWORD m_dwEditbkColor;
	DWORD m_dwEditTextColor;
	int m_iWindowStyls;

	CDuiString m_sNormalArrow;
	CDuiString m_sHotArrow;
	CDuiString m_sPushedArrow;
	CDuiString m_sDisabledArrow;
	CDuiString m_sDropArrow;
	int m_nArrowWidth;
	CDuiString m_sBalloonTip;
	bool m_bDropState;
};

}
#endif // __UIEDIT_H__