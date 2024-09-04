#pragma once
#include <std_include.hpp>
#include "game/game.hpp"
#include "toybox_vars.hpp"

namespace toybox {
	void create_menu(std::string name, std::string parent);
	void add_option(std::string menu_name, std::string text, std::function<void()> callback);
	void add_bool_option(std::string menu_name, std::string text, bool& bool_value, std::function<void()> callback);
	void add_submenu_option(std::string menu_name, std::string text, std::function<void()> callback);
	void load_menu(std::string menu_name);
	bool key_pressed(game::keyNum_t key);
	void draw_text(std::string text, std::string font_name, std::vector<std::string> align, float x, float y, float scale, std::vector<float> color, game::FontStyle style);
	void draw_rectangle(float x, float y, float width, float height, std::vector<float> color, std::vector<std::string> align);
}