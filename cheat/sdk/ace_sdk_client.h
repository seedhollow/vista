#pragma once

namespace SDK {
	using UR = UnityResolve;

	class AceSdkClient {
	public:
		static UR::Class* GetClass();

		static void Initialize( );
		// Function pointers
		inline static UR::Method* ace_sdk_client_init = nullptr;
		inline static UR::Method* ace_sdk_client_log_in = nullptr;
		inline static UR::Method* ace_sdk_client_get_packet = nullptr;
		inline static UR::Method* ace_sdk_client_on_packet_received = nullptr;
        inline static UR::Method* ace_sdk_client_get_light_feature_packet = nullptr;
	private:
		inline static UR::Class* pClass = nullptr;
		inline static bool initialized = false;
	};
} // namespace SDK
