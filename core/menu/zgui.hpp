#pragma once

#include <cstdint>
#include <stack>
#include <vector>
#include <string>
#include <string_view>
#include <type_traits>
#include <windows.h>
#include <algorithm>
#include <iomanip>
#include <sstream>

#include "variables.hpp"

#define ZGUI_API __declspec(dllexport)

// zgui by zxvnme (https://github.com/zxvnme) and all the community contributors
#define ZGUI_VER "1.4.9" // the number after second dot is snapshot version.
/* =============================[general]===============================
 *
 * zgui is an simple framework created to help people with GUI rendering during their game hacking (but not only) journey.
 * here is glance zgui feature presentation:
 *	- easy to use immediate mode rendering (all you need is to include zgui header and source files to your project).
 *	- simple and aesthetic design.
 *	- extensible code.
 *	... and function documentation in section below:
 *
 *    ================================[pseudo documentation]=====================================
 *
 *    get_functions()
 *      -- function that is used to get our wrapped ones.
 *
 *    begin_window(std::string_view title, vec2 default_size, unsigned long font, int flags);
 *    end_window();
 *      -- functions used to create and end window.
 *
 *    begin_groupbox(std::string_view name, vec2 size);
 *    end_groupbox();
 *      -- functions uses to create our groupbox with desired size and end it.
 *
 *    slider_int(std::string_view id, int min, int max, int* value);
 *    slider_float(std::string_view id, float min, float max, float* value);
 *      -- functions used to create sliders with value type described in function name.
 *
 *    combobox(std::string_view id, std::vector<std::string> items, int* value);
 *    multi_combobox(std::string id, std::vector<multi_select_item> items)
 *      -- functions used for creating combo boxes.
 *
 *    checkbox(std::string_view id, bool* value);
 *      -- function that creates checkbox.
 *
 *    toggle_button(std::string_view id, vec2 size, bool* value);
 *      -- function that creates toggle button.
 *
 *    button(std::string_view id, vec2 size);
 *      -- function that creates button.
 *
 *    key_bind(std::string_view id, int* value);
 *      -- function that creates key binder.
 *
 *    text_input(std::string_view id, std::string* value, int max_length = 18);
 *      -- functions that creates text input box.
 *
 *    clickable_text(std::string_view text);
 *      -- function that creates text that can be clicked and eventually perform an action.
 *
 *    text(std::string_view text);
 *      -- function that creates text.
 *
 *    dummy();
 *      -- function that pushes cursor_pos.x to make empty space between our controls.
 *
 *    same_line(float x_axis = -1);
 *    backup_line();
 *      -- functions used for inline controls positioning. NOTE: there can be only one item between these two functions called.
 *
 *    ================================[hashing controls names]================================
 *
 *    the '#' thing in control name is separator that splits our name to two elements; actual displayed name & the one that is "hidden"
 *
 *    bad example:
 *      zgui::button("button", { 120, 30 });
 *      zgui::button("button", { 120, 30 });
 *
 *    code above won't work correctly because of same name provided. (known consequence is clicking 2 buttons at once)
 *
 *    good example:
 *      zgui::button("button#button_1", { 120, 30 });
 *      zgui::button("button#button_2", { 120, 30 });
 *
 *    and now, code above works fine because unique id (used in window input blocking) is provided after '#'
 *
 *    ==================================[input handling]======================================
 *
 *    IMPORTANT NOTE: poll_input(); HAS to be called before everything. Otherwise zgui will throw an exception or won't work properly.
 *
 *    poll_input("type_your_window_name") is function used to start reading input from window we specify in function parameter (string_view)
 *
 *    bad example:
 *      zgui::poll_input("");
 *      ... not calling this before whole zgui :)
 *
 *    code above won't work correctly because window name string_view size is equal to 0.
 *
 *    good example:
 *      zgui::poll_input("zgui directx9 example");
 *      zgui::poll_input("Minecraft 1.8.9");
 *
 *    and now, code above will work fine if your window titles are "zgui directx9 example" or "Minecraft 1.8.9"
 *
 *    ================================================================================================
 *
 * ================================================================================================
*/

namespace zgui {
    // Multi selectable item.
    struct multi_select_item {
        std::string_view name;
        bool* value;
    };
    // Two dimensional vec3_t.
    struct vec2 {
        float x, y;
    };
    // Color with 4 paremeters; red, green, blue and alpha.
    struct color {
        int r, g, b, a;
    };

