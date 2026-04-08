#include "pch.h"
#include "c_visual_feature.h"
#include "cheat/tools/entity_manager.h"
#include "cheat/sdk/game_util.h"
#include "cheat/sdk/entity.h"
#include "cheat/sdk/skeletal_morph_component.h"
#include "cheat/sdk/animator.h"
#include "cheat/sdk/transform.h"
#include "cheat/sdk/object_type_enum.h"

namespace Features {
	constexpr float kDefaultCharHeight			= 2.0f;
	constexpr float kFeetOffset					= 0.2f;
	constexpr float kBoxAspectDivisor			= 4.0f;
	constexpr float kLabelPadding				= 5.0f;
	constexpr float kHealthBarExtraX			= kLabelPadding + kLabelPadding; // 10 px
	constexpr float kLabelLineGap				= 2.0f;
	constexpr float kHealthBarThickMin			= 2.0f;
	constexpr float kHealthBarThickMax			= 6.0f;

	using HumanBone = UT::Animator::HumanBodyBones;
	using BoneLink = std::pair<HumanBone, HumanBone>;

	const std::vector<BoneLink> kBoneConnections = {
		{ HumanBone::Hips,          HumanBone::Spine         },
		{ HumanBone::Spine,         HumanBone::Chest         },
		{ HumanBone::Chest,         HumanBone::Neck          },
		{ HumanBone::Neck,          HumanBone::Head          },
		// left arm
		{ HumanBone::Chest,         HumanBone::LeftShoulder  },
		{ HumanBone::LeftShoulder,  HumanBone::LeftUpperArm  },
		{ HumanBone::LeftUpperArm,  HumanBone::LeftLowerArm  },
		{ HumanBone::LeftLowerArm,  HumanBone::LeftHand      },
		// right arm
		{ HumanBone::Chest,         HumanBone::RightShoulder },
		{ HumanBone::RightShoulder, HumanBone::RightUpperArm },
		{ HumanBone::RightUpperArm, HumanBone::RightLowerArm },
		{ HumanBone::RightLowerArm, HumanBone::RightHand     },
		// left leg
		{ HumanBone::Hips,          HumanBone::LeftUpperLeg  },
		{ HumanBone::LeftUpperLeg,  HumanBone::LeftLowerLeg  },
		{ HumanBone::LeftLowerLeg,  HumanBone::LeftFoot      },
		// right leg
		{ HumanBone::Hips,          HumanBone::RightUpperLeg },
		{ HumanBone::RightUpperLeg, HumanBone::RightLowerLeg },
		{ HumanBone::RightLowerLeg, HumanBone::RightFoot     },
	};

	void CVisualFeature::Initialize() {
		LOG("[+] Visual Feature initialized\n");
	}

	void CVisualFeature::Render() {
		DrawESP();
	}

	ImU32 CVisualFeature::ToImColor(const float rgba[4]) {
		return IM_COL32(
			static_cast<int>(rgba[0] * 255),
			static_cast<int>(rgba[1] * 255),
			static_cast<int>(rgba[2] * 255),
			static_cast<int>(rgba[3] * 255)
		);
	}

	float CVisualFeature::CalcLabelOffsetX(const EntityBox& eb) const {
		float x = eb.rect.Max.x + kLabelPadding;
		if (esp_func->health && esp_func->health_type_index == 0)
			x += kHealthBarExtraX;
		return x;
	}

	bool CVisualFeature::Filter(const std::string& entity_name) const {
		bool any_enabled = false;

		for (int i = 0; i < esp_func->name_filter_count; ++i) {
			if (esp_func->name_filter_enabled[i]) {
				any_enabled = true;
				break;
			}
		}
		if (!any_enabled)
			return false;

		for (int i = 0; i < esp_func->name_filter_count; ++i) {
			if (!esp_func->name_filter_enabled[i])
				continue;
			const auto& prefix = esp_func->name_filter_keys[i];
			if (prefix.empty() || entity_name.starts_with(prefix))
				return true;
		}
		return false;
	}

