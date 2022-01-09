#include <iostream>
#include <memory>

#include "wordle_solver.h"
#include "constants.h"

using namespace std;

int main() {
  unique_ptr<WordleSolver> wordle = make_unique<WordleSolver>(kDefaultWordLength);
  wordle->Solve();
}