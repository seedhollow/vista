#include "pch.h"
#include "ace_sdk_interface_win.h"

namespace SDK {
	UR::Class* AceSdkInterfaceWin::GetClass() {
		if (!pClass) {
			pClass = UR::Get("ACESDK_WIN.dll")->Get("AceSDKInterfaceWin");
		}
		return pClass;
	}
	void AceSdkInterfaceWin::Initialize( ) {
		if (initialized)
			return;

        if (!GetClass()) {
			LOG("[-] AceSdkInterfaceWin class not found! \n");
			return;
        }

		InitACESDK			= GetClass()->Get<UR::Method>("InitACESDK");
		StartNetThread		= GetClass()->Get<UR::Method>("StartNetThread");
		NetworkThread		= GetClass()->Get<UR::Method>("NetworkThread", {"AceSdkClient"});
		SendPktToGameServer = GetClass()->Get<UR::Method>("SendPktToGameServer", {"System.Byte[]"});

		LOG("[+] AceSdkInterfaceWin initialized.\n");
		LOG("[*] AceSdkInterfaceWin::InitACESDK: %p\n", InitACESDK->function);
		LOG("[*] AceSdkInterfaceWin::StartNetThread: %p\n", StartNetThread->function);
		LOG("[*] AceSdkInterfaceWin::NetworkThread: %p\n", NetworkThread->function);
		LOG("[*] AceSdkInterfaceWin::SendPktToGameServer: %p\n", SendPktToGameServer->function);

		initialized = true;
	}
} // namespace SDK
