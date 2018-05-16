#include "DesktopToast.h"

const wchar_t AppId[] = L"wpn.toast.win";
const char WINDOW_CLASS_NAME[] = "wpnToast";
const char WINDOW_TITLE[] = "wpn toast";
DesktopToast* DesktopToast::s_currentInstance = nullptr;

std::wstring utf8wstring(const std::string& s, const UINT codepage = CP_UTF8)
{
	int len;
	int slength = (int) s.length() + 1;
	len = MultiByteToWideChar(codepage, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(codepage, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

DesktopToast *DesktopToast::GetInstance()
{
	return s_currentInstance;
}

DesktopToast::DesktopToast()
{
	s_currentInstance = this;
	HRESULT hr = RegisterAppForNotificationSupport();
}

DesktopToast::~DesktopToast()
{
	UnregisterActivator();
	s_currentInstance = nullptr;
}

// In order to display toasts, a desktop application must have a shortcut on the Start menu.
// Also, an AppUserModelID must be set on that shortcut.
//
// For the app to be activated from Action Center, it needs to register a COM server with the OS
// and register the CLSID of that COM server on the shortcut.
//
// The shortcut should be created as part of the installer. The following code shows how to create
// a shortcut and assign the AppUserModelID and ToastActivatorCLSID properties using Windows APIs.
//
// Included in this project is a wxs file that be used with the WiX toolkit
// to make an installer that creates the necessary shortcut. One or the other should be used.
//
// This sample doesn't clean up the shortcut or COM registration.

HRESULT DesktopToast::RegisterAppForNotificationSupport()
{
	CoTaskMemString appData;
	auto hr = ::SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, appData.GetAddressOf());
	if (SUCCEEDED(hr))
	{
		wchar_t shortcutPath[MAX_PATH];
		hr = ::PathCchCombine(shortcutPath, ARRAYSIZE(shortcutPath), appData.Get(), LR"(Microsoft\Windows\Start Menu\Programs\wpn notify.lnk)");
		if (SUCCEEDED(hr))
		{
			DWORD attributes = ::GetFileAttributesW(shortcutPath);
			bool fileExists = attributes < 0xFFFFFFF;
			if (!fileExists)
			{
				wchar_t exePath[MAX_PATH];
				DWORD charWritten = ::GetModuleFileNameW(nullptr, exePath, ARRAYSIZE(exePath));
				hr = charWritten > 0 ? S_OK : HRESULT_FROM_WIN32(::GetLastError());
				if (SUCCEEDED(hr))
				{
					hr = InstallShortcut(shortcutPath, exePath);
					if (SUCCEEDED(hr))
					{
						hr = RegisterComServer(exePath);
					}
				}
			}
		}
	}
	return hr;
}

_Use_decl_annotations_
HRESULT DesktopToast::InstallShortcut(PCWSTR shortcutPath, PCWSTR exePath)
{
	ComPtr<IShellLink> shellLink;
	HRESULT hr = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&shellLink));
	if (SUCCEEDED(hr))
	{
		hr = shellLink->SetPath((LPCSTR) exePath);
		if (SUCCEEDED(hr))
		{
			ComPtr<IPropertyStore> propertyStore;

			hr = shellLink.As(&propertyStore);
			if (SUCCEEDED(hr))
			{
				PROPVARIANT propVar;
				propVar.vt = VT_LPWSTR;
				propVar.pwszVal = const_cast<PWSTR>(AppId); // for _In_ scenarios, we don't need a copy
				hr = propertyStore->SetValue(PKEY_AppUserModel_ID, propVar);
				if (SUCCEEDED(hr))
				{
					propVar.vt = VT_CLSID;
					propVar.puuid = const_cast<CLSID*>(&__uuidof(NotificationActivator));
					hr = propertyStore->SetValue(PKEY_AppUserModel_ToastActivatorCLSID, propVar);
					if (SUCCEEDED(hr))
					{
						hr = propertyStore->Commit();
						if (SUCCEEDED(hr))
						{
							ComPtr<IPersistFile> persistFile;
							hr = shellLink.As(&persistFile);
							if (SUCCEEDED(hr))
							{
								hr = persistFile->Save(shortcutPath, TRUE);
							}
						}
					}
				}
			}
		}
	}
	return hr;
}

_Use_decl_annotations_
HRESULT DesktopToast::RegisterComServer(PCWSTR exePath)
{
	// We don't need to worry about overflow here as ::GetModuleFileName won't
	// return anything bigger than the max file system path (much fewer than max of DWORD).
	DWORD dataSize = static_cast<DWORD>((::wcslen(exePath) + 1) * sizeof(WCHAR));

	// In this sample, the app UI is registered to launch when the COM callback is needed.
	// Other options might be to launch a background process instead that then decides to launch
	// the UI if needed by that particular notification.
	return HRESULT_FROM_WIN32(::RegSetKeyValueW(
		HKEY_CURRENT_USER,
		LR"(SOFTWARE\Classes\CLSID\{23A5B06E-20BB-4E7E-A0AC-6982ED6A6041}\LocalServer32)",
		nullptr,
		REG_SZ,
		reinterpret_cast<const BYTE*>(exePath),
		dataSize));
}

