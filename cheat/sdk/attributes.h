#pragma once

namespace SDK {
    using UR = UnityResolve;

    class Attributes {
    public:
		static UR::Class* GetClass();
        static void Initialize( );

        // Function pointers
        inline static void* GetValue = nullptr;
    private:
		inline static UR::Class* pClass = nullptr;
        inline static bool initialized = false;
    };
} // namespace SDK
