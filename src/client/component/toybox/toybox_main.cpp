#include <std_include.hpp>
#include "loader/component_loader.hpp"
#include "game/game.hpp"
#include <utils/hook.hpp>
#include "component/scheduler.hpp"
#include "toybox_utils.hpp"
#include "toybox_vars.hpp"

namespace toybox {
	void draw_option(menu_structs::menu_option& option, float x, float y) {
		toybox::draw_text(option.text, "fonts/blender_pro_bold.ttf", { "left", "center" }, x, y, 16.f, { 225, 225, 225, 255 }, game::FONT_STYLE_NONE);

		if (option.type == "bool") {
			std::vector<float> color;
			if (option.bool_value) {
				color = { 35, 35, 35, 255 };
			}
			else {
				color = { 15, 15, 15, 255 };
			}

			toybox::draw_rectangle(x + vars::menu_width - 20, y, 12, 12, { 0, 0, 0, 255 }, { "right", "center" });
			toybox::draw_rectangle(x + vars::menu_width - 21, y, 10, 10, color, { "right", "center" });
		}
		else if (option.type == "submenu") {
			toybox::draw_text(">", "fonts/blender_pro_bold.ttf", { "right", "center" }, x + vars::menu_width - 22, y, 14.f, { 225, 225, 225, 255 }, game::FONT_STYLE_NONE);
		}
	}

	void draw_menu_hud() {
		auto& menu = vars::menus.at(vars::current_menu);
		int options_size = static_cast<int>(menu.options.size());
		int size = options_size > 10 ? 10 : options_size;
		int scroll = vars::menu_scrolls.at(vars::current_menu);

		vars::menu_height = 120 + (size * 32.f);

		toybox::draw_rectangle(vars::menu_x, vars::menu_y - 2, vars::menu_width + 4, vars::menu_height + 4, { 0, 0, 0, 255 }, { "center", "top" });//black background
		toybox::draw_rectangle(vars::menu_x, vars::menu_y, vars::menu_width, vars::menu_height, { 35, 35, 35, 255 }, { "center", "top" });//lighter background
		toybox::draw_rectangle(vars::menu_x, vars::menu_y + 80, vars::menu_width, vars::menu_height - 118, { 15, 15, 15, 255 }, { "center", "top" });//dark options background
		toybox::draw_rectangle(vars::menu_x, vars::menu_y + 78, vars::menu_width, 2, { 0, 0, 0, 255 }, { "center", "top" });//top black line
		toybox::draw_rectangle(vars::menu_x, vars::menu_y + (vars::menu_height - 38), vars::menu_width, 2, { 0, 0, 0, 255 }, { "center", "top" });//bottom black line

		int start = scroll < size ? 0 : (scroll - (size -1));
		int end = scroll < size ? size : (scroll + 1);
		int scroll_index = scroll < size ? scroll : (size -1);

		toybox::draw_rectangle(vars::menu_x, (vars::menu_y + 80) + (scroll_index * 32), vars::menu_width, 34, { 35, 35, 35, 255 }, { "center", "top" });//scroller
		for (auto i = start; i < end; i++) {
			if (i >= options_size)
				break;

			auto& option = menu.options[i];
			draw_option(option, vars::menu_x - ((vars::menu_width / 2) - 10), (vars::menu_y + 97) + ((i - start) * 32));
		}

		//i hate math bruhhhhhhhhhhhhhhhh
	}

	int frames = 0;
	void monitor_buttons() {
		if (frames < 10)
			return;

		if (vars::current_menu == "none") {
			if (toybox::key_pressed(game::K_BUTTON_LTRIG) && toybox::key_pressed(game::K_DPAD_UP)) {
				toybox::load_menu("main");
				frames = 0;
			}

			return;
		}

		auto& menu = vars::menus.at(vars::current_menu);
		auto& scroll = vars::menu_scrolls.at(vars::current_menu);
		int size = static_cast<int>(menu.options.size());

		if (toybox::key_pressed(game::K_DPAD_UP) || toybox::key_pressed(game::K_DPAD_DOWN)) {
			scroll += toybox::key_pressed(game::K_DPAD_UP) ? -1 : 1;

			if (scroll < 0) {
				scroll = size - 1;
			}
			else if (scroll >= size) {
				scroll = 0;
			}

			frames = 0;
		}

		if (toybox::key_pressed(game::K_BUTTON_X)) {
			auto& option = menu.options[scroll];
			option.callback();
			toybox::load_menu(vars::current_menu);//update variable drawing
			frames = 0;
		}

		if (toybox::key_pressed(game::K_BUTTON_RSTICK)) {
			toybox::load_menu(menu.parent);
			frames = 0;
		}
	}

	void main_loop() {
		monitor_buttons();

		if (vars::current_menu != "none") {
			draw_menu_hud();
		}

		frames++;
		if (frames > 1000)
			frames = 10;
	}


	class component final : public component_interface {
	public:
		void post_unpack() override {
			scheduler::loop(main_loop, scheduler::pipeline::renderer);
		}
	};
}

REGISTER_COMPONENT(toybox::component)