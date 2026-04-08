#pragma once

#include <string>
#include <vector>
#include <wtypes.h>
#include <d3d11.h>
#include <memory>

enum watermark_position : int {
	mark_top_left = 0,
	mark_top_right = 1,
	mark_bottom_left = 2,
	mark_bottom_right = 3,
};

class c_variables
{
public:
	struct
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

		std::string window_name = "SEEDHOLLOW";

		ImVec2 window_padding{ 0, 10 };
		ImVec2 item_spacing{ 0, 0 };
		ImVec2 size{ 695, 675 };

		float layout_rounding = 12;
		float scroll_size = 20;
		float border_size = 0;
	} window;

	struct {
		std::string version = "1.0.0";
		bool is_debug_mode = false;
	} flavor;

	struct
	{
		ImVec2 child_padding{ 15, 0 };
		ImVec2 child_spacing{ 0, 0 };

		float child_rounding = 12;
		int unique_id = -1;
	} child;

	struct
	{
		std::vector<std::vector<std::string>> selection_list = { { "A", "B", "D", "C"}, {"Visual", "Player", "World", "Config"}};

		int page = 0;
		int selection = 0;
		int selection_active = 0;

		float selection_alpha = 1, selection_add;
	} selection;

	struct
	{
		std::vector<std::string> theme_list = { "N", "M" };
		std::vector<std::string> language_list = { "ID", "ENG" };

		int selection_theme = 0;
		int selection_language = 1, selection_language_active = 1;
	} list_page;

	struct
	{
		std::vector<std::string> watermark_content = { "SEEDHOLLOW", "Endfield" };
		std::vector<std::string> watermark_list = { "Top left", "Top right", "Bottom left", "Bottom right" };

		bool watermark = true;
		bool water_name = false;

		int watermark_selection = 1;
		float rounding = 8.f;
	} watermark;

	struct
	{
		ImFont* inter_bold[3];
		ImFont* inter_medium[3];
		ImFont* inter_semibold[3];
		ImFont* icon[6];
	} font;

	struct
	{
		bool show_menu = true;
		bool particle = false;
		float particle_color[4] = { 118 / 255.f, 119 / 255.f, 130 / 255.f, 0.3f };
		float accent[4] = { 249 / 255.f, 48 / 255.f, 120 / 255.f, 1.f };
	} gui;

	struct
	{
		HWND hwnd;
		RECT rc;
	} winapi;

	struct
	{
		bool dpi_changed = true;

		float dpi = 1.f;
		int stored_dpi = 100;
	} dpi;

};

inline std::unique_ptr<c_variables> var = std::make_unique<c_variables>();

class c_esp_function
{
public:

	static constexpr int name_filter_count = 16; // must be the same as the size of name_filter_labels and name_filter_keys

	std::vector<std::string> name_filter_labels = {
		"All",
		"Enemy",
		"Mushroom",        "Aurylene",       "Flower",
		"Chest",           "Chest Key",      "Protocol",
		"Challenge",       "Originium Ore",  "Ametis Ore",
		"Drop",            "Item Drop",      "SOS Robot",
		"Teleport Point",  "Kaboom Spawner",
	};

	std::vector<std::string> name_filter_keys = {
		"",
		"eny_",
		"int_doodad_mushroom",             "int_collection_coin_puzzle",			"int_doodad_flower",
		"int_trchest",                     "int_trchest_bubble",					"int_blackbox_entry",
		"int_challenge_start_point_once",  "int_doodad_ore_cluster_originium",		"int_doodad_ore_cluster_quartz",
		"int_collection",                  "int_drop",								"int_fixable_robot",
		"int_campfire",                    "int_interact_bomb_spawner",
	};

	// enable for enemy for the first time
	bool name_filter_enabled[name_filter_count] = { 
		false, true, false, false, 
		false, false, false, false, 
		false, false, false, false, 
		false, false, false, false 
	};

	bool enable_esp = false;
	int esp_keybind = 0;
	int esp_mode = 0;
	float esp_distance = 100.f;

	bool box = false;
	float box_thickness = 1.f;
	// green
	float box_color[4] = { 0 / 255.f, 255 / 255.f, 0 / 255.f, 1.f };
	std::vector<std::string> box_type_list{ "2D", "Corner", "3D" };
	int box_type_index = 1;

