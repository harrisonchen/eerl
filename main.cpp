#include <iostream>
#include "grid.h"
#include "stdlib.h"
#include "stdio.h"

using namespace std;

int main() {

  srand(time(NULL));

  int n = 6;
  int m = 1;

  Grid grid(n, m);
  grid.setupGrid(n, m);
  grid.initPolicy(n);

  pair<int, int> p = pair<int, int>(n+1, n+1);

  grid.print(p);

  return 0;
}