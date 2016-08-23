#include "StdAfx.h"
#include "UITree.h"

namespace DuiLib{

//////////////////////////���ݽڵ�//////////////////////////////////////////////////
CTreeDataUI::CTreeDataUI()
{
	m_cxyMax.cy = 2147483647;
}

LPCTSTR CTreeDataUI::GetClass() const
{
	return _T("TreeDataUI");
}

LPVOID CTreeDataUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, kTreeDataInterface) == 0 ) return static_cast<CTreeDataUI*>(this);
	return CContainerUI::GetInterface(pstrName);
}

//////////////////////////���ӽڵ�//////////////////////////////////////////////////
CTreeChildElementUI::CTreeChildElementUI()
{
	m_cxyMax.cy = 2147483647;
}

LPCTSTR CTreeChildElementUI::GetClass() const
{
	return _T("TreeChildElementUI");
}

LPVOID CTreeChildElementUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, kTreeChildElementInterface) == 0 ) return static_cast<CTreeChildElementUI*>(this);
	return CContainerUI::GetInterface(pstrName);
}

bool CTreeChildElementUI::GetVisible()
{
	return m_bVisible;
}

//////////////////////////���ڵ�////////////////////////////////////////////////////
CTreeElementUI::CTreeElementUI()
{
	m_nExpandHeight = 0;
	m_cxyMax.cy = 2147483647;
}

LPCTSTR CTreeElementUI::GetClass() const
{
	return _T("TreeElementUI");
}

LPVOID CTreeElementUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, kTreeElementInterface) == 0 ) return static_cast<CTreeElementUI*>(this);
	return CContainerUI::GetInterface(pstrName);
}

CTreeDataUI * CTreeElementUI::GetDataElement()
{
	CTreeDataUI * pTreeData = NULL;
	for (int i = 0; i < GetCount(); i++)
	{
		pTreeData = static_cast<CTreeDataUI *>(GetItemAt(i)->GetInterface(kTreeDataInterface));
		if (pTreeData) break;
	}
	return pTreeData;
}

////////////////////////////��///////////////////////////////////////////////////////
CTreeLayoutUI::CTreeLayoutUI(void)
{
	m_dwItemHotColor = 0xFFE9F5FF;
	m_dwItemSelectedColor = 0xFFC1E3FF;
	m_pSelectedItem = NULL;
	m_pUpHotItem = NULL;
	m_nIndentation = 10;
	m_cxyMax.cy = 2147483647;
}

LPCTSTR CTreeLayoutUI::GetClass() const
{
	return _T("TreeLayoutUI");
}

LPVOID CTreeLayoutUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, kTreeLayoutInterface) == 0 ) return static_cast<CTreeLayoutUI*>(this);
	return CContainerUI::GetInterface(pstrName);
}

UINT CTreeLayoutUI::GetControlFlags() const
{
	if( IsEnabled() && m_iSepHeight != 0 ) return UIFLAG_SETCURSOR;
	else return 0;
}

