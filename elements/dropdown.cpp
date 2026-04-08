#include "../settings/functions.h"

static float calc_combo_size(int items_count, float item_size)
{
    ImGuiContext& g = *GImGui;
    if (items_count <= 0) return FLT_MAX;

    return item_size * items_count + g.Style.ItemSpacing.y * (items_count - 1);
}

static const char* items_array_getter(void* data, int idx)
{
    const char* const* items = (const char* const*)data;
    return items[idx];
}

bool selectable_ex(std::string_view label, bool active, ImDrawFlags rounded_flag)
{
    struct c_selectable
    {
        float alpha = 0.f, offset = 0.f;
        ImVec4 text = clr->text.text_inactive;
    };

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label.data());

    const float width = gui->content_region_avail().x;
    float rect_size = SCALE(element->dropdown.selection_size);

    const ImVec2 pos = window->DC.CursorPos;
    const ImRect rect(pos, pos + ImVec2(width, rect_size));

    ImGui::ItemSize(rect, 0);
    if (!ItemAdd(rect, id)) return false;

    bool hovered = IsItemHovered(), pressed = hovered && g.IO.MouseClicked[0];
    if (pressed) MarkItemEdited(id);

    c_selectable* state{ gui->anim_container(&state, id) };

    state->offset = ImClamp(state->offset + (gui->fixed_speed(35.f) * (active ? 5.f : -5.f)), SCALE(10.f), SCALE(35.f));
    state->alpha = ImClamp(state->alpha + (gui->fixed_speed(6.f) * (active ? 1.f : -1.f)), 0.f, 1.f);
    state->text = ImLerp(state->text, active ? clr->text.text_active : clr->text.text_inactive, gui->fixed_speed(element->dropdown.animation_tickrate));

    draw->rect_filled(window->DrawList, rect.Min, rect.Max, draw->get_clr(clr->dropdown.selection, state->alpha), SCALE(element->dropdown.rounding), rounded_flag);
    draw->render_check_mark(window->DrawList, rect.Min + ImVec2(rect_size - SCALE(9), rect_size - SCALE(9)) / 2, draw->get_clr(clr->other.accent, state->alpha), SCALE(9), SCALE(1.5f));

    draw->text_clipped(window->DrawList, var->font.inter_medium[1], rect.Min + ImVec2(state->offset, 0), rect.Max, draw->get_clr(state->text), label.data(), NULL, NULL, { 0.0, 0.5 }, NULL);

    return pressed;
}

bool selectable(const char* label, bool* p_selected, ImDrawFlags rounded_flag)
{
    if (selectable_ex(label, *p_selected, rounded_flag))
    {
        *p_selected = !*p_selected;
        return true;
    }
    return false;
}

