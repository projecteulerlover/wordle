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

using namespace std;

namespace {

constexpr int kCandidateSizeToPrintAll = 15;
constexpr string_view kAllCorrectResult = "22222";

bool IsLetterAllowed(int bitmask, char chr) {
  return !(bitmask >> (chr - 'a') & 1);
}

void SetLetterNotAllowed(int &bitmask, char chr) {
  bitmask |= 1UL << (chr - 'a');
}

}  // namespace

bool Wordle::ValidConstraints(string_view guess, string_view result) {
  bool ok = true;
  // Length check.
  ok &= (static_cast<int>(guess.size()) == length_ &&
         static_cast<int>(result.size()) == length_);
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
  for (const auto &chr : word) {
    if (!IsLetterAllowed(constraints_.letters[index++], chr)) {
      return false;
    }
  }
  for (const auto &chr : constraints_.must_contain) {
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
    candidates_.insert({word, true});
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
        for (char c = 'a'; c <= 'z'; ++c) {
          if (c == chr) {
            continue;
          }
          SetLetterNotAllowed(constraints_.letters[i], c);
        }
        constraints_.must_contain.insert(chr);
        break;
      default:
        cout << "Should not happen" << endl;
    }
  }
  // cout << "logging constraints" << endl;
  // for (char must : constraints_.must_contain)
  // {
  //     cout << must << endl;
  // }
  // for (int bitmask : constraints_.letters)
  // {
  //     cout << bitmask << endl;
  // }
}

void Wordle::UpdateCandidates() {
  for (auto iter = candidates_.begin(), last = candidates_.end(); iter != last;
       ++iter) {
    if (!FitsConstraints(iter->first)) {
      iter->second = false;
    }
  }
}

pair<string, double> Wordle::GetBestGuess() {
  vector<vector<double>> letter_count;
  for (int i = 0; i < length_; ++i) {
    letter_count.push_back(vector<double>(26, 0.0));
  }

  for (const auto &candidate : candidates_) {
    string_view word = candidate.first;
    int index = 0;
    for (const auto &chr : word) {
      ++letter_count[index++][chr - 'a'];
    }
  }

  double best_score = 0.0;
  string best_guess;
  for (const auto &candidate : candidates_) {
    bool is_candidate = candidate.second;
    if (!is_candidate) {
      continue;
    }
    string_view word = candidate.first;
    vector<bool> unique_letters(26, false);

    double curr_score = 0.0;
    int index = 0;
    for (const auto &chr : word) {
      int letter_index = chr - 'a';
      if (unique_letters[letter_index]) {
        continue;
      }
      for (int i = 0; i < length_; ++i) {
        curr_score +=
            (i == index++ ? 1.5 : 1.0) * letter_count[i][letter_index];
      }
      unique_letters[letter_index] = true;
    }
    if (curr_score > best_score) {
      best_score = curr_score;
      best_guess = word;
    }
  }
  return make_pair(best_guess, best_score);
}

void Wordle::SolveOnce() {
  UpdateCandidates();
  int candidate_size = count_if(
      candidates_.begin(), candidates_.end(),
      [](const pair<string, bool> &candidate) { return candidate.second; });
  cout << "Currently " << candidate_size << " candidates." << endl;
  if (candidate_size <= kCandidateSizeToPrintAll) {
    cout << "All candidates: ";
    for (const auto &candidate : candidates_) {
      bool is_candidate = candidate.second;
      string_view word = candidate.first;
      if (is_candidate) {
        cout << word << " ";
      }
    }
    cout << endl;
  }
  const auto best = GetBestGuess();
  cout << "Best guess: " << best.first << " with score " << best.second << endl;
}