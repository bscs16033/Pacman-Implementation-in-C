#include <iostream>
#include <queue>
#include <string>
#include <conio.h>
#include <windows.h>

using namespace std;


// m means global variables
// the total characters in a column are 30, but VS reserves a character, maybe for \0 (not sure though)
/*
const int mBoardRows = 4;
const int mBoardCols = 6;

char mBoardDesign[mBoardRows][mBoardCols] = {
"#####",
"#   #",
"#   #",
"#####"
};
*/

/*
const int mBoardRows = 18;
const int mBoardCols = 32;

char mBoardDesign[mBoardRows][mBoardCols] = {
"+#############################+",
"|                             |",
"|                             |",
"|--  --------  ---   ---------|",
"|   |                         |",
"| | |--  |  |           |     |",
"| |      |  | |---  |   |  |  |",
"| |  ----|  | |      -- |     |",
"| |           |---  |      |  |",
"| |-----  --         --       |",
"|           ------  -----  ---|",
"|                             |",
"|- --- ----      ---   -------|",
"|                             |",
"|                             |",
"|                             |",
"|                             |",
"+#############################+"
};

*/

const int mBoardRows = 15;
const int mBoardCols = 31;

char mBoardDesign[mBoardRows][mBoardCols] = {
	"##############################",
	"#@                           #",
	"#==   ====   |    ====    ===#",
	"#   |   |    |               #",
	"#   |   |    | ===           #",
	"#   |   |    |      =====    #",
	"#   |                        #",
	"#      ================      #",
	"#                |           #",
	"#   |       |   |        |   #",
	"#   |       |   |   ==== |   #",
	"#   |            |           #",
	"#      ========    ==   ==   #",
	"#                           &#",
	"##############################" };

// These are the positions where a movement is possible
int mBoardMinRow = 1;
int mBoardMaxRow = mBoardRows - 2;
// top row of the board is reserved for wall of #
int mBoardMinCol = 1;
// -1 because indexing starts from 0, and -1 because of # character of wall, -1 because of '\0'
int mBoardMaxCol = mBoardCols - 3;
//const char mPlayerCharacter = '@';
//const char mGhostCharacter = '&';

//const int mTotalBoxes = mBoardRows*mBoardCols;

struct mNode {
	int row;
	int col;
	int parentRow;
	int parentCol;
	bool isValid;
};

mNode mPlayerNode = { mBoardMinRow, mBoardMinCol, -1, -1, true };
mNode mGhostNode = { mBoardMaxRow, mBoardMaxCol, -1, -1, true };

bool mBoardVisitedBoxes[mBoardRows][mBoardCols] = { false };

void printBoardDesign(){
	for (int a = 0; a < mBoardRows; a++){
		for (int b = 0; b < mBoardCols; b++){
			cout << mBoardDesign[a][b];
		}
		cout << endl;
	}
}


// BFS() and Astar() helper methods
mNode getLeftNode(mNode current){
	if ((current.col - 1) < mBoardMinCol){
		// Do nothing
		current.isValid = false;
	}
	else{
		current.col--;
		if (((mBoardDesign[current.row][current.col] == ' ') ||
			(mBoardDesign[current.row][current.col] == '*') ||
			(mBoardDesign[current.row][current.col] == '@')) &&
			(!mBoardVisitedBoxes[current.row][current.col])){
			mNode leftNode = current;
			leftNode.parentRow = current.row;
			leftNode.parentCol = current.col;
			return leftNode;
		}
		else{
			current.isValid = false;
		}
	}
	return current;
}

mNode getRightNode(mNode current){
	if ((current.col + 1) > mBoardMaxCol){
		// Do nothing
		current.isValid = false;
	}
	else{
		current.col++;
		if (((mBoardDesign[current.row][current.col] == ' ') ||
			(mBoardDesign[current.row][current.col] == '*') ||
			(mBoardDesign[current.row][current.col] == '@')) &&
			(!mBoardVisitedBoxes[current.row][current.col])){
			mNode rightNode = current;
			rightNode.parentRow = current.row;
			rightNode.parentCol = current.col;
			return rightNode;
		}
		else{
			current.isValid = false;
		}
	}
	return current;
}

