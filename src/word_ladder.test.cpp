#include "word_ladder.h"

#include <catch2/catch.hpp>

// helper functions for testing
/**
 * @brief testing helper function to check whether a solution is sorted in alphabetical order
 *
 * @param paths - the generated solution
 * @return true - the paths within the solution are presented in alphabetical order
 * @return false - the paths within the solution are not presented in alphabetical order
 */
auto paths_alphabetical_order(const std::vector<std::vector<std::string>>& paths) -> bool {
	for (auto i = std::size_t{0}; i < paths.size() - 1; ++i) {
		if (paths[i] > paths[i + 1]) {
			return false;
		}
	}
	return true;
}
/**
 * @brief testing helper function to check whether all paths within a solution are the shortest
 *
 * @param paths - the generated solution
 * @param shortest_length - the correct length for each path
 * @return true - all paths are the shortest
 * @return false - not all paths are the shortest
 */
auto paths_shortest_length(const std::vector<std::vector<std::string>>& paths, const int shortest_length) -> bool {
	auto casted_length = static_cast<size_t>(shortest_length);
	for (auto i = std::size_t{0}; i < paths.size(); ++i) {
		if (paths[i].size() != casted_length) {
			return false;
		}
	}
	return true;
}
/**
 * @brief check if two words are adjacent. That is to say, was teh hop from word 1 to word 2 legal?
 *
 * @param word1 - the first word, before the hop
 * @param word2 - the second word, after the hop
 * @return true - the hop was legal
 * @return false - the hop was illegal
 */
auto is_adjacent_word(const std::string& word1, const std::string& word2) -> bool {
	auto word_copy = word1;
	for (auto c = 'a'; c <= 'z'; ++c) {
		for (auto i = std::size_t{0}; i < word1.size(); ++i) {
			auto const original_char = word1[i];
			if (c == original_char) {
				continue;
			}
			word_copy[i] = c;
			if (word_copy == word2) {
				return true;
			}
			word_copy[i] = original_char;
		}
	}
	return false;
}

// test correct details for each path (i.e start with source, end with target, each word only differs by one letter)
/**
 * @brief testing helper function to determine that each path in the solution is valid. That is
 * they start with source, end with target and each word only differs by one letter
 *
 * @param paths - the solution
 * @param from - the start word
 * @param to - the end word
 * @return true - all paths are valid
 * @return false - not all paths are valid
 */
auto path_correct_structure(const std::vector<std::vector<std::string>>& paths,
                            const std::string& from,
                            const std::string& to) -> bool {
	for (auto i = std::size_t{0}; i < paths.size(); ++i) {
		if (*paths[i].begin() != from) {
			return false;
		}
		if (*paths[i].rbegin() != to) {
			return false;
		}
		for (auto j = std::size_t{0}; j < paths[i].size() - 1; ++j) {
			if (not is_adjacent_word(paths[i][j], paths[i][j + 1])) {
				return false;
			}
		}
	}
	return true;
}
TEST_CASE("at -> it") {
	auto const lexicon = std::unordered_set<std::string>{"at", "it"};

	const auto expected = std::vector<std::vector<std::string>>{{"at", "it"}};

	auto const ladders = word_ladder::generate("at", "it", lexicon);

	CHECK(ladders == expected);
}

