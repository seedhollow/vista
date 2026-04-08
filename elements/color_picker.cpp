#include "../settings/functions.h"

void color_edit_restore_hs(const float* col, float* H, float* S, float* V)
{
    ImGuiContext& g{ *GImGui };

    if (*S == 0.0f || (*H == 0.0f && g.ColorEditSavedHue == 1)) *H = g.ColorEditSavedHue;
    if (*V == 0.0f) *S = g.ColorEditSavedSat;
}

void render_color_rect_alpha_checkboard(ImDrawList* draw_list, ImVec2 p_min, ImVec2 p_max, ImU32 col, float grid_step, ImVec2 grid_off, float rounding, ImDrawFlags flags)
{
    if ((flags & ImDrawFlags_RoundCornersMask_) == 0) flags = ImDrawFlags_RoundCornersDefault_;
    if (((col & IM_COL32_A_MASK) >> IM_COL32_A_SHIFT) < 0xFF)
    {
        ImU32 col_bg1 = GetColorU32(ImAlphaBlendColors(draw->get_clr(clr->color_picker.col_bg_alpha_0), col));
        ImU32 col_bg2 = GetColorU32(ImAlphaBlendColors(draw->get_clr(clr->color_picker.col_bg_alpha_1), col));

        draw->rect_filled(draw_list, p_min, p_max, col_bg1, rounding, flags);

        int yi = 0;
        for (float y = p_min.y + grid_off.y; y < p_max.y; y += grid_step, yi++)
        {
            float y1 = ImClamp(y, p_min.y, p_max.y), y2 = ImMin(y + grid_step, p_max.y);
            if (y2 <= y1)
                continue;
            for (float x = p_min.x + grid_off.x + (yi & 1) * grid_step; x < p_max.x; x += grid_step * 2.0f)
            {
                float x1 = ImClamp(x, p_min.x, p_max.x), x2 = ImMin(x + grid_step, p_max.x);
                if (x2 <= x1)
                    continue;
                ImDrawFlags cell_flags = ImDrawFlags_RoundCornersNone;
                if (y1 <= p_min.y) { if (x1 <= p_min.x) cell_flags |= ImDrawFlags_RoundCornersTopLeft; if (x2 >= p_max.x) cell_flags |= ImDrawFlags_RoundCornersTopRight; }
                if (y2 >= p_max.y) { if (x1 <= p_min.x) cell_flags |= ImDrawFlags_RoundCornersBottomLeft; if (x2 >= p_max.x) cell_flags |= ImDrawFlags_RoundCornersBottomRight; }

                cell_flags = (flags == ImDrawFlags_RoundCornersNone || cell_flags == ImDrawFlags_RoundCornersNone) ? ImDrawFlags_RoundCornersNone : (cell_flags & flags);
                draw->rect_filled(draw_list, ImVec2(x1, y1), ImVec2(x2, y2), col_bg2, rounding, cell_flags);
            }
        }
    }
    else
    {
        draw->rect_filled(draw_list, p_min, p_max, col, rounding, flags);
    }
}

bool pipette_button(std::string_view icon, const ImVec2& size, bool open)
{
    struct c_button
    {
        float alpha = 0;
    };

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(icon.data());

    const ImVec2 pos = window->DC.CursorPos;
    const ImRect rect(pos, pos + SCALE(size));

    ImGui::ItemSize(rect, 0);
    if (!ItemAdd(rect, id)) return false;

    bool hovered, held, pressed = ButtonBehavior(rect, id, &hovered, &held);
    c_button* state = gui->anim_container(&state, id);

    state->alpha = ImLerp(state->alpha, open || hovered ? 0.3f : 0.f, gui->fixed_speed(8.f));

    {
        draw->rect_filled(window->DrawList, rect.Min, rect.Max, draw->get_clr(clr->color_picker.button_layout), SCALE(element->color_picker.rounding));
        draw->rect_filled(window->DrawList, rect.Min, rect.Max, draw->get_clr(clr->other.accent, state->alpha), SCALE(element->color_picker.rounding));

        draw->text_clipped(window->DrawList, var->font.icon[2], rect.Min, rect.Max, draw->get_clr(clr->text.text_active), icon.data(), NULL, NULL, { 0.5, 0.5 }, NULL);
    }

    return pressed;
}

