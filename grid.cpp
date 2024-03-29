////////////////////////////////
//
// University of California, Riverside
// CS170 Spring 2015 Assignment 3
// grid.cpp
//
// You are encouraged to modify and add to this file
//////////////////////////////
#include "grid.h"

#include <iostream>
#include <iomanip>
#include <cfloat>
#include <cmath>
#include <sstream>
#include "time.h"

using namespace std;
///////////////
/// Utility ///
///////////////

///////////
// Function centerStr
///////////
// In -> string s      - value of string we are centering.
//       int reserve   - # of characters (width).
//       char autofill - char to print for the left and right remaining space.
// Out -> string
//
// Returns a string with length of the reserved space with the string, s, centered.
string centerStr(const string & s, int reserve, char autofill)
{
    int blanks = reserve - s.length();
    int lbuf = blanks / 2; // Add one to favor remainder to right
    int rbuf = (blanks+1) / 2;
    stringstream ss;
    ss << setfill(autofill) << setw(lbuf) << "" << s << setw(rbuf) << "";
    return ss.str();
}

///////////
// Function toString
///////////
// In -> double
// Out -> string
//
// Returns a double converted to a string with fixed precision.
string toString(double value, int prec)
{
    stringstream ss;
    if (value > 0.0)
        ss << '+';
    ss << setprecision(prec) << fixed << value;
    return ss.str();
}

///////////
// Function toString
///////////
// In -> Direction
// Out -> string
//
// Returns an ascii version of Direction as a string.
string toString(Direction dir)
{
    switch (dir)
    {
        case NORTH:
            return "^";
        case EAST:
            return ">";
        case SOUTH:
            return "v";
        case WEST:
            return "<";
        default:
            return "?";
    }
}

///////////////////////
/// GridCell Object ///
///////////////////////

// Constructors
GridCell::GridCell()
    : type(BLANK), reward(0.0), start(false), policy(NONE, 0.0)
{
}

GridCell::GridCell(GridCellType type, double reward, bool start)
    : type(type), reward(reward), start(start), policy(NONE, 0.0)
{
}
    
///////////
// Function print
///////////
// In -> string pstr - agent symbol, can be the empty string if not need print.
//       int reserve - the amount of characters (width) we can print for each line.
// Out -> vector<string> - index by row, the output contents of this cell
//
// Returns the output contents of this cell in four different rows.
// Refer to Grid class for more information of the ascii-grid layout
vector<string> GridCell::print(const string & pstr, int reserve) const
{
    vector<string> ret(4);
        
    string prefix1st = "+";
    string prefixedge = "|";
    if (type == OBSTACLE)
    {
        ret[0] = prefix1st + centerStr("", reserve - prefix1st.length(), '-');
        ret[1] = prefixedge + centerStr("xxxx", reserve - prefixedge.length(), ' ');
        ret[2] = prefixedge + centerStr("xxxx", reserve - prefixedge.length(), ' ');
        ret[3] = prefixedge + centerStr("xxxx", reserve - prefixedge.length(), ' ');
    }
    else
    {
        ret[0] = prefix1st + centerStr("", reserve - prefix1st.length(), '-');
        ret[1] = prefixedge + centerStr(toString(reward + policy.second), reserve - prefixedge.length());
        ret[2] = prefixedge + centerStr(start ? pstr + "S" : pstr, reserve - prefixedge.length());
        if (type == TERMINAL)
            ret[3] = prefixedge + centerStr("*", reserve - prefixedge.length());
        else
            ret[3] = prefixedge + centerStr(toString(policy.first), reserve - prefixedge.length());
    }
        
    return ret;
}
    
///////////
// Function getPolicy
///////////
// In ->
// Out -> pair<Direction, double>
//
// Returns the current policy in the form of the pair,
// The first being the action and the the second being the value.
pair<Direction, double> GridCell::getPolicy() const
{
    return policy;
}

///////////////////
/// Grid Object ///
///////////////////
    
///////////////
// Constructors
///////////////
// In -> int n, parameter described by assignment prompt
//       int m, parameter described by assignment prompt
// Out ->
Grid::Grid(int n, int m)
{
    bounds = pair<int,int>(n, n);
    grid = vector<vector<GridCell> >(bounds.first, vector<GridCell>(bounds.second));
    Grid::setupGrid(n, m);

    discount = 0.75;
    iteration = 1;
    prevDir = NONE;

    /* Currently ignores the m parameter in terms of rewards and penalty placement.
        Also ignores n obstacle placement and assigns startLocation to invalid
        location on the grid.
     */
}