    /// "Proxy" functions definitions.
    using line_t = std::add_pointer_t<void(int x, int y, int x2, int y2, color color) noexcept>;
    using rect_t = std::add_pointer_t<void(int x, int y, int x2, int y2, color color) noexcept>;
    using filled_rect_t = std::add_pointer_t<void(int x, int y, int x2, int y2, color color) noexcept>;
    using text_t = std::add_pointer_t<void(int x, int y, color color, int font, bool center, const char* text) noexcept>;
    using get_text_size_t = std::add_pointer_t<void(unsigned long font, const char* text, int& width, int& height) noexcept>;
    using get_frametime = std::add_pointer_t<float() noexcept>;
    using get_curtime = std::add_pointer_t<float() noexcept>;
    ///

    // "Proxy" functions stuff...
    struct functions_t {
        line_t draw_line;
        rect_t draw_rect;
        filled_rect_t draw_filled_rect;
        text_t draw_text;
        get_text_size_t get_text_size;
        get_frametime get_frametime;
        get_curtime get_curtime;
    };
    extern functions_t functions;

    // Flags for window appereance and its behavior.
    // ex: (zgui_window_flags_no_border | zgui_window_flags_no_titlebar) will cause window to be borderless and without title bar.
    enum zgui_window_flags {
        zgui_window_flags_none = 0,
        zgui_window_flags_no_ontoggle_animation = 1 << 0,
        zgui_window_flags_no_move = 1 << 1,
        zgui_window_flags_always_open = 1 << 2,
    };

    // Flags for text input appereance.
    // ex: (zgui_text_input_flags_password) will convert text input (ex: "abcdef") to "******".
    enum zgui_text_input_flags {
        zgui_text_input_flags_none = 0,
        zgui_text_input_flags_password = 1 << 0
    };

    enum class zgui_render_type {
        zgui_line = 1,
        zgui_rect,
        zgui_filled_rect,
        zgui_text
    };

    struct zgui_control_render_t {
        vec2 draw_position;
        zgui_render_type render_type;
        color color;
        std::string text;
        vec2 size;
        unsigned long font = 0;
    };

    struct gui_window_context_t {
        uint32_t blocking;
        std::stack<vec2> cursor_pos;
        std::stack<unsigned long> fonts;
        std::vector<zgui_control_render_t> render;
        vec2 position = { 0, 0 }, size;
        vec2 next_cursor_pos;
        bool dragging;
        bool opened;
        int alpha;
    };

    struct key_code_info {
        int vk;

        char regular;
        char shift;
    };

    namespace globals {

        constexpr std::string_view keys_list[]{
                "unbinded", "left mouse", "right mouse", "break", "middle mouse", "m4", "m5",
                "unbinded", "backspace", "tab", "unbinded", "unbinded", "unbinded", "enter", "unbinded", "unbinded", "shift",
                "ctrl", "alt", "pause", "caps", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded",
                "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "space", "page up", "page down", "end", "home", "left arrow",
                "up", "right", "down", "unbinded", "print", "unbinded", "print screen", "insert", "delete", "unbinded", "0", "1",
                "2", "3", "4", "5", "6", "7", "8", "9", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded",
                "unbinded", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u",
                "v", "w", "x", "y", "z", "left windows", "right windows", "unbinded", "unbinded", "unbinded", "num 0", "num 1",
                "num 2", "num 3", "num 4", "num 5", "num 6", "num 7", "num 8", "num 9", "*", "+", "_", "-", ".", "/", "f1", "f2", "f3",
                "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15", "f16", "f17", "f18", "f19", "f20", "f21",
                "f22", "f23", "f24", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded",
                "num lock", "scroll lock", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded",
                "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "left shift", "right shift", "left control",
                "right control", "left menu", "right menu", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded",
                "unbinded", "unbinded", "unbinded", "next track", "previous track", "stop", "play/pause", "unbinded", "unbinded",
                "unbinded", "unbinded", "unbinded", "unbinded", ";", "+", ",", "-", ".", "/?", "~", "unbinded", "unbinded",
                "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded",
                "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded",
                "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "[{", "\\|", "}]", "'\"", "unbinded",
                "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded",
                "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded",
                "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded", "unbinded",
                "unbinded", "unbinded"
        };

        static key_code_info special_characters[22] = {
                {48,  '0',  ')'},
                {49,  '1',  '!'},
                {50,  '2',  '@'},
                {51,  '3',  '#'},
                {52,  '4',  '$'},
                {53,  '5',  '%'},
                {54,  '6',  '^'},
                {55,  '7',  '&'},
                {56,  '8',  '*'},
                {57,  '9',  '('},
                {32,  ' ',  ' '},
                {192, '`',  '~'},
                {189, '-',  '_'},
                {187, '=',  '+'},
                {219, '[',  '{'},
                {220, '\\', '|'},
                {221, ']',  '}'},
                {186, ';',  ':'},
                {222, '\'', '"'},
                {188, ',',  '<'},
                {190, '.',  '>'},
                {191, '/',  '?'}
        };
        
