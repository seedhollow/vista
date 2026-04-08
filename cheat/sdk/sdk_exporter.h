#pragma once

#include "object_translator_extensions.h"
#include "event_log_manager.h"
#include "gameplay_network.h"
#include "battle_recorder.h"
#include "battle_manager.h"
#include "ace_sdk_client.h"
#include "ace_sdk_interface_win.h"
#include "entity.h"
#include "attributes.h"
#include "player_controller.h"
#include "ability_system.h"
#include "object_container.h"
#include "entity_render_helper.h"
#include "game_util.h"
#include "skeletal_morph_component.h"
#include "animator.h"
#include "transform.h"
#include "chessboard.h"
#include "movement_component.h"
#include "movement_setting.h"
#include "modifier.h"
#include "skill.h"
#include "puzzle_game.h"
#include "game_instance.h"
#include "game_world.h"
#include "ui_text.h"
#include "complex_animator_component.h"
#include "play_cutscene_action.h"

namespace SDK {
    // need to call this once
    inline void InitializeSDK( ) {
        ObjectTranslatorExtensions::Initialize( );
        EventLogManager::Initialize( );
        GameplayNetwork::Initialize( );
        BattleRecorder::Initialize( );
        BattleManager::Initialize( );
        AceSdkClient::Initialize( );
        AceSdkInterfaceWin::Initialize( );
        Entity::Initialize( );
        Attributes::Initialize( );
        PlayerController::Initialize( );
        AbilitySystem::Initialize( );
        ObjectContainer::Initialize( );
        EntityRenderHelper::Initialize( );
		GameUtil::Initialize();
		SkeletalMorphComponent::Initialize();
		Animator::Initialize();
		Transform::Initialize();
		Chessboard::Initialize();
        MovementComponent::Initialize();
		MovementSetting::Initialize();
		Modifier::Initialize();
		Skill::Initialize();
		PuzzleGame::Initialize();
		GameInstance::Initialize();
		GameWorld::Initialize();
		UIText::Initialize();
        ComplexAnimatorComponent::Initialize();
		PlayCutsceneAction::Initialize();
    }

} // namespace SDK
