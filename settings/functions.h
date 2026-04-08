#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui.h"
#include "imgui_internal.h"
#include "colors.h"
#include "variables.h"
#include "elements.h"
#include <vector>
#include <sstream>
#include "iostream"
#include <fstream>
#include <string>
#include <D3DX11.h>
#include <functional>
#include <memory>

using namespace ImGui;

#define SCALE(...) scale_impl(__VA_ARGS__, var->dpi.dpi)

inline ImVec2 scale_impl(const ImVec2& vec, float dpi) {
    return ImVec2(roundf(vec.x * dpi), roundf(vec.y * dpi));
}

inline ImVec2 scale_impl(float x, float y, float dpi) {
    return ImVec2(roundf(x * dpi), roundf(y * dpi));
}

inline float scale_impl(float var, float dpi) {
    return roundf(var * dpi);
}

struct rich_text
{
    ImU32 color;
    const char* text;
};

inline D3DX11_IMAGE_LOAD_INFO info;
inline ID3DX11ThreadPump* pump{ nullptr };

class c_gui
{
public:

    template <typename T>
    T* anim_container(T** state_ptr, ImGuiID id)
    {
        T* state = static_cast<T*>(GetStateStorage()->GetVoidPtr(id));
        if (!state)
            GetStateStorage()->SetVoidPtr(id, state = new T());

        *state_ptr = state;
        return state;
    }

    float fixed_speed(float speed) { return speed / ImGui::GetIO().Framerate; };

    bool begin(std::string_view name = var->window.window_name, bool* p_open = nullptr, ImGuiWindowFlags flags = var->window.window_flags, ImU32 shadow_col = ImColor(0, 0, 0, 0), float shadow_size = 0.f);

    void end();

    void push_style_color(ImGuiCol idx, ImU32 col);

    void pop_style_color(int count = 1);

    void push_style_var(ImGuiStyleVar idx, float val);

    void push_style_var(ImGuiStyleVar idx, const ImVec2& val);

    void pop_style_var(int count = 1);

    void push_font(ImFont* font);

    void pop_font();

    void set_cursor_pos(const ImVec2& local_pos);

    void set_cursor_pos_x(float x);

    void set_cursor_pos_y(float y);

    ImVec2 get_cursor_pos();

    float get_cursor_pos_x();

    float get_cursor_pos_y();

    void spacing();

    void sameline(float offset_from_start_x = 0.0f, float spacing_w = -1.0f);

    ImVec2 content_region_avail();

    ImVec2 content_region_max();

    ImVec2 get_window_size();

    ImVec2 get_window_pos();

    ImVec2 cursor_screen_pos();

    void begin_group();

    void end_group();

    bool begin_def_child(std::string_view name, const ImVec2& size_arg = ImVec2(0, 0), ImGuiChildFlags child_flags = 0, ImGuiWindowFlags window_flags = 0);

    void end_def_child();

    bool begin_child(std::string_view name, const ImVec2& size_arg = ImVec2(0, 0), ImGuiChildFlags child_flags = 0, ImGuiWindowFlags window_flags = 0);

    void end_child();

    void begin_content(std::string name, const ImVec2& size_arg, const ImVec2& spacing = { 0, 0 }, const ImVec2& padding = { 0, 0 }, bool scroll_bar = true);

    void end_content();

    void set_next_window_pos(const ImVec2& pos, ImGuiCond cond = 0, const ImVec2& pivot = ImVec2(0, 0));

    void set_next_window_size(const ImVec2& size, ImGuiCond cond = 0);

    void water_mark(std::string name, std::vector<std::string> function, watermark_position type, bool* visible);

    void new_frame();

    void end_frame();

    void render();

};

inline std::unique_ptr<c_gui> gui = std::make_unique<c_gui>();

class c_widget
{
public:


    bool checkbox(std::string_view label, bool* callback, std::string_view hinting = "");

    bool checkbox_with_keybind(std::string_view label, bool* callback, int* key, int* mode, std::string_view hinting = "");

    bool checkbox_with_picker(std::string_view label, bool* callback, float col[4], bool alpha, std::string_view hinting = "");

    bool slider_float(std::string_view label, float* v, float v_min, float v_max, const char* format = "%d", std::string_view hinting = "");

