#ifndef WORDLE_SRC_WORDLE_H
#define WORDLE_SRC_WORDLE_H

#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "constants.h"

class Wordle {
 public:
  Wordle(int word_length);

  ~Wordle();

  void Play();

 private:
  void LogGuessesSoFar();

  std::vector<std::string> GetColorizedGuessAndUpdateGameState(std::string_view guess);

  bool GuessOnce();

  // Member variables.
  std::size_t length_;

  std::vector<State> game_state_;

  // A particular word in `candidates_`.
  std::string goal_;

  std::unordered_set<std::string> candidates_;

  // A vector of guesses so far. Using a vector to store each character of
  // previous guesses because they contain boilerplate for logging with color.
  std::vector<std::vector<std::string>> guesses_;
};

#endif  // WORDLE_SRC_WORDLE_H