float hue_bar(float col[4], float h, ImVec2 size, bool* value_changed) {

    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImGuiIO& io = ImGui::GetIO();

    float w = SCALE(size.x);
    static float move;

    ImVec2 pos = window->DC.CursorPos;
    ImRect rect(pos, pos + ImVec2(w, SCALE(size.y)));

    const ImU32 col_hues[6 + 1]{ IM_COL32(255, 0, 0, IM_F32_TO_INT8_SAT(style.Alpha)), IM_COL32(255, 255, 0, IM_F32_TO_INT8_SAT(style.Alpha)), IM_COL32(0, 255, 0, IM_F32_TO_INT8_SAT(style.Alpha)), IM_COL32(0, 255, 255, IM_F32_TO_INT8_SAT(style.Alpha)), IM_COL32(0, 0, 255, IM_F32_TO_INT8_SAT(style.Alpha)), IM_COL32(255, 0, 255, IM_F32_TO_INT8_SAT(style.Alpha)), IM_COL32(255, 0, 0, IM_F32_TO_INT8_SAT(style.Alpha)) };    const ImU32 col_midgrey = IM_COL32(128, 128, 128, IM_F32_TO_INT8_SAT(style.Alpha));

    for (int i = 0; i < 6; ++i)
        draw->rect_filled_multi_color(window->DrawList, ImVec2(pos.x + i * (w / 6) - (i == 5 ? 1 : 0), rect.Min.y + SCALE(2)), ImVec2(pos.x + (i + 1) * (w / 6) + (i == 0 ? 1 : 0), rect.Max.y - SCALE(2)), col_hues[i], col_hues[i + 1], col_hues[i + 1], col_hues[i], SCALE(100.f), i == 0 ? ImDrawFlags_RoundCornersLeft : i == 5 ? ImDrawFlags_RoundCornersRight : ImDrawFlags_RoundCornersNone);

    move = ImLerp(move, h, gui->fixed_speed(25.f));
    draw->rect_filled(window->DrawList, { rect.Min.x + rect.GetHeight() / 2 + (w - rect.GetHeight()) * move - SCALE(3), rect.GetCenter().y - SCALE(5) }, { rect.Min.x + rect.GetHeight() / 2 + (w - rect.GetHeight()) * move + SCALE(3), rect.GetCenter().y + SCALE(5) }, draw->get_clr(clr->other.white), 4.f);

    ImGui::InvisibleButton("hue", rect.GetSize());
    if (IsItemActive()) {
        h = ImSaturate((io.MousePos.x - pos.x) / (rect.GetWidth() - 1));
        *value_changed = true;
    }

    return h;
}

bool alpha_bar(float col[4], float* a, ImVec2 size, bool actived, bool* value_changed) {

    if (!actived) return false;

    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImGuiIO& io = ImGui::GetIO();

    float w = SCALE(size.x);
    static float move;

    ImVec2 pos{ window->DC.CursorPos };
    ImRect rect(pos, pos + ImVec2(w, SCALE(size.y)));

    move = ImLerp(move, *a, gui->fixed_speed(25.f));

    draw->rect_filled_multi_color(window->DrawList, rect.Min + SCALE(0, 2), rect.Max - SCALE(0, 2), ImColor{ col[0], col[1], col[2], 0.f }, ImColor{ col[0], col[1], col[2], GImGui->Style.Alpha }, ImColor{ col[0], col[1], col[2], GImGui->Style.Alpha }, ImColor{ col[0], col[1], col[2], 0.f }, SCALE(100.f));
    draw->rect_filled(window->DrawList, { rect.Min.x + rect.GetHeight() / 2 + (w - rect.GetHeight()) * move - SCALE(3), rect.GetCenter().y - SCALE(5)}, {rect.Min.x + rect.GetHeight() / 2 + (w - rect.GetHeight()) * move + SCALE(3), rect.GetCenter().y + SCALE(5) }, draw->get_clr(clr->other.white), 4.f);

    ImGui::InvisibleButton("alpha", rect.GetSize());
    if (IsItemActive()) {
        *a = ImSaturate((io.MousePos.x - pos.x) / (rect.GetWidth() - 1));
        *value_changed = true;
    }
}

