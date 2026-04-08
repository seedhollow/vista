#pragma once

#include "config_section_interface.h"
#include <filesystem>


class ConfigManager {
public:
	static ConfigManager& getInstance() {
		static ConfigManager instance;
		return instance;
	}

	ConfigManager(const ConfigManager&) = delete;
	ConfigManager& operator=(const ConfigManager&) = delete;

	void set_config_dir(const std::string& directory);
	void install(const std::string& filename, std::shared_ptr<IConfigSection> section);
	void save(const std::string& filename);
	void load(const std::string& filename);
	void load_all();

private:
	ConfigManager() = default;
	std::filesystem::path configDir_{ "./config" };
	std::unordered_map<std::string,
	std::vector<std::shared_ptr<IConfigSection>>> registry_;


	std::vector<std::shared_ptr<IConfigSection>>& section_for_(const std::string& filename) {
		return registry_[filename];
	}

	json load_raw_(const std::string& filename);
	void write_raw_(const std::string& filename, const json& root);
};