#include "word_ladder.h"
// data structures
#include <unordered_set>
#include <queue>
#include <string>
#include <vector>
// file reading
#include <fstream>
#include <iostream>
// other functionality
#include <algorithm>

// helper functions

/**
 * @brief helper function to determine if path has already been encountered in the current search
 *
 * @param path - the target path
 * @param paths - all seen paths
 * @return auto
 */
auto seen_path(std::vector<std::string>& path, std::vector<std::vector<std::string>>& paths) {
	for (auto i = std::size_t{0}; i < paths.size(); ++i) {
		if (paths[i] == path) {
			return true;
		}
	}
	return false;
}
/**
 * @brief helper function to find all legal words (as per some supplied dictionary) that are one
 * letter different from a word
 * @param word  - the base word
 * @param lexicon - the dictionary of all legal words
 * @return std::vector<std::string> - a vector containing all legal 'adjacent' words
 */
auto find_words(std::string& word, const std::unordered_set<std::string>& lexicon) -> std::vector<std::string> {
	auto adjacent_legal_words = std::vector<std::string>{};
	for (auto c = 'a'; c <= 'z'; ++c) {
		for (auto i = std::size_t{0}; i < word.size(); ++i) {
			auto const original_char = word[i];
			if (c == original_char) {
				continue;
			}
			word[i] = c;
			auto adjacent_word = lexicon.find(word);
			if (adjacent_word != lexicon.end()) {
				adjacent_legal_words.push_back(word);
			}
			word[i] = original_char;
		}
	}
	// alphabetical is another way of saying ascending order
	return adjacent_legal_words;
}
/**
 * @brief read in a list of words to act as the dictionary for the word ladder generation
 *
 * @param path - file path of the lexicon
 * @return std::unordered_set<std::string> - the words in the text file contained in an unordered set
 */
auto word_ladder::read_lexicon(const std::string& path) -> std::unordered_set<std::string> {
	auto lexicon = std::unordered_set<std::string>{};
	std::fstream file_stream;
	file_stream.open(path);
	if (file_stream.is_open()) {
		std::string word = "";
		while (getline(file_stream, word)) {
			lexicon.insert(word);
		}
		file_stream.close();
	}
	return lexicon;
}

/**
 * @brief function to generate the list of all shortest word ladders between a source and target word. Returns an empty
 * list if there are no solutions. If there are multiple solutions they are organsied in alphabetical order
 *
 * @param from - the source word
 * @param to - the target word
 * @param lexicon - the supporting dictionary used to generate the ladder solution(s)
 * @return std::vector<std::vector<std::string>> - the list of solutions. Each solution is a list of strings
 * highlighting the progression of the ladder from the source to the target word
 */
auto word_ladder::generate(const std::string& from, const std::string& to, const std::unordered_set<std::string>& lexicon)
    -> std::vector<std::vector<std::string>> {
	auto shortest_paths = std::vector<std::vector<std::string>>{};
	auto path = std::vector<std::string>{from}; // starting path
	auto words = std::queue<std::vector<std::string>>{}; // queue for breadth-first search
	auto visited_globally = std::unordered_set<std::string>{from};

	words.push(path);
	while (not words.empty()) {
		auto level = words.size();
		auto visisted_at_level = std::unordered_set<std::string>{};
		// processes current potential paths in batches such that they are the same length
		// it optimises the algorithm, allowing for paths that will not lead to a solution to
		// be dropped early in the algorithm

		// avoids redundant and length processing of paths that are already known to not be a solution
		for (auto i = std::size_t{0}; i < level; ++i) {
			auto current_path = words.front();
			words.pop();
			auto current_tail = *current_path.rbegin();

			// ignore potential solutions that are greater than the shortest oone
			if (current_tail == to) {
				if (shortest_paths.size() > 0) {
					if (current_path.size() > (*shortest_paths.rbegin()).size()) {
						break;
					} // - a potential optimisation for when you can find multiple paths
				}
				shortest_paths.push_back(current_path);
				continue;
				;
			}
			// find next potential steps in the ladder, iterate through them, mark them and
			// add a new path to the queue for traversal
			auto adjacent_words = find_words(current_tail, lexicon);
			for (auto i = std::size_t{0}; i < adjacent_words.size(); ++i) {
				if (visited_globally.find(adjacent_words[i]) == visited_globally.end()) {
					visisted_at_level.insert(adjacent_words[i]);
					auto new_path = current_path;
					new_path.push_back(adjacent_words[i]);
					words.push(new_path);
				}
			}
		}
		visited_globally.insert(visisted_at_level.begin(), visisted_at_level.end());
	}

	// sort the list of solutions in alphabetical order before returning
	std::sort(shortest_paths.begin(), shortest_paths.end(), std::less_equal<std::vector<std::string>>());
	return shortest_paths;
}