bool dropdown_list(std::string_view label, std::string_view preview_value, int val, ImGuiComboFlags flags, bool multi, std::string_view hinting, int max_count)
{
    struct c_combo
    {
        ImVec4 hint, text = clr->text.text_inactive;
        bool combo_opened = false, hovered = false;

        float alpha = 0.f, offset = 0.f, text_size, selection_size, rotation;
    };

    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();

    ImGuiNextWindowDataFlags backup_next_window_data_flags = g.NextWindowData.Flags;
    g.NextWindowData.ClearFlags();

    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label.data());

    if (val > max_count) val = max_count;
    const ImVec2 pos = window->DC.CursorPos;

    const float width = gui->content_region_avail().x;
    const float rect_size = SCALE(95);
    const float padding = SCALE(15);

    const ImRect rect(pos, pos + ImVec2(width, rect_size));
    const ImRect open_combo(pos + ImVec2(0, rect_size - (padding + SCALE(element->dropdown.dropdown_size))), pos + ImVec2(width, rect_size - padding));

    bool hint_target = IsMouseHoveringRect(rect.Min + ImVec2(CalcTextSize(label.data()).x + SCALE(10), SCALE(15)), rect.Min + ImVec2(CalcTextSize(label.data()).x + SCALE(30), CalcTextSize(label.data()).y + SCALE(15)), true);

    ItemSize(rect, 0.f);
    if (!ItemAdd(rect, id)) return false;

    bool hovered, held, pressed = ButtonBehavior(rect, id, &hovered, &held);

    c_combo* state = gui->anim_container(&state, id);
    if (hovered && g.IO.MouseClicked[0] || state->combo_opened && g.IO.MouseClicked[0] && !state->hovered) state->combo_opened = !state->combo_opened;

    state->offset = ImLerp(state->offset, state->combo_opened ? SCALE(8.f) : SCALE(0.f), gui->fixed_speed(14));
    state->alpha = ImClamp(state->alpha + (gui->fixed_speed(6.f) * (state->combo_opened ? 1.f : -1.f)), 0.f, 1.f);
    state->text = ImLerp(state->text, state->combo_opened ? clr->text.text_active : hovered ? clr->text.text_hovered : /*clr->text.text_inactive*/ clr->text.text_active, gui->fixed_speed(8.f));
    state->hint = ImLerp(state->hint, hint_target ? clr->other.accent : clr->text.text_inactive, gui->fixed_speed(8.f));
    state->rotation = ImLerp(state->rotation, state->combo_opened ? -1.57f : 1.57f, gui->fixed_speed(14.f));

    state->selection_size = ImLerp(state->selection_size, state->combo_opened ? SCALE(element->dropdown.selection_size) : 0, gui->fixed_speed(element->dropdown.animation_tickrate));

    if (!IsRectVisible(rect.Min, rect.Max + ImVec2(0, 2)))
    {
        state->combo_opened = false;
        state->alpha = 0.f;
    }

    {
        draw->rect_filled(window->DrawList, open_combo.Min, open_combo.Max, draw->get_clr(clr->dropdown.layout), SCALE(element->dropdown.rounding));

        draw->text_clipped(window->DrawList, var->font.inter_medium[0], rect.Min + ImVec2(0, padding), rect.Max, draw->get_clr(state->text), label.data(), NULL, NULL, ImVec2(0.f, 0.0f));
        draw->text_clipped(window->DrawList, var->font.inter_medium[0], open_combo.Min + SCALE(15, 0), open_combo.Max, draw->get_clr(clr->text.text_active), preview_value.data(), NULL, NULL, ImVec2(0.f, 0.5f));

        draw->rotate_start(window->DrawList);
        draw->text_clipped(window->DrawList, var->font.icon[1], open_combo.Min, open_combo.Max - SCALE(15, 0), draw->get_clr(clr->text.text_active), "C", NULL, NULL, ImVec2(1.f, 0.5f));
        draw->rotate_end(state->rotation, window->DrawList);

        if (hinting[0])
        {
            draw->text_clipped(window->DrawList, var->font.inter_medium[0], rect.Min + ImVec2(CalcTextSize(label.data()).x + SCALE(10), padding), rect.Max - SCALE(0, 2), draw->get_clr(state->hint), "[ ? ]", NULL, NULL, { 0.0, 0 }, NULL);
            widget->set_tooltip(hinting.data(), hint_target);
        }
    }

    draw->line(window->DrawList, rect.Min + ImVec2(0, rect_size), rect.Max, draw->get_clr(clr->other.element_separator), 1.f);

    if (!state->combo_opened && state->alpha < 0.01f) return false;

    gui->push_style_var(ImGuiStyleVar_Alpha, { state->alpha });
    gui->push_style_var(ImGuiStyleVar_WindowPadding, { 0, 0 });
    gui->push_style_var(ImGuiStyleVar_ItemSpacing, { 0, 0 });
    gui->push_style_var(ImGuiStyleVar_WindowRounding, { element->dropdown.rounding });
    gui->push_style_var(ImGuiStyleVar_WindowBorderSize, { 0.f });

    gui->push_style_color(ImGuiCol_WindowBg, draw->get_clr(clr->dropdown.layout));

    gui->set_next_window_pos(ImClamp(open_combo.GetBL() + SCALE(0, state->offset), ImVec2(0, 0), g.IO.DisplaySize - ImVec2(0, SCALE(15) + calc_combo_size(val, state->selection_size))));
    gui->set_next_window_size(ImVec2(open_combo.GetWidth(), calc_combo_size(val, state->selection_size)));

    gui->begin(label, NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
    {
        gui->begin_group();
        state->hovered = IsWindowHovered();

        element->dropdown.opened_combo = state->combo_opened;

        if (!multi)
            if (IsWindowHovered() && g.IO.MouseClicked[0]) state->combo_opened = false;
    }
    return true;
}

void end_combo()
{
    gui->end_group();

    gui->pop_style_color(1);
    gui->pop_style_var(5);
    gui->end();
}