	void CVisualFeature::DrawESP() {
		if (!esp_func->enable_esp)
			return;

		FrameCtx ctx;
		ctx.camera = SDK::GameUtil::get_mainCamera->Invoke<UT::Camera*>(nullptr);
		ctx.screen_w = UT::Screen::get_width();
		ctx.screen_h = UT::Screen::get_height();

		if (!ctx.camera)
			return;

		for (auto& entity : Manager::EntityManager::GetInstance().GetEntities()) {
			if (!entity.ptr)
				continue;

			auto* name_ptr = SDK::Entity::get_name->Invoke<UT::String*>(entity.ptr);
			if (!name_ptr)
				continue;

			if (!Filter(name_ptr->ToString()))
				continue;

			const bool needs_box = esp_func->box || esp_func->name || esp_func->health || esp_func->distance;

			EntityBox eb;

			if (needs_box)
				eb = CalculateEntityBoundingBox(entity.ptr, ctx);

			if (esp_func->snapline)
				DrawSnaplines(entity, ctx);

			if (esp_func->bones)
				DrawBone(entity.ptr, ctx);

			if (esp_func->box)
				DrawBox(entity.ptr, ctx, eb);

			if (eb.valid) {
				float label_y = eb.rect.Min.y;

				if (esp_func->name)
					label_y = DrawName(entity.ptr, ctx, eb, label_y);

				if (esp_func->health)
					label_y = DrawHealthBar(entity.ptr, ctx, eb, label_y);

				if (esp_func->distance)
					label_y = DrawDistance(entity.ptr, ctx, eb, label_y);
			}
		}
	}

	void CVisualFeature::DrawSnaplines(Entity entity, const FrameCtx& ctx) {
		auto w2s = ctx.camera->WorldToScreenPoint(SDK::Entity::get_position->Invoke<UT::Vector3>(entity.ptr));

		if (w2s.z <= 0.f || w2s.x < 0.f || w2s.x > ctx.screen_w ||
			w2s.y < 0.f || w2s.y > ctx.screen_h)
			return;

		auto draw_from = ImVec2(ctx.screen_w / 2.f, ctx.screen_h / 2.f);

		if (esp_func->snapline_type_index == 0) {
			draw_from = ImVec2(ctx.screen_w / 2.f, 0.f);
		}
		else if (esp_func->snapline_type_index == 1) {
			draw_from = ImVec2(ctx.screen_w / 2.f, ctx.screen_h / 2.f);
		}
		else if (esp_func->snapline_type_index == 2) {
			draw_from = ImVec2(ctx.screen_w / 2.f, ctx.screen_h);
		}

		if (w2s.z > 0.f &&
			w2s.x >= 0.f && w2s.x <= ctx.screen_w &&
			w2s.y >= 0.f && w2s.y <= ctx.screen_h) {

			ImGui::GetBackgroundDrawList()->AddLine(
				draw_from,
				ImVec2(w2s.x, ctx.screen_h - w2s.y),
				ToImColor(esp_func->snapline_color),
				1.f
			);
		}

	}

