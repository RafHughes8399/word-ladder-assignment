#include "word_ladder.h"

#include <iostream>

// Please note: it's not good practice to test your code via a main function that does
//  printing. Instead, you should be using your test folder. This file should only really
//  be used for more "primitive" debugging as we know that working solely with test
//  frameworks might be overwhelming for some.

/**
 * @brief helper function to print the list of paths
 *
 * @param paths - list of paths to be printed
 */
auto path_print(const std::vector<std::vector<std::string>>& paths) -> void {
	for (auto i = std::size_t{0}; i < paths.size(); ++i) {
		std::cout << "{";
		for (auto j = std::size_t{0}; j < paths[i].size(); j++) {
			std::cout << paths[i][j];
			if (j != paths[i].size() - 1) {
				std::cout << ", ";
			}
		}
		std::cout << "}" << std::endl;
	}
}
auto main() -> int {
	// debugging newer iteration (8/6 sat wk2)
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const cat_dog_lexicon = std::unordered_set<
	    std::string>{"cat", "cag", "dag", "dog", "dig", "pig", "cog", "cig", "dat", "cot", "cap", "cop", "dap"};
	std::cout << "read lexicon" << std::endl;
	auto const paths = ::word_ladder::generate("cat", "dog", lexicon);
	path_print(paths);
	std::cout << "------------------------------------------------------------------------" << std::endl;
	auto const custom_paths = ::word_ladder::generate("cat", "dog", cat_dog_lexicon);
	path_print(custom_paths);

	std::cout << "------------------------------------------------------------------------" << std::endl;

	auto const work_play_lexicon = std::unordered_set<std::string>{
	    "fork", "form", "foam", "flam",  "flay", "pork", "perk", "peak", "pean", "plan", "peat", "plat",
	    "porn", "pirn", "pian", "port",  "pert", "word", "wood", "pood", "plod", "ploy", "worm", "bort",
	    "boat", "blat", "wert", "wort",  "base", "peel", "cold", "gore", "bore", "more", "dose", "all",
	    "work", "no",   "play", "makes", "worn", "jack", "a",    "dull", "boy",  "fine", "feel", "leaf",
	    "post", "nice", "nose", "hail",  "jest", "pole", "want", "went", "test"};

	auto const wp_paths = ::word_ladder::generate("work", "play", lexicon);
	path_print(wp_paths);
	std::cout << "------------------------------------------------------------------------" << std::endl;
	auto const wp_c_paths = ::word_ladder::generate("work", "play", work_play_lexicon);
	path_print(wp_c_paths);
	std::cout << "------------------------------------------------------------------------" << std::endl;

	auto const test_path = ::word_ladder::generate("charge", "comedo", lexicon);
	path_print(test_path);
	std::cout << "------------------------------------------------------------------------" << std::endl;
}
