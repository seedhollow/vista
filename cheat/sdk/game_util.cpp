#include "pch.h"
#include "game_util.h"

namespace SDK {
	UR::Class* GameUtil::GetClass() {
		if (!pClass) {
			pClass = UR::Get("Gameplay.Beyond.dll")->Get("GameUtil");
		}
		return pClass;
	}

	void GameUtil::Initialize() {
		if (initialized)
			return;

		if (!GetClass()) {
			LOG("[-] GameUtil class not found!\n");
			return;
		}

		get_mainCharacter	= GetClass()->Get <UR::Method>("get_mainCharacter");
		get_mainCamera		= GetClass()->Get<UR::Method>("get_mainCamera");
		get_playerTrans		= GetClass()->Get<UR::Method>("get_playerTrans");
		get_playerPos		= GetClass()->Get<UR::Method>("get_playerPos");

		initialized = true;
		LOG("[+] GameUtil initialized.\n");
	}
}