mNode getUpNode(mNode current){
	if ((current.row - 1) < mBoardMinRow){
		// Do nothing
		current.isValid = false;
	}
	else{
		current.row--;
		if (((mBoardDesign[current.row][current.col] == ' ') ||
			(mBoardDesign[current.row][current.col] == '*') ||
			(mBoardDesign[current.row][current.col] == '@')) &&
			(!mBoardVisitedBoxes[current.row][current.col])){
			mNode upNode = current;
			upNode.parentRow = current.row;
			upNode.parentCol = current.col;
			return upNode;
		}
		else{
			current.isValid = false;
		}
	}
	return current;
}

mNode getDownNode(mNode current){
	if ((current.row + 1) > mBoardMaxRow){
		// Do nothing
		current.isValid = false;
	}
	else{
		current.row++;
		if (((mBoardDesign[current.row][current.col] == ' ') ||
			(mBoardDesign[current.row][current.col] == '*') ||
			(mBoardDesign[current.row][current.col] == '@')) &&
			(!mBoardVisitedBoxes[current.row][current.col])){
			mNode downNode = current;
			downNode.parentRow = current.row;
			downNode.parentCol = current.col;
			return downNode;
		}
		else{
			current.isValid = false;
		}
	}
	return current;
}

vector<mNode> getPath(mNode current, vector<mNode> parent){
	vector<mNode> path;
	path.insert(path.begin(), current);

	//Preincrement is more efficient than postincrement, because for postincrement the object must
	//increment itself and then return a temporary containing its old value.
	//Note that this is true even for builtins like int.
	// rbegin() returns a reverse iterator that moves from end to begin

	for (auto it = parent.rbegin(); it != parent.rend(); ++it){
		// it is a pointer
		//	cout << (*it).currentBoxNum << endl;

		if (((*it).row == current.parentRow) && ((*it).col == current.parentCol)){
			path.insert(path.begin(), (*it));
			current = (*it);
		}
	}
	return path;
}

// returns a path in the form "ULRDUD", (Up, Left, Right, Down)
vector<mNode> BFS(mNode ghostNode, mNode playerNode){
	// Make all the visited nodes false
	for (int a = 0; a < mBoardRows; a++){
		for (int b = 0; b < mBoardCols; b++){
			mBoardVisitedBoxes[a][b] = false;
		}
	}


	// the tree would have (mBoardRows + mBoardCols) levels and each node 
	// can have at max 4 children (Right(+0, +1), Left(+0, -1), Down(+1, +0), Up(-1, +0))

	queue<mNode> bfsQueue;
	vector<mNode> parent;

	//mNode current = ghostPos;

	bfsQueue.push(ghostNode);
	mBoardVisitedBoxes[ghostNode.row][ghostNode.col] = true;

	mNode current;
	//	int count = 0;
	//while (!bfsQueue.empty()){
	while (!bfsQueue.empty()){
		//		cout << count << endl;
		//	cout << "Queue Size: " << bfsQueue.size() << endl;

		// these lines have to be at the end of the while loop because at every iteration we are checking if the 
		// queue is empty. And in the case where there is only one element in the queue which has no parents, and
		// if we store the last element in the currentNode and then dequeu it, we'll still have a node to explore
		// but queue would be empty and the last iteration would not run
		current = bfsQueue.front();
		bfsQueue.pop();

		if ((current.row == playerNode.row) && (current.col == playerNode.col)){
			//cout << "breaking from loop" << endl;
			break;
		}


		parent.push_back(current);

		// 4 child nodes
		mNode childNode[4] = { getLeftNode(current), getRightNode(current), getUpNode(current), getDownNode(current) };
		for (int a = 0; a < 4; a++){
			// if currentBoxNum == -1, the node is not valid, like there's a wall there
			if (childNode[a].isValid){
				// it is alrigth to mark the node to be visited when it is pushed in the queue
				// this is because the first node that will be discovered is sure to have the shortest path
				// and if there's another path to this node that if of same length as the shortest path, then 
				// no need to explore it, both will have same path cost and end in the same node
				mBoardVisitedBoxes[childNode[a].row][childNode[a].col] = true;
				childNode[a].parentRow = current.row;
				childNode[a].parentCol = current.col;
				bfsQueue.push(childNode[a]);
			}
		}
	}

	//cout << "out of the loop" << endl;

	return	getPath(current, parent);
}

