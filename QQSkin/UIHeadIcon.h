#ifndef __UI_HEAD_ICON_H__
#define __UI_HEAD_ICON_H__

namespace DuiLib{

class CUIHeadIcon : public CControlUI
{
public:
	CUIHeadIcon();
	~CUIHeadIcon();
public:
    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
    void PaintBkImage(HDC hDC);
public:
    void SetCircleBorderWidth(UINT32 nWidth);
    void SetHeadIcon(LPCTSTR szIcon);
protected:
    UINT32 m_nCircleBorderWidth;
	CDuiString m_strUserIcon;
};

}

#endif