bool combo(std::string_view label, int* current_item, std::vector<std::string>& items, int popup_max_height_in_items, std::string_view hinting, int max_count)
{
    ImGuiContext& g = *GImGui;

    const std::string* preview_value = nullptr;
    if (*current_item >= 0 && *current_item < static_cast<int>(items.size()))
        preview_value = &items[*current_item];

    if (popup_max_height_in_items != -1 && !(g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint))
        SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, calc_combo_size(popup_max_height_in_items, 30)));

    if (!dropdown_list(label.data(), preview_value ? preview_value->c_str() : nullptr, static_cast<int>(items.size()), ImGuiComboFlags_None, false, hinting, max_count == 0 ? items.size() : max_count))
        return false;

    bool value_changed = false;
    for (int i = 0; i < static_cast<int>(items.size()); i++)
    {
        const std::string& item_text = items[i];
        PushID(i);
        const bool item_selected = (i == *current_item);
        if (selectable_ex(item_text.c_str(), item_selected, i == 0 ? ImDrawFlags_RoundCornersTop : i == items.size() - 1 ? ImDrawFlags_RoundCornersBottom : ImDrawFlags_RoundCornersNone) && *current_item != i)
        {
            value_changed = true;
            *current_item = i;
        }

        if (g.IO.MouseClicked[1])

            if (item_selected) SetItemDefaultFocus();
        PopID();
    }

    end_combo();

    if (value_changed) MarkItemEdited(g.LastItemData.ID);

    return value_changed;
}

bool c_widget::dropdown(std::string_view label, int* current_item, std::vector<std::string>& items, std::string_view hinting, int max_count)
{
    const bool value_changed{ combo(label, current_item, items, -1, hinting, max_count == 0 ? items.size() : max_count) };
    return value_changed;
}

void c_widget::multi_dropdown(std::string_view label, bool variable[], std::vector<std::string>& labels, std::string_view hinting, int max_count)
{
    ImGuiContext& g{ *GImGui };

    std::string preview{ "-" };

    for (auto i = 0, j = 0; i < labels.size(); i++)
    {
        if (variable[i])
        {
            if (j)
                preview += (", ") + (std::string)labels[i];
            else
                preview = labels[i];

            j++;
        }
    }

    if (dropdown_list(label.data(), preview.c_str(), labels.size(), 0, true, hinting, max_count == 0 ? labels.size() : max_count))
    {
        for (auto i = 0; i < labels.size(); i++) selectable(labels[i].data(), &variable[i], i == 0 ? ImDrawFlags_RoundCornersTop : i == labels.size() - 1 ? ImDrawFlags_RoundCornersBottom : ImDrawFlags_RoundCornersNone);

        end_combo();
    }

    preview = ("-");
}







