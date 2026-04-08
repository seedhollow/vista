#include "pch.h"
#include "menu.h"
#include "settings/functions.h"
#include "../cheat/features/config/teleport_config.h"

particle_system particle_active(165);

namespace Menu
{
	void Render()
	{
		theme_color();
		notify->setup_notify();
		widget->register_keybind();

		gui->set_next_window_size(SCALE(var->window.size));

		gui->begin();
		{
			clr->other.accent = ImVec4(var->gui.accent[0], var->gui.accent[1], var->gui.accent[2], 1.f);

			const ImVec2 pos = gui->get_window_pos();
			const ImVec2 size = gui->get_window_size();

			ImDrawList* draw_list = GetWindowDrawList();
			ImGuiStyle* style = &GetStyle();

			{
				style->WindowBorderSize = SCALE(var->window.border_size);
				style->WindowRounding = SCALE(var->window.layout_rounding);
				style->WindowPadding = SCALE(var->window.window_padding);

				style->ScrollbarSize = SCALE(var->window.scroll_size);
				style->ItemSpacing = SCALE(var->window.item_spacing);
			}

			{
				draw->rect_filled(draw_list, pos, pos + size, draw->get_clr(clr->window.layout), SCALE(var->window.layout_rounding));

				if (var->gui.particle) particle_active.update_and_render();

				draw->rect_filled(draw_list, pos, pos + ImVec2(size.x, SCALE(50)), draw->get_clr(clr->window.header), SCALE(var->window.layout_rounding), ImDrawFlags_RoundCornersTop);
				draw->line(draw_list, pos + ImVec2(0, SCALE(50)), pos + ImVec2(size.x, SCALE(50)), draw->get_clr(clr->window.stroke), 0.5f);

				draw->colored_text(var->font.inter_bold[0], pos + SCALE(15, 0), pos + ImVec2(size.x, SCALE(50)), { {draw->get_clr(clr->other.white), "SEED"} , {draw->get_clr(clr->other.accent), "HOLLOW"} }, { 0.0, 0.5 });
				draw->text_clipped(draw_list, var->font.inter_medium[0], pos + SCALE(15, 0), pos + ImVec2(gui->content_region_avail().x - SCALE(15), SCALE(50)), draw->get_clr(clr->other.accent), var->flavor.version.c_str(), NULL, NULL, ImVec2(1.0f, 0.5f));
			}

			gui->set_cursor_pos(SCALE(0, 50));

			var->selection.selection_alpha = ImClamp(var->selection.selection_alpha + (6.f * ImGui::GetIO().DeltaTime * (var->selection.selection == var->selection.selection_active && var->list_page.selection_language == var->list_page.selection_language_active ? 1.f : -1.f)), 0.f, 1.f);

			if (var->selection.selection_alpha == 0.f && var->selection.selection_add == 0.f)
				var->selection.selection_active = var->selection.selection, var->list_page.selection_language_active = var->list_page.selection_language;

			gui->push_style_var(ImGuiStyleVar_Alpha, var->selection.selection_alpha * style->Alpha);

			gui->begin_content("ELEMENTS", { size.x, gui->content_region_avail().y - SCALE(70) }, SCALE(15, 15), SCALE(15, 15));
			{
				// ----------------------------
				// PLAYER FEATURE
				// ----------------------------
				if (var->selection.selection_active == 0)
				{
					gui->begin_group();
					{
						gui->begin_child("ESP");
						{
							widget->checkbox_with_keybind("Enable ESP", &esp_func->enable_esp, &esp_func->esp_keybind, &esp_func->esp_mode, "where the esp begin, [RIGHT CLICK] to open filter");
							widget->begin_popup(295);
							{
								for (int i = 0; i < esp_func->name_filter_count; i++) {
									widget->checkbox(esp_func->name_filter_labels[i].c_str(), &esp_func->name_filter_enabled[i]);
								}
							}
							widget->end_popup();

							/*if (esp_func->enable_esp) {
								// distance
								widget->slider_float("Distance", &esp_func->esp_distance, 10.f, 1000.f, "%.0f");
							}*/
						}
						gui->end_child();

						widget->text_colored(var->font.inter_semibold[0], draw->get_clr(clr->text.text_active), "ESP Box");
						gui->begin_child("ESP_BOX");
						{
							widget->checkbox("Enable", &esp_func->box, "This will draw a box around enemies");
							widget->color_picker("Box color", esp_func->box_color, true);
							widget->slider_float("Box thickness", &esp_func->box_thickness, 1, 10, "%.1f");
							widget->dropdown("Box type", &esp_func->box_type_index, esp_func->box_type_list);
						}
						gui->end_child();

						widget->text_colored(var->font.inter_semibold[0], draw->get_clr(clr->text.text_active), "ESP Health");
						gui->begin_child("ESP_HEALTH");
						{
							widget->checkbox("Enable", &esp_func->health, "This will draw health bar of enemies");
							widget->color_picker("Health color", esp_func->health_color, true);
							widget->dropdown("Health type", &esp_func->health_type_index, esp_func->health_type_list, "Show health as percentage or as bar");
						}

						gui->end_child();

						widget->text_colored(var->font.inter_semibold[0], draw->get_clr(clr->text.text_active), "UID Changer");
						gui->begin_child("SPOOFER");
						{
							widget->checkbox("Randomize UID", &misc_func->randomize_uid, "This will randomize your UID every second");
							widget->def_text_field("UID Editor", "V", misc_func->fake_uid, IM_ARRAYSIZE(misc_func->fake_uid), draw->get_clr(clr->dropdown.layout));
							// reset button
							if (widget->button("Reset UID")) {
								// check if current_uid is not empty then copy it to fake_uid

								if (strlen(misc_func->current_uid) > 0) {
									std::snprintf(misc_func->fake_uid, sizeof(misc_func->fake_uid), "%s", misc_func->current_uid);
									notify->add_notify("UID has been reset to current UID", notify_status::success);
								}

								// its unstable, idk why after invoking this function like 3-5 times, the game crash and said runtime error bla bla bla..

								/*auto new_string = UnityResolve::UnityType::String::New("UID has been reset to current UID");
								if (new_string) {
									UnityResolve::Get("Gameplay.Beyond.dll")->Get("GameAction")->Get<UnityResolve::Method>("ShowUIToast", { "System.String", "Beyond.Gameplay.Actions.ShowUIToast.ShowToastType" })->Invoke<void>(new_string, 1);
								}*/
							}
						}

						gui->end_child();
					}
					gui->end_group();

					gui->sameline();

					gui->begin_group();
					{
						widget->text_colored(var->font.inter_semibold[0], draw->get_clr(clr->text.text_active), "ESP Snaplines");
						gui->begin_child("ESP_SNAPLINES");
						{
							widget->checkbox("Enable", &esp_func->snapline, "This will draw lines to the entity");
							widget->color_picker("Snaplines color", esp_func->snapline_color, true);
							widget->dropdown("Snaplines type", &esp_func->snapline_type_index, esp_func->snapline_pos_list);
						}

						gui->end_child();

						widget->text_colored(var->font.inter_semibold[0], draw->get_clr(clr->text.text_active), "ESP Distance");
						gui->begin_child("ESP_DISTANCE");
						{
							widget->checkbox("Enable", &esp_func->distance, "This will draw distance from you to entity");
							widget->color_picker("Distance color", esp_func->distance_color, true);
						}
						gui->end_child();

						widget->text_colored(var->font.inter_semibold[0], draw->get_clr(clr->text.text_active), "ESP Name");
						gui->begin_child("ESP_NAME");
						{
							widget->checkbox("Enable", &esp_func->name, "This will draw name of enemies");
							widget->color_picker("Name color", esp_func->name_color, true);
						}
						gui->end_child();

						widget->text_colored(var->font.inter_semibold[0], draw->get_clr(clr->text.text_active), "ESP Skeleton");
						gui->begin_child("ESP_SKELETON");
						{
							widget->checkbox("Enable", &esp_func->bones, "This will draw skeleton of enemies");
							widget->color_picker("Skeleton color", esp_func->bones_color, true);
						}
						gui->end_child();

						// chams
						widget->text_colored(var->font.inter_semibold[0], draw->get_clr(clr->text.text_active), "Chams");
						gui->begin_child("CHAMS");
						{
							widget->checkbox("Enable", &esp_func->chams, "This will draw colored model of enemies through walls");
							widget->color_picker("Chams visible color", esp_func->chams_visible_color, true);
							widget->checkbox("Disable depth", &esp_func->chams_disabled_depth_buffer, "This will make chams visible even when enemies are behind walls");
						}

						gui->end_child();

						widget->text_colored(var->font.inter_semibold[0], draw->get_clr(clr->text.text_active), "Other");
						gui->begin_child("MISC");
						{
							// camera fov
							widget->checkbox("Custom FOV", &misc_func->camera_fov);

							if (misc_func->camera_fov) {
								widget->slider_float("FOV Value", &misc_func->camera_fov_value, 30.f, 150.f, "%.0f");
							}

							widget->checkbox("Uncensored", &player_func->peeking, "Removes censorship from the game, gooners love this");
						}

						gui->end_child();
					}
					gui->end_group();
				}
				else if (var->selection.selection_active == 1)
				{
					gui->begin_group();
					{
						widget->text_colored(var->font.inter_semibold[0], draw->get_clr(clr->text.text_active), "Stats");
						gui->begin_child("PLAYER_STATS");
						{
							widget->checkbox("God Mode", &player_func->godmode);
							widget->checkbox("Infinite Stamina", &player_func->infinite_stamina);
							widget->checkbox("Max Ultimate", &player_func->infinite_skill_ultimate);
							widget->checkbox("No Skill Cost", &player_func->no_skill_cost);
						}

						gui->end_child();

						widget->text_colored(var->font.inter_semibold[0], draw->get_clr(clr->text.text_active), "Movement");

						gui->begin_child("MOVEMENT");
						{
							//widget->checkbox("Perfect Dodge", &player_func->perfect_dodge, "Auto dodge when enemy attacking player");

							widget->slider_int("Movespeed multiplier", &player_func->movespeed_multiplier_value, 1, 10, "%dx");

							widget->slider_int("Jump multiplier", &player_func->jump_multiplier_value, 1, 10, "%dx");

							//widget->slider_int("Attack Speed Multiplier", &player_func->attack_speed_multiplier_value, 1, 10, "%dx");
						}

						gui->end_child();
					}
					gui->end_group();

					gui->sameline();

					gui->begin_group();
					{
						widget->text_colored(var->font.inter_semibold[0], draw->get_clr(clr->text.text_active), "Player Combat");
						gui->begin_child("COMBAT");
						{
							// always crit
							widget->checkbox("Always crit", &player_func->always_crit);

							// hit multiplier
							widget->checkbox_with_keybind("Enable Multi Hit", &player_func->hit_multiplier, &player_func->hit_multiplier_keybind, &player_func->hit_multiplier_mode, "This will multiply the number of hits you deal to enemies by the value you set in the slider below");
							widget->slider_int("Hit multiplier", &player_func->hit_multiplier_value, 1, 10, "%dx");

							// damage multiplier
							widget->checkbox_with_keybind("Enable Damage Multiplier", &player_func->damage_multiplier, &player_func->damage_multiplier_keybind, &player_func->damage_multiplier_mode, "This will multiply the damage you deal to enemies by the value you set in the slider below");
							widget->slider_int("Damage multiplier", &player_func->damage_multiplier_value, 1, 100, "%dx");
						}
						gui->end_child();

						gui->begin_child("NO_CLIP");
						{
							widget->checkbox("No Clip", &player_func->no_clip, "This will allow you to fly and go through walls");
							widget->slider_float("Speed", &player_func->no_clip_speed, 1.0f, 100.f, "%.1fx");
						}

						gui->end_child();
					}
					gui->end_group();
				}
				// world
				else if (var->selection.selection_active == 2)
				{
					gui->begin_group();
					{
						widget->text_colored(var->font.inter_semibold[0], draw->get_clr(clr->text.text_active), "World");
						gui->begin_child("WORLD");
						{
							// auto kill enemies
							widget->checkbox_with_keybind("Auto Kill Enemies", &player_func->auto_kill_enemies, &player_func->auto_kill_enemies_keybind, &player_func->auto_kill_enemies_mode, "This will automatically kill enemies when you're in combat, no matter enemy type");
							// auto solve puzzles
							widget->checkbox("Auto Solve Chessboard", &player_func->auto_solve_puzzles, "This will automatically solve chessboard");

							// dumb enemies
							widget->checkbox("Dumb Enemies", &player_func->dumb_enemies, "This will make enemies ignore you and do nothing");
						}
						gui->end_child();

						gui->sameline();

						gui->begin_child("TIME_SCALE");
						{
							widget->slider_int("Time Scale", &world_func->time_scale_value, 1, 10, "%dx");
							// apply button
							if (widget->button("Apply")) {
								UnityResolve::UnityType::Time::SetTimeScale(world_func->time_scale_value);
							}
						}

						gui->end_child();
					}
					gui->end_group();
				}

				// config
				else if (var->selection.selection_active == 3)
				{
					gui->begin_group();
					{
						gui->begin_child("FPS");
						{
							widget->dropdown("FPS Limit", &misc_func->fps_limit_index, misc_func->fps_limit_list_str, "This will set FPS limit of the game, setting it to unlimited may cause instability and crashes");

							// apply button
							if (widget->button("Apply FPS Limit")) {
								misc_func->fps_limit = misc_func->fps_limit_list[misc_func->fps_limit_index];
								UnityResolve::UnityType::Application::SetTargetFrameRate(misc_func->fps_limit);
							}
						}

						gui->end_child();
					}

					gui->end_group();

					gui->sameline();

					gui->begin_group();
					{
						gui->begin_child("CONFIG");
						{
							widget->checkbox("Watermark", &var->watermark.watermark);
							widget->color_picker("Accent Color", var->gui.accent, true);
						}
						gui->end_child();
					}

					gui->end_group();
				}
			}

			gui->end_content();

			gui->pop_style_var(1);
			gui->begin_content("selections", { size.x, SCALE(80) }, SCALE(15, 15), SCALE(15, 15), false);
			{
				widget->selection_for(var->selection.selection_list, &var->selection.selection);

				gui->sameline(0, gui->content_region_avail().x - SCALE(65 * var->list_page.theme_list.size()));

				widget->page_for(var->font.icon[2], var->list_page.theme_list, &var->list_page.selection_theme, { 60.f });

				//gui->sameline(0, SCALE(15));
				//widget->page_for(var->font.inter_medium[0], var->list_page.language_list, &var->list_page.selection_language, { 60.f });
			}
			gui->end_content();
		}
		gui->end();

		std::vector<std::string> watermark_func;
		// add fps
		watermark_func.push_back("FPS: " + std::to_string(static_cast<int>(ImGui::GetIO().Framerate)));
		// game version
		watermark_func.push_back("Game Version: " + game_info->game_version);

		gui->water_mark("Endfield", watermark_func, watermark_position::mark_top_left, &var->watermark.watermark);
	}
}