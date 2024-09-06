#include <std_include.hpp>
#include "toybox_utils.hpp"

namespace toybox {
	void create_menu(std::string name, std::string parent) {
		menu_structs::menu new_menu;
		new_menu.name = name;
		new_menu.parent = parent;
		vars::menus.insert_or_assign(name, new_menu);
	}

	void add_option(std::string menu_name, std::string text, std::function<void()> callback) {
		if (vars::menus.contains(menu_name)) {
			menu_structs::menu_option new_option;
			new_option.text = text;
			new_option.callback = callback;
			new_option.type = "normal";
			vars::menus.at(menu_name).options.push_back(new_option);
		}
	}

	void add_submenu_option(std::string menu_name, std::string text, std::function<void()> callback) {
		//i know i could just do new_option.callback = [submenu_name]() { load_menu(submenu_name); } but i want to be able to do other stuff in the function
		if (vars::menus.contains(menu_name)) {
			menu_structs::menu_option new_option;
			new_option.text = text;
			new_option.callback = callback;
			new_option.type = "submenu";
			vars::menus.at(menu_name).options.push_back(new_option);
		}
	}

	void add_bool_option(std::string menu_name, std::string text, bool& bool_value, std::function<void()> callback) {
		if (vars::menus.contains(menu_name)) {
			menu_structs::menu_option new_option;
			new_option.text = text;
			new_option.callback = callback;
			new_option.variable.bool_value = bool_value;
			new_option.type = "bool";
			vars::menus.at(menu_name).options.push_back(new_option);
		}
	}

	void add_int_slider(std::string menu_name, std::string text, int* int_value, std::vector<int> min_max_step, std::function<void()> callback) {
		if (vars::menus.contains(menu_name)) {
			menu_structs::menu_option new_option;
			new_option.text = text;
			new_option.callback = callback;
			new_option.slider_variable.int_value = int_value;
			new_option.slider_variable.int_min = min_max_step[0];
			new_option.slider_variable.int_max = min_max_step[1];
			new_option.slider_variable.int_step = min_max_step[2];
			new_option.type = "int_slider";
			vars::menus.at(menu_name).options.push_back(new_option);
		}
	}

	void add_float_slider(std::string menu_name, std::string text, float* float_value, std::vector<float> min_max_step, std::function<void()> callback) {
		if (vars::menus.contains(menu_name)) {
			menu_structs::menu_option new_option;
			new_option.text = text;
			new_option.callback = callback;
			new_option.slider_variable.float_value = float_value;
			new_option.slider_variable.float_min = min_max_step[0];
			new_option.slider_variable.float_max = min_max_step[1];
			new_option.slider_variable.float_step = min_max_step[2];
			new_option.type = "float_slider";
			vars::menus.at(menu_name).options.push_back(new_option);
		}
	}

	void structure();

	void load_menu(std::string menu_name) {
		if (menu_name == "none") {
			vars::current_menu = menu_name;
			return;
		}

		structure();

		if (!vars::menus.contains(menu_name)) {
			return;
		}

		vars::current_menu = menu_name;

		if (!vars::menu_scrolls.contains(menu_name)) {
			vars::menu_scrolls.insert_or_assign(menu_name, 0);
		}
		else {
			auto& menu = vars::menus.at(menu_name);
			int size = static_cast<int>(menu.options.size());

			if (vars::menu_scrolls.at(menu_name) >= size) {
				vars::menu_scrolls.at(menu_name) = size - 1;
			}
		}
	}

	void structure() {
		vars::menus.clear();

		create_menu("main", "none");
		add_submenu_option("main", "Sub Menu", []() { load_menu("sub"); });
		add_bool_option("main", "Test Bool", vars::test_bool, []() { vars::test_bool = !vars::test_bool; });
		add_int_slider("main", "Test Int Slider", &vars::test_int, { 0, 10, 1 }, []() {});
		add_float_slider("main", "Test Float Slider", &vars::test_float, { 0.f, 10.f, 0.5f }, []() {});
		for (int i = 1; i < 21; i++) {
			add_option("main", "Option " + std::to_string(i), []() {});
		}

		create_menu("sub", "main");
		add_option("sub", "Sub Option 1", []() {});
		add_option("sub", "Sub Option 2", []() {});
		add_option("sub", "Sub Option 3", []() {});
	}

	bool key_pressed(game::keyNum_t key) {
		return game::playerKeys[0].keys[key].down;
	}

	void calculate_alignment(std::vector<std::string>& align, float& x, float& y, float width, float height) {
		const auto* placement = game::ScrPlace_GetActivePlacement();
		if (!placement)
			return;

		float center[2] = { placement->realViewportSize[0] * 0.5f, placement->realViewportSize[1] * 0.5f };

		if (align[0] == "center") {
			x += (center[0] - (width * 0.5f));
		}
		else if (align[0] == "right") {
			x += (center[0] - width);
		}
		else if (align[0] == "left") {
			x += center[0];
		}

		if (align[1] == "center") {
			y += (center[1] - (height * 0.5f));
		}
		else if (align[1] == "bottom") {
			y += (center[1] - height);
		}
		else if (align[1] == "top") {
			y += center[1];
		}
	}

	void draw_text(std::string text, std::string font_name, std::vector<std::string> align, float x, float y, float scale, std::vector<float> color, game::FontStyle style) {
		game::vec4_t game_color = { color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, color[3] / 255.0f };

		auto* font = game::R_RegisterFont(font_name.data(), static_cast<int>(scale));
		
		auto text_width = game::R_TextWidth(text.data(), std::numeric_limits<int>::max(), font);
		auto text_height = font->pixelHeight;
		y += text_height;//this makes shader and text drawing align
		calculate_alignment(align, x, y, static_cast<float>(text_width), static_cast<float>(text_height));

		game::R_AddCmdDrawText(text.data(), std::numeric_limits<int>::max(), font, x, y, 1.f, 1.f, 0.0f, game_color, style);
	}

	void draw_rectangle(float x, float y, float width, float height, std::vector<float> color, std::vector<std::string> align) {
		game::vec4_t game_color = { color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, color[3] / 255.0f };

		calculate_alignment(align, x, y, width, height);
		game::R_AddCmdDrawStretchPic(x, y, width, height, 0.0f, 0.0f, 1.0f, 1.0f, game_color, game::Material_RegisterHandle("white"), 0);
	}
}