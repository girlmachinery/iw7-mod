#include <std_include.hpp>
#include "toybox_vars.hpp"

namespace vars {
	std::map<std::string, menu_structs::menu> menus;
	std::map<std::string, int> menu_scrolls;
	std::string current_menu = "none";
	//todo: make the menu colors a variable
	float menu_x = 0;
	float menu_y = -300;
	float menu_width = 420;
	float menu_height = 500;

	bool test_bool = false;
	int test_int = 5;
	float test_float = 5.f;
}