        // Color definition. Can be changed at any time just simply by editing this struct.
        static struct {
            color window_border_fill{ 20, 20, 20, 25 }; // 20, 20, 20, 25
            color window_border_color{ 0, 0, 0, 0 }; // 20, 20, 20, 25
            color window_background{ 20, 20, 20, 255 }; // 20, 20, 20, 255
        
           
            color color_text{255, 255,255,255  }; // 203, 203, 203, 255 // 255, 255, 255, 255
            color color_text_dimmer{ 99, 110, 114, 255 };

            color control_outline{ 4,4,4,255 }; //  4,4,4,255
            color control_active_or_clicked{ 36, 61, 36, 255 }; // 36, 61, 36, 255
            color control_idle{ 28, 28, 28, 255 };

            color color_groupbox_bg{ 24, 24, 24, 255 }; // 24, 24, 24, 255
            color color_groupbox_outline{ 4,4,4, 255 }; // 4,4,4,255
        } global_colors;

        static struct {
            // Base position of first drawn control (px). DO NOT change if its necessary
            zgui::vec2 base_pos{ 10, 17 };
            // Spacing between items (px)
            int item_spacing = 4;
            // Key that will toggle menu visibility unless zgui_window_flags_always_open is set
            int menu_toggle_key = VK_INSERT;
            int menu_toggle_key2 = VK_F11;
        } global_config;

        // Window definitions.
        extern gui_window_context_t window_ctx;

        // "Proxy" functions stuff...
        //inline zgui::functions_t zgui::functions;

        // Globals
        static vec2 mouse_pos;
        static vec2 previous_mouse_pos;

        // Input handling stuff
        static bool key_state[256];
        static bool prev_key_state[256];

        // Check for input polling.
        static bool input_loop_started = false;
    }

    // Start Input loop
    ZGUI_API void poll_input(std::string_view window_name);

    ZGUI_API void poll_input(HWND hwnd);

    namespace utils {
        namespace input {
            bool key_pressed(int key);

            bool key_down(int key);

            bool key_released(int key);

            bool mouse_in_region(int x, int y, int w, int h);

        }

        namespace hash {
            std::vector<std::string> split_str(const char* str, char separator);

            uint32_t hash(const char* str, uint32_t value = 0x811c9dc5);
        }

        namespace misc {
            ZGUI_API void push_cursor_pos(vec2 pos);

            ZGUI_API vec2 pop_cursor_pos();

            ZGUI_API void push_font(unsigned long font);

            ZGUI_API unsigned long pop_font();
        }

    }

    ZGUI_API bool begin_window(std::string_view title, vec2 default_size, unsigned long font, int flags = 0);

    ZGUI_API void end_window();

  

    ZGUI_API void begin_groupbox(std::string_view title, vec2 size);

    ZGUI_API void end_groupbox();

    ZGUI_API void begin_titlebox(std::string_view title, vec2 size);

    ZGUI_API void end_titlebox();

    ZGUI_API void begin_groupbox2(std::string_view title, vec2 size);

    ZGUI_API void end_groupbox2();

    ZGUI_API void checkbox(const char* id, bool& value);

    ZGUI_API void checkboxwk(const char* id, bool& value, const char* id2, int& value2);

    ZGUI_API void toggle_button(const char* id, vec2 size, bool& value);

    ZGUI_API bool button(const char* id, vec2 size);

    ZGUI_API bool tab_button(const char* id, vec2 size, bool toggled = false);

    ZGUI_API void key_bind(const char* id, int& value);

    ZGUI_API void text_input(const char* id, std::string& value, int max_length = 16, int flags = 0);

    ZGUI_API void slider_int(const char* id, int min, int max, int& value);

    ZGUI_API void picker_slider(const char* id, int min, int max, float& value, float* slidercol);

    ZGUI_API void slider_float(const char* id, float min, float max, float& value, int decimales = 1);

    ZGUI_API void combobox(const char*, std::vector<std::string> items, int& value, int size = 100);

    ZGUI_API void colorpicker(const char* id, float* value);

    ZGUI_API void colorpicker_zgui(const char* id, float* value);

    ZGUI_API void multi_combobox(const char* id, std::vector<multi_select_item> items);

    ZGUI_API void listbox(const char* id, std::vector<multi_select_item> items);

    ZGUI_API bool clickable_text(const char* id, bool toggled = false);

    ZGUI_API void text(const char* text);

    ZGUI_API void next_column(int pusher_x = 0, int pusher_y = 0);

    ZGUI_API void fix_spacing(int pusher_y = 0);
}