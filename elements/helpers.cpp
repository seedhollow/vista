#include "../settings/functions.h"

bool c_widget::begin_popup(float size)
{
    struct settings_state
    {
        bool pressed, hovered;
        float begin_offset, alpha_popup, alpha_hint, rotate = 0, content_size_y;
    };

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    settings_state* state = gui->anim_container(&state, GetCurrentWindow()->GetID(g.LastItemData.ID));

    if (ItemHoverable(g.LastItemData.Rect, g.LastItemData.ID, NULL) && g.IO.MouseClicked[1] || state->pressed && g.IO.MouseClicked[0] && !state->hovered && !element->color_picker.open_picker && !element->keybind.open_popup && !element->dropdown.opened_combo)
        state->pressed = !state->pressed;

    state->alpha_popup = ImClamp(state->alpha_popup + (gui->fixed_speed(6.f) * (state->pressed ? 1.f : -1.f)), 0.f, 1.f);
    state->alpha_hint = ImClamp(state->alpha_hint + (gui->fixed_speed(element->settings_popup.animation_tickrate / 1.3f) * (!state->pressed && IsItemHovered() ? 1.f : -1.f)), 0.f, 1.f);

    if (state->alpha_hint > 0.01f && !element->tooltip.hovered && !element->keybind.open_popup && !element->color_picker.open_picker && !element->settings_popup.open_popup) {

        state->rotate += gui->fixed_speed(6.f);

        draw->rotate_start(GetForegroundDrawList());
        draw->text(GetForegroundDrawList(), var->font.icon[0], var->font.icon[1]->FontSize, GetMousePos() + SCALE(25, 25), draw->get_clr(clr->text.text_active, state->alpha_hint), "D");
        draw->rotate_end(state->rotate, GetForegroundDrawList(), { 0, 0 });
    }

    gui->push_style_var(ImGuiStyleVar_Alpha, state->alpha_popup);
    gui->push_style_var(ImGuiStyleVar_WindowPadding, SCALE(0, 0));

    gui->set_next_window_pos(ImClamp(g.LastItemData.Rect.GetBL(), ImVec2(0, 0), g.IO.DisplaySize - SCALE(size, state->content_size_y) - SCALE(15, 15)));
    if (gui->begin((std::stringstream{} << GetCurrentWindow()->GetID(g.LastItemData.ID)).str().c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse))
    {
        state->hovered = IsMouseHoveringRect(GetWindowPos(), GetWindowPos() + GetWindowSize());
        element->settings_popup.open_popup = state->pressed;
    }

    gui->begin_child("content", SCALE(size, 0));

    state->content_size_y = GetWindowSize().y;

    return state->pressed;
}

void c_widget::end_popup()
{
    gui->end_child();

    gui->end();
    gui->pop_style_var(2);
}

void item_hande_shortcut(ImGuiID id)
{
    ImGuiContext& g = *GImGui;
    ImGuiInputFlags flags = g.NextItemData.ShortcutFlags;
    IM_ASSERT((flags & ~ImGuiInputFlags_SupportedBySetNextItemShortcut) == 0);

    if (flags & ImGuiInputFlags_Tooltip)
    {
        g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HasShortcut;
        g.LastItemData.Shortcut = g.NextItemData.Shortcut;
    }
    if (!Shortcut(g.NextItemData.Shortcut, flags & ImGuiInputFlags_SupportedByShortcut, id) || g.NavActivateId != 0) return;

    g.NavActivateId = id;
    g.NavActivateFlags = ImGuiActivateFlags_PreferInput | ImGuiActivateFlags_FromShortcut;

    g.NavActivateDownId = g.NavActivatePressedId = id;
    NavHighlightActivated(id);
}

void c_gui::push_font(ImFont* font)
{
    ImGuiContext& g = *GImGui;
    if (!font) font = GetDefaultFont();

    SetCurrentFont(font);
    g.FontStack.push_back(font);
    g.CurrentWindow->DrawList->PushTextureID(font->ContainerAtlas->TexID);
}