void CTreeLayoutUI::DoEvent(TEventUI& event)
{
	if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
		if( m_pParent != NULL ) m_pParent->DoEvent(event);
		else CControlUI::DoEvent(event);
		return;
	}

	if( event.Type == UIEVENT_SETFOCUS ) 
	{
		m_bFocused = true;
		return;
	}
	if( event.Type == UIEVENT_KILLFOCUS ) 
	{
		m_bFocused = false;
		return;
	}
	if( m_pVerticalScrollBar != NULL && m_pVerticalScrollBar->IsVisible() && m_pVerticalScrollBar->IsEnabled() )
	{
		if( event.Type == UIEVENT_KEYDOWN ) 
		{
			switch( event.chKey ) {
			case VK_DOWN:
				LineDown();
				return;
			case VK_UP:
				LineUp();
				return;
			case VK_NEXT:
				PageDown();
				return;
			case VK_PRIOR:
				PageUp();
				return;
			case VK_HOME:
				HomeUp();
				return;
			case VK_END:
				EndDown();
				return;
			}
		}
		else if( event.Type == UIEVENT_SCROLLWHEEL )
		{
			switch( LOWORD(event.wParam) ) {
			case SB_LINEUP:
				LineUp();
				return;
			case SB_LINEDOWN:
				LineDown();
				return;
			}
		}
	}
	else if( m_pHorizontalScrollBar != NULL && m_pHorizontalScrollBar->IsVisible() && m_pHorizontalScrollBar->IsEnabled() ) {
		if( event.Type == UIEVENT_KEYDOWN ) 
		{
			switch( event.chKey ) {
			case VK_DOWN:
				LineRight();
				return;
			case VK_UP:
				LineLeft();
				return;
			case VK_NEXT:
				PageRight();
				return;
			case VK_PRIOR:
				PageLeft();
				return;
			case VK_HOME:
				HomeLeft();
				return;
			case VK_END:
				EndRight();
				return;
			}
		}
		else if( event.Type == UIEVENT_SCROLLWHEEL )
		{
			switch( LOWORD(event.wParam) ) {
			case SB_LINEUP:
				LineLeft();
				return;
			case SB_LINEDOWN:
				LineRight();
				return;
			}
		}
	}
	//����¼�
	if( event.Type == UIEVENT_DBLCLICK )
	{
		if( IsEnabled() ) {
			POINT	pt = {0};
			::GetCursorPos(&pt);
			::ScreenToClient(GetManager()->GetPaintWindow(), &pt);
			CTreeElementUI * pElement = static_cast<CTreeElementUI *>(HitTest(pt));
			SelectItem(pElement);
			if (pElement){
				m_pManager->SendNotify(this, _T("treedbclick"), 0, MAKELPARAM(pt.x, pt.y));
			}
		}
		return;
	}
	if( event.Type == UIEVENT_KEYDOWN && IsEnabled() )
	{
		if( event.chKey == VK_RETURN ) {
			Activate();
			Invalidate();
			return;
		}
	}
	if( event.Type == UIEVENT_BUTTONUP )
	{
		if( IsEnabled() ){
			POINT	pt = {0};
			::GetCursorPos(&pt);
			::ScreenToClient(GetManager()->GetPaintWindow(), &pt);
			CTreeElementUI * pElement = static_cast<CTreeElementUI *>(HitTest(pt));
			SelectItem(pElement);
			if (pElement){
				m_pManager->SendNotify(this, _T("treeclick"), 0, MAKELPARAM(pt.x, pt.y));
			}
		}
		return;
	}
	if( event.Type == UIEVENT_RBUTTONDOWN )
	{
		if( IsEnabled() ){
			POINT	pt = {0};
			::GetCursorPos(&pt);
			::ScreenToClient(GetManager()->GetPaintWindow(), &pt);
			CTreeElementUI * pElement = static_cast<CTreeElementUI *>(HitTest(pt));
			SelectItem(pElement);
			if (pElement){
				m_pManager->SendNotify(this, _T("treerclick"), 0, MAKELPARAM(pt.x, pt.y));
			}
		}
		return;
	}
	if( event.Type == UIEVENT_MOUSEENTER )
	{
		if( IsEnabled() ) {
			m_uButtonState |= UISTATE_HOT;
			
			POINT	pt = {0};
			::GetCursorPos(&pt);
			::ScreenToClient(GetManager()->GetPaintWindow(), &pt);
			CTreeElementUI * pElement = static_cast<CTreeElementUI *>(HitTest(pt));
			if (m_pUpHotItem != pElement){
				if (pElement){
					m_pManager->SendNotify(this, _T("treeitemhot"), (WPARAM)pElement, MAKELPARAM(pt.x, pt.y));
					m_pUpHotItem = pElement;
				}
			}
			Invalidate();
		}
		return;
	}
	if( event.Type == UIEVENT_MOUSELEAVE )
	{
		if( (m_uButtonState & UISTATE_HOT) != 0 ) {
			m_uButtonState &= ~UISTATE_HOT;

			POINT	pt = {0};
			::GetCursorPos(&pt);
			::ScreenToClient(GetManager()->GetPaintWindow(), &pt);
			CTreeElementUI * pElement = static_cast<CTreeElementUI *>(HitTest(pt));
			if (m_pUpHotItem != pElement){
				m_pManager->SendNotify(this, _T("treeitemleave"), (WPARAM)m_pUpHotItem, MAKELPARAM(pt.x, pt.y));
				m_pUpHotItem = NULL;
			}
			Invalidate();
		}
		return;
	}
	CControlUI::DoEvent(event);
}

void CTreeLayoutUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcscmp(pstrName, _T("itemhotimage")) == 0 ) SetItemHotImage(pstrValue);
	if( _tcscmp(pstrName, _T("itemselectedimage")) == 0 ) SetItemSelectedImage(pstrValue);
	else if( _tcscmp(pstrName, _T("itemhotcolor")) == 0 ) {
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetItemHotColor(clrColor);
	}
	else if( _tcscmp(pstrName, _T("itemselectedcolor")) == 0 ) {
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetItemSelectedColor(clrColor);
	}
	else if( _tcscmp(pstrName, _T("indentation")) == 0 ) {
		SetIndent(_tstoi(pstrValue));
	}
	else __super::SetAttribute(pstrName, pstrValue);
}

LPCTSTR CTreeLayoutUI::GetItemHotImage()
{
	return m_sItemHotImage;
}

void CTreeLayoutUI::SetItemHotImage(LPCTSTR pStrImage)
{
	m_sItemHotImage = pStrImage;
	Invalidate();
}

LPCTSTR CTreeLayoutUI::GetItemSelectedImage()
{
	return m_sItemSelectedImage;
}

void CTreeLayoutUI::SetItemSelectedImage(LPCTSTR pStrImage)
{
	m_sItemSelectedImage = pStrImage;
	Invalidate();
}

void CTreeLayoutUI::SetItemHotColor(DWORD dwColor)
{
	m_dwItemHotColor = dwColor;
}

DWORD CTreeLayoutUI::GetItemHotColor() const
{
	return m_dwItemHotColor;
}

void CTreeLayoutUI::SetItemSelectedColor(DWORD dwColor)
{
	m_dwItemSelectedColor = dwColor;
}

DWORD CTreeLayoutUI::GetItemSelectedColor() const
{
	return m_dwItemSelectedColor;
}

void CTreeLayoutUI::DrawItemBk(HDC hDC)
{
	DWORD				iBackColor = 0;
	RECT				rcHotItem = {0};
	RECT				rcSelectedItem = {0};

	if (m_pUpHotItem && (m_pSelectedItem != m_pUpHotItem)){
		rcHotItem = GetItemRect(m_pUpHotItem);
		iBackColor = GetItemHotColor();
		if ( iBackColor != 0 ) {
			CRenderEngine::DrawColor(hDC, rcHotItem, GetAdjustColor(iBackColor));
		}
		if( !m_sItemHotImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sItemHotImage) ) m_sItemHotImage.Empty();
			else return;
		}
	}
	
	if (m_pSelectedItem){
		iBackColor = GetItemSelectedColor();
		rcSelectedItem = GetItemRect(m_pSelectedItem);
		if ( iBackColor != 0 ) {
			CRenderEngine::DrawColor(hDC, rcSelectedItem, GetAdjustColor(iBackColor));
		}
		if( !m_sItemSelectedImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sItemSelectedImage) ) m_sItemSelectedImage.Empty();
			else return;
		}
	}
	if( m_sBkImage.IsEmpty() ) {
		if( !m_sBkImage.IsEmpty() ) {
			if( !DrawImage(hDC, (LPCTSTR)m_sBkImage) ) m_sBkImage.Empty();
			else return;
		}
	}
}

void CTreeLayoutUI::DoPaint(HDC hDC, const RECT& rcPaint)
{
	DrawItemBk(hDC);
	__super::DoPaint(hDC, rcPaint);
}

