#include "pch.h"
#include "ability_system.h"

namespace SDK {
    UR::Class* AbilitySystem::GetClass() {
        if (!pClass) {
            pClass = UR::Get("Gameplay.Beyond.dll")->Get("AbilitySystem");
        }
		return pClass;
    }

    void AbilitySystem::Initialize( ) {
        if (initialized)
            return;

        if (!GetClass()) {
            LOG("[-] AbilitySystem class not found!\n");
            return;
        }
        set_ultimateSp      = GetClass()->Get<UR::Method>("set_ultimateSp", {"System.Single"});
        get_ultimateSp      = GetClass()->Get<UR::Method>("get_ultimateSp");
        get_hp              = GetClass()->Get<UR::Method>("get_hp");
        get_maxHp           = GetClass()->Get<UR::Method>("get_maxHp");
        SetHpDirectly       = GetClass()->Get<UR::Method>("SetHpDirectly");
        get_detectedHeight  = GetClass()->Get<UR::Method>("get_detectedHeight");
		get_objectType      = GetClass()->Get<UR::Method>("get_objectType");

        initialized = true;
        LOG("[+] AbilitySystem initialized.\n");
		LOG("[*] AbilitySystem::set_ultimateSp: %p\n", set_ultimateSp->function);
		LOG("[*] AbilitySystem::get_ultimateSp: %p\n", get_ultimateSp->function);
		LOG("[*] AbilitySystem::get_hp: %p\n", get_hp->function);
		LOG("[*] AbilitySystem::get_maxHp: %p\n", get_maxHp->function);
		LOG("[*] AbilitySystem::SetHpDirectly: %p\n", SetHpDirectly->function);
		LOG("[*] AbilitySystem::get_detectedHeight: %p\n", get_detectedHeight->function);
		LOG("[*] AbilitySystem::get_objectType: %p\n", get_objectType->function);
    }
} // namespace SDK
