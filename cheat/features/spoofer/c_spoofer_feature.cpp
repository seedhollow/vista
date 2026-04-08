#include "pch.h"
#include "c_spoofer_feature.h"

namespace Features {
	std::string random_string_number(size_t length) {
		const std::string characters = "0123456789";
		std::random_device rd;
		std::mt19937 generator(rd());
		std::uniform_int_distribution<> distribution(0, characters.size() - 1);
		std::string random_str;
		for (size_t i = 0; i < length; ++i) {
			random_str += characters[distribution(generator)];
		}
		return random_str;
	}


	// use this if you want to know something like this: UINode/UIRoot/UIDPanelPanel/Main/TextNode/Text
	// its not perfect but it works
	std::string ui_path_finder(UnityResolve::UnityType::Component* component) {
		auto transform = component->GetTransform();
		if (transform) {
			auto position = transform->GetPosition();
			std::vector<std::string> path;

			while (transform) {
				path.push_back(transform->GetName()->ToString());
				transform = transform->GetParent();
			}

			std::reverse(path.begin(), path.end());

			std::string full_path;

			for (const auto& p : path) {
				full_path += p + "/";
			}

			LOG("Full path: %s\n", full_path.c_str());
			return full_path;
		}
		else {
			LOG("No transform found for the component.\n");
			return "";
		}
	}

	void CSpooferFeature::Initialize() {
		LOG("[+] CSpooferFeature initialized\n");
	}

	void CSpooferFeature::Update() {
		auto bottom_uid_game_object		 = UnityResolve::UnityType::GameObject::Find("UINode/UIRoot/UIDPanelPanel/Main/TextNode/Text");
		auto clock_uid_game_object		 = UnityResolve::UnityType::GameObject::Find("UINode/WorldUIRoot/WatchPanel/Content/WatchNode/canvas_watch/LeftBottomNode/PlayInfoPosNode/PlayInfoCell(Clone)/PlayerInfo/ManagerNumber");
		auto friend_card_uid_game_object = UnityResolve::UnityType::GameObject::Find("UINode/UIRoot/FriendBusinessCardRootPanel/CardRoot/FriendBusinessCard(Clone)/BusinessCardState/Ani/ContentNode/LeftNode/AniNode/LeftLayout/LayoutInfo/PersonalInfoNode/PlayerUidTxt");

		auto random_str = "UID:" + random_string_number(10); // on the game, uid length is 10, you can change this idfc

		if (!is_current_uid_retrieved || misc_func->current_uid[0] == '\0') { 
			
			auto text_component = bottom_uid_game_object->GetComponent<UnityResolve::UnityType::Component*>(SDK::UIText::GetClass());
			auto buffer			= SDK::UIText::get_text->Invoke<UnityResolve::UnityType::String*>(text_component)->ToString();

			std::snprintf(misc_func->current_uid	, sizeof(misc_func->current_uid) , "%s", buffer.c_str());
			std::snprintf(misc_func->fake_uid		, sizeof(misc_func->fake_uid)	 , "%s", buffer.c_str());
			
			is_current_uid_retrieved = true;
		}

		auto uid_buffer = misc_func->randomize_uid ? random_str : misc_func->fake_uid;

		// apply 
		apply_uid(bottom_uid_game_object, uid_buffer.c_str());
		apply_uid(clock_uid_game_object, uid_buffer.c_str());
		apply_uid(friend_card_uid_game_object, uid_buffer.c_str());
	}


	// game object should be UIText
	void CSpooferFeature::apply_uid(UnityResolve::UnityType::GameObject* game_object, const char* uid) {
		if (game_object) {
			auto text_component = game_object->GetComponent<UnityResolve::UnityType::Component*>(SDK::UIText::GetClass());
			if (text_component) {
				SDK::UIText::set_text->Invoke<void>(text_component, UnityResolve::UnityType::String::New(uid));
			}
		}
	}
}