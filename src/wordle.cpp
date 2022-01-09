#include "wordle.h"

#include <assert.h>

#include <algorithm>
#include <experimental/set>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>
#include <unordered_map>
#include <initializer_list>
#include "constants.h"

using namespace std;

namespace {}

Wordle::Wordle(int word_length) {
  length_ = word_length;

  ifstream infile("word_bank\\" + to_string(length_) + ".words");
  string word;
  vector<string> candidates;
  while (infile >> word) {
    assert(int(word.size()) == length_);
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    candidates.push_back(word);
  }
  goal_ = candidates[rand() % candidates.size()];

  game_state_.reserve(kAlphabetSize);
  for (int i=0; i<kAlphabetSize; ++i) {
    game_state_.push_back(State::kUnknown);
  }
}