// ok let's write some tests
// first, take the ones from the spec, that' easy money
TEST_CASE("work -> fork") {
	// use the given lexicon
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("work", "play", lexicon);
	auto const expected =
	    std::vector<std::vector<std::string>>{{"work", "fork", "form", "foam", "flam", "flay", "play"},
	                                          {"work", "pork", "perk", "peak", "pean", "plan", "play"},
	                                          {"work", "pork", "perk", "peak", "peat", "plat", "play"},
	                                          {"work", "pork", "perk", "pert", "peat", "plat", "play"},
	                                          {"work", "pork", "porn", "pirn", "pian", "plan", "play"},
	                                          {"work", "pork", "port", "pert", "peat", "plat", "play"},
	                                          {"work", "word", "wood", "pood", "plod", "ploy", "play"},
	                                          {"work", "worm", "form", "foam", "flam", "flay", "play"},
	                                          {"work", "worn", "porn", "pirn", "pian", "plan", "play"},
	                                          {"work", "wort", "bort", "boat", "blat", "plat", "play"},
	                                          {"work", "wort", "port", "pert", "peat", "plat", "play"},
	                                          {"work", "wort", "wert", "pert", "peat", "plat", "play"}};
	CHECK(paths_alphabetical_order(paths));
	CHECK(paths_shortest_length(paths, 7));
	CHECK(path_correct_structure(paths, "work", "play"));
	CHECK(paths == expected);

	// use a modified lexicon
	auto const custom_lexicon = std::unordered_set<std::string>{
	    "fork", "form", "foam", "flam",  "flay", "pork", "perk", "peak", "pean", "plan", "peat", "plat",
	    "porn", "pirn", "pian", "port",  "pert", "word", "wood", "pood", "plod", "ploy", "worm", "bort",
	    "boat", "blat", "wert", "wort",  "base", "peel", "cold", "gore", "bore", "more", "dose", "all",
	    "work", "no",   "play", "makes", "worn", "jack", "a",    "dull", "boy",  "fine", "feel", "leaf",
	    "post", "nice", "nose", "hail",  "jest", "pole", "want", "went", "test"};
	auto const new_paths = ::word_ladder::generate("work", "play", custom_lexicon);
	CHECK(paths_alphabetical_order(new_paths));
	CHECK(paths_shortest_length(new_paths, 7));
	CHECK(path_correct_structure(new_paths, "work", "play"));
	CHECK(new_paths == expected);
}

TEST_CASE("awake -> sleep") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("awake", "sleep", lexicon);
	auto const expected = std::vector<std::vector<std::string>>{
	    {"awake", "aware", "sware", "share", "sharn", "shawn", "shewn", "sheen", "sheep", "sleep"},
	    {"awake", "aware", "sware", "share", "shire", "shirr", "shier", "sheer", "sheep", "sleep"}};

	CHECK(paths_alphabetical_order(paths));
	CHECK(paths_shortest_length(paths, 10));
	CHECK(path_correct_structure(paths, "awake", "sleep"));
	CHECK(paths == expected);

	auto const custom_lexicon = std::unordered_set<std::string>{
	    "awake",  "aware",    "sware",    "share",         "sharn",    "shawn",    "shewn",
	    "sheen",  "sheep",    "sleep",    "shire",         "shirr",    "shier",    "sheer",
	    "i",      "am",       "sleepy",   "but",           "my",       "alarm",    "wakes",
	    "me",     "up",       "bloat",    "float",         "penny",    "wise",     "hawk",
	    "files",  "west",     "can",      "yes",           "hinder",   "words",    "testing",
	    "ladder", "fill",     "perform",  "cased",         "cases",    "suitcase", "buttons",
	    "asleep", "at",       "the",      "wheel",         "this",     "lexicon",  "is",
	    "mine",   "these",    "elements", "are",           "randomly", "selected", "from",
	    "brain",  "generate", "using",    "specification", "provided"};

	auto const new_paths = ::word_ladder::generate("awake", "sleep", custom_lexicon);
	CHECK(paths_alphabetical_order(new_paths));
	CHECK(paths_shortest_length(new_paths, 10));
	CHECK(path_correct_structure(new_paths, "awake", "sleep"));
	CHECK(new_paths == expected);
}

TEST_CASE("airplane -> tricycle") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("airplane", "tricycle", lexicon);
	auto const expected = std::vector<std::vector<std::string>>{};

	CHECK(path_correct_structure(paths, "airplane", "tricycle"));
	CHECK(paths == expected);
	auto const new_paths = ::word_ladder::generate("airplane", "tricycle", lexicon);
	CHECK(path_correct_structure(new_paths, "airplane", "tricycle"));
	CHECK(new_paths == expected);
}

