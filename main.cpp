#include <iostream>
#include "grid.h"
#include "stdlib.h"
#include "stdio.h"
#include <unistd.h>

using namespace std;

void explore(Grid *grid) {
  for(int i = 0; i < grid->getBounds().first; ++i) {
    for(int j = 0; j < grid->getBounds().first; ++j) {

    }
  }
}

void moveAgent(Direction direction, pair<int, int> *agentPos, Grid grid) {
  int n = grid.getBounds().first;
  GridCell nextCell;

  switch (direction)
  {
    case NORTH:
      if(agentPos->first > 0) {
        nextCell = grid[pair<int, int>(agentPos->first - 1, agentPos->second)];
        if(nextCell.type != GridCell::OBSTACLE && nextCell.type != GridCell::TERMINAL)
          agentPos->first--;
      }
      break;
    case EAST:
      if(agentPos->second < n-1) {
        nextCell = grid[pair<int, int>(agentPos->first, agentPos->second + 1)];
        if(nextCell.type != GridCell::OBSTACLE && nextCell.type != GridCell::TERMINAL)
          agentPos->second++;
      }
      break;
    case SOUTH:
      if(agentPos->first < n-1) {
        nextCell = grid[pair<int, int>(agentPos->first + 1, agentPos->second)];
        if(nextCell.type != GridCell::OBSTACLE && nextCell.type != GridCell::TERMINAL)
          agentPos->first++;
      }
      break;
    case WEST:
      if(agentPos->second > 0) {
        nextCell = grid[pair<int, int>(agentPos->first, agentPos->second - 1)];
        if(nextCell.type != GridCell::OBSTACLE && nextCell.type != GridCell::TERMINAL)
          agentPos->second--;
      }
      break;
    default:
      break;
  }
}

void clearScreen() {
  cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
}

void shleep() {
  usleep(1000000);
}

pair<int, int> pairer(int first, int second) {
  return pair<int, int>(first, second);
}

int main() {

  srand(time(NULL));

  int n = 6;
  int m = 1;

  Grid grid(n, m);
  grid.setupGrid(n, m);
  grid.initPolicy(n);

  pair<int, int> agentPos = pair<int, int>(grid.getStartLocation().first, grid.getStartLocation().second);

  clearScreen();
  grid.print(agentPos);
  shleep();

  clearScreen();
  moveAgent(grid[agentPos].policy.first, &agentPos, grid);
  grid.print(agentPos);
  shleep();

  clearScreen();
  moveAgent(grid[agentPos].policy.first, &agentPos, grid);
  grid.print(agentPos);

  return 0;
}