// Register activator for notifications
HRESULT DesktopToast::RegisterActivator()
{
	// Module<OutOfProc> needs a callback registered before it can be used.
	// Since we don't care about when it shuts down, we'll pass an empty lambda here.
	Module<OutOfProc>::Create([] {});

	// If a local server process only hosts the COM object then COM expects
	// the COM server host to shutdown when the references drop to zero.
	// Since the user might still be using the program after activating the notification,
	// we don't want to shutdown immediately.  Incrementing the object count tells COM that
	// we aren't done yet.
	Module<OutOfProc>::GetModule().IncrementObjectCount();

	return Module<OutOfProc>::GetModule().RegisterObjects();
}

// Unregister our activator COM object
void DesktopToast::UnregisterActivator()
{
	Module<OutOfProc>::GetModule().UnregisterObjects();
	Module<OutOfProc>::GetModule().DecrementObjectCount();
}

// Display the toast using classic COM. Note that is also possible to create and
// display the toast using the new C++ /ZW options (using handles, COM wrappers, etc.)
HRESULT DesktopToast::DisplayToast(const NotifyMessage *request)
{
	ComPtr<IToastNotificationManagerStatics> toastStatics;
	HRESULT hr = Windows::Foundation::GetActivationFactory(
		HStringReference(RuntimeClass_Windows_UI_Notifications_ToastNotificationManager).Get(),
		&toastStatics);
	if (SUCCEEDED(hr))
	{
		ComPtr<IXmlDocument> toastXml;
		hr = CreateToastXml(toastStatics.Get(), &toastXml, request);
		if (SUCCEEDED(hr))
		{
			hr = CreateToast(toastStatics.Get(), toastXml.Get());
		}
	}
	return hr;
}

// Create the toast XML from a template
_Use_decl_annotations_
HRESULT DesktopToast::CreateToastXml
(
	IToastNotificationManagerStatics* toastManager, 
	IXmlDocument** inputXml, 
	const NotifyMessage *request
)
{
	*inputXml = nullptr;

	// Retrieve the template XML
	HRESULT hr = toastManager->GetTemplateContent(ToastTemplateType_ToastImageAndText04, inputXml);
	if (SUCCEEDED(hr))
	{
		PWSTR imagePath = _wfullpath(nullptr, L"toastImageAndText.png", MAX_PATH);
		hr = imagePath != nullptr ? S_OK : HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
		if (SUCCEEDED(hr))
		{
			hr = SetImageSrc(imagePath, *inputXml);
			if (SUCCEEDED(hr))
			{
				std::vector<std::string> a;
				if (request)
				{
					a.push_back(request->title);
					a.push_back(request->body);
				}
				hr = SetTextValues(a, *inputXml);
			}
			free(imagePath);
		}
	}
	return hr;
}

// Set the value of the "src" attribute of the "image" node
_Use_decl_annotations_
HRESULT DesktopToast::SetImageSrc(PCWSTR imagePath, IXmlDocument* toastXml)
{
	wchar_t imageSrcUri[MAX_PATH];
	DWORD size = ARRAYSIZE(imageSrcUri);

	HRESULT hr = ::UrlCreateFromPathW(imagePath, imageSrcUri, &size, 0);
	if (SUCCEEDED(hr))
	{
		ComPtr<IXmlNodeList> nodeList;
		hr = toastXml->GetElementsByTagName(HStringReference(L"image").Get(), &nodeList);
		if (SUCCEEDED(hr))
		{
			ComPtr<IXmlNode> imageNode;
			hr = nodeList->Item(0, &imageNode);
			if (SUCCEEDED(hr))
			{
				ComPtr<IXmlNamedNodeMap> attributes;

				hr = imageNode->get_Attributes(&attributes);
				if (SUCCEEDED(hr))
				{
					ComPtr<IXmlNode> srcAttribute;

					hr = attributes->GetNamedItem(HStringReference(L"src").Get(), &srcAttribute);
					if (SUCCEEDED(hr))
					{
						hr = SetNodeValueString(HStringReference(imageSrcUri).Get(), srcAttribute.Get(), toastXml);
					}
				}
			}
		}
	}
	return hr;
}

// Set the values of each of the text nodes
_Use_decl_annotations_
HRESULT DesktopToast::SetTextValues
(
	const std::vector<std::string> &lines, 
	IXmlDocument* toastXml
)
{
	ComPtr<IXmlNodeList> nodeList;
	HRESULT hr = toastXml->GetElementsByTagName(HStringReference(L"text").Get(), &nodeList);
	if (SUCCEEDED(hr))
	{
		UINT32 nodeListLength;
		hr = nodeList->get_Length(&nodeListLength);
		if (SUCCEEDED(hr))
		{
			// If a template was chosen with fewer text elements, also change the amount of strings passed to this method.
			hr = lines.size() <= nodeListLength ? S_OK : E_INVALIDARG;
			if (SUCCEEDED(hr))
			{
				for (UINT32 i = 0; i < lines.size(); i++)
				{
					ComPtr<IXmlNode> textNode;
					hr = nodeList->Item(i, &textNode);
					if (SUCCEEDED(hr))
					{
						std::wstring s = utf8wstring(lines[i]);
						LPCWSTR v = s.c_str();
						hr = SetNodeValueString(HStringReference(v).Get(), textNode.Get(), toastXml);
					}
				}
			}
		}
	}

	return hr;
}

