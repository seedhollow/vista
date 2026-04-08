#include "../settings/functions.h"

void c_notify::add_notify(std::string_view text, notify_status status_notify)
{
    notifications.push_back({ notify_count++, text, status_notify });
}

void c_notify::setup_notify()
{
    int cur_notify_value = 0;

    for (auto& notification : notifications)
    {

        if (notification.active_notify) notification.notify_timer += 1000.f / (notification.notify_delay * 10);
        if (notification.notify_timer >= notify_time) notification.active_notify = false;

        notification.notify_alpha = ImClamp(notification.notify_alpha + (gui->fixed_speed(8.f) * (notification.active_notify ? 1.f : -1.f)), 0.f, 1.f);
        notification.notify_pos = ImLerp(notification.notify_pos, cur_notify_value * SCALE(element->notify.height + element->notify.screen_padding), gui->fixed_speed(8.f));

        if (notification.notify_alpha <= 0.f && !notification.active_notify) cur_notify_value--;

        render_notify(cur_notify_value, notification.notify_alpha, notification.notify_timer, notification.notify_pos, notification.text, notification.status_notify);
        cur_notify_value++;
    }
}

void c_notify::render_notify(int cur_notify_value, float notify_alpha, float notify_percentage, float notify_pos, std::string_view text, notify_status status_notify)
{
    struct spectator_state
    {
        ImVec2 position, content_size;
    };

    spectator_state* state = gui->anim_container(&state, ImGui::GetID((std::stringstream() << "notify" << cur_notify_value).str().c_str()));

    const float text_width = var->font.inter_medium[0]->CalcTextSizeA(var->font.inter_medium[0]->FontSize, FLT_MAX, -1.f, text.data()).x;
    const float max_width = max(text_width, text_width) + SCALE(element->notify.padding.x * 2);

    switch (element->notify.notify_pos)
    {
    case top_left:
        state->position = ImVec2(SCALE(element->notify.screen_padding), notify_pos + SCALE(element->notify.screen_padding));
        break;
    case top_right:
        state->position = ImVec2(GetIO().DisplaySize.x - state->content_size.x, notify_pos + SCALE(element->notify.screen_padding));
        break;
    case bottom_left:
        state->position = ImVec2(SCALE(element->notify.screen_padding), GetIO().DisplaySize.y - (notify_pos + state->content_size.y));
        break;
    case bottom_right:
        state->position = ImVec2(GetIO().DisplaySize.x - state->content_size.x, GetIO().DisplaySize.y - (notify_pos + state->content_size.y));
        break;
    }

    gui->set_next_window_size(ImVec2(max_width > SCALE(element->notify.min_line_size.x) ? max_width : SCALE(element->notify.min_line_size.x), SCALE(element->notify.height)));
    gui->set_next_window_pos(state->position);

    gui->push_style_var(ImGuiStyleVar_Alpha, notify_alpha);
    gui->push_style_var(ImGuiStyleVar_WindowRounding, SCALE(element->notify.rounding));
    gui->push_style_var(ImGuiStyleVar_WindowPadding, SCALE(element->notify.padding));
    gui->push_style_var(ImGuiStyleVar_PopupBorderSize, 0.f);
    gui->push_style_color(ImGuiCol_PopupBg, draw->get_clr(clr->window.layout));

    gui->begin((std::stringstream() << "notify" << cur_notify_value).str(), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Tooltip);
    {
        const ImVec2 content_region = gui->content_region_avail(), pos = ImGui::GetWindowPos(), size = GetWindowSize();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        ImVec4 col = status_notify == notify_status::success ? clr->notify.success : status_notify == notify_status::warning ? clr->notify.warning : clr->notify.error;
        const float progress = ImClamp(notify_percentage / notify_time, 0.f, 1.f);
        const float line_width = (size.x - GetStyle().WindowPadding.x * 2) * progress;

        { // decorations

            draw->rect_filled(draw_list, pos + ImVec2(GetStyle().WindowPadding.x, size.y - GetStyle().WindowPadding.y - SCALE(5)), pos + size - GetStyle().WindowPadding, draw->get_clr(clr->notify.line), SCALE(element->notify.rounding));
            draw->rect_filled(draw_list, pos + ImVec2(GetStyle().WindowPadding.x, size.y - GetStyle().WindowPadding.y - SCALE(5)), pos + size - GetStyle().WindowPadding - ImVec2(line_width, 0), draw->get_clr(col), SCALE(element->notify.rounding));

            draw->text_clipped(draw_list, var->font.inter_medium[0], { pos + GetStyle().WindowPadding }, { pos.x + size.x, pos.y + size.y }, draw->get_clr(clr->text.text_active), text.data(), NULL, NULL, { 0.0, 0.0 }, NULL);
        }

        state->content_size = GetWindowSize() + SCALE(element->notify.padding);
    }
    gui->end();

    gui->pop_style_var(4);
    gui->pop_style_color(1);
}