#pragma once
#include <std_include.hpp>
#include "game/game.hpp"

namespace menu_structs {
	struct menu_option {
		std::string text;
		std::function<void()> callback;
		std::string type;//will be used later
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
}