// now some of your own
TEST_CASE("cat -> dog") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("cat", "dog", lexicon);
	auto const expected = std::vector<std::vector<std::string>>{};
	CHECK(paths_alphabetical_order(paths));
	CHECK(paths_shortest_length(paths, 4));
	CHECK(path_correct_structure(paths, "cat", "dog"));
	// CHECK(paths == expected);
}
TEST_CASE("cat -> cut") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("cat", "cut", lexicon);
	auto const expected = std::vector<std::vector<std::string>>{{"cat", "cut"}};
	CHECK(paths_alphabetical_order(paths));
	CHECK(paths_shortest_length(paths, 2));
	CHECK(path_correct_structure(paths, "cat", "cut"));
	CHECK(paths == expected);
}
TEST_CASE("fly -> dip") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("fly", "dip", lexicon);
	auto const expected = std::vector<std::vector<std::string>>{
	    {"fly", "fay", "day", "dap", "dip"}
	    //{"fly", "foy", "fop", "dop", "dip"},
	    // {"fly", "foy", "doy", "dop", "dip"} make a custom lexicon with dop and doy to have the extra paths
	};
	CHECK(paths_alphabetical_order(paths));
	CHECK(paths_shortest_length(paths, 5));
	CHECK(path_correct_structure(paths, "fly", "dip"));
	CHECK(paths == expected);
}

TEST_CASE("ploy -> base") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("ploy", "base", lexicon);
	CHECK(paths_alphabetical_order(paths));
	CHECK(paths_shortest_length(paths, 8));
	CHECK(path_correct_structure(paths, "ploy", "base"));

	/* 	auto const custom_lexicon = ::word_ladder::read_lexicon("./custom.txt");
	    auto const custom_paths = ::word_ladder::generate("ploy", "base", custom_lexicon);
	    CHECK(paths_alphabetical_order(paths));
	    CHECK(paths_shortest_length(paths,7));
	    CHECK(path_correct_structure(paths, "ploy", "base")); */
}
TEST_CASE("luck -> fail") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("luck", "fail", lexicon);
	CHECK(paths_alphabetical_order(paths));
	CHECK(paths_shortest_length(paths, 6));
	CHECK(path_correct_structure(paths, "luck", "fail"));

	// create a custom lexicon that is far more constrained so you can do the expected == paths
}
TEST_CASE("wise -> wont") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("wise", "wont", lexicon);
	CHECK(paths_alphabetical_order(paths));
	CHECK(paths_shortest_length(paths, 4));
	CHECK(path_correct_structure(paths, "wise", "wont"));
}
TEST_CASE("seen -> sent") {
	// seen -> seed -> send -> sent
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("seen", "sent", lexicon);
	CHECK(paths_alphabetical_order(paths));
	CHECK(paths_shortest_length(paths, 4));
	CHECK(path_correct_structure(paths, "seen", "sent"));
}
TEST_CASE("user -> void") {
	// user -> seed -> send -> sent
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("user", "void", lexicon);
	CHECK(paths.size() == 0);
}