	bool health = false;
	float health_color[4] = { 255 / 255.f, 255 / 255.f, 0 / 255.f, 1.f };
	std::vector<std::string> health_type_list{ "Bar", "Number" };
	int health_type_index = 0;

	bool name = false;
	float name_color[4] = { 0 / 255.f, 255 / 255.f, 0 / 255.f, 1.f };

	bool snapline = false;
	float snapline_color[4] = { 0 / 255.f, 255 / 255.f, 0 / 255.f, 1.f };
	std::vector<std::string> snapline_pos_list{ "Top Center", "Center", "Bottom Center" };
	int snapline_type_index = 0;

	bool bones = false;
	float bones_color[4] = { 0 / 255.f, 255 / 255.f, 0 / 255.f, 1.f };
	float bones_thickness = 1.f;

	bool chams = false;
	float chams_visible_color[4] = { 0 / 255.f, 255 / 255.f, 0 / 255.f, 1.f };
	bool chams_disabled_depth_buffer = false;

	bool distance = false;
	float distance_color[4] = { 0 / 255.f, 255 / 255.f, 0 / 255.f, 1.f };
};

inline c_esp_function* esp_func = new c_esp_function();

class c_misc_function
{
public:
	// uid spoof
	bool randomize_uid = false;
	bool show_real_uid = false;
	char fake_uid[64] = { "" };
	char current_uid[64] = { "" };

	// camera fov
	bool camera_fov = false;
	float camera_fov_value = 60.f;

	// world level
	int world_level_value = 1;

	// fps unlock
	int fps_limit = 60;
	int fps_limit_index = 0;
	int fps_limit_list[5] = { 60, 120, 144, 240, 0 };
	std::vector<std::string> fps_limit_list_str{ "60", "120", "144", "240", "Uncapped" };
};

inline c_misc_function* misc_func = new c_misc_function();

class c_player_function
{
public:
	bool godmode = false;
	bool infinite_skill_ultimate = false;
	bool no_skill_cost = false;
	bool no_cooldown = false;

	bool no_clip = false;
	float no_clip_speed = 10.f;

	int no_clip_keybind_forward = 0;
	int no_clip_keybind_back = 0;
	int no_clip_keybind_left = 0;
	int no_clip_keybind_right = 0;
	int no_clip_keybind_up = 0;
	int no_clip_keybind_down = 0;
	std::vector<std::string> no_clip_keybind_list_str{"A", "D", "W", "S", "Space", "Left Ctrl" };
	int no_clip_keybind_index = 0;

	// movement
	bool perfect_dodge = false;

	bool movespeed_multiplier = false;
	int movespeed_multiplier_value = 1;
	int movespeed_keybind = 0;
	int movespeed_mode = 0;

	bool jump_multiplier = false;
	int jump_multiplier_value = 1;
	int jump_keybind = 0;
	int jump_mode = 0;

	bool infinite_stamina = false;

	// combat
	bool hit_multiplier = false;
	int hit_multiplier_value = 1;
	int hit_multiplier_keybind = 0;
	int hit_multiplier_mode = 0;

	bool attack_speed_multiplier = false;
	int attack_speed_multiplier_value = 1;

	bool damage_multiplier = false;
	int damage_multiplier_value = 1;
	int damage_multiplier_keybind = 0;
	int damage_multiplier_mode = 0;

	bool dumb_enemies = false;

	bool always_crit = false;

	bool peeking = false;

	bool auto_kill_enemies = false;
	int auto_kill_enemies_keybind = 0;
	int auto_kill_enemies_mode = 0;

	bool auto_solve_puzzles = false;
};

inline c_player_function* player_func = new c_player_function();

class c_world_function
{
public:
	bool remove_fog = false;
	bool remove_rain = false;
	bool remove_snow = false;
	bool time_of_day = false;
	float time_of_day_value = 12.f;
	bool weather = false;
	int weather_type_index = 0;
	std::vector<std::string> weather_type_list{ "Clear", "Rain", "Snow" };

	// fog color
	float fog_distance = 300.f;
	float fog_color[4] = { 0.5f, 0.5f, 0.5f, 1.f };

	// time scale
	int time_scale_value = 1;
};

inline c_world_function* world_func = new c_world_function();


class c_game_information
{
public:
	std::string game_version;
};

inline c_game_information* game_info = new c_game_information();