/*******************************************************************
* �������ƣ� Add
* ��    �ܣ� ��ӽڵ�
* ��    ���� pParent���ڵ㣬pControl��Ҫ��ӵĿؼ���bInsertLast������ڸ��ڵ����ǰ
* �� �� ֵ�� ��ӳɹ�����true�����򷵻�false
* ��    �ߣ� houbin
* �������ڣ� 2012��6��29��������
*******************************************************************/
bool CTreeLayoutUI::Add(CTreeElementUI* pParent, CTreeElementUI * pControl)
{
	bool	bRet = false;  

	do 
	{
		if (NULL == pControl) break;
		//��ѯ�Լ��ڵ����Ƿ����ӽڵ�
		{
			CTreeChildElementUI * pChildElementUI = NULL;
			for(int n = 0; n < pControl->GetCount(); n++){
				pChildElementUI = static_cast<CTreeChildElementUI *>(pControl->GetItemAt(n)->GetInterface(kTreeChildElementInterface));
				if(NULL != pChildElementUI) break;
			}
			if (NULL == pChildElementUI){
				CTreeChildElementUI * pChild = NULL;
				try{
					pChildElementUI = new CTreeChildElementUI();
					pControl->Add(pChildElementUI);
				}
				catch (...){
					if (pChildElementUI){
						delete pChildElementUI;
						pChildElementUI = NULL;
						break;
					}
				}
			}
		}

		if (pParent){
			//��鸸�ؼ��Ƿ����ӿؼ�����
			CTreeChildElementUI * pChildElementUI = NULL;
			for(int n = 0; n < pParent->GetCount(); n++){
				pChildElementUI = static_cast<CTreeChildElementUI *>(pParent->GetItemAt(n)->GetInterface(kTreeChildElementInterface));
				if(NULL != pChildElementUI) break;
			}
			if (NULL == pChildElementUI){
				break;
			}
			//����ӽڵ�
			bRet = pChildElementUI->Add(pControl);
		}
		else{
			bRet = __super::Add(pControl);
		}
	} while (0);
	
	if (bRet){
		//����������ظ��ڵ�ĸ߶�
		if (pParent){
			//��������
			CTreeChildElementUI * pChildElementUI = NULL;
			for(int n = 0; n < pParent->GetCount(); n++){
				pChildElementUI = static_cast<CTreeChildElementUI *>(pParent->GetItemAt(n)->GetInterface(kTreeChildElementInterface));
				if(NULL != pChildElementUI) break;
			}
			if (pChildElementUI){
				pChildElementUI->SetPadding(CDuiRect(GetIndent(), 0, 0, 0));
			}

			while(pParent){
				if (!IsExpand(pParent)) break;
				int nHeight = pParent->GetFixedHeight() + pControl->GetFixedHeight();
				pParent->SetFixedHeight(nHeight);
				pParent = GetParentItem(pParent);
			}
		}
	}
	return bRet;
}

