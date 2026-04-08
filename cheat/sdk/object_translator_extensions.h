#pragma once
#include <cstdint>

namespace SDK {
    using UR = UnityResolve;

    class ObjectTranslatorExtensions {
    public:

		static UR::Class* GetClass();
        static void Initialize( );

        // Function pointers
        inline static UR::Method* UpdateBeyondGEnumsAntiCheatBehaviourEnum = nullptr;
        inline static UR::Method* PushBeyondGEnumsAntiCheatBehaviourEnum = nullptr;
        inline static UR::Method* Get = nullptr;
		inline static UR::Method* UpdateBeyondNetworkBanRoleType = nullptr;
		inline static UR::Method* PushBeyondNetworkBanRoleType = nullptr;

    private:
        inline static bool initialized = false;
		inline static UR::Class* pClass = nullptr;
    };
} // namespace SDK