    bool slider_int(std::string_view label, int* v, int v_min, int v_max, const char* format = "%d", std::string_view hinting = "");

    bool range_slider_float(std::string_view label, float* v1, float* v2, float v_min, float v_max, float range, const char* display_format = "%.1f", std::string_view hinting = "");

    bool range_slider_int(std::string_view label, int* v1, int* v2, int v_min, int v_max, int range, const char* display_format = "%d", std::string_view hinting = "");

    bool dropdown(std::string_view label, int* current_item, std::vector<std::string>& items, std::string_view hinting = "", int max_count = 0);

    void multi_dropdown(std::string_view label, bool variable[], std::vector<std::string>& labels, std::string_view hinting = "", int max_count = 0);

    bool keybind_dropdown(std::string_view label, int* current_item, std::vector<std::string>& items, int max_count);

    bool set_tooltip(std::string_view tooltip_text, bool show_tooltip);

    bool keybind(std::string_view label, int* key, const ImVec2 size);

    void text_colored(ImFont* font, const ImU32 col, std::string text);

    bool color_picker(std::string_view label, float col[4], bool alpha);

    bool text_field(std::string_view hint, std::string_view label, char* buf, size_t buf_size, const ImVec2& size, ImU32 bg_color, float rounding, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = 0, void* user_data = 0);

    bool def_text_field(std::string_view hint, std::string_view label, char* buf, size_t buf_size, ImU32 bg_color, ImGuiInputTextFlags flags = 0, ImGuiInputTextCallback callback = 0, void* user_data = 0);

    void selection_for(std::vector<std::vector<std::string>> page, int* selection);

    void page_for(ImFont* font, std::vector<std::string> page, int* selection, float current_size);

    bool button(std::string_view label);

    bool begin_list(std::string_view name, const ImVec2& size_arg);

    void end_list();

    bool list_content(std::string_view label, bool active);

    void register_keybind();

    bool begin_popup(float size);

    void end_popup();

};

inline std::unique_ptr<c_widget> widget = std::make_unique<c_widget>();

enum fade_direction : int
{
    vertically,
    horizontally,
    diagonally,
    diagonally_reversed,
};

class c_draw
{
public:
    ImU32 get_clr(const ImVec4& col, float alpha = 1.f);

    void text(ImDrawList* draw_list, const ImFont* font, float font_size, const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end = NULL, float wrap_width = NULL, const ImVec4* cpu_fine_clip_rect = 0);

    void text_clipped(ImDrawList* draw_list, ImFont* font, const ImVec2& pos_min, const ImVec2& pos_max, ImU32 color, const char* text, const char* text_display_end = NULL, const ImVec2* text_size_if_known = NULL, const ImVec2& align = ImVec2(0.f, 0.f), const ImRect* clip_rect = NULL);

    void colored_text(ImFont* font, ImVec2 pos1, ImVec2 pos2, const std::vector<rich_text>& texts, ImVec2 align = ImVec2(0.5, 0.5));

    void radial_gradient(ImDrawList* draw_list, const ImVec2& center, float radius, ImU32 col_in, ImU32 col_out);

    void set_linear_color_alpha(ImDrawList* draw_list, int vert_start_idx, int vert_end_idx, ImVec2 gradient_p0, ImVec2 gradient_p1, ImU32 col0, ImU32 col1);

    void line(ImDrawList* draw_list, const ImVec2& p1, const ImVec2& p2, ImU32 col, float thickness = 1.0f);

    void rect(ImDrawList* draw_list, const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding = 0.0f, ImDrawFlags flags = 0, float thickness = 1.0f);

    void rect_filled(ImDrawList* draw_list, const ImVec2& p_min, const ImVec2& p_max, ImU32 col, float rounding = 0.0f, ImDrawFlags flags = 0);

    void rect_filled_multi_color(ImDrawList* draw, const ImVec2& p_min, const ImVec2& p_max, ImU32 col_upr_left, ImU32 col_upr_right, ImU32 col_bot_right, ImU32 col_bot_left, float rounding = 0.f, ImDrawFlags flags = 0);

    void fade_rect_filled(ImDrawList* draw, const ImVec2& pos_min, const ImVec2& pos_max, ImU32 col_one, ImU32 col_two, fade_direction direction, float rounding = 0.f, ImDrawFlags flags = 0);