	void CVisualFeature::DrawBox(void* entity, const FrameCtx& ctx, const EntityBox& eb) {
		if (!eb.valid)
			return;

		const ImRect& box = eb.rect;
		const ImU32    color = ToImColor(esp_func->box_color);
		const float    thickness = esp_func->box_thickness;
		auto* dl = ImGui::GetBackgroundDrawList();

		switch (esp_func->box_type_index) {
		case 0:
			dl->AddRect(box.Min, box.Max, color, 0.f, 0, thickness);
			break;


		case 1: {
			const float bw = box.Max.x - box.Min.x;
			const float bh = box.Max.y - box.Min.y;
			const float corner = (std::min)(bw * 0.5f, bh * 0.15f);

			// tl
			dl->AddLine(box.Min, { box.Min.x + corner, box.Min.y }, color, thickness);
			dl->AddLine(box.Min, { box.Min.x, box.Min.y + corner }, color, thickness);
			// tr
			dl->AddLine({ box.Max.x, box.Min.y }, { box.Max.x - corner, box.Min.y }, color, thickness);
			dl->AddLine({ box.Max.x, box.Min.y }, { box.Max.x, box.Min.y + corner }, color, thickness);
			// bl
			dl->AddLine({ box.Min.x, box.Max.y }, { box.Min.x + corner, box.Max.y }, color, thickness);
			dl->AddLine({ box.Min.x, box.Max.y }, { box.Min.x, box.Max.y - corner }, color, thickness);
			// br
			dl->AddLine(box.Max, { box.Max.x - corner, box.Max.y }, color, thickness);
			dl->AddLine(box.Max, { box.Max.x, box.Max.y - corner }, color, thickness);
			break;
		}

		case 2: {
			static auto* rvo_class = UnityResolve::Get("Gameplay.Beyond.dll")->Get("RVOComponent");

			auto* rvo = SDK::Entity::get_rvoComponent->Invoke<void*>(entity);
			if (!rvo) return;

			const float height = rvo_class->GetValue<float>(rvo, "m_height");
			const float radius = rvo_class->GetValue<float>(rvo, "m_radius");
			auto pos = SDK::Entity::get_position->Invoke<UT::Vector3>(entity);

			const UT::Vector3 corners[8] = {
				{ pos.x - radius, pos.y,          pos.z - radius },
				{ pos.x + radius, pos.y,          pos.z - radius },
				{ pos.x + radius, pos.y,          pos.z + radius },
				{ pos.x - radius, pos.y,          pos.z + radius },
				{ pos.x - radius, pos.y + height, pos.z - radius },
				{ pos.x + radius, pos.y + height, pos.z - radius },
				{ pos.x + radius, pos.y + height, pos.z + radius },
				{ pos.x - radius, pos.y + height, pos.z + radius },
			};

			UT::Vector3 sc[8];
			for (int i = 0; i < 8; ++i) {
				sc[i] = ctx.camera->WorldToScreenPoint(corners[i]);
				if (sc[i].z <= 0.f) return; // any corner behind camera — skip ok.
			}

			auto screen_pt = [&](int i) -> ImVec2 {
				return { sc[i].x, ctx.screen_h - sc[i].y };
				};

			// pillar
			for (int i = 0; i < 4; ++i) {
				dl->AddLine(screen_pt(i), screen_pt((i + 1) % 4), color, thickness); // bottom
				dl->AddLine(screen_pt(i + 4), screen_pt((i + 1) % 4 + 4), color, thickness); // top
				dl->AddLine(screen_pt(i), screen_pt(i + 4), color, thickness); // pillar
			}
			break;
		}

		default: break;
		}
	}

	void CVisualFeature::DrawBone(void* entity, const FrameCtx& ctx) {
		auto* root = SDK::Entity::get_rootCom->Invoke<void*>(entity);
		auto* go = UnityResolve::Get("Gameplay.Beyond.dll")
			->Get("RootComponent")
			->Get<UnityResolve::Method>("get_gameObject")
			->Invoke<UT::GameObject*>(root);

		auto* animator = go->GetComponentInChildren<UT::Animator*>(SDK::Animator::GetClass());
		if (!animator)
			return;

		const ImU32   color = ToImColor(esp_func->bones_color);
		const float   thickness = esp_func->bones_thickness;
		auto* dl = ImGui::GetBackgroundDrawList();

		for (const auto& [parent_bone, child_bone] : kBoneConnections) {
			auto* t_start = SDK::Animator::GetBoneTransformInternal->Invoke<UT::Transform*>(animator, static_cast<int>(parent_bone));
			auto* t_end = SDK::Animator::GetBoneTransformInternal->Invoke<UT::Transform*>(animator, static_cast<int>(child_bone));

			if (!t_start || !t_end)
				continue;

			auto w2s_start = ctx.camera->WorldToScreenPoint(
				SDK::Transform::get_position->Invoke<UT::Vector3>(t_start));
			auto w2s_end = ctx.camera->WorldToScreenPoint(
				SDK::Transform::get_position->Invoke<UT::Vector3>(t_end));

			if (w2s_start.z <= 0.f || w2s_end.z <= 0.f)
				continue;

			dl->AddLine(
				{ w2s_start.x, ctx.screen_h - w2s_start.y },
				{ w2s_end.x, ctx.screen_h - w2s_end.y },
				color, thickness
			);
		}
	}

	float CVisualFeature::DrawName(void* entity, const FrameCtx& ctx, const EntityBox& eb, float offset_y) {
		auto* name_ptr = SDK::Entity::get_name->Invoke<UT::String*>(entity);

		if (!name_ptr)
			return offset_y;

		const auto    name = name_ptr->ToString();
		const ImVec2  size = ImGui::CalcTextSize(name.c_str());

		ImGui::GetBackgroundDrawList()->AddText(
			{ CalcLabelOffsetX(eb), offset_y },
			ToImColor(esp_func->name_color),
			name.c_str()
		);

		return offset_y + size.y + kLabelLineGap;
	}


