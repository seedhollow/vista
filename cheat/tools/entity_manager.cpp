#include "pch.h"
#include "entity_manager.h"
#include "cheat/sdk/game_util.h"
#include <algorithm>

namespace Manager {
	void EntityManager::AddEntity(Entity src) {
		// Check if entity already exists
		auto it = std::find_if(m_entities.begin(), m_entities.end(),
			[src](const Entity& ent) { return ent.ptr == src.ptr; });

		if (it == m_entities.end()) {
			m_entities.push_back({ src });
		}
	}

	void EntityManager::SetEntities(const std::vector<Entity>& src) {
		m_entities.clear();
		for (Entity entity : src) {
			if (entity.ptr != nullptr) {
				m_entities.push_back({ entity });
			}
		}
	}

	void EntityManager::RemoveEntity(Entity src) {
		m_entities.erase(
			std::remove_if(m_entities.begin(), m_entities.end(),
				[src](const Entity& entity) { return entity.ptr == src.ptr; }),
			m_entities.end()
		);
	}

	void EntityManager::Clear() {
		m_entities.clear();
	}

	Entity EntityManager::GetLocalPlayer() const {
		auto main_char = SDK::GameUtil::get_mainCharacter->Invoke<void*>();
		if (!main_char) {
			throw std::runtime_error("Failed to get local player in EntityManager::GetLocalPlayer");
		}

		Entity localPlayer;
		localPlayer.ptr = main_char;

		return localPlayer;
	}

	const std::vector<Entity>& EntityManager::GetEntities() const {
		return m_entities;
	}

	Entity EntityManager::GetEntity(size_t index) {
		if (index >= m_entities.size()) {
			throw std::out_of_range("Index out of range in EntityManager::GetEntity");
		}
		return m_entities[index];
	}

	size_t EntityManager::GetEntityCount() const {
		return m_entities.size();
	}
}