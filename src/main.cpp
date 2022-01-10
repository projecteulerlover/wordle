#include <string.h>

#include <iostream>
#include <memory>

#include "constants.h"
#include "wordle.h"
#include "wordle_solver.h"

using namespace std;

int main(int argc, char** argv) {
  if (argc == 1 || (argc > 1 && strcmp(argv[1], "play") == 0)) {
    unique_ptr<Wordle> wordle = make_unique<Wordle>(kDefaultWordLength);
    wordle->Play();
  } else if (argc > 1 && strcmp(argv[1], "solve") == 0) {
    unique_ptr<WordleSolver> wordle_solver =
        make_unique<WordleSolver>(kDefaultWordLength);
    wordle_solver->Solve();
  } else {
    cout << "Unrecognized command." << endl;
    return 1;
  }
  return 0;
}