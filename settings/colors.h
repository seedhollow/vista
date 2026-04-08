#pragma once
#include "imgui.h"
#include <memory>

	class c_colors
	{
	public:

		struct
		{

			ImColor layout = ImColor(17, 17, 20);
			ImColor header = ImColor(22, 22, 27);
			ImColor stroke = ImColor(32, 32, 44);

		} window;

		struct
		{
			ImColor layout = ImColor(21, 21, 26);

		} child;

		struct
		{
			ImColor accent = ImColor(119, 112, 255);

			ImColor white = ImColor(255, 255, 255, 255);
			ImColor black = ImColor(0, 0, 0, 255);

			ImColor element_separator = ImColor(28, 28, 37, 255);
			ImColor open_popup = ImColor(24, 24, 31, 255);

			ImColor particle = ImColor(26, 26, 34, 255);

		} other;

		struct
		{
			ImColor circle = ImColor(63, 63, 80);
			ImColor layout = ImColor(27, 27, 34);

		} checkbox;

		struct
		{
			ImColor layout = ImColor(27, 27, 34);

		} slider;

		struct
		{
			ImColor layout = ImColor(27, 27, 34);
			ImColor layout_hovered = ImColor(34, 34, 44);

		} button;

		struct
		{
			ImColor layout = ImColor(27, 27, 34);
			ImColor selection = ImColor(34, 34, 44);
			ImColor another_dropdown = ImColor(31, 31, 40);

		} dropdown;

		struct
		{
			ImColor layout_selection = ImColor(28, 28, 36);
			ImColor layout = ImColor(22, 22, 27);

		} selection;

		struct
		{
			ImColor layout = ImColor(31, 31, 40);		

		} keybind;

		struct
		{
			ImColor col_bg_alpha_0 = ImColor(65, 65, 75);
			ImColor col_bg_alpha_1 = ImColor(45, 45, 55);

			ImColor button_layout = ImColor(31, 31, 40);
			ImColor color_selection = ImColor(46, 46, 61);

			ImColor white_color = ImColor(255, 255, 255);
			ImColor black_color = ImColor(0, 0, 0);

		} color_picker;

		struct
		{
			ImColor layout = ImColor(17, 17, 20);
			ImColor line = ImColor(27, 27, 34);
			ImColor desc = ImColor(255, 255, 255);

			ImColor success = ImColor(130, 255, 128);
			ImColor warning = ImColor(255, 167, 99);
			ImColor error = ImColor(255, 124, 124);

		} notify;

		struct
		{
			ImColor layout = ImColor(17, 17, 20, 255);
			ImColor border = ImColor(17, 17, 20, 80);

		} watermark;

		struct
		{
			ImColor text_active = ImColor(255, 255, 255);
			ImColor text_hovered = ImColor(105, 105, 135);
			ImColor text_inactive = ImColor(63, 63, 80);

		} text;
	};

inline std::unique_ptr<c_colors> clr = std::make_unique<c_colors>();