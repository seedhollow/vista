#include "../settings/functions.h"

bool selection_list(std::string_view icon, std::string_view label, int selection_id, int& selection_variable, int total_items)
{
    struct c_page
    {
        ImVec4 text = clr->text.text_inactive;
        ImVec4 icon = clr->text.text_inactive;

        float width_scale = 0, offset = 0, alpha = 0;
    };

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id{ window->GetID(label.data()) };

    const ImVec2 pos = window->DC.CursorPos;
    const bool selected = selection_id == selection_variable;

    const float width = gui->content_region_avail().x;
    const float height = SCALE(50);

    c_page* state = gui->anim_container(&state, id);
    state->width_scale = ImLerp(state->width_scale, selected ? (SCALE(75) + CalcTextSize(label.data()).x) : SCALE(50), gui->fixed_speed(element->selection.animation_tickrate));

    const ImRect rect(pos, pos + ImVec2(state->width_scale, height));

    ItemSize(rect, 0);
    if (!ItemAdd(rect, id)) { return false; }

    bool hovered, held, pressed{ ButtonBehavior(rect, id, &hovered, &held) };
    if (pressed) selection_variable = selection_id;

    state->alpha = ImLerp(state->alpha, selected ? 1.f : 0.f, gui->fixed_speed(element->selection.animation_tickrate));
    state->text = ImLerp(state->text, selected ? clr->text.text_active : clr->text.text_inactive, gui->fixed_speed(element->selection.animation_tickrate));
    state->icon = ImLerp(state->icon, selected ? clr->other.accent : clr->text.text_inactive, gui->fixed_speed(element->selection.animation_tickrate));

    {
        draw->rect_filled(window->DrawList, rect.Min, rect.Max, draw->get_clr(clr->selection.layout), SCALE(element->selection.rounding), (selection_id == 0) ? ImDrawFlags_RoundCornersLeft : (selection_id == total_items - 1) ? ImDrawFlags_RoundCornersRight : ImDrawFlags_RoundCornersNone);
        draw->rect_filled(window->DrawList, rect.Min, rect.Max, draw->get_clr(clr->selection.layout_selection, state->alpha), SCALE(element->selection.rounding));

        draw->text_clipped(window->DrawList, var->font.icon[3], rect.Min, rect.Min + ImVec2(height, height), draw->get_clr(state->icon), icon.data(), NULL, NULL, { 0.5, 0.5 }, NULL);
        draw->text_clipped(window->DrawList, var->font.inter_medium[0], rect.Min + ImVec2(height, 0), rect.Max, draw->get_clr(state->text, state->alpha), label.data(), NULL, NULL, { 0.0, 0.5 }, NULL);
    }

    return pressed;
}

void c_widget::selection_for(std::vector<std::vector<std::string>> page, int* selection)
{
    for (int j = 0; j < page[0].size(); j++) {
        selection_list(page[0][j].data(), page[1][j].data(), j, *selection, page[0].size());
        gui->sameline(0, 0);
    }
}

bool page_list(ImFont* font, std::string_view label, int selection_id, int& selection_variable, int total_items, float x_size)
{
    struct c_list
    {
        ImVec4 text = clr->text.text_inactive;
        ImVec4 icon = clr->text.text_inactive;

        float width_scale = 0, offset = 0, alpha = 0;
    };

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id{ window->GetID(label.data()) };

    const bool selected = selection_id == selection_variable;

    const float width = gui->content_region_avail().x;
    const float height = SCALE(50);

    c_list* state = gui->anim_container(&state, id);

    const ImVec2 pos{ window->DC.CursorPos };
    const ImRect rect(pos, pos + ImVec2(SCALE(x_size), height));

    ItemSize(rect, 0);
    if (!ItemAdd(rect, id)) { return false; }

    bool hovered, held, pressed{ ButtonBehavior(rect, id, &hovered, &held) };
    if (pressed) selection_variable = selection_id;

    state->alpha = ImLerp(state->alpha, selected ? 1.f : 0.f, gui->fixed_speed(element->selection.animation_tickrate));
    state->text = ImLerp(state->text, selected ? clr->text.text_active : clr->text.text_inactive, gui->fixed_speed(element->selection.animation_tickrate));
    state->icon = ImLerp(state->icon, selected ? clr->other.accent : clr->text.text_inactive, gui->fixed_speed(element->selection.animation_tickrate));

    {
        draw->rect_filled(window->DrawList, rect.Min, rect.Max, draw->get_clr(clr->selection.layout), SCALE(element->selection.rounding), (selection_id == 0) ? ImDrawFlags_RoundCornersLeft : (selection_id == total_items - 1) ? ImDrawFlags_RoundCornersRight : ImDrawFlags_RoundCornersNone);
        draw->rect_filled(window->DrawList, rect.Min, rect.Max, draw->get_clr(clr->selection.layout_selection, state->alpha), SCALE(element->selection.rounding));

        draw->text_clipped(window->DrawList, font, rect.Min, rect.Max, draw->get_clr(state->icon), label.data(), NULL, NULL, { 0.5, 0.5 }, NULL);
    }

    return pressed;
}

void c_widget::page_for(ImFont* font, std::vector<std::string> page, int* selection, float current_size)
{
    for (int j = 0; j < page.size(); j++) {
        page_list(font, page[j].data(), j, *selection, page.size(), current_size);
        gui->sameline(0, 0);
    }
}