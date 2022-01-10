#include <iostream>
#include <memory>

#include "wordle_solver.h"
#include "wordle.h"
#include "constants.h"

using namespace std;

int main() {
  unique_ptr<Wordle> wordle = make_unique<Wordle>(kDefaultWordLength);
  wordle->Play();

  // unique_ptr<WordleSolver> wordle_solver = make_unique<WordleSolver>(kDefaultWordLength);
  // wordle_solver->Solve();
}