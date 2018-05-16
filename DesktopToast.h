#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <SDKDDKVer.h>
#include <Windows.h>
#include <Psapi.h>
#include <strsafe.h>
#include <ShObjIdl.h>
#include <Shlobj.h>
#include <Pathcch.h>
#include <propvarutil.h>
#include <propkey.h>
#include <wrl.h>
#include <wrl\wrappers\corewrappers.h>
#include <windows.ui.notifications.h>
#include "NotificationActivationCallback.h"

//  Name:     System.AppUserModel.ToastActivatorCLSID -- PKEY_AppUserModel_ToastActivatorCLSID
//  Type:     Guid -- VT_CLSID
//  FormatID: {9F4C2855-9F79-4B39-A8D0-E1D42DE1D5F3}, 26
//  
//  Used to CoCreate an INotificationActivationCallback interface to notify about toast activations.
EXTERN_C const PROPERTYKEY DECLSPEC_SELECTANY PKEY_AppUserModel_ToastActivatorCLSID = { { 0x9F4C2855, 0x9F79, 0x4B39,{ 0xA8, 0xD0, 0xE1, 0xD4, 0x2D, 0xE1, 0xD5, 0xF3 } }, 26 };

using namespace ABI::Windows::Data::Xml::Dom;
using namespace ABI::Windows::UI::Notifications;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

struct CoTaskMemStringTraits
{
	typedef PWSTR Type;
	inline static bool Close(_In_ Type h) throw() { ::CoTaskMemFree(h); return true; }
	inline static Type GetInvalidValue() throw() { return nullptr; }
};
typedef HandleT<CoTaskMemStringTraits> CoTaskMemString;

class DesktopToast
{
public:
	static DesktopToast* GetInstance();
	DesktopToast();
	~DesktopToast();
	HRESULT Initialize(_In_ HINSTANCE hInstance);
	void RunMessageLoop();

	HRESULT SetMessage(PCWSTR message);

	HRESULT DisplayToast();

private:
	HRESULT RegisterAppForNotificationSupport();
	HRESULT InstallShortcut(_In_ PCWSTR shortcutPath, _In_ PCWSTR exePath);
	HRESULT RegisterComServer(_In_ PCWSTR exePath);

	HRESULT RegisterActivator();
	void UnregisterActivator();

	static LRESULT CALLBACK WndProc(
		_In_ HWND hWnd,
		_In_ UINT message,
		_In_ WPARAM wParam,
		_In_ LPARAM lParam
	);


	HRESULT CreateToastXml(
		_In_ ABI::Windows::UI::Notifications::IToastNotificationManagerStatics* toastManager,
		_COM_Outptr_ ABI::Windows::Data::Xml::Dom::IXmlDocument** xml
	);

	HRESULT CreateToast(
		_In_ ABI::Windows::UI::Notifications::IToastNotificationManagerStatics* toastManager,
		_In_ ABI::Windows::Data::Xml::Dom::IXmlDocument* xml
	);
	HRESULT DesktopToast::SetImageSrc(
		_In_ PCWSTR imagePath,
		_In_ ABI::Windows::Data::Xml::Dom::IXmlDocument* toastXml
	);
	HRESULT DesktopToast::SetTextValues(
		_In_reads_(textValuesCount) const PCWSTR* textValues,
		_In_ UINT32 textValuesCount,
		_Inout_ ABI::Windows::Data::Xml::Dom::IXmlDocument* toastXml
	);
	HRESULT DesktopToast::SetNodeValueString(
		_In_ HSTRING onputString,
		_Inout_ ABI::Windows::Data::Xml::Dom::IXmlNode* node,
		_In_ ABI::Windows::Data::Xml::Dom::IXmlDocument* xml
	);

	HWND m_hwnd = nullptr;
	HWND m_hEdit = nullptr;

	static const WORD HM_TEXTBUTTON = 1;

	static DesktopToast* s_currentInstance;
};


// For the app to be activated from Action Center, it needs to provide a COM server to be called
// when the notification is activated.  The CLSID of the object needs to be registered with the
// OS via its shortcut so that it knows who to call later.
class DECLSPEC_UUID("23A5B06E-20BB-4E7E-A0AC-6982ED6A6041") NotificationActivator WrlSealed
	: public RuntimeClass < RuntimeClassFlags<ClassicCom>,
	INotificationActivationCallback > WrlFinal
{
public:
	virtual HRESULT STDMETHODCALLTYPE Activate
	(
		_In_ LPCWSTR /*appUserModelId*/,
		_In_ LPCWSTR /*invokedArgs*/,
		/*_In_reads_(dataCount)*/ const NOTIFICATION_USER_INPUT_DATA* /*data*/,
		ULONG /*dataCount*/) override;
};