// Generate obstacles
void Grid::setupObstacles(int n, int m) {
    for(int i = 0; i < n; ++i) {
        int first;
        int second;
        
        do {
            first = rand() % n;
            second = rand() % n;
        } while(grid[first][second].type == GridCell::OBSTACLE ||
                grid[first][second].type == GridCell::TERMINAL ||
                grid[first][second].start == true);

        grid[first][second].type = GridCell::OBSTACLE;
    }
}

// Generate rewards
void Grid::setupRewards(int n, int m) {
    for(int i = 0; i < m; ++i) {
        int first;
        int second;
        
        do {
            first = rand() % n;
            second = rand() % n;
        } while(grid[first][second].type == GridCell::OBSTACLE ||
                grid[first][second].type == GridCell::TERMINAL ||
                grid[first][second].start == true);

        grid[first][second].type = GridCell::TERMINAL;
        grid[first][second].reward = 10;
    }
}

// Generate penalties
void Grid::setupPenalties(int n, int m) {
    for(int i = 0; i < m; ++i) {
        int first;
        int second;
        
        do {
            first = rand() % n;
            second = rand() % n;
        } while(grid[first][second].type == GridCell::OBSTACLE ||
                grid[first][second].type == GridCell::TERMINAL ||
                grid[first][second].start == true);

        grid[first][second].type = GridCell::TERMINAL;
        grid[first][second].reward = -10;
    }
}

void Grid::setupGrid(int n, int m) {
    srand(time(NULL));
    // srand(1431537608);

    int startFirst = rand() % n;
    int startSecond = rand() % n;
    startLocation = pair<int, int>(startFirst, startSecond);
    agentLocation = pair<int, int>(startFirst, startSecond);
    grid[startFirst][startSecond].start = true;
    
    Grid::setupObstacles(n, m);
    Grid::setupRewards(n, m);
    Grid::setupPenalties(n, m);
}

// Check if GridCell is within Grid bounds
bool Grid::inBound(int i, int j) {
    if(i < 0 || i >= bounds.first || j < 0 || j >= bounds.first)
        return false;

    return true;
}

// Check if GridCell is of OBSTACLE type
bool Grid::isObstacle(int i, int j) {
    if(grid[i][j].type == GridCell::OBSTACLE)
        return true;

    return false;
}

// Check if GridCell allows agent movement
bool Grid::movable(int i, int j) {
    return Grid::inBound(i, j) && !Grid::isObstacle(i, j);
}

// Check if GridCell is of type TERMINAL
bool Grid::isTerminal(int i, int j) {
    if(grid[i][j].type == GridCell::TERMINAL)
        return true;

    return false;
}

// Generate a random DIRECTION
Direction randomDirection(int r) {
    switch(r) {
        case 0:
            return NORTH;
            break;
        case 1:
            return EAST;
            break;
        case 2:
            return SOUTH;
            break;
        case 3:
            return WEST;
            break;
        default:
            return NORTH;
            break;
    }
}

// Initiate a policy for a GridCell
void Grid::initPolicy(int i, int j) {
    int r = rand() % 4;
    grid[i][j].policy = pair<Direction, double>(randomDirection(r), 0.0);
}

