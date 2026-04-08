#pragma once
#include "pch.h"

using json = nlohmann::json;

class IConfigSection {
public:
	virtual ~IConfigSection() = default;
	virtual void serialize(json& out) const = 0;
	virtual void deserialize(const json& in) = 0;
	virtual std::string section_key() const = 0;
};