vector<mNode> Astar(mNode ghostNode, mNode playerNode){
	// Make all the visited nodes false
	for (int a = 0; a < mBoardRows; a++){
		for (int b = 0; b < mBoardCols; b++){
			mBoardVisitedBoxes[a][b] = false;
		}
	}


	// the tree would have (mBoardRows + mBoardCols) levels and each node 
	// can have at max 4 children (Right(+0, +1), Left(+0, -1), Down(+1, +0), Up(-1, +0))

	vector<mNode> priorityQueue;
	vector<mNode> parent;
	vector<int> manhattanDist;

	//mNode current = ghostPos;

	// initialiy the queue is empty, so pushing back is the first element
	priorityQueue.push_back(ghostNode);
	manhattanDist.push_back(0);
	mBoardVisitedBoxes[ghostNode.row][ghostNode.col] = true;

	mNode current;
	int currentManhattanDist;
	//	int count = 0;
	while (!priorityQueue.empty()){
		//		cout << count << endl;
		//	cout << "Queue Size: " << bfsQueue.size() << endl;

		current = priorityQueue.front();
		currentManhattanDist = manhattanDist.front();

		// erase an element from the beginning of the vector
		manhattanDist.erase(manhattanDist.begin());
		priorityQueue.erase(priorityQueue.begin());

		if ((current.row == playerNode.row) && (current.col == playerNode.col)){
			break;
		}


		parent.push_back(current);

		// 4 child nodes
		mNode childNode[4] = { getLeftNode(current), getRightNode(current), getUpNode(current), getDownNode(current) };

		for (int a = 0; a < 4; a++){
			// if currentBoxNum == -1, the node is not valid, like there's a wall there
			if (childNode[a].isValid){
				// it is alrigth to mark the node to be visited when it is pushed in the queue
				// this is because the first node that will be discovered is sure to have the shortest path
				// and if there's another path to this node that if of same length as the shortest path, then 
				// no need to explore it, both will have same path cost and end in the same node
				mBoardVisitedBoxes[childNode[a].row][childNode[a].col] = true;
				childNode[a].parentRow = current.row;
				childNode[a].parentCol = current.col;

				// since the path cost g in this case is 1 or constant
				// f = g + h = 0 + h = h
				int heur = abs(childNode[a].row - playerNode.row) + abs(childNode[a].col - playerNode.col);

				auto itManhat = manhattanDist.begin();
				bool shouldAppend = true;
				for (auto itNode = priorityQueue.begin(); itNode != priorityQueue.end() && itManhat != manhattanDist.end(); ++itNode, ++itManhat){
					if ((*itManhat) >= heur){

						manhattanDist.emplace(itManhat, heur);
						priorityQueue.emplace(itNode, childNode[a]);
						shouldAppend = false;
						break;
					}
				}
				// The element was not pushed in the vector in the above loop because it had the greatest value
				if (shouldAppend){
					manhattanDist.push_back(heur);
					priorityQueue.push_back(childNode[a]);
				}
			}
		}
	}

	return	getPath(current, parent);
}

void gotorc(int r, int c)
{
	static HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coor = { c, r };
	SetConsoleCursorPosition(h, coor);
}

void gotoxy(int x, int y)
{
	static HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coor = { x, y };
	SetConsoleCursorPosition(h, coor);
}

void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}


