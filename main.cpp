#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <sstream>
#include <map>
#include <cstdlib>

//prints board to cout
std::vector<std::string> board;
std::vector<int> rows;
std::vector<int> cols;
std::vector<int> ships;
std::vector<int> amountinr;
std::vector<int> amountinc;

std::vector<std::vector<std::string> > allboards;
std::vector<std::vector<int> > allrows;
std::vector<std::vector<int> > allcols;
std::vector<std::vector<int> > allships;
std::vector<std::vector<int> > allamountinr;
std::vector<std::vector<int> > allamountinc;
std::vector<std::string> insertedShips;
std::vector<std::vector<std::string> > printedboards;
std::map<std::vector<std::string>, int > failedBoards;
std::map< std::pair<int,int>, char > constraints;

int skip;
std::vector<int> totalskip;
int currentShip;
bool stop;
int go;
bool unknown;

//prints the board to cout
void printBoard()
{
	std::cout << "Solution:" << std::endl;
	for(int x=0; x < insertedShips.size(); x++)
		std::cout<<insertedShips[x]<<std::endl;
	std::cout << '+';
	for(unsigned int x=0; x < cols.size(); x++)
		std::cout << '-';
	std::cout << '+' << std::endl;
	for(unsigned int x=0; x < board.size(); x++){
		std::cout << '|';
		for(unsigned int y=0; y < board[x].size(); y++){
			if(board[x][y] != '_')
				std::cout << board[x][y];
			else std::cout << ' ';}
			std::cout << '|' << allrows[0][x] << std::endl;
	}
	std::cout << '+';
	for(unsigned int x=0; x < cols.size(); x++)
		std::cout << '-';
	std::cout << '+' << std::endl << ' ';
	for(unsigned int x=0; x < cols.size(); x++)
		std::cout << allcols[0][x];
	std::cout<< std::endl << std::endl;
}
//checks if x,y should be water if so makes it into '_' otherwise increments
void fillInWater(int x, int y)
{	//turns sides of pieces into water
	if(y > 0 && board[x][y-1] != '.' && board[x][y-1] != '_' && board[x][y] == '.')
		{board[x][y] = '_'; amountinr[x]--; amountinc[y]--;}

	if(x > 0 && board[x-1][y] != '.' && board[x-1][y] != '_' && board[x][y] == '.')
		{board[x][y] = '_'; amountinr[x]--; amountinc[y]--;}

	if(y < cols.size()-1 && board[x][y+1] != '.' && board[x][y+1] != '_' && board[x][y] == '.')
		{board[x][y] = '_'; amountinr[x]--; amountinc[y]--;}

	if(x < rows.size()-2 && board[x+1][y] != '.' && board[x+1][y] != '_' && board[x][y] == '.')
		{board[x][y] = '_'; amountinr[x]--; amountinc[y]--;}

	//turns corners of pieces into water
	if(y > 0 && x > 0 && board[x-1][y-1] != '.' && board[x-1][y-1] != '_' && board[x][y] == '.')
		{board[x][y] = '_'; amountinr[x]--; amountinc[y]--;}

	if(x > 0 && y < cols.size()-1 && board[x-1][y+1] != '.' && board[x-1][y+1] != '_' && board[x][y] == '.')
		{board[x][y] = '_'; amountinr[x]--; amountinc[y]--;}

	if(y < cols.size()-1 && x < rows.size()-1 && board[x+1][y+1] != '.' && board[x+1][y+1] != '_' && board[x][y] == '.')
		{board[x][y] = '_'; amountinr[x]--; amountinc[y]--;}

	if(x < rows.size()-2 && y > 0 && board[x+1][y-1] != '.' && board[x+1][y-1] != '_' && board[x][y] == '.')
		{board[x][y] = '_'; amountinr[x]--; amountinc[y]--;}

	//turns full rows and cols into water
	if((rows[x] == 0 || cols[y] == 0) && board[x][y] == '.')
		{board[x][y] = '_'; amountinr[x]--; amountinc[y]--;}
	//recusive call
	if(y < cols.size())
		return fillInWater(x, y+1);
	else if(x < rows.size()-1)
		return fillInWater(x+1, 0);
	return;

}
//adds new boardstate
void updateBoard()
{
	allboards.push_back(board);
	allrows.push_back(rows);
	allcols.push_back(cols);
	allships.push_back(ships);
	allamountinr.push_back(amountinr);
	allamountinc.push_back(amountinc);
}
//adds one skip to current ship and sets ones after to 0
void addTotalSkip(int currentShip)
{
	totalskip[currentShip]++;
	for(int x=0; x< totalskip.size(); x++)
	{
		if(x<currentShip)
			totalskip[x]=0;
	}
}
//resets board to previous board states
void goBackOne()
{
	if(allboards.size() == 1)
	{
		stop=true;
		return;
	}

	allboards.pop_back();
	allrows.pop_back();
	allcols.pop_back();
	allships.pop_back();
	allamountinr.pop_back();
	allamountinc.pop_back();
	insertedShips.pop_back();

	board=allboards.back();
	rows=allrows.back();
	cols=allcols.back();
	ships=allships.back();
	amountinr=allamountinr.back();
	amountinc=allamountinc.back();
	//skips the ship that failed
	currentShip = ships.size()-1;
	addTotalSkip(currentShip);
	skip=totalskip[currentShip];
	fillInWater(0, 0);
}
//sets up ships for prining out
void addInsertedShip(int size, int r, int c, int type)
{
	std::stringstream ship;

	if(size == 7)
		ship << "tanker      ";
	if(size == 6)
		ship << "cargo       ";
	if(size == 5)
		ship << "carrier     ";
	if(size == 4)
		ship << "battleship  ";
	if(size == 3)
		ship << "cruiser     ";
	if(size == 2)
		ship << "destroyer   ";
	if(size == 1)
		ship << "submarine   ";
	
	ship << r << " " << c;
	if(size!=1){
		if(type == 0)
			ship << " horizontal";
		if(type == 1)
			ship << " vertical";
	}
	insertedShips.push_back(ship.str());
}
//inserts largest shipe at location x
void insertShip(int r, int c, int type, int size)
{
	if(type == 0)// if horizontal
	{
		if(size==-1)
			size=rows[r];
		addInsertedShip(size, r, c, type);//for printing ships
		//for checking if puzzle is done
		amountinr[r]-=size;
		if(rows[r]>0)//if row size is known
			rows[r]-=size;
		for(int x=0; x < size; x++)//adds ship to board
		{
			if(size==1)
				board[r][c+x] = 'o';
			else if(x==0)
				board[r][c+x] = '<';
			else if(x==size-1)
				board[r][c+x] = '>';
			else
				board[r][c+x] = 'x';
			cols[c+x]--;
		}
		ships.erase(ships.begin());//erases ship from ships left 
	}
	if(type == 1)//if vertical
	{
		if(size==-1)
			size=amountinc[c];
		//for printing ships
		addInsertedShip(size, r, c, type);
		//for checking if puzzle is done
		amountinc[c]-=size;
		if(cols[c]>0)
			cols[c]-=size;
		//adds ship to board
		for(int x=0; x < size; x++)
		{
			if(size==1)
				board[r+x][c] = 'o';
			else if(x==0)
				board[r+x][c] = '^';
			else if(x==size-1)
				board[r+x][c] = 'v';
			else
				board[r+x][c] = 'x';
			rows[r+x]--;
		}
		ships.erase(ships.begin());//removes ship from ships left
	}
	updateBoard();
}
//makes sure board is filled in and all ships have 
//been used and the constraints fit
bool ifCorrect()
{
	if(unknown)
	{
		for(int x=0; x < rows.size(); x++)
			if(rows[x]!=0 && rows[x]!=-1)
				return false;
		for(int x=0; x < cols.size(); x++)
			if(cols[x]!=0 && cols[x]!=-1)
				return false;
	}
	else{
		for(int x=0; x < rows.size(); x++)
			if(rows[x]!=0)
				return false;
		for(int x=0; x < cols.size(); x++)
			if(cols[x]!=0)
				return false;
	}
	if(constraints.size() > 0)
	{
		std::map< std::pair<int,int>, char >::const_iterator itr;
		for(itr = constraints.begin(); itr != constraints.end(); ++itr)
		{
			if(board[itr->first.first][itr->first.second] != itr->second)
				return false;
		}
	}
	if(insertedShips.size() != allships[0].size())
		return false;
	return true;
}
//insert ship then attempt to solve the rest of the puzzle. return 
//false if not possible and true if possible
bool ifPossible(int r, int c, int type, int size)
{
	if(failedBoards.find(board) != failedBoards.end())
		return false;

	//std::cout<< r << "," << c << std::endl;
	if(type == 0)// if horizontal
	{
		//std::cout<<r<<","<<c<<std::endl;
		if(size==-1)//if size unknown
			size=1;
		if(rows[r]==0)
			return false;
		if(rows[r]-size < 0 && rows[r]!=-1)//checks if it can fit on that row
			return false;
		for(int x=0; x < size; x++)
		{
			if(x+c >= cols.size())//makes sure theres enough space left in the rows
				return false;
			if(cols[x+c] - 1 < 0 )//checks if it can fit on that col
				return false;
			if(board[r][c+x] != '.' )//checks that a ship can go there
				return false;
		}
	}
	if(type == 1)//if vertical
	{
		if(size==-1)
			size=amountinc[c];
		if(cols[c]==0)
			return false;
		if(cols[c]-size < 0 && cols[c]!=-1)//checks if it can fit on that col
			return false;
		for(int x=0; x < size; x++)
		{
			if(x+r >= rows.size())//makes sure theres enough space left in the cols
				return false;
			if(rows[x+r]-1 < 0)//checks if it can fit on that row
				return false;
			if(board[r+x][c] != '.' )//checks if a ship can go there
				return false;
		}
	}
	//checks if the piece has been tried
	if(skip > 0 || totalskip[currentShip] < skip)
	{
		skip--;
		return false;
	}
	return true;
}
//tries to insert the biggest ship into somewhere it fits
//after checking if it is possible with ifPossible
//if its not possible then it adds it to failed boards and 
//calls goBackOne which removes the last piece
void addbiggestShip()
{
	go++;
	if(ships.size()>0){
		for(int x=0; x< rows.size(); x++)
		{
			if(rows[x] >= ships[0] || rows[x] == -1)
				for(int y=0; y< cols.size(); y++)
				{
					if(ifPossible(x, y, 0, ships[0])){
						insertShip(x, y, 0, ships[0]);
						return;}
				}
		}
		for(int x=0; x< cols.size(); x++)
		{
			if(cols[x] >= ships[0] || cols[x] == -1)
				for(int y=0; y < rows.size(); y++)
				{	
					if(ifPossible(y, x, 1, ships[0])){
						insertShip(y, x, 1, ships[0]);
						return;}
				}
		}
	}
	failedBoards[board];
	goBackOne();
	//if no where found get info of previous ship
}
//checks whether the board has been printed before
bool notPrinted(int amount)
{
	if(amount == 0)
		return true;
	//goes through boards that have been printed
	for(int x=0; x<printedboards.size(); x++)
	{
		if(printedboards[x]==board)
			return false;
	}
	return true;
}
//attempts to solve the puzzle
void solve(int amount)
{
	//checks if possible board and hasn't been printed
	//if both true then it prints the board and if 
	//all solutions it attempts to find another solution
	if(ifCorrect() && notPrinted(amount))
	{
		printBoard();

		if(amount == 0)//if printing once
			return;
		//if printing all solutions
		printedboards.push_back(board);
		goBackOne();
		amount++;
	}
	if(stop || go > 64000)//if all ships have been put in
	{
		if(amount==0)
			std::cout << "No solutions" << std::endl;
		else 
			std::cout << "Found " << amount-1 << " solution(s)" << std::endl;
		return;
	}
	fillInWater(0, 0);//fills in water spaces
	addbiggestShip();//attempts to insert biggest ship
	//printBoard();
	solve(amount);//recusive call
}
//reads in information in text file then calls solve
int main(int argc, char* argv[])
{
	unknown=false;//if unknown sides
	std::ifstream in_str(argv[1]);
	int row = 0;
	int col = 0;
	int val=0;
	std::string temp;
	in_str >> temp >> row >> col >> temp; //gets size of board
	for(int x=0; x < row; ++x)//reads in board and row values
	{
		board.push_back(std::string(col,'.'));
		in_str >> temp;
		if(temp != "?"){//if not unknown
			rows.push_back(atoi(temp.c_str()));
		}
		else {rows.push_back(-1);
			unknown=true;}

		amountinr.push_back(col);
	}
	in_str >> temp;
	for(int x=0; x < col; ++x)//reads in col values
	{
		in_str >> temp;
		if(temp != "?"){//if not unknown
			cols.push_back(atoi(temp.c_str()));
		}
		else {cols.push_back(-1);
			unknown=true;}
		amountinc.push_back(row);
	}
	int r1=0;
	int c1=0;
	char type;
	while(in_str >> temp)//reads in ships
	{
		if(temp == "tanker")
			ships.push_back(7);
		if(temp == "cargo")
			ships.push_back(6);
		if(temp == "carrier")
			ships.push_back(5);
		if(temp == "battleship")
			ships.push_back(4);
		if(temp == "cruiser")
			ships.push_back(3);
		if(temp == "destroyer")
			ships.push_back(2);
		if(temp == "submarine")
			ships.push_back(1);
		if(temp == "unknown")
			ships.push_back(-1);
		if(temp == "constraint")//reads in constraints
		{
			in_str >> r1 >> c1 >> type;
			constraints[std::pair<int,int>(r1, c1)] = type;
		}
	}
	for(int x=0; x < ships.size(); x++)
	{
		totalskip.push_back(0);
	}
	//sorts ships in decending order
	std::sort(ships.begin(), ships.end(), std::greater<int>());
	updateBoard();//remembers possible ship combinations
	skip=0;//used if puzzle fails
	currentShip=0;
	stop=false;
	go=0;
	if(argc == 2)
		solve(0);
	if(argc == 3)
		solve(1);
	return 0;
}