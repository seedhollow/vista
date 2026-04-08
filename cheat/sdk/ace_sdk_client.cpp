#include "pch.h"
#include "ace_sdk_client.h"
namespace SDK {
	UR::Class* AceSdkClient::GetClass() {
		if (!pClass) {
			pClass = UR::Get("ACESDK_WIN.dll")->Get("AceSdkClient");
		}
		return pClass;
	}

	void AceSdkClient::Initialize( ) {
		if (initialized)
			return;
	
		if (!GetClass()) {
			LOG("[-] AceSdkClient class not found!\n");
			return;
        }

		ace_sdk_client_init						= GetClass()->Get<UR::Method>("ace_sdk_client_init", {"AceSdkClient&"});
        ace_sdk_client_log_in					= GetClass()->Get<UR::Method>("ace_sdk_client_log_in", {"System.String", "AceSdkAccountType", "System.UInt32", "System.String"});
        ace_sdk_client_get_packet				= GetClass()->Get<UR::Method>("ace_sdk_client_get_packet", {"System.Byte[]&"});
        ace_sdk_client_on_packet_received		= GetClass()->Get<UR::Method>("ace_sdk_client_on_packet_received", {"System.Byte[]"});
        ace_sdk_client_get_light_feature_packet = GetClass()->Get<UR::Method>("ace_sdk_client_get_light_feature_packet", {"System.Byte[]&"});

		LOG("[+] AceSdkClient initialized.\n");
		LOG("[*] AceSdkClient::ace_sdk_client_init: %p\n", ace_sdk_client_init->function);
		LOG("[*] AceSdkClient::ace_sdk_client_log_in: %p\n", ace_sdk_client_log_in->function);
		LOG("[*] AceSdkClient::ace_sdk_client_get_packet: %p\n", ace_sdk_client_get_packet->function);
		LOG("[*] AceSdkClient::ace_sdk_client_on_packet_received: %p\n", ace_sdk_client_on_packet_received->function);
		LOG("[*] AceSdkClient::ace_sdk_client_get_light_feature_packet: %p\n", ace_sdk_client_get_light_feature_packet->function);

		initialized = true;
	}
} // namespace SDK