void c_gui::pop_font()
{
    ImGuiContext& g = *GImGui;
    g.CurrentWindow->DrawList->PopTextureID();
    g.FontStack.pop_back();

    SetCurrentFont(g.FontStack.empty() ? GetDefaultFont() : g.FontStack.back());
}

void c_gui::set_next_window_pos(const ImVec2& pos, ImGuiCond cond, const ImVec2& pivot)
{
    ImGuiContext& g = *GImGui;

    g.NextWindowData.Flags |= ImGuiNextWindowDataFlags_HasPos;
    g.NextWindowData.PosVal = pos;
    g.NextWindowData.PosPivotVal = ImVec2(ImClamp(pivot.x, 0.0f, 1.0f), ImClamp(pivot.y, 0.0f, 1.0f));
    g.NextWindowData.PosCond = (cond != 0 && ImIsPowerOfTwo(cond)) ? cond : ImGuiCond_Always;
}
void c_gui::set_next_window_size(const ImVec2& size, ImGuiCond cond)
{
    ImGuiContext& g = *GImGui;
    IM_ASSERT(cond == 0 || ImIsPowerOfTwo(cond));

    g.NextWindowData.Flags |= ImGuiNextWindowDataFlags_HasSize;
    g.NextWindowData.SizeVal = size;
    g.NextWindowData.SizeCond = cond != 0 ? cond : ImGuiCond_Always;
}

ImVec2 c_gui::get_cursor_pos()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.CursorPos - window->Pos + window->Scroll;
}

float c_gui::get_cursor_pos_x()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.CursorPos.x - window->Pos.x + window->Scroll.x;
}

float c_gui::get_cursor_pos_y()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.CursorPos.y - window->Pos.y + window->Scroll.y;
}


void c_gui::set_cursor_pos(const ImVec2& local_pos)
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.CursorPos = window->Pos - window->Scroll + local_pos;
    window->DC.IsSetPos = true;
}

void c_gui::set_cursor_pos_x(float x)
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.CursorPos.x = window->Pos.x - window->Scroll.x + x;
    window->DC.IsSetPos = true;
}

void c_gui::set_cursor_pos_y(float y)
{
    ImGuiWindow* window = GetCurrentWindow();
    window->DC.CursorPos.y = window->Pos.y - window->Scroll.y + y;
    window->DC.IsSetPos = true;
}


ImVec2 content_region_max_abs()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    ImVec2 mx = (window->DC.CurrentColumns || g.CurrentTable) ? window->WorkRect.Max : window->ContentRegionRect.Max;
    return mx;
}

ImVec2 c_gui::content_region_avail()
{
    ImGuiWindow* window = GImGui->CurrentWindow;
    return content_region_max_abs() - window->DC.CursorPos;
}

ImVec2 c_gui::get_window_size()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->Size;
}

ImVec2 c_gui::get_window_pos()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    return window->Pos;
}

ImVec2 c_gui::cursor_screen_pos()
{
    ImGuiWindow* window = GetCurrentWindowRead();
    return window->DC.CursorPos;
}

ImVec2 c_gui::content_region_max()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    ImVec2 mx = (window->DC.CurrentColumns || g.CurrentTable) ? window->WorkRect.Max : window->ContentRegionRect.Max;
    return mx - window->Pos;
}

void c_gui::new_frame()
{
    NewFrame();
}

void c_gui::end_frame()
{
    Render();
}

void c_gui::push_style_color(ImGuiCol idx, ImU32 col)
{
    ImGuiContext& g = *GImGui;
    ImGuiColorMod backup;
    backup.Col = idx;
    backup.BackupValue = g.Style.Colors[idx];
    g.ColorStack.push_back(backup);
    if (g.DebugFlashStyleColorIdx != idx) g.Style.Colors[idx] = ColorConvertU32ToFloat4(col);
}