TEST_CASE("tiles -> dingo") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("tiles", "dingo", lexicon);
	auto const expected = std::vector<std::vector<std::string>>{{"tiles", "tines", "dines", "dings", "dingo"},
	                                                            {"tiles", "tines", "tings", "dings", "dingo"}};
	CHECK(paths_alphabetical_order(paths));
	CHECK(paths_shortest_length(paths, 5));
	CHECK(path_correct_structure(paths, "tiles", "dingo"));
	CHECK(paths == expected);
}
TEST_CASE("tough -> whale") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("tough", "whale", lexicon);

	CHECK(paths_alphabetical_order(paths));
	CHECK(paths_shortest_length(paths, 11));
	CHECK(path_correct_structure(paths, "tough", "whale"));
}
TEST_CASE("trust -> lousy") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("trust", "lousy", lexicon);
	auto const expected =
	    std::vector<std::vector<std::string>>{{"trust", "crust", "cruse", "druse", "douse", "louse", "lousy"}};
	CHECK(paths_alphabetical_order(paths));
	CHECK(paths_shortest_length(paths, 7));
	CHECK(path_correct_structure(paths, "trust", "lousy"));
	CHECK(paths == expected);
}
TEST_CASE("sense -> dense") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("sense", "dense", lexicon);
	auto const expected = std::vector<std::vector<std::string>>{{"sense", "dense"}};
	CHECK(paths_alphabetical_order(paths));
	CHECK(paths_shortest_length(paths, 2));
	CHECK(path_correct_structure(paths, "sense", "dense"));
	CHECK(paths == expected);
}
TEST_CASE("super -> sabre") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("super", "sabre", lexicon);
	auto const expected = std::vector<std::vector<std::string>>{
	    {"super", "surer", "curer", "carer", "cares", "carls", "carle", "cable", "sable", "sabre"},
	    {"super", "surer", "curer", "cures", "cares", "carls", "carle", "cable", "sable", "sabre"},
	    {"super", "surer", "curer", "cures", "curls", "carls", "carle", "cable", "sable", "sabre"}};
	CHECK(paths_alphabetical_order(paths));
	CHECK(paths_shortest_length(paths, 10));
	CHECK(path_correct_structure(paths, "super", "sabre"));
	CHECK(paths == expected);
}
TEST_CASE("poise -> snarl") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("poise", "snarl", lexicon);
	auto const expected = std::vector<std::vector<std::string>>{
	    {"poise", "peise", "pease", "cease", "chase", "chare", "share", "snare", "snarl"},
	    {"poise", "peise", "pease", "phase", "chase", "chare", "share", "snare", "snarl"},
	    {"poise", "peise", "seise", "seine", "shine", "shire", "share", "snare", "snarl"},
	    {"poise", "peise", "seise", "seine", "spine", "spire", "spare", "snare", "snarl"},
	    {"poise", "prise", "prase", "phase", "chase", "chare", "share", "snare", "snarl"}};
	CHECK(paths_alphabetical_order(paths));
	CHECK(paths_shortest_length(paths, 9));
	CHECK(path_correct_structure(paths, "poise", "snarl"));
	CHECK(paths == expected);
}
TEST_CASE("axiom -> zooid") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("axiom", "zooid", lexicon);
	auto const expected = std::vector<std::vector<std::string>>{};
	CHECK(paths == expected);
}

TEST_CASE("elixir -> hunter") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("elixir", "hunter", lexicon);
	auto const expected = std::vector<std::vector<std::string>>{};
	CHECK(paths == expected);
}
TEST_CASE("dinner -> supper") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("dinner", "supper", lexicon);
	CHECK(paths_alphabetical_order(paths));
	CHECK(paths_shortest_length(paths, 9));
	CHECK(path_correct_structure(paths, "dinner", "supper"));
}

