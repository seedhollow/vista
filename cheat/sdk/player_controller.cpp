#include "pch.h"
#include "player_controller.h"

namespace SDK {
    UR::Class* PlayerController::GetClass( ) {
		if (!pClass) {
			pClass = UR::Get("Gameplay.Beyond.dll")->Get("PlayerController");
		}
		return pClass;
    }

    void PlayerController::Initialize( ) {
        if (initialized)
            return;


		if (!GetClass()) {
			LOG("[-] PlayerController class not found!\n");
			return;
		}

		// fields
        f_mainCharacter                 = GetClass()->Get<UR::Field>("mainCharacter");

		// methods
		Tick                            = GetClass()->Get<UR::Method>("Tick");
		get_currentDashCount            = GetClass()->Get<UR::Method>("get_currentDashCount");
		set_currentDashCount            = GetClass()->Get<UR::Method>("set_currentDashCount");
		get_maxDashCount                = GetClass()->Get<UR::Method>("get_maxDashCount");
		set_maxDashCount                = GetClass()->Get<UR::Method>("set_maxDashCount");
		Jump                            = GetClass()->Get<UR::Method>("Jump");
		CheckCanDashB                   = GetClass()->Get<UR::Method>("CheckCanDashB");
		get_isMainCharacterSprinting    = GetClass()->Get<UR::Method>("get_isMainCharacterSprinting");
		CastNormalAttack                = GetClass()->Get<UR::Method>("CastNormalAttack");
		set_lastShownMaxDashCount       = GetClass()->Get<UR::Method>("set_lastShownMaxDashCount");
		get_lastShownMaxDashCount       = GetClass()->Get<UR::Method>("get_lastShownMaxDashCount");
		get_isFirstDash                 = GetClass()->Get<UR::Method>("get_isFirstDash");
		get_isWalk                      = GetClass()->Get<UR::Method>("get_isWalk");
		set_isWalk                      = GetClass()->Get<UR::Method>("set_isWalk");
		get_ultimateSpGainScalar        = GetClass()->Get<UR::Method>("get_ultimateSpGainScalar");
		DecreaseDashCount				= GetClass()->Get<UR::Method>("DecreaseDashCount");

        LOG("[+] PlayerController initialized.\n");
		LOG("[*] PlayerController::Tick: %p\n", Tick->function);
		LOG("[*] PlayerController::get_currentDashCount: %p\n", get_currentDashCount->function);
		LOG("[*] PlayerController::set_currentDashCount: %p\n", set_currentDashCount->function);
		LOG("[*] PlayerController::get_maxDashCount: %p\n", get_maxDashCount->function);
		LOG("[*] PlayerController::set_maxDashCount: %p\n", set_maxDashCount->function);
		LOG("[*] PlayerController::Jump: %p\n", Jump->function);
		LOG("[*] PlayerController::CheckCanDashB: %p\n", CheckCanDashB->function);
		LOG("[*] PlayerController::get_isMainCharacterSprinting: %p\n", get_isMainCharacterSprinting->function);
		LOG("[*] PlayerController::CastNormalAttack: %p\n", CastNormalAttack->function);
		LOG("[*] PlayerController::set_lastShownMaxDashCount: %p\n", set_lastShownMaxDashCount->function);
		LOG("[*] PlayerController::get_lastShownMaxDashCount: %p\n", get_lastShownMaxDashCount->function);
		LOG("[*] PlayerController::get_isFirstDash: %p\n", get_isFirstDash->function);
		LOG("[*] PlayerController::get_isWalk: %p\n", get_isWalk->function);
		LOG("[*] PlayerController::set_isWalk: %p\n", set_isWalk->function);
		LOG("[*] PlayerController::get_ultimateSpGainScalar: %p\n", get_ultimateSpGainScalar);
		LOG("[*] PlayerController::DecreaseDashCount: %p\n", DecreaseDashCount->function);


        initialized = true;

    }
} // namespace SDK
