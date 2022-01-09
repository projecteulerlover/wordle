#include <string_view>
#include <initializer_list>

// Word length.
constexpr int kDefaultWordLength = 5;

// Number of candidates left where we choose to print all candidates to CLI.
constexpr int kCandidateSizeToPrintAll = 15;

// State of `results` when the guess is correct. 
constexpr std::string_view kAllCorrectResult = "22222";

// Size of the alphabet.
constexpr int kAlphabetSize = 26;

// Alphabet display.
constexpr std::initializer_list<std::initializer_list<char>> kKeyBoardView = {
    {'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'},
    {'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L'},
    {'Z', 'X', 'C', 'V', 'B', 'N', 'M'}};