#include "../settings/functions.h"

void c_gui::water_mark(std::string name, std::vector<std::string> function, watermark_position type, bool* visible)
{

    struct spectator_state
    {
        float alpha;
        ImVec2 content_size, current_pos, pos;

    };

    spectator_state* state = gui->anim_container(&state, ImGui::GetID(name.c_str()));

    state->alpha = ImClamp(state->alpha + (gui->fixed_speed(12.f) * (*visible ? 1.f : -1.f)), 0.f, 1.f);

    push_style_var(ImGuiStyleVar_Alpha, state->alpha);

    if (state->alpha >= 0.01f) {

        switch (type)
        {
        case mark_top_left:
            state->pos = SCALE(20, 20);
            break;
        case mark_top_right:
            state->pos = ImVec2(ImGui::GetIO().DisplaySize.x - state->content_size.x, SCALE(20));
            break;
        case mark_bottom_left:
            state->pos = ImVec2(SCALE(20), ImGui::GetIO().DisplaySize.y - (state->content_size.y));
            break;
        case mark_bottom_right:
            state->pos = ImVec2(ImGui::GetIO().DisplaySize.x - state->content_size.x, ImGui::GetIO().DisplaySize.y - state->content_size.y);
            break;
        }

        state->current_pos = ImLerp(state->current_pos, state->pos, fixed_speed(25.f));

        gui->push_style_var(ImGuiStyleVar_WindowBorderSize, SCALE(12));
        gui->push_style_var(ImGuiStyleVar_WindowRounding, SCALE(var->watermark.rounding));
        gui->push_style_var(ImGuiStyleVar_WindowPadding, SCALE(20, 20));
        gui->push_style_var(ImGuiStyleVar_ItemSpacing, SCALE(20, 20));

        gui->push_style_color(ImGuiCol_WindowBg, draw->get_clr(clr->watermark.layout));
        gui->push_style_color(ImGuiCol_Border, draw->get_clr(clr->watermark.border));

        gui->set_next_window_pos(state->current_pos);

        gui->begin("watermark", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);
        {
            ImDrawList* draw_list = ImGui::GetWindowDrawList();

            if (var->watermark.water_name)
            {
                draw->text_clipped(draw_list, var->font.icon[0], gui->cursor_screen_pos(), gui->cursor_screen_pos() + SCALE(35, 20), draw->get_clr(clr->other.accent), "A", NULL, NULL, { 0.0, 0.5 });
                draw->text_clipped(draw_list, var->font.icon[0], gui->cursor_screen_pos(), gui->cursor_screen_pos() + SCALE(35, 20), draw->get_clr(clr->other.white), "B", NULL, NULL, { 0.0, 0.5 });

                gui->push_style_var(ImGuiStyleVar_ItemSpacing, SCALE(0, 0));
                {
                    draw->colored_text(var->font.inter_bold[0], gui->cursor_screen_pos() + SCALE(40, 0), gui->cursor_screen_pos() + ImVec2(gui->content_region_avail().x, SCALE(20)), { {draw->get_clr(clr->other.white), "ARC"} , {draw->get_clr(clr->other.accent), "ANE"} }, { 0.0, 0.5 });
                }
                gui->pop_style_var();

                gui->set_cursor_pos_y(gui->get_cursor_pos_y() + SCALE(20));
            }

            gui->begin_group();
            {

                for (int i = 0; i < function.size(); i++)
                {
                    widget->text_colored(var->font.inter_semibold[0], draw->get_clr(clr->text.text_active), function[i]);
                    gui->sameline();
                }

            }
            gui->end_group();

            state->content_size = GetWindowSize() + SCALE(20, 20);
        }
        gui->end();

        gui->pop_style_var(4);
        gui->pop_style_color(2);

    }
    pop_style_var();
}