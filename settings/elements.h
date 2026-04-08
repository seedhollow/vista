#pragma once
#include <string>
#include "imgui.h"
#include <memory>
#include <vector>

enum notify_position : int
{
	top_left = 0,
	top_right = 1,
	bottom_left = 2,
	bottom_right = 3,
};

struct keybind_checkbox
{
	bool* callback;
	int* key;
	int* mode;
};

class c_elements
{
public:

	struct
	{

		float animation_tickrate = 12.f;
		float rounding = 100.f;

	} checkbox;

	struct
	{
		float animation_tickrate = 15.f;
		float slider_size = 6.f;
		float rounding = 100.f;

		float value_padding = 10.f;
		float clickable_height = 11.f;
		float line_padding = 3.f;
		float value_height = 20.f;

	} slider;

	struct
	{
		float animation_tickrate = 14.f;
		float selection_size = 35.f;
		float dropdown_size = 35.f;
		float rounding = 6.f;

		bool opened_combo = false;

	} dropdown;

	struct
	{
		float animation_tickrate = 14.f;
		float rounding = 12.f;

	} selection;

	struct
	{
		float animation_tickrate = 14.f;
		float rounding = 6.f;

		float popup_rounding = 10.f;
		ImVec2 padding{ 10, 10 };
		ImVec2 spacing{ 10, 10 };

		bool open_popup = false;

		int index = -1;
		std::vector<keybind_checkbox> update_keybind_system;

	} keybind;

	struct
	{
		float animation_tickrate = 14.f;
		float rounding = 6.f;

		ImVec2 padding{ 10, 10 };
		ImVec2 spacing{ 10, 10 };

		float selection_rounding = 6.f;

	} listbox;

	struct
	{
		float border_size = 0.5f;
		float rounding = 6.f;
		ImVec2 padding{ 15, 15 };

		bool hovered = false;

	} tooltip;

	struct
	{
		float animation_tickrate = 14.f;
		float rounding = 6.f;

		float popup_rounding = 10.f;
		ImVec2 padding{ 10, 10 };
		ImVec2 spacing{ 10, 10 };

		bool open_picker = false;

	} color_picker;

	struct
	{
		float animation_tickrate = 10.f;
		float rounding = 6.f;

		bool open_popup = false;

	} settings_popup;

	struct
	{
		float animation_tickrate = 16.f;
		float popup_rounding = 10.f;
		float rounding = 6.f;

	} text_field;

	struct
	{
		float animation_tickrate = 22.f;
		float rounding = 6.f;

	} button;

	struct
	{

		std::vector<std::string> notify_list{ "Top left", "Top right", "Bottom left", "Bottom right" };

		int transparent_walls = 100;

		ImVec2 min_line_size{ 240, 5 };
		ImVec2 padding{ 15, 15 };

		float rounding = 6.f;
		float screen_padding = 20;
		float cover_screen = 100;
		float height = 60;

		int notify_pos = 0;

	} notify;
};

inline std::unique_ptr<c_elements> element = std::make_unique<c_elements>();