void color(float col[4], float h, float* s, float* v, const ImVec2& size_arg, bool* value_changed) {

    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImGuiIO& io = ImGui::GetIO();

    static ImVec2 move;

    ImVec2 pos = window->DC.CursorPos;
    ImRect rect(pos, pos + SCALE(size_arg));

    ImVec4 hue_color_f(1, 1, 1, style.Alpha); ImGui::ColorConvertHSVtoRGB(h, 1, 1, hue_color_f.x, hue_color_f.y, hue_color_f.z);
    ImU32 hue_color32 = ColorConvertFloat4ToU32(hue_color_f);

    draw->rect_filled_multi_color(window->DrawList, rect.Min, rect.Max, draw->get_clr(clr->color_picker.white_color), hue_color32, hue_color32, draw->get_clr(clr->color_picker.white_color), SCALE(element->color_picker.rounding));
    draw->rect_filled_multi_color(window->DrawList, rect.Min - SCALE(1, 1), rect.Max + SCALE(1, 1), 0, 0, draw->get_clr(clr->color_picker.black_color), draw->get_clr(clr->color_picker.black_color), SCALE(element->color_picker.rounding));

    ImVec2 cursor_pos;
    cursor_pos.x = ImClamp(IM_ROUND(rect.Min.x + ImSaturate(*s) * rect.GetWidth()), rect.Min.x + SCALE(2), rect.Max.x - SCALE(2));
    cursor_pos.y = ImClamp(IM_ROUND(rect.Min.y + ImSaturate(1 - *v) * rect.GetHeight()), rect.Min.y + SCALE(2), rect.Max.y - SCALE(2));

    auto circle_col{ ImColor(col[0], col[1], col[2], 1.f) };
    move = ImLerp(move, cursor_pos, gui->fixed_speed(25.f));

    draw->circle_filled(window->DrawList, move, SCALE(5), draw->get_clr(circle_col));
    draw->circle(window->DrawList, move, SCALE(5), ImColor(255, 255, 255, int(style.Alpha * 255)), 100.f, SCALE(2.f));

    ImGui::InvisibleButton("sv", rect.GetSize());
    if (IsItemActive())
    {
        *s = ImSaturate((io.MousePos.x - rect.Min.x) / (rect.GetWidth() - 1)) <= 0 ? 0.0f : ImSaturate((io.MousePos.x - rect.Min.x) / (rect.GetWidth() - 1));
        *v = (1.0f - ImSaturate((io.MousePos.y - rect.Min.y) / (rect.GetHeight() - 1))) <= 0 ? 0.0f : 1.0f - ImSaturate((io.MousePos.y - rect.Min.y) / (rect.GetHeight() - 1));

        *value_changed = true;
    }
}

