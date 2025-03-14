#include "word_ladder.h"
#include <catch2/catch.hpp>

// the vibe is checking the number of paths and their lengths
TEST_CASE("atlases -> cabaret") {
	auto const english_lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const ladders = ::word_ladder::generate("atlases", "cabaret", english_lexicon);

	CHECK(std::size(ladders) != 0);
}
TEST_CASE("go -> if"){
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("go", "if", lexicon);
	CHECK(std::size(paths) != 0);
}
TEST_CASE("chi -> ego"){
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("chi", "ego", lexicon);
	CHECK(std::size(paths) != 0);
}
TEST_CASE("atom -> unau"){
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");
	auto const paths = ::word_ladder::generate("atom", "unau", lexicon);
	CHECK(std::size(paths) != 0);
}
