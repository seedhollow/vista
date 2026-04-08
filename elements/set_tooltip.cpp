#include "../settings/functions.h"

void c_widget::text_colored(ImFont* font, const ImU32 col, std::string text)
{
    gui->push_font(font);
    TextColored(ImColor(col), text.data());
    gui->pop_font();
}

bool c_widget::set_tooltip(std::string_view tooltip_text, bool show_tooltip)
{

    struct popup_state
    {
        bool hovered;
        float begin_offset, alpha_popup;
        ImVec2 content_size;
    };

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    popup_state* state = gui->anim_container(&state, GetCurrentWindow()->GetID(tooltip_text.data()));
    state->alpha_popup = ImClamp(state->alpha_popup + (gui->fixed_speed(6.f) * (!element->settings_popup.open_popup && show_tooltip ? 1.f : -1.f)), 0.f, 1.f);

        gui->push_style_var(ImGuiStyleVar_Alpha, state->alpha_popup);
        gui->push_style_var(ImGuiStyleVar_WindowBorderSize, element->tooltip.border_size);
        gui->push_style_var(ImGuiStyleVar_WindowRounding, SCALE(element->tooltip.rounding));
        gui->push_style_var(ImGuiStyleVar_WindowPadding, SCALE(element->tooltip.padding));
        gui->push_style_color(ImGuiCol_WindowBg, draw->get_clr(clr->other.open_popup));
        gui->push_style_color(ImGuiCol_Border, draw->get_clr(clr->other.accent, 0.2f));

        gui->set_next_window_pos(ImClamp(ImGui::GetMousePos() + SCALE(20, 20), ImVec2(0, 0), g.IO.DisplaySize - state->content_size));

        if (gui->begin((std::stringstream{} << GetCurrentWindow()->GetID(tooltip_text.data()) << "popup").str().c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse))
        {
            state->hovered = IsMouseHoveringRect(GetWindowPos(), GetWindowPos() + GetWindowSize()), state->content_size = GetWindowSize() + SCALE(15, 15);
            element->tooltip.hovered = state->alpha_popup > 0.1f;

            text_colored(var->font.inter_medium[0], draw->get_clr(clr->text.text_active), tooltip_text.data());
        }
        gui->end();

        gui->pop_style_var(4);
        gui->pop_style_color(2);

    return state->hovered;
}