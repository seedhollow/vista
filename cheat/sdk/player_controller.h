#pragma once
namespace SDK {
    using UR = UnityResolve;

    class PlayerController {
    public:
        static void Initialize( );

        static UR::Class* GetClass();

        // Function pointers
        inline static UR::Method* Tick = nullptr;
        inline static UR::Method* get_maxDashCount = nullptr;
        inline static UR::Method* set_maxDashCount = nullptr;
        inline static UR::Method* get_currentDashCount = nullptr;
        inline static UR::Method* set_currentDashCount = nullptr;
        inline static UR::Method* Jump = nullptr;
        inline static UR::Method* CheckCanDashB = nullptr;
        inline static UR::Method* get_isMainCharacterSprinting = nullptr;
        inline static UR::Method* CastNormalAttack = nullptr;
        inline static UR::Method* set_lastShownMaxDashCount = nullptr;
        inline static UR::Method* get_lastShownMaxDashCount = nullptr;
        inline static UR::Method* get_isFirstDash = nullptr;
        inline static UR::Method* get_isWalk = nullptr;
        inline static UR::Method* set_isWalk = nullptr;
        inline static UR::Method* get_ultimateSpGainScalar = nullptr;
		inline static UR::Method* DecreaseDashCount = nullptr;

        inline static UR::Field* f_mainCharacter = nullptr;
    private:
        inline static bool initialized = false;
		inline static UR::Class* pClass = nullptr;
    };
} // namespace SDK
