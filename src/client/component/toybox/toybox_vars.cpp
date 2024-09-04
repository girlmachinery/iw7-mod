#include <std_include.hpp>
#include "toybox_vars.hpp"

namespace vars {
	std::map<std::string, menu_structs::menu> menus;
	std::map<std::string, int> menu_scrolls;
	std::string current_menu = "none";
}