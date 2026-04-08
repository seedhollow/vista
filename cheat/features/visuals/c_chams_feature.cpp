#include "pch.h"
#include "c_chams_feature.h"
#include "cheat/tools/entity_manager.h"
#include <unordered_set>

namespace Features {

    // -------------------------------------------------------------------------
	// Notes : these features are very heavy on performance
	//         i think adding cache for materials would be a good idea
	// -------------------------------------------------------------------------

    void CChamsFeature::Initialize() {
        LOG("[+] Chams Feature initialized\n");
    }

    void CChamsFeature::Render() {

    }

    void CChamsFeature::Update() {
        bool chamsEnabled = esp_func->chams;
        if (!chamsEnabled)
			return;

        const auto& entities = Manager::EntityManager::GetInstance().GetEntities();

        for (auto& entity : entities) {
            if (!entity.ptr)
                continue;
            DrawChams(entity.ptr);
        }
    }


    void CChamsFeature::DrawChams(void* entity) {
        static auto root_component_class    = UnityResolve::Get("Gameplay.Beyond.dll")->Get("RootComponent");

        auto root_component = SDK::Entity::get_rootCom->Invoke<void*>(entity);
        if (!root_component)
            return;

        auto player_game_object = root_component_class->Get<UnityResolve::Method>("get_gameObject") ->Invoke<UnityResolve::UnityType::GameObject*>(root_component);
        if (!player_game_object)
            return;

        auto renderers = player_game_object->GetComponentsInChildren<UT::Renderer*>(UnityResolve::Get("UnityEngine.CoreModule.dll")->Get("Renderer"));

        for (auto& renderer : renderers) {
            if (!renderer)
                continue;

            auto materials = renderer->GetMaterials();
            for (auto& material : materials) {
                if (!material)
                    continue;

                auto color = UT::Color(
                    esp_func->chams_visible_color[0],
                    esp_func->chams_visible_color[1],
                    esp_func->chams_visible_color[2],
                    esp_func->chams_visible_color[3]
				);

                bool has_base_color = material->HasProperty("_BaseColor");
                bool has_tint_color = material->HasProperty("_TintColor");
                bool has_z_buffer   = material->HasProperty("_ZTest");

                // set color
                if (has_base_color) {
					material->SetColor("_BaseColor", color);
                }

                if (has_tint_color) {
					material->SetColor("_TintColor", color);
                }

                if (has_z_buffer) {
					material->SetFloat("_ZTest", esp_func->chams_disabled_depth_buffer ? 8.f : 4.f); // 8 = always, 4 = less equal
				}
            }
        }
    }
} // namespace Features