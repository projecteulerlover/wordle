#include <unordered_map>
#include <unordered_set>
#include <string_view>
#include <string>
#include <vector>

// Constraints from the current guesses.
struct Constraints {
    // A vector of size `length_` (5 currently). Each contained int is a bitmask of currently
    // acceptable letters for that letter location.
    // That is, the i-th bit of letters[j = 1 <=> the j-th character of the answer cannot be
    // the i-th letter of the alphabet.
    //   e.g., if the vector is <0_2, 1_2, 10_2, 100_2, 1000_2>, it means the
    //           - 1st letter has no restrictions
    //           - 2nd letter cannot be 'a'
    //           - 3rd letter cannot be 'b'
    //           - 4th letter cannot be 'c'
    //           - 5th letter cannot be 'd'
    std::vector<int> letters;
    // The set of letters the answer MUST contain (used to deal with yellow results).
    std::unordered_set<char> must_contain;
};

// State corresponding to result for each letter.
enum class State {
    kNone = 0,  // gray in UI
    kAppears = 1,  // yellow in UI
    kCorrect = 2,  // green in UI
};

class Wordle {
    public:
        Wordle(int word_length);

        // Solves the current game, with user input.
        // 1. Logs the "best" guess.
        //    e.g. $ Best guess: "carbs"
        // 2. Prompts the user for the guess the made, and the result from the UI. The result
        //    is a sequence of 5 characters, corresponding to State enum values.
        //    e.g. $ Input guess: "carbs"
        //         $ Input result: "21100"
        //    Implies "C" is green, "A" and "B" are yellow, "B" and "S" are gray.
        // 3. Uses the given `result` to update `constraints_`. Uses the updated `constraints`
        //    to update whether words in `candidates_` can still be valid solutions (but does
        //    not prevent them from being guessed).
        // 4. Repeat from step 1.
        void Solve();

    private:
        // Performs one iteration; steps 1-3 above.
        void SolveOnce();
        
        // Checks if the given word fits `constraints`.
        bool FitsConstraints(std::string_view word);

        // Uses the `guess` and `result` to update `constraints_`.
        void UpdateConstraints(std::string_view guess, string_view result);

        // Uses `constraints` to modify whether a word in `candidates_` is still in the running
        // as the answer.
        void UpdateCandidates();

        // Scores all the words in `candidates_`, regardless of whether or not they can still be
        // the answer, and returns the word with the highest score.
        string GetBestGuess();

        // Checks if the user inputs for `guess` and `result` are syntactically correct, otherwise
        // prompts the user again for these values.
        bool ValidConstraints(std::string_view guess, std::string_view result);


        // Member variables.
        int length_;

        std::unordered_map<string, bool> candidates_;

        Constraints constraints_;
};