/*******************************************************************
* �������ƣ� Remove
* ��    �ܣ� ɾ��ָ���ڵ�
* ��    ���� pControlҪɾ���Ľڵ㣬bDeleteɾ����ʱ���Ƿ�ѽڵ���ڴ�����Ҳɾ��
* �� �� ֵ�� ɾ���ɹ�����true�����򷵻�false
* ��    �ߣ� houbin
* �������ڣ� 2012��6��29��������
*******************************************************************/
bool CTreeLayoutUI::Remove(CTreeElementUI* pControl, bool bDelete)
{
	bool	bRet = false;

	do 
	{
		if (pControl == NULL) break;
		CControlUI * pParentControl = pControl->GetParent();
		if (NULL == pParentControl) break;
		if (static_cast<CTreeChildElementUI*>(pParentControl->GetInterface(kTreeChildElementInterface))){
			CTreeChildElementUI * pChildElement = static_cast<CTreeChildElementUI*>(pParentControl->GetInterface(kTreeChildElementInterface));
			int nDelHeight = pControl->GetFixedHeight();
			if (bDelete){
				bRet = pChildElement->Remove(pControl);
			}
			else{
				bRet = pChildElement->Erase(pControl);
			}
			
			//���ø��ڵ�ĸ߶�
			if (bRet){
				CTreeElementUI * pParentElement = static_cast<CTreeElementUI*>(pChildElement->GetParent()->GetInterface(kTreeElementInterface));
				while(pParentElement){
					if (!IsExpand(pParentElement)) break;
					int nHeight = pParentElement->GetFixedHeight() - nDelHeight;
					pParentElement->SetFixedHeight(nHeight);
					pParentElement = GetParentItem(pParentElement);
				}
			}
		}
		else if (static_cast<CTreeLayoutUI*>(pParentControl->GetInterface(kTreeLayoutInterface)) == this){
			if (bDelete){
				bRet = __super::Remove(pControl);
			}
			else{
				bRet = __super::Erase(pControl);
			}
		}
		m_pSelectedItem = NULL;
		m_pUpHotItem = NULL;
	} while (0);
	
	return bRet;
}

/*******************************************************************
* �������ƣ� RemoveAll
* ��    �ܣ� ɾ����
* ��    ���� ��
* �� �� ֵ�� ��
* ��    �ߣ� houbin
* �������ڣ� 2012��6��29��������
*******************************************************************/
void CTreeLayoutUI::RemoveAll()
{
	__super::RemoveAll();
	m_pSelectedItem = NULL;
	m_pUpHotItem = NULL;
}

/*******************************************************************
* �������ƣ� GetRootItem
* ��    �ܣ� ��ȡ���ڵ�
* ��    ���� ��
* �� �� ֵ�� ���ظ��ڵ�ָ��
* ��    �ߣ� houbin
* �������ڣ� 2012��7��2������һ
*******************************************************************/
CTreeElementUI * CTreeLayoutUI::GetRootItem()
{
	CTreeElementUI * pElementUI = NULL;
	for( int i = 0; i < GetCount(); i++ ) {
		if( static_cast<CTreeElementUI*>(GetItemAt(i)->GetInterface(kTreeElementInterface)) != NULL ) {
			pElementUI = static_cast<CTreeElementUI*>(GetItemAt(i)->GetInterface(kTreeElementInterface));
			break;
		}
	}
	return pElementUI;
}

/*******************************************************************
* �������ƣ� GetNextItem
* ��    �ܣ� ��ȡ��һ���ڵ�
* ��    ���� ��ǰ�ڵ�ָ��
* �� �� ֵ�� ��һ���ڵ�ָ��
* ��    �ߣ� houbin
* �������ڣ� 2012��7��2������һ
*******************************************************************/
CTreeElementUI * CTreeLayoutUI::GetNextItem(CTreeElementUI * pElement)
{
	CTreeElementUI * pElementRet = NULL;
	
	do 
	{
		if (NULL == pElement) break;
		CContainerUI * pParentElement = static_cast<CContainerUI *>(pElement->GetParent());
		if (NULL == pParentElement) break;
		int nIndex = pParentElement->GetItemIndex(pElement);
		for(int i = nIndex + 1; i < pParentElement->GetCount(); i++)
		{
			pElementRet = static_cast<CTreeElementUI *>(pParentElement->GetItemAt(i)->GetInterface(kTreeElementInterface));
			if (pElementRet) break;
		}
	} while (0);

	return pElementRet;
}


