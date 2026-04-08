#include "../settings/functions.h"

int parse_format_precision(const char* fmt, int default_precision)
{
    int precision = default_precision;
    while ((fmt = strchr(fmt, '%')) != NULL)
    {
        fmt++;
        if (fmt[0] == '%') { fmt++; continue; }
        while (*fmt >= '0' && *fmt <= '9')
            fmt++;
        if (*fmt == '.')
        {
            precision = atoi(fmt + 1);
            if (precision < 0 || precision > 10)
                precision = default_precision;
        }
        break;
    }
    return precision;
}

static inline float slider_berhavior_calc_ratio_value(float v, float v_min, float v_max, float power, float linear_zero_pos)
{
    if (v_min == v_max)
        return 0.0f;

    const bool is_non_linear = (power < 1.0f - 0.00001f) || (power > 1.0f + 0.00001f);
    const float v_clamped = (v_min < v_max) ? ImClamp(v, v_min, v_max) : ImClamp(v, v_max, v_min);
    if (is_non_linear)
    {
        if (v_clamped < 0.0f)
        {
            const float f = 1.0f - (v_clamped - v_min) / (ImMin(0.0f, v_max) - v_min);
            return (1.0f - powf(f, 1.0f / power)) * linear_zero_pos;
        }
        else
        {
            const float f = (v_clamped - ImMax(0.0f, v_min)) / (v_max - ImMax(0.0f, v_min));
            return linear_zero_pos + powf(f, 1.0f / power) * (1.0f - linear_zero_pos);
        }
    }

    return (v_clamped - v_min) / (v_max - v_min);
}

template<typename T>
T round_scalar(T value, int decimal_precision) {
    if constexpr (std::is_integral_v<T>)
        return value;
    else {
        float multiplier = std::pow(10.0f, decimal_precision);
        return std::round(value * multiplier) / multiplier;
    }
}

template<typename T>
void upd_format_value(char* value_buf, size_t buf_size, const char* display_format, T* value, T& slow_value) {
    static_assert(std::is_arithmetic_v<T>, "T must be an arithmetic type");

    T target_value = *value;
    T step = (target_value - slow_value) * static_cast<T>(gui->fixed_speed(element->slider.animation_tickrate));
    if (step == 0 && slow_value != target_value) step = (target_value > slow_value) ? 1 : -1;

    slow_value += step;

    if (std::fabs(slow_value) < std::numeric_limits<float>::epsilon()) {
        slow_value = 0.f;
    }

    if constexpr (std::is_floating_point_v<T>) {
        snprintf(value_buf, buf_size, display_format, slow_value);
    }
    else {
        snprintf(value_buf, buf_size, display_format, static_cast<int>(slow_value));
    }
}

