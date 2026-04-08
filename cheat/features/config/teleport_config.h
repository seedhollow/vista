#pragma once
#include "pch.h"
#include "../../tools/config_section_interface.h"

using UT = UnityResolve::UnityType;

struct TeleportEntry {
	std::string name;
	UT::Vector3 position;
};

class TeleportConfig : public IConfigSection {
public:
	std::vector<TeleportEntry> entries;

	std::string section_key() const override { return "teleports"; }

	void serialize(json& out) const override {
		out = json::array();
		for (const auto& e : entries) {
			out.push_back(
				{
					{"name", e.name},
					{"x",    e.position.x},
					{"y",    e.position.y},
					{"z",    e.position.z}
				}
			);
		}
	}

	void deserialize(const json& in) override {
		entries.clear();
		if (!in.is_array()) return;
		for (const auto& item : in) {
			TeleportEntry e;
			e.name = item.value("name", "Unnamed");
			e.position.x = item.value("x", 0.f);
			e.position.y = item.value("y", 0.f);
			e.position.z = item.value("z", 0.f);
			entries.push_back(std::move(e));
		}
	}

	void add(const std::string& name, UT::Vector3 pos) {
		entries.push_back({ name, pos });
	}

	void remove(std::size_t index) {
		if (index < entries.size())
			entries.erase(entries.begin() + static_cast<std::ptrdiff_t>(index));
	}

	const TeleportEntry* get(std::size_t index) const {
		if (index < entries.size())
			return &entries[index];
		return nullptr;
	}
};