bool c_widget::color_picker(std::string_view label, float col[4], bool alpha)
{
    struct c_edit
    {
        ImVec4 text = clr->text.text_inactive;
        ImVec2 position{ 0, 0 };

        float alpha = 0.f;
        bool pressed = false, hovered = false;
        bool pipette_active = false;

        ImVec2 content_size;
    };

    ImGuiContext& g_1 = *GImGui;
    const ImGuiStyle& style = g_1.Style;
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    const ImGuiID id = window->GetID(label.data());

    const ImVec2 pos = GetCurrentWindow()->DC.CursorPos;
    const float width = gui->content_region_avail().x;
    const float rect_correct_size = SCALE(45);

    const ImVec2 rect_size = SCALE(15, 15);
    const ImRect rect(pos, pos + ImVec2(width, rect_correct_size));
    ImRect picker_button(pos + ImVec2(width - SCALE(15), SCALE(15)), pos + ImVec2(width, rect_correct_size - SCALE(15)));

    char buf[64];
    bool value_changed = false;
    float h = 1.f, s = 1.f, v = 1.f, r = col[0], g = col[1], b = col[2], a = col[3];

    ItemSize(rect, 0);
    if (!ItemAdd(rect, id)) { return false; }

    bool hovered, held, pressed{ ButtonBehavior(rect, id, &hovered, &held) };

    c_edit* state = gui->anim_container(&state, id);

    if (ItemHoverable(g_1.LastItemData.Rect, g_1.LastItemData.ID, NULL) && g_1.IO.MouseClicked[0] || state->pressed && g_1.IO.MouseClicked[0] && !state->hovered && !state->pipette_active) state->pressed = !state->pressed;
    state->text = ImLerp(state->text, state->pressed ? clr->text.text_active : hovered ? clr->text.text_hovered : /*clr->text.text_inactive*/ clr->text.text_active, gui->fixed_speed(8.f));
    state->alpha = ImClamp(state->alpha + (gui->fixed_speed(element->color_picker.animation_tickrate) * (state->pressed ? 1.f : -1.f)), 0.f, 1.f);

    draw->text_clipped(window->DrawList, var->font.inter_medium[0], rect.Min, rect.Max, draw->get_clr(state->text), label.data(), NULL, NULL, ImVec2(0.0f, 0.5f));

    float f[4] = { col[0], col[1], col[2], alpha ? col[3] : 1.0f };
    int i[4] = { IM_F32_TO_INT8_UNBOUND(f[0]), IM_F32_TO_INT8_UNBOUND(f[1]), IM_F32_TO_INT8_UNBOUND(f[2]), IM_F32_TO_INT8_UNBOUND(f[3]) };

    ColorConvertRGBtoHSV(r, g, b, h, s, v);
    color_edit_restore_hs(col, &h, &s, &v);

    draw->circle_filled(window->DrawList, picker_button.GetCenter(), SCALE(7.f), draw->get_clr(ImVec4(col[0], col[1], col[2], col[3])), SCALE(100.f));
    draw->line(window->DrawList, rect.Min + ImVec2(0, rect_correct_size), rect.Max, draw->get_clr(clr->other.element_separator), 1.f);

    if (state->alpha > 0.1f)
    {
        gui->push_style_var(ImGuiStyleVar_Alpha, { state->alpha });
        gui->push_style_var(ImGuiStyleVar_WindowPadding, SCALE(element->color_picker.padding));
        gui->push_style_var(ImGuiStyleVar_ItemSpacing, SCALE(element->color_picker.spacing));
        gui->push_style_var(ImGuiStyleVar_WindowRounding, SCALE(element->color_picker.popup_rounding));
        gui->push_style_var(ImGuiStyleVar_WindowBorderSize, 0);

        gui->push_style_color(ImGuiCol_WindowBg, draw->get_clr(clr->other.open_popup));
        gui->set_next_window_pos(ImClamp(g_1.LastItemData.Rect.GetBR(), ImVec2(0, 0), g_1.IO.DisplaySize - state->content_size));

        gui->begin((std::stringstream{} << label << "picker").str().c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration);
        {
            state->hovered = IsWindowHovered(), state->content_size = GetWindowSize() + SCALE(15, 15);
            element->color_picker.open_picker = state->pressed;

            color(col, h, &s, &v, { 200, 80 }, &value_changed);

            gui->begin_group();
            {

                if (!alpha) gui->set_cursor_pos_y(gui->get_cursor_pos_y() + SCALE(10));

                h = hue_bar(col, h, { 140, 10 }, &value_changed);

                alpha_bar(col, &a, { 140, 10 }, alpha, &value_changed);

            }
            gui->end_group();

            gui->sameline();

            if (pipette_button("K", { 50, 30 }, state->pipette_active)) state->pipette_active = true;

            if (state->pipette_active && g_1.IO.MouseClicked[0] && !state->hovered)
            {
                HDC hdc_screen = GetDC(NULL);
                COLORREF pick_color = GetPixel(hdc_screen, GetMousePos().x, GetMousePos().y);
                ReleaseDC(NULL, hdc_screen);

                col[0] = GetRValue(pick_color) / 255.0f;
                col[1] = GetGValue(pick_color) / 255.0f;
                col[2] = GetBValue(pick_color) / 255.0f;

                state->pipette_active = false;
            }

            if (alpha)
                ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X%02X", ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255), ImClamp(i[3], 0, 255));
            else
                ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X", ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255));

            if (widget->text_field("HEX", "V", buf, IM_ARRAYSIZE(buf), SCALE(200, 30), draw->get_clr(clr->color_picker.button_layout), element->text_field.rounding, ImGuiInputTextFlags_CharsUppercase))
            {
                value_changed = true;
                char* p = buf;
                while (*p == '#' || ImCharIsBlankA(*p)) p++;

                i[0] = i[1] = i[2] = 0;
                i[3] = 0xFF;

                int ri = sscanf(p, "%02X%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2], (unsigned int*)&i[3]);

                r = i[0] / 255.f, g = i[1] / 255.f, b = i[2] / 255.f;

                ImGui::ColorConvertRGBtoHSV(r, g, b, h, s, v);
                IM_UNUSED(ri);

            }

            ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);
            color_edit_restore_hs(col, &f[0], &f[1], &f[2]);
        }
        gui->end();

        gui->pop_style_var(5);
        gui->pop_style_color(1);
    }
    ColorConvertHSVtoRGB(h, s, v, r, g, b);

    if (value_changed) {

        g_1.ColorEditSavedHue = h;
        g_1.ColorEditSavedSat = s;
        g_1.ColorEditSavedColor = ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0));

        col[0] = r;
        col[1] = g;
        col[2] = b;
        col[3] = a;
    }

    return value_changed;
}

