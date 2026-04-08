#include "../settings/functions.h"

bool c_widget::checkbox(std::string_view label, bool* callback, std::string_view hinting)
{
    struct checkbox_state
    {
        ImVec4 layout, circle, hint, text;    
        float circle_spring, alpha;

    };

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label.data());

    ImVec2 pos = window->DC.CursorPos;
    float width = gui->content_region_avail().x;
    float height = SCALE(45);

    ImRect rect(pos, pos + ImVec2(width, height));
    ImRect switch_rect(pos + ImVec2(width - SCALE(30), SCALE(15) ), pos + ImVec2(width, height - SCALE(15) ));

    bool hint_target = IsMouseHoveringRect(rect.Min + ImVec2(CalcTextSize(label.data()).x + SCALE(10), SCALE(15)), rect.Min + ImVec2(CalcTextSize(label.data()).x + SCALE(30), CalcTextSize(label.data()).y + SCALE(15)), true);

    ItemSize(rect, 0);
    if (!ItemAdd(rect, id)) return false;

    bool hovered, held, pressed = ButtonBehavior(rect, id, &hovered, &held);

    if (IsItemClicked())
    {
        *callback = !(*callback);
        MarkItemEdited(id);
    }

    checkbox_state* state = gui->anim_container(&state, id);

    state->circle_spring = ImLerp(state->circle_spring, *callback ?  SCALE(15 / 2) : SCALE(-15 / 2), gui->fixed_speed(element->checkbox.animation_tickrate));
    state->circle = ImLerp(state->circle, *callback ? clr->other.accent : clr->checkbox.circle, gui->fixed_speed(element->checkbox.animation_tickrate));
    state->text = ImLerp(state->text, *callback ? clr->text.text_active : hovered ? clr->text.text_hovered : /*clr->text.text_inactive*/ clr->text.text_active, gui->fixed_speed(8.f));
    state->hint = ImLerp(state->hint, hint_target ? clr->other.accent : clr->text.text_inactive, gui->fixed_speed(8.f));

    state->alpha = ImLerp(state->alpha, *callback ? 0.2f : 0.f, gui->fixed_speed(element->checkbox.animation_tickrate));

    {
        draw->rect_filled(window->DrawList, switch_rect.Min, switch_rect.Max, draw->get_clr(clr->checkbox.layout), SCALE(element->checkbox.rounding));
        draw->rect_filled(window->DrawList, switch_rect.Min, switch_rect.Max, draw->get_clr(clr->other.accent, state->alpha), SCALE(element->checkbox.rounding));

        draw->circle_filled(window->DrawList, switch_rect.GetCenter() + ImVec2(state->circle_spring, 0), SCALE(4.f), draw->get_clr(state->circle), SCALE(element->checkbox.rounding));
        draw->text_clipped(window->DrawList, var->font.inter_medium[0], rect.Min, rect.Max, draw->get_clr(state->text), label.data(), NULL, NULL, { 0.0, 0.5 }, NULL);

        if (hinting[0]) {
            draw->text_clipped(window->DrawList, var->font.inter_medium[0], rect.Min + ImVec2(CalcTextSize(label.data()).x + SCALE(10), 0), rect.Max - SCALE(0, 2), draw->get_clr(state->hint), "[ ? ]", NULL, NULL, { 0.0, 0.5 }, NULL);
            widget->set_tooltip(hinting.data(), hint_target);
        }
    }

    draw->line(window->DrawList, rect.Min + ImVec2(0, height), rect.Max, draw->get_clr(clr->other.element_separator), 1.f);

    return pressed;
}

