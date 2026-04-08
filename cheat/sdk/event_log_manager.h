#pragma once
namespace SDK {
    using UR = UnityResolve;

    class EventLogManager {
    public:
		static UR::Class* GetClass();
        static void Initialize( );

        // Function pointers
        inline static UR::Method* GameEvent_BattleAnticheat = nullptr;
    private:
        inline static bool initialized = false;
		inline static UR::Class* pClass = nullptr;
    };
}
