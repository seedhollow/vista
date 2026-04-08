#include "pch.h"
#include "c_dumb_enemies_feature.h"

namespace Features {

	static void hk_EnemyAIBrain_Tick(void* __this, float deltaTime) {

		if (player_func->dumb_enemies) 
		{
			UnityResolve::Get("Gameplay.Beyond.dll")->Get("EnemyAIBrain")->Get<UnityResolve::Method>("Pause")->Invoke<void>(__this);
		}
		else 
		{
			UnityResolve::Get("Gameplay.Beyond.dll")->Get("EnemyAIBrain")->Get<UnityResolve::Method>("Resume")->Invoke<void>(__this);
		}

		CALL_ORIGIN(hk_EnemyAIBrain_Tick, __this, deltaTime);
	}

	// System.Void TriggerSensor(UnityEngine.Vector3 pos, Beyond.Gameplay.AI.EnemyVigilanceSensorSource source);
	static void hk_EnemyAIBrain_TriggerSensor(void* __this, UnityResolve::UnityType::Vector3 pos, int source) {
		LOG("[+] TriggerSensor called with source: %d\n", source);
		if (player_func->dumb_enemies)
		{
			return;
		}
		CALL_ORIGIN(hk_EnemyAIBrain_TriggerSensor, __this, pos, source);
	}

	void CDumbEnemiesFeature::Initialize() {
		LOG("[+] CDumbEnemiesFeature initialized\n");
		//HookManager::install(UnityResolve::Get("Gameplay.Beyond.dll")->Get("EnemyAIBrain")->Get<UnityResolve::Method>("Tick")->function, hk_EnemyAIBrain_Tick);
		HookManager::install(UnityResolve::Get("Gameplay.Beyond.dll")->Get("EnemyAIBrain")->Get<UnityResolve::Method>("TriggerSensor")->function, hk_EnemyAIBrain_TriggerSensor);
	}
}