bool c_widget::checkbox_with_keybind(std::string_view label, bool* callback, int* key, int* mode, std::string_view hinting)
{

    struct checkbox_state
    {
        ImVec4 layout, circle, hint, text, cog;
        float circle_spring, alpha, popup_alpha;

        bool pressed = false, hovered = false;

        ImVec2 content_size;

        int mode_selection = 0;
        std::vector<std::string> mode_list { "Toggle", "Hold" };

        bool initialized = false;
    };

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label.data());

    ImVec2 pos = window->DC.CursorPos;
    float width = gui->content_region_avail().x;
    float height = SCALE(45);

    ImRect rect(pos, pos + ImVec2(width, height));
    ImRect switch_rect(pos + ImVec2(width - SCALE(30), SCALE(15)), pos + ImVec2(width, height - SCALE(15)));
    ImRect keybind_button(pos + ImVec2(width - SCALE(60), SCALE(15)), pos + ImVec2(width - SCALE(45), height - SCALE(15)));

    bool hint_target = IsMouseHoveringRect(rect.Min + ImVec2(CalcTextSize(label.data()).x + SCALE(10), SCALE(15)), rect.Min + ImVec2(CalcTextSize(label.data()).x + SCALE(30), CalcTextSize(label.data()).y + SCALE(15)), true);
    bool keybind_target = IsMouseHoveringRect(keybind_button.Min, keybind_button.Max, true);

    ItemSize(rect, 0);
    if (!ItemAdd(rect, id)) return false;

    bool hovered, held, pressed = ButtonBehavior(!keybind_target ? rect : ImRect(), id, &hovered, &held);

    if (IsItemClicked() && !keybind_target)
    {
        *callback = !(*callback);
        MarkItemEdited(id);
    }

    checkbox_state* state = gui->anim_container(&state, id);

    if (keybind_target && ItemHoverable(g.LastItemData.Rect, g.LastItemData.ID, NULL) && g.IO.MouseClicked[0] || state->pressed && g.IO.MouseClicked[0] && !state->hovered && !element->dropdown.opened_combo) state->pressed = !state->pressed;

    state->circle_spring = ImLerp(state->circle_spring, *callback ? SCALE(15 / 2) : SCALE(-15 / 2), gui->fixed_speed(element->checkbox.animation_tickrate));
    state->circle = ImLerp(state->circle, *callback ? clr->other.accent : clr->checkbox.circle, gui->fixed_speed(element->checkbox.animation_tickrate));
    state->text = ImLerp(state->text, *callback ? clr->text.text_active : hovered ? clr->text.text_hovered : /*clr->text.text_inactive*/ clr->text.text_active, gui->fixed_speed(element->checkbox.animation_tickrate));
    state->hint = ImLerp(state->hint, hint_target ? clr->other.accent : clr->text.text_inactive, gui->fixed_speed(element->checkbox.animation_tickrate));
    state->cog = ImLerp(state->cog, state->pressed || keybind_target ? clr->text.text_active : clr->text.text_inactive, gui->fixed_speed(element->checkbox.animation_tickrate));

    state->alpha = ImLerp(state->alpha, *callback ? 0.2f : 0.f, gui->fixed_speed(element->checkbox.animation_tickrate));
    state->popup_alpha = ImClamp(state->popup_alpha + (gui->fixed_speed(element->checkbox.animation_tickrate) * (state->pressed ? 1.f : -1.f)), 0.f, 1.f);

    if (state->popup_alpha > 0.05f)
    {
        gui->push_style_var(ImGuiStyleVar_Alpha, state->popup_alpha);
        gui->push_style_var(ImGuiStyleVar_WindowBorderSize, 0.f);
        gui->push_style_var(ImGuiStyleVar_WindowRounding, SCALE(element->keybind.popup_rounding));
        gui->push_style_var(ImGuiStyleVar_WindowPadding, SCALE(element->keybind.padding));
        gui->push_style_var(ImGuiStyleVar_ItemSpacing, SCALE(element->keybind.spacing));

        gui->push_style_color(ImGuiCol_WindowBg, draw->get_clr(clr->other.open_popup));

        gui->set_next_window_pos(ImClamp(g.LastItemData.Rect.GetBR(), ImVec2(0, 0), g.IO.DisplaySize - state->content_size));

        if (gui->begin((std::stringstream{} << GetCurrentWindow()->GetID(label.data()) << "keybind").str().c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse))
        {
            state->hovered = IsWindowHovered(), state->content_size = GetWindowSize() + SCALE(15, 15);
            element->keybind.open_popup = state->pressed;

            keybind("Keybind", key, { 165, 35 } );

            keybind_dropdown("Mode", mode, state->mode_list, state->mode_list.size());

        }
        gui->end();

        gui->pop_style_var(5);
        gui->pop_style_color(1);
    }

    {
        draw->rect_filled(window->DrawList, switch_rect.Min, switch_rect.Max, draw->get_clr(clr->checkbox.layout), SCALE(element->checkbox.rounding));
        draw->rect_filled(window->DrawList, switch_rect.Min, switch_rect.Max, draw->get_clr(clr->other.accent, state->alpha), SCALE(element->checkbox.rounding));
        draw->text_clipped(window->DrawList, var->font.icon[5], keybind_button.Min, keybind_button.Max, draw->get_clr(state->cog), "C", NULL, NULL, {0.5, 0.5}, NULL);

        draw->circle_filled(window->DrawList, switch_rect.GetCenter() + ImVec2(state->circle_spring, 0), SCALE(4.f), draw->get_clr(state->circle), SCALE(element->checkbox.rounding));
        draw->text_clipped(window->DrawList, var->font.inter_medium[0], rect.Min, rect.Max, draw->get_clr(state->text), label.data(), NULL, NULL, { 0.0, 0.5 }, NULL);

        if (hinting[0]) {
            draw->text_clipped(window->DrawList, var->font.inter_medium[0], rect.Min + ImVec2(CalcTextSize(label.data()).x + SCALE(10), 0), rect.Max - SCALE(0, 2), draw->get_clr(state->hint), "[ ? ]", NULL, NULL, {0.0, 0.5}, NULL);
            widget->set_tooltip(hinting.data(), hint_target);
        }
    }

    draw->line(window->DrawList, rect.Min + ImVec2(0, height), rect.Max, draw->get_clr(clr->other.element_separator), 1.f);

    if (!state->initialized)
    {
        element->keybind.index++;
        state->initialized = true;
    }

    bool keyFound = false;
    for (auto& bind : element->keybind.update_keybind_system)
    {
        if (*bind.key == *key && *key)
        {
            *bind.mode = *mode;
            *bind.callback = *callback;
            keyFound = true;
            break;
        }
    }

    if (!keyFound)
        element->keybind.update_keybind_system.push_back({ callback, key, mode });

    return pressed;
}