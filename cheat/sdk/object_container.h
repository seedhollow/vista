#pragma once


namespace SDK {
    using UR = UnityResolve;

    class ObjectContainer {
    public:
		static UR::Class* GetClass();
        static void Initialize( );

        // Function pointers
        inline static UR::Method* Tick = nullptr;
        inline static UR::Method* get_allEntities = nullptr;
        inline static UR::Method* get_allCharacters = nullptr;
        inline static UR::Method* get_allEnemies = nullptr;
        inline static UR::Method* get_allInteractives = nullptr;
        inline static UR::Method* get_allAbilityEntities = nullptr;
		inline static UR::Method* Register = nullptr;
		inline static UR::Method* UnRegister = nullptr;
		inline static UR::Method* GetInteractiveByDistance = nullptr;

    private:
        inline static bool initialized = false;
		inline static UR::Class* pClass = nullptr;
    };
} // namespace SDK