/*******************************************************************
* �������ƣ� GetChildItem
* ��    �ܣ� ��ȡ�ڵ���ӽڵ�
* ��    ���� pElement��Ҫ��ȡ���ӽڵ�ؼ�
* �� �� ֵ�� �ɹ�����ֱ�ӵ�ָ�룬���򷵻�NULL
* ��    �ߣ� houbin
* �������ڣ� 2012��6��29��������
*******************************************************************/
CTreeElementUI * CTreeLayoutUI::GetChildItem(CTreeElementUI * pElement)
{
	CTreeElementUI * pElementRet = NULL;

	do 
	{
		if (NULL == pElement) break;
		CTreeChildElementUI * pChildElementUI = NULL;
		for(int n = 0; n < pElement->GetCount(); n++){
			pChildElementUI = static_cast<CTreeChildElementUI *>(pElement->GetItemAt(n)->GetInterface(kTreeChildElementInterface));
			if(NULL != pChildElementUI) break;
		}
		if (NULL == pChildElementUI) break;
		if (pChildElementUI->GetCount() <= 0) break;
		pElementRet = static_cast<CTreeElementUI*>(pChildElementUI->GetItemAt(0)->GetInterface(kTreeElementInterface));
	} while (0);
	
	return pElementRet;
}

//�����ڵ�
void CTreeLayoutUI::CollapseElement(CTreeElementUI * pElement)
{
	do 
	{
		if (NULL == pElement) break;
		CTreeChildElementUI * pChildElement = NULL;
		for(int n = 0; n < pElement->GetCount(); n++){
			pChildElement = static_cast<CTreeChildElementUI *>(pElement->GetItemAt(n)->GetInterface(kTreeChildElementInterface));
			if(NULL != pChildElement) break;
		}
		if (NULL == pChildElement) break;

		if (!IsExpand(pElement)) break;
		pChildElement->SetVisible(false);
		CTreeDataUI * pDataUI = pElement->GetDataElement();
		if (pDataUI){
			int nOldHeight = pElement->GetFixedHeight();
			int nHeight = pDataUI->GetFixedHeight();
			pElement->SetFixedHeight(nHeight);

			//���ø��ڵ�ĸ߶�
			CTreeElementUI * pParentElement = GetParentItem(pElement);
			while(pParentElement)
			{
				//���ڵ�չ����������չ���ĸ߶�
				if (IsExpand(pParentElement)){
					int nParentHeight = pParentElement->GetFixedHeight() + nHeight - nOldHeight;
					pParentElement->SetFixedHeight(nParentHeight);
				}
				pParentElement = GetParentItem(pParentElement);
			}
			//���ø��ڵ�߶����////////////
		}
	} while (0);
}

//չ���ڵ�
void CTreeLayoutUI::ExpandElement(CTreeElementUI * pElement)
{
	do 
	{
		if (NULL == pElement) break;
		CTreeChildElementUI * pChildElement = NULL;
		for(int n = 0; n < pElement->GetCount(); n++){
			pChildElement = static_cast<CTreeChildElementUI *>(pElement->GetItemAt(n)->GetInterface(kTreeChildElementInterface));
			if(NULL != pChildElement) break;
		}
		if (NULL == pChildElement) break;

		if (IsExpand(pElement)) break;
		pChildElement->SetVisible(true);
		CTreeDataUI * pDataUI = pElement->GetDataElement();
		if (pDataUI){
			int nHeight = pDataUI->GetFixedHeight();
			int nOldHeight = nHeight;
			CTreeElementUI * pChildNode = GetChildItem(pElement);
			while(pChildNode)
			{
				nHeight += pChildNode->GetFixedHeight();
				pChildNode = GetNextItem(pChildNode);
			}
			pElement->SetFixedHeight(nHeight);
			//���ø��ڵ�ĸ߶�
			CTreeElementUI * pParentElement = GetParentItem(pElement);
			while(pParentElement)
			{
				//���ڵ�չ����������չ���ĸ߶�
				if (IsExpand(pParentElement)){
					int nParentHeight = pParentElement->GetFixedHeight() + nHeight - nOldHeight;
					pParentElement->SetFixedHeight(nParentHeight);
				}
				pParentElement = GetParentItem(pParentElement);
			}
			//���ø��ڵ�߶����////////////
		}
	} while (0);
}

