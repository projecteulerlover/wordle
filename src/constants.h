#ifndef WORDLE_SRC_CONSTANTS_H
#define WORDLE_SRC_CONSTANTS_H

#include <initializer_list>
#include <string_view>

// State corresponding to result for each letter.
enum class State {
  kNone = 0,     // gray in UI
  kAppears = 1,  // yellow in UI
  kCorrect = 2,  // green in UI
  kUnknown = 3,  // black in UI
};

// Word length.
constexpr int kDefaultWordLength = 5;

// State of `results` when the guess is correct.
constexpr std::string_view kAllCorrectResult = "22222";

// Number of candidates left where we choose to print all candidates to CLI.
constexpr int kCandidateSizeToPrintAll = 15;

// Size of the alphabet.
constexpr int kAlphabetSize = 26;

// Alphabet display. Pairs of row left padding and row letters.
constexpr std::initializer_list<std::initializer_list<char>> kKeyboardView = {
    {'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'},
    {'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L'},
    {'Z', 'X', 'C', 'V', 'B', 'N', 'M'}};

#endif  // WORDLE_SRC_CONSTANTS_H