void c_gui::pop_style_color(int count)
{
    ImGuiContext& g = *GImGui;
    if (g.ColorStack.Size < count)
    {
        count = g.ColorStack.Size;
    }
    while (count > 0)
    {
        ImGuiColorMod& backup = g.ColorStack.back();
        g.Style.Colors[backup.Col] = backup.BackupValue;
        g.ColorStack.pop_back();
        count--;
    }
}

void c_gui::push_style_var(ImGuiStyleVar idx, float val)
{
    ImGuiContext& g = *GImGui;
    const ImGuiDataVarInfo* var_info = GetStyleVarInfo(idx);
    if (var_info->Type == ImGuiDataType_Float && var_info->Count == 1)
    {
        float* pvar = (float*)var_info->GetVarPtr(&g.Style);
        g.StyleVarStack.push_back(ImGuiStyleMod(idx, *pvar));
        *pvar = val;
        return;
    }
}

void c_gui::push_style_var(ImGuiStyleVar idx, const ImVec2& val)
{
    ImGuiContext& g = *GImGui;
    const ImGuiDataVarInfo* var_info = GetStyleVarInfo(idx);
    if (var_info->Type == ImGuiDataType_Float && var_info->Count == 2)
    {
        ImVec2* pvar = (ImVec2*)var_info->GetVarPtr(&g.Style);
        g.StyleVarStack.push_back(ImGuiStyleMod(idx, *pvar));
        *pvar = val;
        return;
    }
}

void c_gui::pop_style_var(int count)
{
    ImGuiContext& g = *GImGui;
    if (g.StyleVarStack.Size < count)
    {
        count = g.StyleVarStack.Size;
    }
    while (count > 0)
    {
        ImGuiStyleMod& backup = g.StyleVarStack.back();
        const ImGuiDataVarInfo* info = GetStyleVarInfo(backup.VarIdx);
        void* data = info->GetVarPtr(&g.Style);
        if (info->Type == ImGuiDataType_Float && info->Count == 1) { ((float*)data)[0] = backup.BackupFloat[0]; }
        else if (info->Type == ImGuiDataType_Float && info->Count == 2) { ((float*)data)[0] = backup.BackupFloat[0]; ((float*)data)[1] = backup.BackupFloat[1]; }
        g.StyleVarStack.pop_back();
        count--;
    }
}

void c_gui::spacing()
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return;
    ImGui::ItemSize(ImVec2(0, 0));
}

void c_gui::sameline(float offset_from_start_x, float spacing_w)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    if (window->SkipItems) return;

    if (offset_from_start_x != 0.0f)
    {
        if (spacing_w < 0.0f) spacing_w = 0.0f;
        window->DC.CursorPos.x = window->Pos.x - window->Scroll.x + offset_from_start_x + spacing_w + window->DC.GroupOffset.x + window->DC.ColumnsOffset.x;
        window->DC.CursorPos.y = window->DC.CursorPosPrevLine.y;
    }
    else
    {
        if (spacing_w < 0.0f) spacing_w = GetStyle().ItemSpacing.x;

        window->DC.CursorPos.x = window->DC.CursorPosPrevLine.x + spacing_w;
        window->DC.CursorPos.y = window->DC.CursorPosPrevLine.y;
    }
    window->DC.CurrLineSize = window->DC.PrevLineSize;
    window->DC.CurrLineTextBaseOffset = window->DC.PrevLineTextBaseOffset;
    window->DC.IsSameLine = true;
}

