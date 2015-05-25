#include <iostream>
#include "grid.h"
#include "stdlib.h"
#include "stdio.h"
#include <unistd.h>

using namespace std;

void clearScreen() {
  cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
}

void shleep() {
  usleep(250000);
}

void explore(Grid *grid) {
  for(int i = 0; i < 10; i++) {
    clearScreen();
    grid->print(grid->getAgentLocation());
    grid->moveAgent();
    cout << i << endl;
    shleep();
  }
}

int main() {

  srand(time(NULL));

  int n = 6;
  int m = 1;

  Grid grid(n, m);

  explore(&grid);

  // clearScreen();
  // moveAgent(grid[agentPos].policy.first, &agentPos, grid);
  // grid.print(agentPos);
  // shleep();

  // clearScreen();
  // moveAgent(grid[agentPos].policy.first, &agentPos, grid);
  // grid.print(agentPos);

  return 0;
}