void CTreeLayoutUI::Expand(CTreeElementUI * pElement, UINT nCode)
{
	if (TVE_COLLAPSE == nCode){
		CollapseElement(pElement);
	}
	else if(TVE_EXPAND == nCode){
		ExpandElement(pElement);
	}
	else if(TVE_TOGGLE == nCode){
		//�����������չ�������չ��������
		if (IsExpand(pElement)){
			CollapseElement(pElement);
		}
		else{
			ExpandElement(pElement);
		}
	}
}

/*******************************************************************
* �������ƣ� IsExpand
* ��    �ܣ� ���ָ���ڵ��Ƿ�չ��
* ��    ���� pElement�ڵ�
* �� �� ֵ�� trueΪչ��,falseΪû��չ��
* ��    �ߣ� houbin
* �������ڣ� 2012��7��2������һ
*******************************************************************/
bool CTreeLayoutUI::IsExpand(CTreeElementUI * pElement)
{
	bool	bRet = false;
	
	do 
	{
		if (NULL == pElement) break;
		CTreeChildElementUI * pChildElement = NULL;
		for(int n = 0; n < pElement->GetCount(); n++){
			pChildElement = static_cast<CTreeChildElementUI *>(pElement->GetItemAt(n)->GetInterface(kTreeChildElementInterface));
			if(NULL != pChildElement) break;
		}
		if (NULL == pChildElement) break;
		bRet = pChildElement->GetVisible();
	} while (0);
	return bRet;
}

bool CTreeLayoutUI::ItemHasChildren(CTreeElementUI * pElement)
{
	bool	bRet = false;

	if (GetChildItem(pElement))
	{
		bRet = true;
	}
	return bRet;
}

void CTreeLayoutUI::SelectItem(CTreeElementUI * pItem)
{
	CTreeElementUI * pOldSelected = m_pSelectedItem;
	m_pSelectedItem = pItem;
	if (m_pSelectedItem != pOldSelected){
		m_pManager->SendNotify(this, _T("treeitemselected"), (WPARAM)m_pSelectedItem, (LPARAM)pOldSelected);
	}
	Invalidate();
}

CTreeElementUI * CTreeLayoutUI::GetSelectedItem()
{
	return m_pSelectedItem;
}

CTreeElementUI * CTreeLayoutUI::GetParentItem(CTreeElementUI * pElement)
{
	CTreeElementUI * pElementRet = NULL;

	do 
	{
		if (NULL == pElement) break;
		CControlUI * pParent =  pElement->GetParent();
		if (pParent && (pParent = pParent->GetParent())){
			pElementRet = static_cast<CTreeElementUI*>(pParent->GetInterface(kTreeElementInterface));
		}
	} while (0);

	return pElementRet;
}

CTreeElementUI * CTreeLayoutUI::HitTest(POINT pt)
{
	CTreeElementUI * pElementRet = NULL;
	CControlUI * pControl = GetManager()->FindSubControlByPoint(this, pt);
	while(pControl)
	{
		if (pControl->GetInterface(kTreeLayoutInterface)){
			break;
		}
		else if (pControl->GetInterface(kTreeChildElementInterface)){
			break;
		}
		else if (pControl->GetInterface(kTreeElementInterface)){
			pElementRet = static_cast<CTreeElementUI *>(pControl->GetInterface(kTreeElementInterface));
			break;
		}
		else{
			pControl = pControl->GetParent();
		}
	}
	return pElementRet;
}

void CTreeLayoutUI::SetItemHeight(CTreeElementUI * pControl, int nHeight)
{
	int nOldHeight = pControl->GetFixedHeight();
	CTreeDataUI * pDataUI = pControl->GetDataElement();
	if (pDataUI){
		pDataUI->SetFixedHeight(nHeight);
	}
	pControl->SetFixedHeight(nHeight);
	CTreeElementUI * pParent = GetParentItem(pControl);
	while(pParent){
		if (!IsExpand(pParent)) break;
		int nHeight = pParent->GetFixedHeight() + pControl->GetFixedHeight() - nOldHeight;
		pParent->SetFixedHeight(nHeight);
		pParent = GetParentItem(pParent);
	}
}