// Returns maxargs of utilities and updates policy along the way for optimal policy
double Grid::updatePolicy(int i, int j) {
    // Best policy direction utility
    double maxUtility = 0.0;

    // Calculated policy rewards for next optimal movement
    double totalUtility = 0.0;

    // Utitlity of neighboring GridCell
    double utility = 0.0;
    bool allZeros = true;
    pair<int, int> al = agentLocation;

    GridCell* gc = &grid[al.first][al.second];

    Direction dir = gc->policy.first;
    Direction newDir = dir;

    // Check NORTH
    if(Grid::movable(al.first - 1, al.second) && prevDir != SOUTH) {
        utility = Grid::getUtility(al.first - 1, al.second);
        if(dir == NORTH)
            totalUtility += utility * 0.8;
        else
            totalUtility += utility * 0.1;

        if(utility > maxUtility) {
            maxUtility = utility;
            newDir = NORTH;
            allZeros = false;
        }
    }

    // Check EAST
    if(Grid::movable(al.first, al.second + 1) && prevDir != WEST) {
        utility = Grid::getUtility(al.first, al.second + 1);
        if(dir == EAST)
            totalUtility += utility * 0.8;
        else
            totalUtility += utility * 0.1;

        if(utility > maxUtility) {
            maxUtility = utility;
            newDir = EAST;
            allZeros = false;
        }
    }

    // Check SOUTH
    if(Grid::movable(al.first + 1, al.second) && prevDir != NORTH) {
        utility = Grid::getUtility(al.first + 1, al.second);
        if(dir == SOUTH)
            totalUtility += utility * 0.8;
        else
            totalUtility += utility * 0.1;
        
        if(utility > maxUtility) {
            maxUtility = utility;
            newDir = SOUTH;
            allZeros = false;
        }
    }

    // Check WEST
    if(Grid::movable(al.first, al.second - 1) && prevDir != EAST) {
        utility = Grid::getUtility(al.first, al.second - 1);
        if(dir == WEST)
            totalUtility += utility * 0.8;
        else
            totalUtility += utility * 0.1;

        if(utility > maxUtility) {
            maxUtility = utility;
            newDir = WEST;
            allZeros = false;
        }
    }

    // Neighbors are 0
    if(allZeros) {
        gc->policy.first = Direction((dir + 1) % 4);
    } else {
        gc->policy.first = newDir;

        // Generate policy if NONE
        if(grid[al.first][al.second].policy.first == NONE) {
            Grid::initPolicy(al.first, al.second);
        }
    }

    return totalUtility;
}

// Gets utitlity of GridCell
double Grid::getUtility(int i, int j) {
    GridCell gc = grid[i][j];
    return gc.reward + gc.policy.second;
}

// Move agent across grid. This is the exploration feature of the program.
// It adjusts the policy based on rewards and utility from exploring the grid.
void Grid::moveAgent() {
    pair<int, int> agentLoc = Grid::getAgentLocation();

    grid[agentLoc.first][agentLoc.second].policy.second = discount * Grid::updatePolicy(agentLoc.first, agentLoc.second);

    while(true) {
        Direction dir = NONE;
        if(iteration % 5 != 0) {
            // Random movement with high priority following policy.
            dir = Grid::moveStochastically();
        } else {
            // allow agent to explore areas not following policy.
            dir = Grid::explore();
        }
        pair<int, int> newAgentLoc = agentLoc;

        switch(dir) {
            case NORTH:
                newAgentLoc.first -= 1;
                break;
            case EAST:
                newAgentLoc.second += 1;
                break;
            case SOUTH:
                newAgentLoc.first += 1;
                break;
            case WEST:
                newAgentLoc.second -= 1;
                break;
            default:
                break;
        }

        if(Grid::inBound(newAgentLoc.first, newAgentLoc.second) &&
            !Grid::isObstacle(newAgentLoc.first, newAgentLoc.second)) {

            if(Grid::isTerminal(newAgentLoc.first, newAgentLoc.second)) {
                iteration += 1;
                agentLocation = Grid::getStartLocation();
            } else {
                agentLocation = newAgentLoc;
            }
    // cout << "Direction: " << toString(dir) << endl;

            prevDir = dir;
            break;
        } else {
            grid[agentLoc.first][agentLoc.second].policy.first =
                Direction((grid[agentLoc.first][agentLoc.second].policy.first + 1) % 4);
        }
    }
}

// Generates a Direction to move to based on current GridCell policy.
// Probability of intended policy is 0.8 while other left and right of
// direction is 0.1 each
Direction Grid::moveStochastically() {
    pair<int, int> agentLoc = Grid::getAgentLocation();
    Direction intendedDir = grid[agentLoc.first][agentLoc.second].policy.first;
    Direction resultDir = NONE;
    int r = rand() % 101;

    switch(intendedDir) {
        case NORTH:
            if(r < 10) {
                resultDir = WEST;
            } else if(r < 20) {
                resultDir = EAST;
            } else {
                resultDir = NORTH;
            }
            break;
        case EAST:
            if(r < 10) {
                resultDir = NORTH;
            } else if(r < 20) {
                resultDir = SOUTH;
            } else {
                resultDir = EAST;
            }
            break;
        case SOUTH:
            if(r < 10) {
                resultDir = EAST;
            } else if(r < 20) {
                resultDir = WEST;
            } else {
                resultDir = SOUTH;
            }
            break;
        case WEST:
            if(r < 10) {
                resultDir = SOUTH;
            } else if(r < 20) {
                resultDir = NORTH;
            } else {
                resultDir = WEST;
            }
            break;
        default:
            resultDir = NORTH;
            break;
    }

    return resultDir;
}