    void shadow_rect(ImDrawList* draw_list, const ImVec2& obj_min, const ImVec2& obj_max, ImU32 shadow_col, float shadow_thickness, const ImVec2& shadow_offset, ImDrawFlags flags = 0, float obj_rounding = 0.0f);

    void circle(ImDrawList* draw_list, const ImVec2& center, float radius, ImU32 col, int num_segments = 0, float thickness = 1.0f);

    void circle_filled(ImDrawList* draw_list, const ImVec2& center, float radius, ImU32 col, int num_segments = 0);

    void shadow_circle(ImDrawList* draw_list, const ImVec2& obj_center, float obj_radius, ImU32 shadow_col, float shadow_thickness, const ImVec2& shadow_offset, ImDrawFlags flags = 0, int obj_num_segments = 12);

    void triangle(ImDrawList* draw_list, const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, ImU32 col, float thickness = 1.f);

    void triangle_filled(ImDrawList* draw_list, const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, ImU32 col);

    void image(ImDrawList* draw_list, ImTextureID user_texture_id, const ImVec2& p_min, const ImVec2& p_max, const ImVec2& uv_min = ImVec2(0, 0), const ImVec2& uv_max = ImVec2(1, 1), ImU32 col = IM_COL32_WHITE);

    void image_rounded(ImDrawList* draw_list, ImTextureID user_texture_id, const ImVec2& p_min, const ImVec2& p_max, const ImVec2& uv_min = ImVec2(0, 0), const ImVec2& uv_max = ImVec2(1, 1), ImU32 col = IM_COL32_WHITE, float rounding = 1.f, ImDrawFlags flags = 0);

    void shadow_convex_poly(ImDrawList* draw_list, const ImVec2* points, int points_count, ImU32 shadow_col, float shadow_thickness, const ImVec2& shadow_offset, ImDrawFlags flags = 0);

    void shadow_ngon(ImDrawList* draw_list, const ImVec2& obj_center, float obj_radius, ImU32 shadow_col, float shadow_thickness, const ImVec2& shadow_offset, ImDrawFlags flags, int obj_num_segments);

    void rotate_start(ImDrawList* draw_list);

    void rotate_end(float rad, ImDrawList* draw_list, ImVec2 center = ImVec2(0, 0));

    float deg_to_rad(float deg);

    void push_clip_rect(ImDrawList* draw_list, const ImVec2& clip_rect_min, const ImVec2& clip_rect_max, bool intersect_with_current_clip_rect = false);

    void pop_clip_rect(ImDrawList* draw_list);

    void render_check_mark(ImDrawList* draw_list, ImVec2 pos, ImU32 col, float sz, float stroke_size = 1.f);

};

inline std::unique_ptr<c_draw> draw = std::make_unique<c_draw>();

enum notify_status
{
    success = 0,
    warning = 1,
    error = 2,
};

struct notify_state
{
    int notify_id;
    std::string_view text;
    notify_status status_notify;

    bool active_notify = true;
    float notify_alpha = 0.f, notify_timer = 0.f, notify_delay = 4000, notify_pos = 0.f;
};

class c_notify
{
public:

    void setup_notify();
    void add_notify(std::string_view text, notify_status status);

private:
    void render_notify(int cur_notify_value, float notify_alpha, float notify_percentage, float notify_pos, std::string_view text, notify_status status);
    std::vector<notify_state> notifications;

    float notify_time = 15.f;
    int notify_count = 0;
};

inline std::unique_ptr<c_notify> notify = std::make_unique<c_notify>();

class particle_system {
public:
    particle_system(int particle_count) {
        srand(static_cast<unsigned int>(time(0)));
        for (int i = 0; i < particle_count; ++i) {
            particles.push_back(create_random_particle());
        }
    }

