#include "../settings/functions.h"

bool begin_list_box_ex(std::string_view name, ImGuiID id, const ImVec2& size_arg, ImGuiChildFlags child_flags, ImGuiWindowFlags window_flags)
{
    struct c_list
    {
        float state = 0;
        float slow = 0;
    };

    ImGuiContext& g = *GImGui;
    ImGuiWindow* parent_window = g.CurrentWindow;
    IM_ASSERT(id != 0);

    const ImGuiChildFlags ImGuiChildFlags_SupportedMask_ = ImGuiChildFlags_Border | ImGuiChildFlags_AlwaysUseWindowPadding | ImGuiChildFlags_ResizeX | ImGuiChildFlags_ResizeY | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_FrameStyle;
    IM_UNUSED(ImGuiChildFlags_SupportedMask_);

#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
    if (window_flags & ImGuiWindowFlags_AlwaysUseWindowPadding) child_flags |= ImGuiChildFlags_AlwaysUseWindowPadding;
#endif

    if (child_flags & ImGuiChildFlags_AutoResizeX) child_flags &= ~ImGuiChildFlags_ResizeX;
    if (child_flags & ImGuiChildFlags_AutoResizeY) child_flags &= ~ImGuiChildFlags_ResizeY;

    window_flags |= ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_NoTitleBar;
    window_flags |= (parent_window->Flags & ImGuiWindowFlags_NoMove);

    if (child_flags & (ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize)) window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
    if ((child_flags & (ImGuiChildFlags_ResizeX | ImGuiChildFlags_ResizeY)) == 0) window_flags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;

    g.NextWindowData.Flags |= ImGuiNextWindowDataFlags_HasChildFlags;
    g.NextWindowData.ChildFlags = child_flags;

    const ImVec2 size_avail = GetContentRegionAvail();
    const ImVec2 size_default((child_flags & ImGuiChildFlags_AutoResizeX) ? 0.0f : size_avail.x, (child_flags & ImGuiChildFlags_AutoResizeY) ? 0.0f : size_avail.y);

    c_list* state = gui->anim_container(&state, id);

    ImVec2 size = size_arg;

    if (size.y <= 0) size.y = state->state + SCALE(element->listbox.padding.y) * 2;

    gui->set_next_window_size(size);
    gui->set_next_window_pos(parent_window->DC.CursorPos);

    draw->rect_filled(GetWindowDrawList(), parent_window->DC.CursorPos, parent_window->DC.CursorPos + ImVec2(size.x, size.y), draw->get_clr(clr->window.layout), SCALE(element->listbox.rounding));
    draw->line(GetWindowDrawList(), parent_window->DC.CursorPos + ImVec2(0, size.y + SCALE(15)), parent_window->DC.CursorPos + ImVec2(size.x, size.y + SCALE(15)), draw->get_clr(clr->other.element_separator), 1.f);

    const char* temp_window_name;

    if (name.data())
        ImFormatStringToTempBuffer(&temp_window_name, NULL, "%s/%s_%08X", parent_window->Name, name, id);
    else
        ImFormatStringToTempBuffer(&temp_window_name, NULL, "%s/%08X", parent_window->Name, id);

    const bool ret = gui->begin(temp_window_name, NULL, window_flags);

    ImGuiWindow* child_window = g.CurrentWindow;
    child_window->ChildId = id;

    state->slow = ImLerp(state->slow, child_window->ContentSize.y, gui->fixed_speed(12));
    state->state = state->slow;

    if (child_window->BeginCount == 1) parent_window->DC.CursorPos = child_window->Pos;

    if (child_window->ContentSize.y <= 0)
        draw->text_clipped(GetWindowDrawList(), var->font.inter_medium[0], parent_window->DC.CursorPos, parent_window->DC.CursorPos + ImVec2(size.x, size.y), draw->get_clr(clr->text.text_inactive), "It's still empty here", NULL, NULL, { 0.5, 0.5 }, NULL);

    const ImGuiID temp_id_for_activation = ImHashStr("##Child", 0, id);
    if (g.ActiveId == temp_id_for_activation) ClearActiveID();

    if (g.NavActivateId == id && !(window_flags & ImGuiWindowFlags_NavFlattened) && (child_window->DC.NavLayersActiveMask != 0 || child_window->DC.NavWindowHasScrollY))
    {
        FocusWindow(child_window);
        NavInitWindow(child_window, false);
        SetActiveID(temp_id_for_activation, child_window);
        g.ActiveIdSource = g.NavInputSource;
    }
    return ret;
}

bool c_widget::begin_list(std::string_view name, const ImVec2& size_arg)
{
    ImGuiID id = GetCurrentWindow()->GetID(name.data());

    gui->push_style_var(ImGuiStyleVar_WindowPadding, SCALE(element->listbox.padding));
    gui->push_style_var(ImGuiStyleVar_ItemSpacing, SCALE(element->listbox.spacing));

    gui->set_cursor_pos_y(gui->get_cursor_pos_y() + SCALE(15));

    return begin_list_box_ex(name.data(), id, size_arg, ImGuiChildFlags_None, !size_arg.y <= 0 ? (ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoMove) : (ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar));
}

void c_widget::end_list()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* child_window = g.CurrentWindow;

    gui->pop_style_var(2);

    IM_ASSERT(g.WithinEndChild == false);
    IM_ASSERT(child_window->Flags & ImGuiWindowFlags_ChildWindow);

    g.WithinEndChild = true;
    ImVec2 child_size = child_window->Size;
    gui->end();

    if (child_window->BeginCount == 1)
    {
        ImGuiWindow* parent_window = g.CurrentWindow;
        ImRect bb(parent_window->DC.CursorPos, parent_window->DC.CursorPos + child_size);
        ItemSize(child_size + SCALE(0, 15));

        if (child_window->Flags & ImGuiWindowFlags_NavFlattened) parent_window->DC.NavLayersActiveMaskNext |= child_window->DC.NavLayersActiveMaskNext;
        if (g.HoveredWindow == child_window)  g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HoveredWindow;
    }
    g.WithinEndChild = false;
    g.LogLinePosY = -FLT_MAX;
}

bool c_widget::list_content(std::string_view label, bool active)
{
    struct c_list
    {
        float alpha = 0.f, offset = 10.f;
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

    c_list* state{ gui->anim_container(&state, id) };

    state->offset = ImClamp(state->offset + (gui->fixed_speed(35.f) * (active ? 5.f : -5.f)), SCALE(10.f), SCALE(35.f));
    state->alpha = ImClamp(state->alpha + (gui->fixed_speed(6.f) * (active ? 1.f : -1.f)), 0.f, 1.f);
    state->text = ImLerp(state->text, active ? clr->text.text_active : clr->text.text_inactive, gui->fixed_speed(element->listbox.animation_tickrate));

    draw->rect_filled(window->DrawList, rect.Min, rect.Max, draw->get_clr(clr->dropdown.layout, state->alpha), SCALE(element->listbox.selection_rounding));
    draw->render_check_mark(window->DrawList, rect.Min + ImVec2(rect_size - SCALE(9), rect_size - SCALE(9)) / 2, draw->get_clr(clr->other.accent, state->alpha), SCALE(9), SCALE(1.5f));

    draw->text_clipped(window->DrawList, var->font.inter_medium[1], rect.Min + ImVec2(state->offset, 0), rect.Max, draw->get_clr(state->text), label.data(), NULL, NULL, { 0.0, 0.5 }, NULL);

    return pressed;
}