RECT CTreeLayoutUI::GetItemRect(CTreeElementUI * pElement)
{
	RECT	rcItem = {0};
	RECT	rc = {0};
	do 
	{
		if (NULL == pElement) break;
		CTreeDataUI * pDataUI = NULL;
		for (int i = 0; i < pElement->GetCount(); i++){
			pDataUI = static_cast<CTreeDataUI *>(pElement->GetItemAt(i)->GetInterface(kTreeDataInterface));
			if (pDataUI) break;
		}
		if (NULL == pDataUI) break;
		rc = GetPos();
		rcItem = pDataUI->GetPos();
		rcItem.left = rc.left;
	} while (0);

	return rcItem;
}

void CTreeLayoutUI::QuickSort(CContainerUI * pElement, TREESORTPROC pSortProc, int nLow, int nHigh)
{	
    if (nLow < nHigh)
	{
        int nIndex = Partition(pElement, pSortProc, nLow, nHigh);
        QuickSort(pElement, pSortProc, nLow, nIndex - 1);
        QuickSort(pElement, pSortProc, nIndex + 1, nHigh);
	}
}

int CTreeLayoutUI::Partition(CContainerUI * pElement, TREESORTPROC pSortProc, int nLow, int nHigh)
{
    CTreeElementUI * pTempNode = static_cast<CTreeElementUI *>(pElement->GetItemAt(nHigh));
    int i = nLow, j = nLow - 1;
    for (; i < nHigh; i++)
    {
        if (pSortProc(static_cast<CTreeElementUI *>(pElement->GetItemAt(i)), pTempNode, this) <= 0)
        {
            j++;
            if (i != j)
            {
                Swap(pElement, static_cast<CTreeElementUI *>(pElement->GetItemAt(i)), static_cast<CTreeElementUI *>(pElement->GetItemAt(j)));
            }
        }
    }
    Swap(pElement, static_cast<CTreeElementUI *>(pElement->GetItemAt(j + 1)), static_cast<CTreeElementUI *>(pElement->GetItemAt(nHigh)));
    return j + 1;
}

void CTreeLayoutUI::Swap(CContainerUI * pElement, CTreeElementUI * pFirst, CTreeElementUI * pSecond)
{
    int nIndex1 = pElement->GetItemIndex(pFirst);
    int nIndex2 = pElement->GetItemIndex(pSecond);
    if (nIndex1 != nIndex2){
        pElement->Erase(pSecond);
        pElement->AddAt(pSecond, nIndex1);
        pElement->Erase(pFirst);
        pElement->AddAt(pFirst, nIndex2);
    }
}

void CTreeLayoutUI::SortChildren(CContainerUI * pElement, TREESORTPROC pSortProc)
{
	if (pElement)
	{
		CTreeElementUI * pNode = static_cast<CTreeElementUI *>(pElement->GetInterface(kTreeElementInterface));
		if (pNode){
			CTreeChildElementUI * pChildElement = NULL;
			for (int i = 0; i < pElement->GetCount(); i++){
				pChildElement = static_cast<CTreeChildElementUI *>(pElement->GetItemAt(i)->GetInterface(kTreeChildElementInterface));
				if (pChildElement) break;
			}
			if (pChildElement){
				QuickSort(pChildElement, pSortProc, 0, pChildElement->GetCount() - 1);
			}
		}
		else{
			CTreeLayoutUI * pTreeLayout = static_cast<CTreeLayoutUI *>(pElement->GetInterface(kTreeLayoutInterface));
			if (pTreeLayout){
				QuickSort(pTreeLayout, pSortProc, 0, pTreeLayout->GetCount() - 1);
			}
		}
		Invalidate();
	}
}

void CTreeLayoutUI::SetIndent(int nIndentation)
{
	m_nIndentation = nIndentation;
}

int CTreeLayoutUI::GetIndent()
{
	return m_nIndentation;
}

}