    void update_and_render() {

        width = GetIO().DisplaySize.x;
        height = GetIO().DisplaySize.y;

        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        for (auto& particle : particles) {
            move_particle(particle);

            for (auto& other : particles) {
                if (&particle != &other && distance(particle.pos, other.pos) < 170.0f) {
                    float dist = distance(particle.pos, other.pos);
                    float alpha = max(0.0f, var->gui.particle_color[3] - dist / 170.0f);

                    draw->line(draw_list, particle.pos, other.pos, draw->get_clr(ImVec4(var->gui.particle_color[0], var->gui.particle_color[1], var->gui.particle_color[2], alpha)));
                }
            }
        }
    }

private:
    struct particle_state {
        ImVec2 pos;
        ImVec2 vel;
        ImU32 color;
        float size;
    };

    particle_state create_random_particle() {
        particle_state particle;
        particle.pos = ImVec2(rand() % width, rand() % height);

        particle.vel = ImVec2((rand() % 100 / 100.0f - 0.5f) * 1.5f, (rand() % 100 / 100.0f - 0.5f) * 7.f);
        particle.color = IM_COL32(rand() % 256, rand() % 256, rand() % 256, 255);
        particle.size = rand() % 4 + 2;
        return particle;
    }

    void move_particle(particle_state& particle) {
        particle.pos.x += particle.vel.x;
        particle.pos.y += particle.vel.y;

        if (particle.pos.x < 0 || particle.pos.x > width) particle.vel.x *= -1;
        if (particle.pos.y < 0 || particle.pos.y > height) particle.vel.y *= -1;
    }

    float distance(const ImVec2& a, const ImVec2& b) const {
        return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    }

    int width = 500, height = 500;
    std::vector<particle_state> particles;
};