TEST_CASE("fryers -> flyers") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("fryers", "flyers", lexicon);
	auto const expected = std::vector<std::vector<std::string>>{{"fryers", "flyers"}};
	CHECK(paths == expected);
}
TEST_CASE("animal -> grazed") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("animal", "grazed", lexicon);
	auto const expected = std::vector<std::vector<std::string>>{
	    {"animal", "animas", "animes", "anises", "arises", "crises", "crases", "crazes", "crazed", "grazed"},
	    {"animal", "animas", "animes", "anises", "arises", "crises", "crases", "crazes", "grazes", "grazed"}};
	CHECK(paths_alphabetical_order(paths));
	CHECK(paths_shortest_length(paths, 10));
	CHECK(path_correct_structure(paths, "animal", "grazed"));
	CHECK(expected == paths);
}
TEST_CASE("basics -> beaker") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("basics", "beaker", lexicon);
	CHECK(paths_alphabetical_order(paths));
	CHECK(paths_shortest_length(paths, 13));
	CHECK(path_correct_structure(paths, "basics", "beaker"));
}
TEST_CASE("bested -> fester") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("bested", "fester", lexicon);
	CHECK(paths_alphabetical_order(paths));
	CHECK(paths_shortest_length(paths, 4));
	CHECK(path_correct_structure(paths, "bested", "fester"));
}
TEST_CASE("running -> walking") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("running", "walking", lexicon);
	auto const expected =
	    std::vector<std::vector<std::string>>{{"running", "tunning", "tanning", "tanking", "talking", "walking"}};
	CHECK(paths_alphabetical_order(paths));
	CHECK(paths_shortest_length(paths, 6));
	CHECK(path_correct_structure(paths, "running", "walking"));
	CHECK(expected == paths);
}
TEST_CASE("zyzzyva -> mitosis") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("zyzzyva", "mitosis", lexicon);
	auto const expected = std::vector<std::vector<std::string>>{};
	CHECK(paths == expected);
	CHECK(paths.size() == 0);
}
TEST_CASE("bangers -> rustles") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("bangers", "rustles", lexicon);
	CHECK(paths_alphabetical_order(paths));
	CHECK(paths_shortest_length(paths, 12));
	CHECK(path_correct_structure(paths, "bangers", "rustles"));
}
TEST_CASE("benched -> hunches") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("benched", "hunches", lexicon);
	CHECK(paths_alphabetical_order(paths));
	CHECK(paths_shortest_length(paths, 4));
	CHECK(path_correct_structure(paths, "benched", "hunches"));
}

// testing the longest paths for words use this for benchmark testing
TEST_CASE("go -> if") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("go", "if", lexicon);
	CHECK(paths_alphabetical_order(paths));
	CHECK(paths_shortest_length(paths, 6));
	CHECK(path_correct_structure(paths, "go", "if"));
}
TEST_CASE("chi -> ego") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("chi", "ego", lexicon);
	CHECK(paths_alphabetical_order(paths));
	CHECK(paths_shortest_length(paths, 10));
	CHECK(path_correct_structure(paths, "chi", "ego"));
}
TEST_CASE("atom -> unau") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("atom", "unau", lexicon);
	CHECK(paths_alphabetical_order(paths));
	CHECK(paths_shortest_length(paths, 18));
	CHECK(path_correct_structure(paths, "atom", "unau"));
}
/* TEST_CASE("nylon -> iller"){
    auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
    auto const paths = ::word_ladder::generate("nylon", "iller", lexicon);
    CHECK(paths_alphabetical_order(paths));
    CHECK(paths_shortest_length(paths, 31));
    CHECK(path_correct_structure(paths, "nylon", "iller"));
} */
TEST_CASE("boyish -> painch") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("boyish", "painch", lexicon);
	CHECK(paths_alphabetical_order(paths));
	CHECK(paths_shortest_length(paths, 40));
	CHECK(path_correct_structure(paths, "boyish", "painch"));
}
TEST_CASE("atlases -> cabaret") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("atlases", "cabaret", lexicon);
	CHECK(paths_alphabetical_order(paths));
	CHECK(paths_shortest_length(paths, 58));
	CHECK(path_correct_structure(paths, "atlases", "cabaret"));
}

TEST_CASE("decanting -> derailing") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("decanting", "derailing", lexicon);
	CHECK(paths_alphabetical_order(paths));
	CHECK(paths_shortest_length(paths, 18));
	CHECK(path_correct_structure(paths, "decanting", "derailing"));
}
TEST_CASE("blistering -> swithering") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("blistering", "swithering", lexicon);
	CHECK(paths_alphabetical_order(paths));
	CHECK(paths_shortest_length(paths, 10));
	CHECK(path_correct_structure(paths, "blistering", "swithering"));
}
TEST_CASE("collectivists -> collectivized") {
    auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
    auto const paths = ::word_ladder::generate("collectivisits", "collectivized", lexicon);
    CHECK(paths.size() == 0);
}
TEST_CASE("charge -> comedo") {
    auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
    auto const paths = ::word_ladder::generate("charge", "comedo", lexicon);
    CHECK(paths_alphabetical_order(paths));
    CHECK(paths_shortest_length(paths, 23));
    CHECK(path_correct_structure(paths, "charge", "comedo"));
}
