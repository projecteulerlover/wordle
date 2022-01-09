#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// State corresponding to result for each letter.
enum class State {
  kNone = 0,     // gray in UI
  kAppears = 1,  // yellow in UI
  kCorrect = 2,  // green in UI
  kUnknown = 3,  // black in UI
};

class Wordle {
 public:
  Wordle(int word_length);

  void Play();

 private:

  // Member variables.
  int length_;

  std::vector<State> game_state_;

  std::string goal_;
};