_Use_decl_annotations_
HRESULT DesktopToast::SetNodeValueString(HSTRING inputString, IXmlNode* node, IXmlDocument* xml)
{
	ComPtr<IXmlText> inputText;
	HRESULT hr = xml->CreateTextNode(inputString, &inputText);
	if (SUCCEEDED(hr))
	{
		ComPtr<IXmlNode> inputTextNode;
		hr = inputText.As(&inputTextNode);
		if (SUCCEEDED(hr))
		{
			ComPtr<IXmlNode> appendedChild;
			hr = node->AppendChild(inputTextNode.Get(), &appendedChild);
		}
	}

	return hr;
}

// Create and display the toast
_Use_decl_annotations_
HRESULT DesktopToast::CreateToast
(
	IToastNotificationManagerStatics* toastManager, 
	IXmlDocument* xml
)
{
	ComPtr<IToastNotifier> notifier;
	HRESULT hr = toastManager->CreateToastNotifierWithId(HStringReference(AppId).Get(), &notifier);
	if (SUCCEEDED(hr))
	{
		ComPtr<IToastNotificationFactory> factory;
		hr = Windows::Foundation::GetActivationFactory(
			HStringReference(RuntimeClass_Windows_UI_Notifications_ToastNotification).Get(),
			&factory);
		if (SUCCEEDED(hr))
		{
			ComPtr<IToastNotification> toast;
			hr = factory->CreateToastNotification(xml, &toast);
			if (SUCCEEDED(hr))
			{
				// Register the event handlers
				//
				// These handlers are called asynchronously.  This sample doesn't handle the
				// the fact that these events could be raised after the app object has already
				// been decontructed.
				EventRegistrationToken activatedToken, dismissedToken, failedToken;

				using namespace ABI::Windows::Foundation;

				hr = toast->add_Activated
				(
					Callback < Implements < RuntimeClassFlags<ClassicCom>,
					ITypedEventHandler<ToastNotification*, IInspectable* >> >(
						[](IToastNotification*, IInspectable*
				)
				{
					// When the user clicks or taps on the toast, the registered
					// COM object is activated, and the Activated event is raised.
					// There is no guarantee which will happen first. If the COM
					// object is activated first, then this message may not show.
					// TODO add some reaction
					return S_OK;
				}).Get(),
					&activatedToken);

				if (SUCCEEDED(hr))
				{
					hr = toast->add_Dismissed(Callback < Implements < RuntimeClassFlags<ClassicCom>,
						ITypedEventHandler<ToastNotification*, ToastDismissedEventArgs* >> >(
							[](IToastNotification*, IToastDismissedEventArgs* e)
					{
						ToastDismissalReason reason;
						if (SUCCEEDED(e->get_Reason(&reason)))
						{
							PCWSTR outputText;
							switch (reason)
							{
							case ToastDismissalReason_ApplicationHidden:
								outputText = L"The application hid the toast using ToastNotifier.hide()";
								break;
							case ToastDismissalReason_UserCanceled:
								outputText = L"The user dismissed this toast";
								break;
							case ToastDismissalReason_TimedOut:
								outputText = L"The toast has timed out";
								break;
							default:
								outputText = L"Toast not activated";
								break;
							}

							// TODO show result outputText
						}
						return S_OK;
					}).Get(),
						&dismissedToken);
					if (SUCCEEDED(hr))
					{
						hr = toast->add_Failed(Callback < Implements < RuntimeClassFlags<ClassicCom>,
							ITypedEventHandler<ToastNotification*, ToastFailedEventArgs* >> >(
								[](IToastNotification*, IToastFailedEventArgs* /*e */)
						{
							// TODO Show error "The toast encountered an error."
							return S_OK;
						}).Get(),
							&failedToken);

						if (SUCCEEDED(hr))
						{
							hr = notifier->Show(toast.Get());
						}
					}
				}

			}
		}
	}
	return hr;
}

HRESULT STDMETHODCALLTYPE NotificationActivator::Activate
(
	_In_ LPCWSTR /*appUserModelId*/,
	_In_ LPCWSTR /*invokedArgs*/,
	/*_In_reads_(dataCount)*/ const NOTIFICATION_USER_INPUT_DATA* /*data*/,
	ULONG /*dataCount*/
)
{
	// TODO "NotificationActivator - The user clicked on the toast." << std::endl;
	return S_OK;
}

CoCreatableClass(NotificationActivator);
