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
  usleep(50000);
}

void explore(Grid *grid) {
  for(int i = 0; i < 5000000; i++) {
    grid->moveAgent();
    if(grid->iteration >= 100000) {
      return;
    }
  }
}

int main(int argc, char *argv[]) {

  if(argc != 3) {
    cout << "Incorrect input!" << endl;
    return 0;
  }

  int n = 4;
  int m = 1;

  n = atoi(argv[1]);
  m = atoi(argv[2]);

  Grid grid(n, m);

  cout << "Initial ------------------------" << endl;
  cout << "n: " << n << " m: " << m << endl;
  // cout << "seed: " << "1431537608" << endl;

  grid.print(grid.getAgentLocation());  

  grid.initPolicy(grid.getAgentLocation().first, grid.getAgentLocation().second);
  explore(&grid);

  cout << endl << "Final ------------------------" << endl;
  cout << "Iterations: " << grid.iteration << endl;

  grid.print(grid.getAgentLocation());

  return 0;
}