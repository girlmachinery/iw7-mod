#pragma once
#include <std_include.hpp>
#include "game/game.hpp"

namespace menu_structs {
	struct option_variable {
		bool bool_value;
		int int_value;
		float float_value;
		std::string string_value;
	};

	struct slider_variable {
		//this whole thing sucks ass but i just wokeup
		int* int_value = nullptr;
		int int_step;
		int int_min;
		int int_max;
		float* float_value = nullptr;
		float float_step;
		float float_min;
		float float_max;
		std::vector<std::string> string_values;//array slider setup
		std::vector<int> int_values;//array slider setup
		std::vector<float> float_values;//array slider setup
	};

	struct menu_option {
		std::string text;
		std::function<void()> callback;
		std::string type;//will be used later
		option_variable variable;
		slider_variable slider_variable;
	};

	struct menu {
		std::string name;
		std::string parent;
		std::vector<menu_option> options;
	};
}

namespace vars {
	extern std::map<std::string, menu_structs::menu> menus;
	extern std::map<std::string, int> menu_scrolls;
	extern std::string current_menu;

	extern float menu_x;
	extern float menu_y;
	extern float menu_width;
	extern float menu_height;

	extern bool test_bool;
	extern int test_int;
	extern float test_float;
}