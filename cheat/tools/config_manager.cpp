#include "pch.h"
#include "config_manager.h"


void ConfigManager::install(const std::string& filename, std::shared_ptr<IConfigSection> section) 
{
	registry_[filename].push_back(std::move(section));
}

void ConfigManager::set_config_dir(const std::string& directory)
{
    configDir_ = directory;
    std::filesystem::create_directories(configDir_);
}

void ConfigManager::save(const std::string& filename) 
{
    json root = load_raw_(filename);

    for (auto& sec : section_for_(filename))
        sec->serialize(root[sec->section_key()]);

    write_raw_(filename, root);
}

void ConfigManager::load(const std::string& filename)
{
    json root = load_raw_(filename);

    for (auto& sec : section_for_(filename)) {
        const std::string key = sec->section_key();
        if (root.contains(key))
            sec->deserialize(root[key]);
    }
}

void ConfigManager::load_all()
{
    std::vector<std::string> files;
    {
        files.reserve(registry_.size());
        for (auto& [f, _] : registry_) files.push_back(f);
    }
    for (auto& f : files) load(f);
}


json ConfigManager::load_raw_(const std::string& filename)
{
    std::filesystem::path p = configDir_ / filename;
    if (!std::filesystem::exists(p)) return json::object();
    std::ifstream ifs(p);
    if (!ifs.is_open()) return json::object();
    try { 
        return json::parse(ifs); 
    } catch (...) { 
        return json::object(); 
    }
}


void ConfigManager::write_raw_(const std::string& filename, const json& root)
{
    std::filesystem::path p = configDir_ / filename;
    std::ofstream ofs(p);
    if (!ofs.is_open())
        throw std::runtime_error("ConfigManager: cannot write " + p.string());
    ofs << root.dump(4);
}