bool c_widget::checkbox_with_picker(std::string_view label, bool* callback, float col[4], bool alpha, std::string_view hinting)
{
    struct checkbox_state
    {
        ImVec4 layout, circle, hint, text, cog;
        float circle_spring, alpha, popup_alpha;

        bool pipette_active = false;
        bool pressed = false, hovered = false;

        ImVec2 content_size;
    };

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    ImGuiContext& g_1 = *GImGui;
    const ImGuiStyle& style = g_1.Style;
    const ImGuiID id = window->GetID(label.data());

    ImVec2 pos = window->DC.CursorPos;
    float width = gui->content_region_avail().x;
    float height = SCALE(45);

    ImRect rect(pos, pos + ImVec2(width, height));
    ImRect switch_rect(pos + ImVec2(width - SCALE(30), SCALE(15)), pos + ImVec2(width, height - SCALE(15)));
    ImRect picker_button(pos + ImVec2(width - SCALE(60), SCALE(15)), pos + ImVec2(width - SCALE(45), height - SCALE(15)));

    bool hint_target = IsMouseHoveringRect(rect.Min + ImVec2(CalcTextSize(label.data()).x + SCALE(10), SCALE(15)), rect.Min + ImVec2(CalcTextSize(label.data()).x + SCALE(30), CalcTextSize(label.data()).y + SCALE(15)), true);
    bool picker_target = IsMouseHoveringRect(picker_button.Min, picker_button.Max, true);
    ImVec2 content_size;

    char buf[64];

    bool value_changed = false;
    float h = 1.f, s = 1.f, v = 1.f, r = col[0], g = col[1], b = col[2], a = col[3];

    ItemSize(rect, 0);
    if (!ItemAdd(rect, id)) return false;

    bool hovered, held, pressed = ButtonBehavior(!picker_target ? rect : ImRect(), id, &hovered, &held);

    float f[4] = { col[0], col[1], col[2], alpha ? col[3] : 1.0f };
    int i[4] = { IM_F32_TO_INT8_UNBOUND(f[0]), IM_F32_TO_INT8_UNBOUND(f[1]), IM_F32_TO_INT8_UNBOUND(f[2]), IM_F32_TO_INT8_UNBOUND(f[3]) };

    ColorConvertRGBtoHSV(r, g, b, h, s, v);
    color_edit_restore_hs(col, &h, &s, &v);

    if (IsItemClicked() && !picker_target)
    {
        *callback = !(*callback);
        MarkItemEdited(id);
    }

    checkbox_state* state = gui->anim_container(&state, id);

    if (picker_target && ItemHoverable(g_1.LastItemData.Rect, g_1.LastItemData.ID, NULL) && g_1.IO.MouseClicked[0] || state->pressed && g_1.IO.MouseClicked[0] && !state->hovered && !state->pipette_active) state->pressed = !state->pressed;

    state->circle_spring = ImLerp(state->circle_spring, *callback ? SCALE(15 / 2) : SCALE(-15 / 2), gui->fixed_speed(element->checkbox.animation_tickrate));
    state->circle = ImLerp(state->circle, *callback ? clr->other.accent : clr->checkbox.circle, gui->fixed_speed(element->checkbox.animation_tickrate));
    state->text = ImLerp(state->text, *callback ? clr->text.text_active : hovered ? clr->text.text_hovered : clr->text.text_inactive, gui->fixed_speed(element->checkbox.animation_tickrate));
    state->hint = ImLerp(state->hint, hint_target ? clr->other.accent : clr->text.text_inactive, gui->fixed_speed(element->checkbox.animation_tickrate));
    state->cog = ImLerp(state->cog, state->pressed || picker_target ? clr->text.text_active : clr->text.text_inactive, gui->fixed_speed(element->checkbox.animation_tickrate));

    state->alpha = ImLerp(state->alpha, *callback ? 0.2f : 0.f, gui->fixed_speed(element->checkbox.animation_tickrate));
    state->popup_alpha = ImClamp(state->popup_alpha + (gui->fixed_speed(element->color_picker.animation_tickrate) * (state->pressed ? 1.f : -1.f)), 0.f, 1.f);

    if (state->popup_alpha > 0.1f)
    {

        gui->push_style_var(ImGuiStyleVar_Alpha, { state->popup_alpha });
        gui->push_style_var(ImGuiStyleVar_WindowPadding, SCALE(element->color_picker.padding));
        gui->push_style_var(ImGuiStyleVar_ItemSpacing, SCALE(element->color_picker.spacing));
        gui->push_style_var(ImGuiStyleVar_WindowRounding, SCALE(element->color_picker.popup_rounding));
        gui->push_style_var(ImGuiStyleVar_WindowBorderSize, 0);

        gui->push_style_color(ImGuiCol_WindowBg, draw->get_clr(clr->other.open_popup));

        gui->set_next_window_pos(ImClamp(g_1.LastItemData.Rect.GetBR(), ImVec2(0, 0), g_1.IO.DisplaySize - state->content_size));

        if (gui->begin((std::stringstream{} << GetCurrentWindow()->GetID(label.data()) << "popup").str().c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse))
        {
            state->hovered = IsWindowHovered(), state->content_size = GetWindowSize() + SCALE(15, 15);
            element->color_picker.open_picker = state->pressed;

            color(col, h, &s, &v, { 200, 80 }, &value_changed);

            gui->begin_group();
            {
                if (!alpha) gui->set_cursor_pos_y(gui->get_cursor_pos_y() + SCALE(10));

                h = hue_bar(col, h, { 140, 10 }, &value_changed);

                alpha_bar(col, &a, { 140, 10 }, alpha, &value_changed);

            }
            gui->end_group();

            gui->sameline();

            if (pipette_button("K", { 50, 30 }, state->pipette_active)) state->pipette_active = true;

            if (state->pipette_active && g_1.IO.MouseClicked[0] && !state->hovered)
            {
                HDC hdc_screen = GetDC(NULL);
                COLORREF pick_color = GetPixel(hdc_screen, GetMousePos().x, GetMousePos().y);
                ReleaseDC(NULL, hdc_screen);

                col[0] = GetRValue(pick_color) / 255.0f;
                col[1] = GetGValue(pick_color) / 255.0f;
                col[2] = GetBValue(pick_color) / 255.0f;

                state->pipette_active = false;
            }

            if (alpha)
                ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X%02X", ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255), ImClamp(i[3], 0, 255));
            else
                ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X", ImClamp(i[0], 0, 255), ImClamp(i[1], 0, 255), ImClamp(i[2], 0, 255));

            if (widget->text_field("HEX", "V", buf, IM_ARRAYSIZE(buf), SCALE(200, 30), draw->get_clr(clr->color_picker.button_layout), element->text_field.rounding, ImGuiInputTextFlags_CharsUppercase))
            {
                value_changed = true;
                char* p = buf;
                while (*p == '#' || ImCharIsBlankA(*p))
                    p++;
                i[0] = i[1] = i[2] = 0;
                i[3] = 0xFF;
                int ri = sscanf(p, "%02X%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2], (unsigned int*)&i[3]);
                r = i[0] / 255.f;
                g = i[1] / 255.f;
                b = i[2] / 255.f;
                ImGui::ColorConvertRGBtoHSV(r, g, b, h, s, v);
                IM_UNUSED(ri);

            }

            ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);
            color_edit_restore_hs(col, &f[0], &f[1], &f[2]);

        }
        gui->end();

        gui->pop_style_var(5);
        gui->pop_style_color(1);
    }

    ColorConvertHSVtoRGB(h, s, v, r, g, b);

    if (value_changed) {

        g_1.ColorEditSavedHue = h;
        g_1.ColorEditSavedSat = s;
        g_1.ColorEditSavedColor = ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 0));

        col[0] = r;
        col[1] = g;
        col[2] = b;
        col[3] = a;
    }


    ImVec4 col_rgb{ col[0], col[1], col[2], col[3] };

    {
        draw->rect_filled(window->DrawList, switch_rect.Min, switch_rect.Max, draw->get_clr(clr->checkbox.layout), SCALE(element->checkbox.rounding));
        draw->rect_filled(window->DrawList, switch_rect.Min, switch_rect.Max, draw->get_clr(clr->other.accent, state->alpha), SCALE(element->checkbox.rounding));
        draw->circle_filled(window->DrawList, picker_button.GetCenter(), SCALE(7.f), draw->get_clr(col_rgb), SCALE(100.f));

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