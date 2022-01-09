#include <unordered_map>
#include <unordered_set>
#include <string_view>
#include <string>
#include <vector>

using namespace std;

struct Constraints {
    vector<int> letters;
    unordered_set<char> must_contain;
};

enum class State {
    kNone = 0,
    kAppears = 1,
    kCorrect = 2,
};

class Wordle {
    struct WordFormat {

    };
    
    public:
        Wordle(int word_length);

        void Solve();

    private:
        void SolveOnce();

        bool FitsConstraints(string_view word);

        void UpdateConstraints(string_view guess, string_view result);

        void PurgeCandidates();

        string GetBestGuess();

        bool ValidConstraints(string_view guess, string_view result);


        // Member variables.
        int length_;

        unordered_set<string> candidates_;

        Constraints constraints_;
};