// Generates a Direction to move to based on current GridCell policy.
// Probability of intended policy is equal to other directions to allow
// exploration nature of program.
Direction Grid::explore() {
    pair<int, int> agentLoc = Grid::getAgentLocation();
    Direction intendedDir = grid[agentLoc.first][agentLoc.second].policy.first;
    Direction resultDir = NONE;
    int r = rand() % 101;

    switch(intendedDir) {
        case NORTH:
            if(r < 33) {
                resultDir = WEST;
            } else if(r < 66) {
                resultDir = EAST;
            } else {
                resultDir = NORTH;
            }
            break;
        case EAST:
            if(r < 33) {
                resultDir = NORTH;
            } else if(r < 66) {
                resultDir = SOUTH;
            } else {
                resultDir = EAST;
            }
            break;
        case SOUTH:
            if(r < 33) {
                resultDir = EAST;
            } else if(r < 66) {
                resultDir = WEST;
            } else {
                resultDir = SOUTH;
            }
            break;
        case WEST:
            if(r < 33) {
                resultDir = SOUTH;
            } else if(r < 66) {
                resultDir = NORTH;
            } else {
                resultDir = WEST;
            }
            break;
        default:
            resultDir = NORTH;
            break;
    }

    return resultDir;
}
    
// Accessors to the 2D grid
GridCell& Grid::operator[](const pair<int,int> & pos)
{
    return grid[pos.first][pos.second];
}
const GridCell& Grid::operator[](const pair<int,int> & pos) const
{
    return grid[pos.first][pos.second];
}
pair<int,int> Grid::getBounds() const
{
    return bounds;
}
int Grid::getRows() const
{
    return bounds.first;
}
int Grid::getCols() const
{
    return bounds.second;
}
pair<int,int> Grid::getStartLocation() const
{
    return startLocation;
}
pair<int,int> Grid::getAgentLocation() {
    return agentLocation;
}
    
///////////
// Function print
///////////
// In -> pair<int,int> agentPos -
//          position of agent provided. If indicies are negative or out
//          of bounds. This parameter is ignored.
// Out ->
//
// Prints each cell content with special information centered on its row in the following format:
//
// +------
// |value
// |start
// |policy
//
// value - displays the value of the current policy in the format [+/-]##.##
// start - displays 'S' for whether this cell is a start location. A 'P' is also appended
//          if the agentPos is located at that cell.
// policy - displays '^' '>' 'v' '<' if the best policy is NORTH, EAST, SOUTH, or WEST respectively.
//              If policy is set to NONE, it will display '?'
//
// Special cases:
// - If cell is an obstacle, the contents of the cell for each line contain "xxxx" instead
// - if cell is a terminal, the policy displayed for that cell will be '*' instead
//
// Once each cell is printed, it will append a final right and bottom edge
// to complete the ascii table.
void Grid::print(const pair<int,int> & agentPos) const
{
    const int RESERVE = 7;
    for (int i = 0; i < bounds.first; ++i)
    {
        vector<string> outputs(4);
            
        for (int j = 0; j < bounds.second; ++j)
        {
            vector<string> ret;
            if (i == agentPos.first && j == agentPos.second)
                ret = grid[i][j].print("P", RESERVE);
            else
                ret = grid[i][j].print(" ", RESERVE);
            outputs[0] += ret[0];
            outputs[1] += ret[1];
            outputs[2] += ret[2];
            outputs[3] += ret[3];
        }
        cout << outputs[0] << "+" << endl
        << outputs[1] << "|" << endl
        << outputs[2] << "|" << endl
        << outputs[3] << "|" << endl;
    }
    cout << left << setfill('-');
    for (int j = 0; j < bounds.second; ++j)
        cout << setw(RESERVE) << "+";
    cout << "+";
    cout << endl;
}