int playGame(int speed){
	system("cls");
	printBoardDesign();

	//bool isCaught = false;

	int x = 1;
	int y = 1;

	//gotoxy(x, y);

	//int read;
	//int read2;

	int points = 0;

	while (true){
		bool shouldFindPath = false;
		//		while (!_kbhit()){}
		// Make Ghost Follow The Player
		vector<mNode> returnedPath = Astar(mGhostNode, mPlayerNode);

		//gotoxy(ghostNode.col, ghostNode.row);
		auto previousIterator = returnedPath.begin();
		for (auto it = returnedPath.begin(); it != returnedPath.end(); ++it){
			shouldFindPath = false;
			if (GetAsyncKeyState(VK_RIGHT))
			{
				char move = mBoardDesign[y][x + 1];
				if (move == ' ' || move == '*'){
					gotoxy(x, y);
					cout << ' ';
					mBoardDesign[y][x] = ' ';

					x = x + 1;
					mPlayerNode.row = y;
					mPlayerNode.col = x;
					gotoxy(x, y);
					cout << '@';
					mBoardDesign[y][x] = '@';

					if (move == '*'){
						points++;
						gotoxy(mBoardCols + 10, 0);
						cout << points;
					}

				}
				shouldFindPath = true;
			}
			else if (GetAsyncKeyState(VK_UP)){
				// UP ARROW key pressed

				char move = mBoardDesign[y - 1][x];
				if (move == ' ' || move == '*'){
					gotoxy(x, y);
					cout << ' ';
					mBoardDesign[y][x] = ' ';

					y = y - 1;
					mPlayerNode.row = y;
					mPlayerNode.col = x;
					gotoxy(x, y);
					cout << '@';
					mBoardDesign[y][x] = '@';

					if (move == '*'){
						points++;
						gotoxy(mBoardCols + 10, 0);
						cout << points;
					}
				}
				shouldFindPath = true;
			}
			else if (GetAsyncKeyState(VK_DOWN)){
				// DOWN ARROW key pressed

				char move = mBoardDesign[y + 1][x];
				if (move == ' ' || move == '*'){
					gotoxy(x, y);
					cout << ' ';
					mBoardDesign[y][x] = ' ';

					y = y + 1;
					mPlayerNode.row = y;
					mPlayerNode.col = x;
					gotoxy(x, y);
					cout << '@';
					mBoardDesign[y][x] = '@';

					if (move == '*'){
						points++;
						gotoxy(mBoardCols + 10, 0);
						cout << points;
					}
				}
				shouldFindPath = true;
			}
			else if (GetAsyncKeyState(VK_LEFT)){
				// LEFT ARROW key pressed

				char move = mBoardDesign[y][x - 1];
				if (move == ' ' || move == '*'){
					gotoxy(x, y);
					cout << ' ';
					mBoardDesign[y][x] = ' ';

					x = x - 1;
					mPlayerNode.row = y;
					mPlayerNode.col = x;
					gotoxy(x, y);
					cout << '@';
					mBoardDesign[y][x] = '@';

					if (move == '*'){
						points++;
						gotoxy(mBoardCols + 10, 0);
						cout << points;
					}
				}
				shouldFindPath = true;
			}

			char move = mBoardDesign[(*it).row][(*it).col];
			if (move == '@'){
				// isCaught = true;
				// return score here
				return points;
			}
			else{
				gotorc((*previousIterator).row, (*previousIterator).col);
				cout << '*';
				mBoardDesign[(*previousIterator).row][(*previousIterator).col] = '*';

				mGhostNode.row = (*it).row;
				mGhostNode.col = (*it).col;
				gotorc((*it).row, (*it).col);
				cout << '&';
				mBoardDesign[(*it).row][(*it).col] = '&';
			}

			// an arrow key was pressed, means player 1 moved
			if (shouldFindPath){
				// if you do this when the key is pressed, the player would have moved his avatar but the ghost would lose his move
				// because when a new path is computer both 'it' and 'previousIterator' would have same value and the cursor would
				// both of them on the same spot, and it would seem like the ghost is not moving
				returnedPath.clear();
				returnedPath = Astar(mGhostNode, mPlayerNode);
				it = returnedPath.begin();
			}

			Sleep(speed);

			previousIterator = it;
		}
	}
}

int main(){
	printBoardDesign();


	//mNode playerNode = { mBoardMinRow, mBoardMinCol, -1, -1, true };
	//mNode ghostNode = { mBoardMaxRow, mBoardMaxCol, -1, -1, true };

	/*
	vector<mNode> returnedPath = BFS(ghostNode, playerNode);

	for (auto it = returnedPath.begin(); it != returnedPath.end(); ++it){
	// it is a pointer
	cout << "(" << (*it).row << ", " << (*it).col << ")->";
	}
	cout << endl;
	*/

	// Easy = 400 
	// Normal = 200
	// Hard = 100;

	int speed = 200;

	ShowConsoleCursor(false);
	int points = playGame(speed);

	system("cls");
	cout << "Your Points are: " << points << endl;

	system("pause");
	return 0;
}


