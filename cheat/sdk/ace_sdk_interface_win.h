#pragma once

namespace SDK {
    using UR = UnityResolve;

    class AceSdkInterfaceWin {
    public:
		static UR::Class* GetClass();
        static void Initialize( );
        // Function pointers
        inline static UR::Method* InitACESDK = nullptr;
        inline static UR::Method* StartNetThread = nullptr;
        inline static UR::Method* NetworkThread = nullptr;
        inline static UR::Method* SendPktToGameServer = nullptr;
    private:
		inline static UR::Class* pClass = nullptr;
        inline static bool initialized = false;
    };
} // namespace SDK