void c_gui::begin_group()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;

    g.GroupStack.resize(g.GroupStack.Size + 1);
    ImGuiGroupData& group_data = g.GroupStack.back();
    group_data.WindowID = window->ID;
    group_data.BackupCursorPos = window->DC.CursorPos;
    group_data.BackupCursorPosPrevLine = window->DC.CursorPosPrevLine;
    group_data.BackupCursorMaxPos = window->DC.CursorMaxPos;
    group_data.BackupIndent = window->DC.Indent;
    group_data.BackupGroupOffset = window->DC.GroupOffset;
    group_data.BackupCurrLineSize = window->DC.CurrLineSize;
    group_data.BackupCurrLineTextBaseOffset = window->DC.CurrLineTextBaseOffset;
    group_data.BackupActiveIdIsAlive = g.ActiveIdIsAlive;
    group_data.BackupHoveredIdIsAlive = g.HoveredId != 0;
    group_data.BackupIsSameLine = window->DC.IsSameLine;
    group_data.BackupActiveIdPreviousFrameIsAlive = g.ActiveIdPreviousFrameIsAlive;
    group_data.EmitItem = true;

    window->DC.GroupOffset.x = window->DC.CursorPos.x - window->Pos.x - window->DC.ColumnsOffset.x;
    window->DC.Indent = window->DC.GroupOffset;
    window->DC.CursorMaxPos = window->DC.CursorPos;
    window->DC.CurrLineSize = ImVec2(0.0f, 0.0f);
    if (g.LogEnabled) g.LogLinePosY = -FLT_MAX;
}

void c_gui::end_group()
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = g.CurrentWindow;
    IM_ASSERT(g.GroupStack.Size > 0);

    ImGuiGroupData& group_data = g.GroupStack.back();
    IM_ASSERT(group_data.WindowID == window->ID);

    if (window->DC.IsSetPos) ErrorCheckUsingSetCursorPosToExtendParentBoundaries();

    ImRect group_bb(group_data.BackupCursorPos, ImMax(window->DC.CursorMaxPos, group_data.BackupCursorPos));

    window->DC.CursorPos = group_data.BackupCursorPos;
    window->DC.CursorPosPrevLine = group_data.BackupCursorPosPrevLine;
    window->DC.CursorMaxPos = ImMax(group_data.BackupCursorMaxPos, window->DC.CursorMaxPos);
    window->DC.Indent = group_data.BackupIndent;
    window->DC.GroupOffset = group_data.BackupGroupOffset;
    window->DC.CurrLineSize = group_data.BackupCurrLineSize;
    window->DC.CurrLineTextBaseOffset = group_data.BackupCurrLineTextBaseOffset;
    window->DC.IsSameLine = group_data.BackupIsSameLine;
    if (g.LogEnabled) g.LogLinePosY = -FLT_MAX;

    if (!group_data.EmitItem)
    {
        g.GroupStack.pop_back();
        return;
    }

    window->DC.CurrLineTextBaseOffset = ImMax(window->DC.PrevLineTextBaseOffset, group_data.BackupCurrLineTextBaseOffset);
    ImGui::ItemSize(group_bb.GetSize());
    ItemAdd(group_bb, 0, NULL, ImGuiItemFlags_NoTabStop);

    const bool group_contains_curr_active_id = (group_data.BackupActiveIdIsAlive != g.ActiveId) && (g.ActiveIdIsAlive == g.ActiveId) && g.ActiveId;
    const bool group_contains_prev_active_id = (group_data.BackupActiveIdPreviousFrameIsAlive == false) && (g.ActiveIdPreviousFrameIsAlive == true);
    if (group_contains_curr_active_id) g.LastItemData.ID = g.ActiveId;
    else if (group_contains_prev_active_id) g.LastItemData.ID = g.ActiveIdPreviousFrame;
    g.LastItemData.Rect = group_bb;

    const bool group_contains_curr_hovered_id = (group_data.BackupHoveredIdIsAlive == false) && g.HoveredId != 0;
    if (group_contains_curr_hovered_id) g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HoveredWindow;

    if (group_contains_curr_active_id && g.ActiveIdHasBeenEditedThisFrame)  g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_Edited;

    g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_HasDeactivated;
    if (group_contains_prev_active_id && g.ActiveId != g.ActiveIdPreviousFrame) g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_Deactivated;

    g.GroupStack.pop_back();
    if (g.DebugShowGroupRects) window->DrawList->AddRect(group_bb.Min, group_bb.Max, IM_COL32(255, 0, 255, 255));
}