	float CVisualFeature::DrawHealthBar(void* entity, const FrameCtx& ctx, const EntityBox& eb, float offset_y) {
		auto* ability = SDK::Entity::get_abilityCom->Invoke<void*>(entity);

		if (!ability)
			return offset_y;

		const double hp		= SDK::AbilitySystem::get_hp->Invoke<double>(ability);
		const double max_hp = SDK::AbilitySystem::get_maxHp->Invoke<double>(ability);
		const double pct	= (max_hp > 0.0) ? (hp / max_hp) : 0.0;
		const ImU32  color	= ToImColor(esp_func->health_color);
		auto* dl = ImGui::GetBackgroundDrawList();

		switch (esp_func->health_type_index) {
		case 0: {
			const float dist_sq		= SDK::Entity::GetClass()->GetValue<float>(entity, SDK::Entity::f_distance2MainCharSq->offset);
			const float distance	= std::sqrt(dist_sq);
			const float thickness	= std::clamp(8.f / (distance * 0.1f), kHealthBarThickMin, kHealthBarThickMax);
			const float bar_x		= eb.rect.Max.x + kLabelPadding;
			const float bh			= eb.rect.Max.y - eb.rect.Min.y;

			dl->AddRectFilled(
				{ bar_x,             eb.rect.Max.y - static_cast<float>(bh * pct) },
				{ bar_x + thickness, eb.rect.Max.y },
				color
			);

			dl->AddRect(
				{ bar_x,             eb.rect.Min.y },
				{ bar_x + thickness, eb.rect.Max.y },
				color
			);
			return offset_y;
		}

			  // --- Text label "100HP / 200HP" ---
		case 1: {
			const std::string text =
				std::to_string(static_cast<int>(hp)) + "HP / " +
				std::to_string(static_cast<int>(max_hp)) + "HP";
			const ImVec2 size = ImGui::CalcTextSize(text.c_str());

			dl->AddText({ CalcLabelOffsetX(eb), offset_y }, color, text.c_str());
			return offset_y + size.y + kLabelLineGap;
		}

		default:
			return offset_y;
		}
	}

	float CVisualFeature::DrawDistance(void* entity, const FrameCtx& ctx, const EntityBox& eb, float offset_y) {
		const float dist_sq = SDK::Entity::GetClass()->GetValue<float>(entity, SDK::Entity::f_distance2MainCharSq->offset);
		const std::string text = std::to_string(static_cast<int>(std::sqrt(dist_sq))) + " meters";
		const ImVec2 size = ImGui::CalcTextSize(text.c_str());

		ImGui::GetBackgroundDrawList()->AddText(
			{ CalcLabelOffsetX(eb), offset_y },
			ToImColor(esp_func->distance_color),
			text.c_str()
		);

		return offset_y + size.y + kLabelLineGap;
	}

	CVisualFeature::EntityBox CVisualFeature::CalculateEntityBoundingBox(void* entity, const FrameCtx& ctx)	{
		EntityBox result;

		static auto* rvo_class = UnityResolve::Get("Gameplay.Beyond.dll")->Get("RVOComponent");
		auto* rvo = SDK::Entity::get_rvoComponent->Invoke<void*>(entity);

		const float height = rvo ? rvo_class->GetValue<float>(rvo, "m_height") : kDefaultCharHeight;

		auto origin = SDK::Entity::get_position->Invoke<UT::Vector3>(entity);

		UT::Vector3 head = origin; head.y += height;
		UT::Vector3 feet = origin; feet.y -= kFeetOffset;

		auto w2s_head = ctx.camera->WorldToScreenPoint(head);
		auto w2s_feet = ctx.camera->WorldToScreenPoint(feet);

		if (w2s_head.z <= 0.f || w2s_feet.z <= 0.f)
			return result;

		const float screen_head_y = ctx.screen_h - w2s_head.y;
		const float screen_feet_y = ctx.screen_h - w2s_feet.y;
		const float box_h = std::abs(screen_head_y - screen_feet_y);
		const float box_w = box_h / kBoxAspectDivisor;
		const float center_x = w2s_feet.x;

		result.rect = ImRect({ center_x - box_w, screen_head_y },
			{ center_x + box_w, screen_feet_y });
		result.valid = (result.rect.Min.x >= 0.f && result.rect.Max.x <= ctx.screen_w &&
			result.rect.Min.y >= 0.f && result.rect.Max.y <= ctx.screen_h);
		return result;
	}
} // namespace Features