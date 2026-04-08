#include "../settings/functions.h"

const char* keys[] =
{
    "None",
    "Mouse 1",
    "Mouse 2",
    "CN",
    "Mouse 3",
    "Mouse 4",
    "Mouse 5",
    "-",
    "Back",
    "Tab",
    "-",
    "-",
    "CLR",
    "Enter",
    "-",
    "-",
    "Shift",
    "CTL",
    "Menu",
    "Pause",
    "Caps",
    "KAN",
    "-",
    "JUN",
    "FIN",
    "KAN",
    "-",
    "Escape",
    "CON",
    "NCO",
    "ACC",
    "MAD",
    "Space",
    "PGU",
    "PGD",
    "End",
    "Home",
    "Left",
    "Up",
    "Right",
    "Down",
    "SEL",
    "PRI",
    "EXE",
    "PRI",
    "INS",
    "Delete",
    "HEL",
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "-",
    "-",
    "-",
    "-",
    "-",
    "-",
    "-",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "WIN",
    "WIN",
    "APP",
    "-",
    "SLE",
    "Num 0",
    "Num 1",
    "Num 2",
    "Num 3",
    "Num 4",
    "Num 5",
    "Num 6",
    "Num 7",
    "Num 8",
    "Num 9",
    "MUL",
    "ADD",
    "SEP",
    "MIN",
    "Delete",
    "DIV",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
    "F13",
    "F14",
    "F15",
    "F16",
    "F17",
    "F18",
    "F19",
    "F20",
    "F21",
    "F22",
    "F23",
    "F24",
    "-",
    "-",
    "-",
    "-",
    "-",
    "-",
    "-",
    "-",
    "NUM",
    "SCR",
    "EQU",
    "MAS",
    "TOY",
    "OYA",
    "OYA",
    "-",
    "-",
    "-",
    "-",
    "-",
    "-",
    "-",
    "-",
    "-",
    "Shift",
    "Shift",
    "Ctrl",
    "Ctrl",
    "Alt",
    "Alt"
};

bool c_widget::keybind(std::string_view label, int* key, const ImVec2 size)
{

    struct c_keybind
    {
        ImVec4 text = clr->text.text_inactive;
        float alpha, slow = 0;
    };

    ImGuiWindow* window = ImGui::GetCurrentWindow();

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    const ImGuiID id = window->GetID(label.data());
    const ImVec2 pos = window->DC.CursorPos;

    const ImRect total_bb(pos, pos + SCALE(size));

    ItemSize(total_bb, 0.f);
    if (!ItemAdd(total_bb, id, &total_bb)) return false;

    char buf_display[64] = "None";

    bool value_changed = false;
    int k = *key;

    std::string active_key = "";
    active_key += keys[*key];

    if (*key != 0 && g.ActiveId != id) {
        strcpy_s(buf_display, active_key.c_str());
    }
    else if (g.ActiveId == id) {
        strcpy_s(buf_display, "...");
    }

    bool hovered = ItemHoverable(total_bb, id, 0);

    if (hovered && GetIO().MouseClicked[0])
    {
        if (g.ActiveId != id) {
            memset(GetIO().MouseDown, 0, sizeof(GetIO().MouseDown));
            memset(GetIO().KeysDown, 0, sizeof(GetIO().KeysDown));
            *key = 0;
        }
        ImGui::SetActiveID(id, window);
        ImGui::FocusWindow(window);
    }
    else if (GetIO().MouseClicked[0]) {

        if (g.ActiveId == id) ImGui::ClearActiveID();
    }

    if (g.ActiveId == id) {
        const int key_escape = 0x1B;

        if (GetIO().KeysDown[key_escape]) {
            *key = 0;
            value_changed = true;
            ImGui::ClearActiveID();
        }
        else {

            for (auto i = 0; i < 5; i++) {
                if (GetIO().MouseDown[i]) {
                    switch (i) {
                    case 0:
                        k = 0x01;
                        break;
                    case 1:
                        k = 0x02;
                        break;
                    case 2:
                        k = 0x04;
                        break;
                    case 3:
                        k = 0x05;
                        break;
                    case 4:
                        k = 0x06;
                        break;
                    }
                    value_changed = true;
                    ImGui::ClearActiveID();
                }
            }

            if (!value_changed) {
                for (auto i = 0x08; i <= 0xA5; i++) {
                    if (i == key_escape) continue;
                    if (GetIO().KeysDown[i]) {
                        k = i;
                        value_changed = true;
                        ImGui::ClearActiveID();
                    }
                }
            }

            if (value_changed) {
                *key = k;
            }
        }
    }

    c_keybind* state = gui->anim_container(&state, id);

    state->alpha = ImLerp(state->alpha, g.ActiveId == id ? 0.2f : 0.f, gui->fixed_speed(12.f));
    state->text = ImLerp(state->text, g.ActiveId == id ? clr->text.text_active : clr->text.text_inactive, gui->fixed_speed(12.f));

    {
        draw->rect_filled(window->DrawList, ImVec2(state->slow + total_bb.Min.x, total_bb.Min.y), total_bb.Max, draw->get_clr(clr->keybind.layout), SCALE(element->keybind.rounding));
        draw->rect_filled(window->DrawList, ImVec2(state->slow + total_bb.Min.x, total_bb.Min.y), total_bb.Max, draw->get_clr(clr->other.accent, state->alpha), SCALE(element->keybind.rounding));

        draw->text_clipped(window->DrawList, var->font.inter_medium[0], total_bb.Min, total_bb.Max, draw->get_clr(clr->text.text_active), buf_display, NULL, NULL, { 0.5, 0.5 }, NULL);
    }

    return value_changed;
}

void c_widget::register_keybind()
{
    for (auto& bind : element->keybind.update_keybind_system)
    {
        if (*bind.mode == 0)
        {
            if ((GetAsyncKeyState(*bind.key) & 0x0001) != 0)
            {
                *bind.callback = !*bind.callback;

                if (element->keybind.open_popup) *bind.callback = !*bind.callback;
            }
        }
        else if (*bind.mode == 1)
        {
            *bind.callback = (GetAsyncKeyState(*bind.key) & 0x8000) != 0;
        }
    }
}