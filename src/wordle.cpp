#include "wordle.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <algorithm>
#include <experimental/set>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>
#include <unordered_map>

#include "constants.h"

using namespace std;

namespace {

std::string GetColorizedLetter(State state, char chr) {
  switch (state) {
    case State::kNone:
      return "\033[1;31m" + string(1, chr) + "\033[0m";
    case State::kAppears:
      return "\033[1;33m" + string(1, chr) + "\033[0m";
    case State::kCorrect:
      return "\033[1;32m" + string(1, chr) + "\033[0m";
    case State::kUnknown:
      return "\033[1;37m" + string(1, chr) + "\033[0m";
    default:
      cout << "Should not happen" << endl;
      assert(false);
  }
}

}  // namespace

Wordle::Wordle(int word_length) {
  length_ = word_length;

  ifstream infile("word_bank\\" + to_string(length_) + ".words");
  string word;

  vector<string> candidates;
  while (infile >> word) {
    assert(word.size() == length_);
    transform(word.begin(), word.end(), word.begin(), ::toupper);
    candidates_.insert(word);
    candidates.push_back(word);
  }

  srand(time(NULL));
  goal_ = candidates[rand() % candidates.size()];

  game_state_.reserve(kAlphabetSize);
  for (size_t i = 0; i < kAlphabetSize; ++i) {
    game_state_.push_back(State::kUnknown);
  }
  // Delete this.
  cout << goal_ << endl;
}

void Wordle::Play() {
  cout << "Starting Game\n\n";
  do {
    bool won = GuessOnce();
    if (won) {
      int tries = guesses_.size() + 1;
      cout << "\033[1;32mCongrats, your guess was correct!\033[0m "
           << "It only took you " << tries << (tries == 1 ? " try " : " tries ") << ":)" << endl;
      return;
    } else {
      cout << "Game state so far: \n";
      for (const auto& row : kKeyboardView) {
        for (const char chr : row) {
          cout << GetColorizedLetter(game_state_[chr - 'A'], chr) << "  ";
        }
        cout << "\n";
      }
      cout << "\n";
    }
  } while (true);
}

vector<std::string> Wordle::GetColorizedGuessAndUpdateGameState(
    string_view guess) {
  vector<std::string> guess_with_color;
  vector<State> states(length_, State::kUnknown);
  for (size_t i = 0; i < length_; ++i) {
    const char chr = guess[i];
    if (chr == goal_[i]) {
      states[i] = State::kCorrect;
    }
  }
  for (size_t i = 0; i < length_; ++i) {
    const char chr = guess[i];
    size_t index = 0;
    const auto position = find_if(
        goal_.begin(), goal_.end(), [&index, &states, &chr](const char c) {
          return states[index++] != State::kCorrect && c == chr;
        });
    State state = states[i];
    if (state == State::kUnknown) {
      if (position == goal_.end()) {
        state = State::kNone;
      } else {
        state = State::kAppears;
      }
    }
    guess_with_color.push_back(GetColorizedLetter(state, chr));
    if (game_state_[chr - 'A'] == State::kCorrect && state == State::kAppears) {
      continue;
    }
    game_state_[chr - 'A'] = state;
  }

  return guess_with_color;
}

void Wordle::LogGuessesSoFar() {
  if (!guesses_.empty()) {
    cout << "Guesses so far:\n";
    for (const auto& colorized_guess : guesses_) {
      for (const auto& colorized_letter : colorized_guess) {
        cout << colorized_letter;
      }
      cout << endl;
    }
    cout << endl;
  }
}

bool Wordle::GuessOnce() {
  LogGuessesSoFar();

  string guess;
  cout << "Make your guess: ";
  cin >> guess;
  cout << endl;

  transform(guess.begin(), guess.end(), guess.begin(), ::toupper);

  if (candidates_.find(guess) == candidates_.end() || guess.size() != length_) {
    cout << "You didn't guess a valid word; try again." << endl;
    return false;
  }

  if (guess == goal_) {
    return true;
  }

  guesses_.push_back(GetColorizedGuessAndUpdateGameState(guess));

  return false;
}