bool key_dropdown(std::string_view label, std::string_view preview_value, int val, int max_count, ImGuiComboFlags flags)
{
    struct c_combo
    {
        ImVec4 hint, text = clr->text.text_inactive;
        bool combo_opened = false, hovered = false;

        float alpha = 0.f, offset = 0.f, text_size, selection_size, rotation;
    };

    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();

    ImGuiNextWindowDataFlags backup_next_window_data_flags = g.NextWindowData.Flags;
    g.NextWindowData.ClearFlags();

    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label.data());

    if (val > max_count) val = max_count;
    const ImVec2 pos = window->DC.CursorPos;

    const float width = gui->content_region_avail().x;

    const float rect_size = SCALE(35);
    const ImRect rect(pos, pos + ImVec2(width, rect_size));

    bool hint_target = IsMouseHoveringRect(rect.Min + ImVec2(CalcTextSize(label.data()).x + SCALE(10), SCALE(15)), rect.Min + ImVec2(CalcTextSize(label.data()).x + SCALE(30), CalcTextSize(label.data()).y + SCALE(15)), true);

    ImGui::ItemSize(rect, 0.f);
    if (!ItemAdd(rect, id)) return false;

    bool hovered, held, pressed = ButtonBehavior(rect, id, &hovered, &held);

    c_combo* state = gui->anim_container(&state, id);
    if (hovered && g.IO.MouseClicked[0] || state->combo_opened && g.IO.MouseClicked[0] && !state->hovered) state->combo_opened = !state->combo_opened;

    state->offset = ImLerp(state->offset, state->combo_opened ? SCALE(8.f) : SCALE(0.f), gui->fixed_speed(14));
    state->alpha = ImClamp(state->alpha + (gui->fixed_speed(6.f) * (state->combo_opened ? 1.f : -1.f)), 0.f, 1.f);
    state->text = ImLerp(state->text, state->combo_opened ? clr->text.text_active : hovered ? clr->text.text_hovered : clr->text.text_inactive, gui->fixed_speed(8.f));
    state->hint = ImLerp(state->hint, hint_target ? clr->other.accent : clr->text.text_inactive, gui->fixed_speed(8.f));
    state->rotation = ImLerp(state->rotation, state->combo_opened ? -1.57f : 1.57f, gui->fixed_speed(14.f));

    state->selection_size = ImLerp(state->selection_size, state->combo_opened ? SCALE(element->dropdown.selection_size) : 0, gui->fixed_speed(element->dropdown.animation_tickrate));

    if (!IsRectVisible(rect.Min, rect.Max + ImVec2(0, 2)))
    {
        state->combo_opened = false;
        state->alpha = 0.f;
    }

    {
        draw->rect_filled(window->DrawList, rect.Min, rect.Max, draw->get_clr(clr->dropdown.another_dropdown), SCALE(element->dropdown.rounding));

        draw->text_clipped(window->DrawList, var->font.inter_medium[0], rect.Min + SCALE(10, 0), rect.Max, draw->get_clr(clr->text.text_active), preview_value.data(), NULL, NULL, ImVec2(0.f, 0.5f));

        draw->rotate_start(window->DrawList);
        draw->text_clipped(window->DrawList, var->font.icon[1], rect.Min, rect.Max - SCALE(15, 0), draw->get_clr(clr->text.text_active), "C", NULL, NULL, ImVec2(1.f, 0.5f));
        draw->rotate_end(state->rotation, window->DrawList);
    }

    element->dropdown.opened_combo = state->combo_opened;
    if (!state->combo_opened && state->alpha < 0.01f) return false;

    gui->push_style_var(ImGuiStyleVar_Alpha, { state->alpha });
    gui->push_style_var(ImGuiStyleVar_WindowPadding, { 0, 0 });
    gui->push_style_var(ImGuiStyleVar_ItemSpacing, { 0, 0 });
    gui->push_style_var(ImGuiStyleVar_WindowRounding, { element->dropdown.rounding });
    gui->push_style_var(ImGuiStyleVar_WindowBorderSize, { 0.f });

    gui->push_style_color(ImGuiCol_WindowBg, draw->get_clr(clr->dropdown.layout));

    gui->set_next_window_pos(ImClamp(rect.GetBL() + SCALE(0, state->offset), ImVec2(0, 0), g.IO.DisplaySize - ImVec2(0, SCALE(15) + calc_combo_size(val, state->selection_size))));
    gui->set_next_window_size(ImVec2(rect.GetWidth(), calc_combo_size(val, state->selection_size)));

    gui->begin(label, NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
    {
        gui->begin_group();
        state->hovered = IsWindowHovered();

        if (IsWindowHovered() && g.IO.MouseClicked[0]) state->combo_opened = false;
    }
    return true;
}

void end_keybind_dropdown()
{
    gui->end_group();

    gui->pop_style_color(1);
    gui->pop_style_var(5);
    gui->end();
}

bool combo_dropdown(std::string_view label, int* current_item, std::vector<std::string>& items, int popup_max_height_in_items, int max_count)
{
    ImGuiContext& g = *GImGui;

    const std::string* preview_value = nullptr;
    if (*current_item >= 0 && *current_item < static_cast<int>(items.size()))
        preview_value = &items[*current_item];

    if (popup_max_height_in_items != -1 && !(g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint))
        SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, calc_combo_size(popup_max_height_in_items, 30)));

    if (!key_dropdown(label.data(), preview_value ? preview_value->c_str() : nullptr, static_cast<int>(items.size()), max_count, ImGuiComboFlags_None))
        return false;

    bool value_changed = false;
    for (int i = 0; i < static_cast<int>(items.size()); i++)
    {
        const std::string& item_text = items[i];
        PushID(i);
        const bool item_selected = (i == *current_item);
        if (selectable_ex(item_text.c_str(), item_selected, i == 0 ? ImDrawFlags_RoundCornersTop : i == items.size() - 1 ? ImDrawFlags_RoundCornersBottom : ImDrawFlags_RoundCornersNone) && *current_item != i)
        {
            value_changed = true;
            *current_item = i;
        }

        if (g.IO.MouseClicked[1])

            if (item_selected) SetItemDefaultFocus();
        PopID();
    }

    end_combo();

    if (value_changed) MarkItemEdited(g.LastItemData.ID);

    return value_changed;
}

bool c_widget::keybind_dropdown(std::string_view label, int* current_item, std::vector<std::string>& items, int max_count)
{
    const bool value_changed{ combo_dropdown(label, current_item, items, -1, max_count) };

    return value_changed;
}