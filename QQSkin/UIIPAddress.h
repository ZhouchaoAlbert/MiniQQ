#ifndef __UIIPADDRESS_H__
#define __UIIPADDRESS_H__

//���ÿؼ����һ������dtstyle

namespace DuiLib
{
	class CIPAddressWnd;

	class  CIPAddressUI : public CLabelUI
	{
		friend class CIPAddressWnd;
	public:
		CIPAddressUI();
		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);

		DWORD GetIP();
		void SetIP(DWORD dwIP);

		void SetReadOnly(bool bReadOnly);
		bool IsReadOnly() const;

		void UpdateText();

		void DoEvent(TEventUI& event);

		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	protected:
		DWORD	m_dwIP;
		bool       m_bReadOnly;
		int		m_nIPUpdateFlag;

		CIPAddressWnd* m_pWindow;
	};
}
#endif // __UIIPADDRESS_H__