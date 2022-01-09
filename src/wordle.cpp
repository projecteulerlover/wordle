#include "wordle.h"

#include <unordered_map>
#include <string_view>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <assert.h>
#include <experimental/set>
#include <numeric>

using namespace std;

namespace {

    constexpr int kCandidateSizeToPrintAll = 15;
    constexpr string_view kAllCorrectResult = "22222";

    bool IsLetterAllowed(int bitmask, char chr) {
        return !(bitmask >> (chr - 'a') & 1);
    }

    void SetLetterNotAllowed(int& bitmask, char chr) {
        bitmask |= 1UL << (chr - 'a');
    }

}

bool Wordle::ValidConstraints(string_view guess, string_view result) {
    bool ok = true;
    // Length check.
    ok &= (guess.size() == length_ && result.size() == length_);
    // Guess check.
    for (const char chr : guess) {
        if (!('a' <= chr && chr <= 'z')) {
            ok = false;
            break;
        }
    }
    // Result check.
    for (const char chr : result) {
        int state = chr - '0';
        if (!(0 <= state && state <= 2)) {
            ok = false;
            break;
        }
    }
    if (!ok) {
        cout << "Inputs were malformed, please guess again." << endl;
    }
    return ok;
}

bool Wordle::FitsConstraints(string_view word) {
    int32_t index = 0;
    for (const auto& chr : word) {
        if (!IsLetterAllowed(constraints_.letters[index++], chr)) {
            return false;
        }
    }
    for (const auto& chr : constraints_.must_contain) {
        if (word.find(chr) == string::npos) {
            return false;
        }
    }
    return true;
}

Wordle::Wordle(int word_length) {
    length_ = word_length;

    constraints_.letters.reserve(length_);
    for (int i = 0; i < length_; ++i) {
        constraints_.letters.push_back(0);
    }
    
    ifstream infile("word_bank\\" + to_string(length_) + ".words");
    string word;
    while (infile >> word) {
        assert(int(word.size()) == length_);
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        candidates_.insert(word);
    }
}

void Wordle::Solve() {
    string guess, result;
    
    SolveOnce();
    while (true) {
        do {
            cout << "Input guess: ";
            cin >> guess;
            cout << "Input result: ";
            cin >> result;
        } while (!ValidConstraints(guess, result));
        if (result == kAllCorrectResult) {
            cout << "Done! Congrats :)" << endl;
            break;
        } else {
            UpdateConstraints(guess, result);
            SolveOnce();
        }
    }
}

void Wordle::UpdateConstraints(string_view guess, string_view result) {
    for (int i = 0; i < length_; ++i) {
        char chr = guess[i];
        State state = static_cast<State>(result[i] - '0');
        switch (state) {
            case State::kNone:
                for (int j = 0; j < length_; ++j) {
                    SetLetterNotAllowed(constraints_.letters[j], chr);
                }
                break;
            case State::kAppears:
                SetLetterNotAllowed(constraints_.letters[i], chr);
                constraints_.must_contain.insert(chr);
                break;
            case State::kCorrect:
                for (int j = 0; j < length_; ++j) {
                    for (char c = 'a'; c <= 'z'; ++c) {
                        if (c == chr) {
                            continue;
                        }
                        SetLetterNotAllowed(constraints_.letters[j], c);
                    }
                }
                constraints_.must_contain.insert(chr);
                break;
            default:
                cout << "Should not happen" << endl;
        }
    }
    // cout << "logging constraints" << endl;
    // for (char must : constraints_.must_contain) {
    //     cout << must << endl;
    // }
    // for (int bitmask : constraints_.letters) {
    //     cout << bitmask << endl;
    // }
}

void Wordle::PurgeCandidates() {
    for (auto iter = candidates_.begin(), last = candidates_.end(); iter != last;) {
        if (!FitsConstraints(*iter)) {
            iter = candidates_.erase(iter);
        } else {
            ++iter;
        }
    }
}

string Wordle::GetBestGuess() {
    vector<vector<int>> letter_count;
    for (int i=0; i < length_; ++i) {
        letter_count.push_back(vector<int>(26, 0));
    }

    for (string_view word : candidates_) {
        int index = 0;
        for (const auto& chr : word) {
            ++letter_count[index++][chr - 'a'];
        }
    }
    
    int best_score = 0;
    string best_guess;
    for (string_view word : candidates_) {
        vector<bool> unique_letters(26, false);

        int curr_score = 0;
        int index = 0;
        for (const auto& chr : word) {
            int letter_index = chr - 'a';
            if (unique_letters[letter_index]) {
                continue;
            }
            curr_score += letter_count[index][letter_index];
            unique_letters[letter_index] = true;
        }
        if (curr_score > best_score) {
            best_score = curr_score;
            best_guess = word;
        }
    }
    return best_guess;
}

void Wordle::SolveOnce() {
    PurgeCandidates();
    cout << "Currently " << candidates_.size() << " candidates." << endl;
    if (candidates_.size() <= kCandidateSizeToPrintAll) {
        cout << "All candidates: ";
        for (string_view candidate : candidates_) {
            cout << candidate << " ";
        }
        cout << endl;
    } else { 
        cout << "Best guess: " << GetBestGuess() << endl;
    }
}