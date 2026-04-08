#pragma once

namespace SDK {
    using UR = UnityResolve;

    class AbilitySystem {
    public:
		static UR::Class* GetClass();
        static void Initialize( );
        // Function pointers
        inline static UR::Method* get_ultimateSp = nullptr;
        inline static UR::Method* set_ultimateSp = nullptr;
        inline static UR::Method* get_hp = nullptr;
        inline static UR::Method* get_maxHp = nullptr;
        inline static UR::Method* SetHpDirectly = nullptr;
        inline static UR::Method* get_detectedHeight = nullptr;
		inline static UR::Method* get_objectType = nullptr;


    private:
		inline static UR::Class* pClass = nullptr;
        inline static bool initialized = false;
    };
} // namespace SDK
