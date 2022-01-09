#include "wordle.h"

#include <iostream>
#include <memory>

using namespace std;

constexpr int kDefaultWordLength = 5;

int main(int argc, char *argv[])
{
	unique_ptr<Wordle> wordle = make_unique<Wordle>(kDefaultWordLength);

	wordle->Solve();
}