template<typename T>
bool range_slider_behavior(const ImRect& frame_bb, ImGuiID id, T* v1, T* v2, T v_min, T v_max, T range, float power, int decimal_precision, ImGuiSliderFlags flags)
{
    struct range_behavior_state {
        float slow_min = 0.f;
        float slow_max = 0.f;
        int active_grab = -1;
    };

    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();
    const ImGuiStyle& style = g.Style;

    const float slider_sz = frame_bb.GetWidth();
    const float slider_usable_pos_min = frame_bb.Min.x;
    const float slider_usable_pos_max = frame_bb.Max.x;

    float grab_sz = 1;

    range_behavior_state* state = gui->anim_container(&state, id);

    const float slider_usable_sz = slider_sz;
    const float linear_zero_pos = (v_min * v_max < 0.0f) ? std::pow(std::abs(0.0f - v_min), 1.0f / power) / (std::pow(std::abs(0.0f - v_min), 1.0f / power) + std::pow(std::abs(v_max - 0.0f), 1.0f / power)) : (v_min < 0.0f ? 1.0f : 0.0f);
    bool value_changed = false;
    if (g.ActiveId == id) {
        if (g.IO.MouseDown[0]) {
            const float mouse_abs_pos = g.IO.MousePos.x;
            float clicked_t = (slider_usable_sz > 0.0f) ? ImClamp((mouse_abs_pos - slider_usable_pos_min) / slider_usable_sz, 0.0f, 1.0f) : 0.0f;

            T new_value = (power != 1.0f) ?
                (clicked_t < linear_zero_pos ? ImLerp(ImMin(v_max, static_cast<T>(0)), v_min, std::pow(1.0f - (clicked_t / linear_zero_pos), power))
                    : ImLerp(ImMax(v_min, static_cast<T>(0)), v_max, std::pow((clicked_t - linear_zero_pos) / (1.0f - linear_zero_pos), power)))
                : ImLerp(v_min, v_max, clicked_t);

            new_value = round_scalar(new_value, decimal_precision);

            if (state->active_grab == 0) {
                *v1 = ImMin(new_value, *v2 - range);
                value_changed = true;
            }
            else if (state->active_grab == 1) {
                *v2 = ImMax(new_value, *v1 + range);
                value_changed = true;
            }
            else {
                if (std::abs(*v1 - new_value) < std::abs(*v2 - new_value)) {
                    state->active_grab = 0;
                    *v1 = ImMin(new_value, *v2 - range);
                }
                else {
                    state->active_grab = 1;
                    *v2 = ImMax(new_value, *v1 + range);
                }
                value_changed = true;
            }
        }
        else {
            SetActiveID(0, NULL);
            state->active_grab = -1;
        }
    }

    float grab_t = slider_berhavior_calc_ratio_value(*v1, v_min, v_max, power, linear_zero_pos);
    float grab_pos = ImLerp(slider_usable_pos_min + SCALE(7), slider_usable_pos_max - SCALE(4), grab_t);

    ImRect grab_bb1 = ImRect(ImVec2(grab_pos - grab_sz, frame_bb.Min.y), ImVec2(grab_pos + grab_sz, frame_bb.Max.y));

    grab_t = slider_berhavior_calc_ratio_value(*v2, v_min, v_max, power, linear_zero_pos);
    grab_pos = ImLerp(slider_usable_pos_min + SCALE(4), slider_usable_pos_max - SCALE(7), grab_t);

    ImRect grab_bb2 = ImRect(ImVec2(grab_pos - grab_sz, frame_bb.Min.y), ImVec2(grab_pos + grab_sz, frame_bb.Max.y));

    state->slow_min = ImLerp(state->slow_min, grab_bb1.GetCenter().x - frame_bb.Min.x, gui->fixed_speed(20.f));
    state->slow_max = ImLerp(state->slow_max, grab_bb2.GetCenter().x - frame_bb.Min.x, gui->fixed_speed(20.f));

    draw->rect_filled(window->DrawList, frame_bb.Min, frame_bb.Max, draw->get_clr(clr->slider.layout), SCALE(element->slider.rounding));
    draw->rect_filled_multi_color(window->DrawList, frame_bb.Min + ImVec2(state->slow_min - SCALE(7), 0), ImVec2(frame_bb.Min.x + state->slow_max + SCALE(7), frame_bb.Max.y), draw->get_clr(clr->other.accent), draw->get_clr(clr->other.accent, 0.5f), draw->get_clr(clr->other.accent, 0.5f), draw->get_clr(clr->other.accent), SCALE(element->slider.rounding));

    draw->rect_filled(window->DrawList, { frame_bb.Min.x + roundf(state->slow_min) - SCALE(7), frame_bb.GetCenter().y - SCALE(4) }, { frame_bb.Min.x + roundf(state->slow_min) + SCALE(7), frame_bb.GetCenter().y + SCALE(4) }, draw->get_clr(clr->other.white), SCALE(clr->other.white));
    draw->rect_filled(window->DrawList, { frame_bb.Min.x + roundf(state->slow_max) - SCALE(7), frame_bb.GetCenter().y - SCALE(4) }, { frame_bb.Min.x + roundf(state->slow_max) + SCALE(7), frame_bb.GetCenter().y + SCALE(4) }, draw->get_clr(clr->other.white), SCALE(clr->other.white));

    if (var->selection.selection_alpha == 0)
    {
        state->slow_min = 0;
        state->slow_max = 0;
    }

    return value_changed;
}

