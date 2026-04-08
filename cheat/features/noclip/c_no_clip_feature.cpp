#include "pch.h"
#include "c_no_clip_feature.h"

namespace Features {
	void CNoClipFeature::Initialize() {
		LOG("[+] CNoClipFeature initialized\n");
	}


	void CNoClipFeature::Update() {
		if (player_func->no_clip) {
			// get player local
			auto player_local	= SDK::GameUtil::get_mainCharacter->Invoke<void*>();
			auto current_camera = SDK::GameUtil::get_mainCamera->Invoke<UnityResolve::UnityType::Camera*>();

			if (player_local && current_camera) {
				// get movement
				auto movement_component = SDK::Entity::get_movementComponent->Invoke<void*>(player_local);

				auto camera_transform = current_camera->GetTransform();

				UnityResolve::UnityType::Vector3 dir = {};

				if (ImGui::IsKeyDown(ImGuiKey_W)) {
					dir = dir + camera_transform->GetForward();
				}

				if (ImGui::IsKeyDown(ImGuiKey_S)) {
					dir = dir - camera_transform->GetForward();
				}

				if (ImGui::IsKeyDown(ImGuiKey_A)) {
					dir = dir - camera_transform->GetRight();
				}

				if (ImGui::IsKeyDown(ImGuiKey_D)) {
					dir = dir + camera_transform->GetRight();
				}

				// character going down
				if (ImGui::IsKeyDown(ImGuiKey_LeftShift)) {
					dir = dir - camera_transform->GetUp();
				}

				// character going up
				if (ImGui::IsKeyDown(ImGuiKey_Space)) {
					dir = dir + camera_transform->GetUp();
				}

				// get current position
				auto current_position = SDK::Entity::get_position->Invoke<UnityResolve::UnityType::Vector3>(player_local);

				// check if vector is valid
				if (dir.x != 0 || dir.y != 0 || dir.z != 0) {
					auto delta_time		= UnityResolve::UnityType::Time::GetDeltaTime();
					auto new_position	= current_position + dir * player_func->no_clip_speed * delta_time;

					SDK::MovementComponent::TeleportTo->Invoke<void>(movement_component, new_position, false);
				}
			}
		}
	}
} // namespace Features