inline void theme_color()
{
    clr->window.layout = ImLerp(ImVec4(clr->window.layout), var->list_page.selection_theme == 0 ? ImColor(17, 17, 20).Value : ImColor(255, 255, 255, 255).Value, gui->fixed_speed(12.f));
    clr->window.header = ImLerp(ImVec4(clr->window.header), var->list_page.selection_theme == 0 ? ImColor(22, 22, 27).Value : ImColor(246, 248, 252, 255).Value, gui->fixed_speed(12.f));
    clr->window.stroke = ImLerp(ImVec4(clr->window.stroke), var->list_page.selection_theme == 0 ? ImColor(32, 32, 44).Value : ImColor(233, 236, 246, 255).Value, gui->fixed_speed(12.f));

    clr->child.layout = ImLerp(ImVec4(clr->child.layout), var->list_page.selection_theme == 0 ? ImColor(21, 21, 26).Value : ImColor(246, 248, 252, 255).Value, gui->fixed_speed(12.f));

    clr->other.white = ImLerp(ImVec4(clr->other.white), var->list_page.selection_theme == 0 ? ImColor(255, 255, 255).Value : ImColor(0, 0, 0, 255).Value, gui->fixed_speed(12.f));
    clr->other.element_separator = ImLerp(ImVec4(clr->other.element_separator), var->list_page.selection_theme == 0 ? ImColor(28, 28, 37).Value : ImColor(233, 236, 246, 255).Value, gui->fixed_speed(12.f));
    clr->other.particle = ImLerp(ImVec4(clr->other.particle), var->list_page.selection_theme == 0 ? ImColor(26, 26, 34).Value : ImColor(230, 230, 235, 255).Value, gui->fixed_speed(12.f));		clr->other.particle = ImLerp(ImVec4(clr->other.particle), var->list_page.selection_theme == 0 ? ImColor(26, 26, 34).Value : ImColor(230, 230, 235, 255).Value, gui->fixed_speed(12.f));
    clr->other.open_popup = ImLerp(ImVec4(clr->other.open_popup), var->list_page.selection_theme == 0 ? ImColor(24, 24, 31).Value : ImColor(234, 236, 242, 255).Value, gui->fixed_speed(12.f));

    clr->checkbox.layout = ImLerp(ImVec4(clr->checkbox.layout), var->list_page.selection_theme == 0 ? ImColor(27, 27, 34).Value : ImColor(231, 236, 248, 255).Value, gui->fixed_speed(12.f));
    clr->checkbox.circle = ImLerp(ImVec4(clr->checkbox.circle), var->list_page.selection_theme == 0 ? ImColor(63, 63, 80).Value : ImColor(189, 195, 209, 255).Value, gui->fixed_speed(12.f));

    clr->slider.layout = ImLerp(ImVec4(clr->slider.layout), var->list_page.selection_theme == 0 ? ImColor(27, 27, 34).Value : ImColor(231, 236, 248, 255).Value, gui->fixed_speed(12.f));

    clr->dropdown.layout = ImLerp(ImVec4(clr->dropdown.layout), var->list_page.selection_theme == 0 ? ImColor(27, 27, 34).Value : ImColor(231, 236, 248, 255).Value, gui->fixed_speed(12.f));
    clr->dropdown.selection = ImLerp(ImVec4(clr->dropdown.selection), var->list_page.selection_theme == 0 ? ImColor(34, 34, 44).Value : ImColor(217, 222, 234, 255).Value, gui->fixed_speed(12.f));

    clr->button.layout = ImLerp(ImVec4(clr->button.layout), var->list_page.selection_theme == 0 ? ImColor(27, 27, 34).Value : ImColor(231, 236, 248, 255).Value, gui->fixed_speed(12.f));
    clr->button.layout_hovered = ImLerp(ImVec4(clr->button.layout_hovered), var->list_page.selection_theme == 0 ? ImColor(34, 34, 44).Value : ImColor(217, 222, 234, 255).Value, gui->fixed_speed(12.f));

    clr->keybind.layout = ImLerp(ImVec4(clr->keybind.layout), var->list_page.selection_theme == 0 ? ImColor(31, 31, 40).Value : ImColor(217, 222, 234, 255).Value, gui->fixed_speed(12.f));
    clr->dropdown.another_dropdown = ImLerp(ImVec4(clr->dropdown.another_dropdown), var->list_page.selection_theme == 0 ? ImColor(31, 31, 40).Value : ImColor(217, 222, 234, 255).Value, gui->fixed_speed(12.f));
    clr->color_picker.button_layout = ImLerp(ImVec4(clr->color_picker.button_layout), var->list_page.selection_theme == 0 ? ImColor(31, 31, 40).Value : ImColor(217, 222, 234, 255).Value, gui->fixed_speed(12.f));

    clr->selection.layout_selection = ImLerp(ImVec4(clr->selection.layout_selection), var->list_page.selection_theme == 0 ? ImColor(28, 28, 36).Value : ImColor(234, 236, 242, 255).Value, gui->fixed_speed(12.f));
    clr->selection.layout = ImLerp(ImVec4(clr->selection.layout), var->list_page.selection_theme == 0 ? ImColor(22, 22, 27).Value : ImColor(246, 248, 252, 255).Value, gui->fixed_speed(12.f));

    clr->text.text_active = ImLerp(ImVec4(clr->text.text_active), var->list_page.selection_theme == 0 ? ImColor(255, 255, 255).Value : ImColor(0, 0, 0, 255).Value, gui->fixed_speed(12.f));
    clr->text.text_hovered = ImLerp(ImVec4(clr->text.text_hovered), var->list_page.selection_theme == 0 ? ImColor(105, 105, 135).Value : ImColor(153, 158, 168, 255).Value, gui->fixed_speed(12.f));
    clr->text.text_inactive = ImLerp(ImVec4(clr->text.text_inactive), var->list_page.selection_theme == 0 ? ImColor(63, 63, 80).Value : ImColor(189, 195, 209, 255).Value, gui->fixed_speed(12.f));

    clr->watermark.layout = ImLerp(ImVec4(clr->watermark.layout), var->list_page.selection_theme == 0 ? ImColor(17, 17, 20).Value : ImColor(255, 255, 255, 255).Value, gui->fixed_speed(12.f));
    clr->watermark.border = ImLerp(ImVec4(clr->watermark.border), var->list_page.selection_theme == 0 ? ImColor(17, 17, 20, 80).Value : ImColor(255, 255, 255, 80).Value, gui->fixed_speed(12.f));

    clr->notify.layout = ImLerp(ImVec4(clr->notify.layout), var->list_page.selection_theme == 0 ? ImColor(17, 17, 20, 255).Value : ImColor(255, 255, 255).Value, gui->fixed_speed(12.f));
    clr->notify.line = ImLerp(ImVec4(clr->notify.line), var->list_page.selection_theme == 0 ? ImColor(27, 27, 34).Value : ImColor(234, 236, 242).Value, gui->fixed_speed(12.f));
}