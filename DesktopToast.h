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

#include <vector>
#include <string>
#include "NotificationActivationCallback.h"
#include "wpn-notify.h"

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
	DesktopToast();
	~DesktopToast();
	HRESULT DisplayToast(const NotifyMessageC *request);
private:
	HRESULT RegisterAppForNotificationSupport();
	HRESULT InstallShortcut(_In_ PCWSTR shortcutPath, _In_ PCWSTR exePath);
	HRESULT RegisterComServer(_In_ PCWSTR exePath);

	HRESULT RegisterActivator();
	void UnregisterActivator();

	HRESULT CreateToastXml(
		_In_ ABI::Windows::UI::Notifications::IToastNotificationManagerStatics* toastManager,
		_COM_Outptr_ ABI::Windows::Data::Xml::Dom::IXmlDocument** xml,
		const NotifyMessageC *request
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
		const std::vector<std::string> &lines,
		_Inout_ ABI::Windows::Data::Xml::Dom::IXmlDocument* toastXml
	);
	HRESULT DesktopToast::SetNodeValueString(
		_In_ HSTRING onputString,
		_Inout_ ABI::Windows::Data::Xml::Dom::IXmlNode* node,
		_In_ ABI::Windows::Data::Xml::Dom::IXmlDocument* xml
	);
};