template<typename T>
bool range_slider(std::string_view label, T* v1, T* v2, T v_min, T v_max, T range, const char* display_format, std::string_view hinting)
{
    struct range_slider_state
    {
        ImVec4 hint, text = clr->text.text_inactive;

        T slow_value{ 0 };
        T slow_value2{ 0 };
    };

    ImGuiWindow* window = GetCurrentWindow();

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label.data());
    const ImGuiID range_id = window->GetID((std::stringstream{} << label.data() << "1").str().c_str());

    const float width = gui->content_region_avail().x;
    const float rect_size = SCALE(65);
    const float padding = SCALE(15);

    const ImVec2 pos = window->DC.CursorPos;

    const ImRect rect(pos, pos + ImVec2(width, rect_size));
    const ImRect slider(pos + ImVec2(0, rect_size - (padding + SCALE(element->slider.slider_size))), pos + ImVec2(width, rect_size - padding));

    bool hint_target = IsMouseHoveringRect(rect.Min + ImVec2(CalcTextSize(label.data()).x + SCALE(10), SCALE(15)), rect.Min + ImVec2(CalcTextSize(label.data()).x + SCALE(30), CalcTextSize(label.data()).y + SCALE(15)), true);

    ItemSize(rect);
    if (!ItemAdd(rect, id, &rect)) return false;

    const bool hovered = ItemHoverable(slider, id, g.LastItemData.InFlags);

    if (hovered) SetHoveredID(id);

    int decimal_precision = parse_format_precision(display_format, 3);

    if (hovered && g.IO.MouseClicked[0])
    {
        SetActiveID(id, window);
        FocusWindow(window);
    }

    range_slider_state* state = gui->anim_container(&state, range_id);
    state->text = ImLerp(state->text, IsItemActive() ? clr->text.text_active : hovered ? clr->text.text_hovered : clr->text.text_inactive, gui->fixed_speed(8.f));
    state->hint = ImLerp(state->hint, hint_target ? clr->other.accent : clr->text.text_inactive, gui->fixed_speed(8.f));

    const bool value_changed = range_slider_behavior({ slider.Min, slider.Max }, id, v1, v2, v_min, v_max, range, 1, decimal_precision, NULL);

    char value_buf[64];
    const char* value_buf_end = value_buf + ImFormatString(value_buf, IM_ARRAYSIZE(value_buf), display_format, *v1, display_format);

    char value_buf2[64];
    const char* value_buf_end2 = value_buf2 + ImFormatString(value_buf2, IM_ARRAYSIZE(value_buf2), display_format, *v2, display_format);

    upd_format_value(value_buf, sizeof(value_buf), display_format, v1, state->slow_value);
    upd_format_value(value_buf2, sizeof(value_buf2), display_format, v2, state->slow_value2);

    if (hinting[0]) {
        draw->text_clipped(window->DrawList, var->font.inter_medium[0], rect.Min + ImVec2(CalcTextSize(label.data()).x + SCALE(10), padding), rect.Max - SCALE(0, 2), draw->get_clr(state->hint), "[ ? ]", NULL, NULL, { 0.0, 0.0 }, NULL);
        widget->set_tooltip(hinting.data(), hint_target);
    }

    draw->text_clipped(window->DrawList, var->font.inter_medium[0], rect.Min + ImVec2(0, padding), rect.Max, draw->get_clr(state->text), label.data(), NULL, NULL, ImVec2(0.f, 0.0f));
    draw->text_clipped(window->DrawList, var->font.inter_medium[0], rect.Min + ImVec2(0, padding), rect.Max, draw->get_clr(clr->text.text_active), (std::stringstream{} << value_buf << ", " << value_buf2).str().c_str(), NULL, NULL, ImVec2(1.0f, 0.0f));

    draw->line(window->DrawList, rect.Min + ImVec2(0, rect_size), rect.Max, draw->get_clr(clr->other.element_separator), 1.f);

    if (var->selection.selection_alpha == 0)
    {
        state->slow_value = 0;
        state->slow_value2 = 0;
    }

    return value_changed;

}

bool c_widget::range_slider_float(std::string_view label, float* v1, float* v2, float v_min, float v_max, float range, const char* display_format, std::string_view hinting)
{
    return range_slider<float>(label, v1, v2, v_min, v_max, range, display_format, hinting);
}

bool c_widget::range_slider_int(std::string_view label, int* v1, int* v2, int v_min, int v_max, int range, const char* display_format, std::string_view hinting)
{
    return range_slider<int>(label, v1, v2, v_min, v_max, range, display_format, hinting);
}