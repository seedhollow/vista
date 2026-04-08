#include "settings/functions.h"

bool c_widget::button(std::string_view label)
{
    struct c_button
    {
        ImVec4 layout = clr->button.layout;
    };

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label.data());

    const ImVec2 pos = window->DC.CursorPos + SCALE(0, 15);
    const ImRect rect(pos, pos + ImVec2(gui->content_region_avail().x, SCALE(35)));

    ImGui::ItemSize(ImRect(rect.Min, rect.Max + SCALE(0, 30)), 0);
    if (!ItemAdd(rect, id)) return false;

    bool hovered, held, pressed = ButtonBehavior(rect, id, &hovered, &held);
    c_button* state = gui->anim_container(&state, id);

    state->layout = ImLerp(state->layout, IsItemActive() ? clr->other.accent : hovered ?  clr->button.layout_hovered : clr->button.layout, gui->fixed_speed(element->button.animation_tickrate));

    {
        draw->rect_filled(window->DrawList, rect.Min, rect.Max, draw->get_clr(state->layout), SCALE(element->button.rounding));
        draw->text_clipped(window->DrawList, var->font.inter_medium[0], rect.Min, rect.Max, draw->get_clr(clr->text.text_active), label.data(), NULL, NULL, { 0.5, 0.5 }, NULL);
    }

    draw->line(window->DrawList, rect.Min + SCALE(0, 50), rect.Max + SCALE(0, 15), draw->get_clr(clr->other.element_separator), 1.f);

    return pressed;
}