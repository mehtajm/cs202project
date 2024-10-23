#include <vector>
#include <iostream>
#include "checkers.h"

using namespace std;

Checkers::Checkers() {
	Board.resize(8);
	for (int rows = 0; rows < 8; rows++) {
		Board[rows].resize(8, ' '); // used to create an 8x8 board
	}
	for (int i = 0; i < 3; i++) { 
		for (int j = 0; j < 8; j++) {
			Board[i][(j + i) % 8] = 'b';
			Board[7 - i][(j++ + i + 1) % 8] = 'r';
		}
	} // places red and black on the board and staggers them accordingly.
}
Checkers::~Checkers() {
}
void Checkers::printBoard() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			cout << "|" << Board[i][j];
		}
		cout << "|" << endl;
	}
}

//**need to add logic for kings eventually**
//pieceToMove[0] is the x position of the piece the player wants to move. pieceToMove[1] is the corresponding y position. 
//whereToMove[0] is the x position of where the player wants to move their piece. whereToMove[1] is the corresponding y position
bool Checkers::isValidMove(int pieceToMove[], int whereToMove[], char redOrBlack) {
	//checks to see if pieceToMove and whereToMove are within the board dimensions
	if(!(pieceToMove[0] < 8) || !(pieceToMove[1] < 8) || !(whereToMove[0] < 8) || !(whereToMove[1] < 8)) return false;

	//check to see if there is the piece at pieceToMove location
	if(Board[pieceToMove[0]][pieceToMove[1]] != redOrBlack) return false;

	//checks to see that the black is moving down the board and red is moving up the board
	if(redOrBlack == 'b') {
		if(pieceToMove[0] > whereToMove[0]) return false;
	}
	else if (redOrBlack == 'r') {
		if(pieceToMove[0] < whereToMove[0]) return false;
	}

	//checks to see if there is a piece where the player wants to move
	if(Board[whereToMove[0]][whereToMove[1]] != ' ') return false;

	//the move is valid if the the piece is moved one space diagonally ***change?***
	if(redOrBlack == 'b') {
		//explanation fo Hein. the line below says the following: for black moving one space diagonally, the target row should be one more than the piece row and the target column should be either one more or one less than the piece row. look at a board to see the logic here
		if(whereToMove[0] == (pieceToMove[0] + 1) && ((whereToMove[1] - 1) == pieceToMove[1] || (whereToMove[1] + 1) == pieceToMove[1])) return true;
	}
	else if (redOrBlack == 'r') {
		if(whereToMove[0] == (pieceToMove[0] - 1) && ((whereToMove[1] - 1) == pieceToMove[1] || (whereToMove[1] + 1) == pieceToMove[1])) return true;
	}

	//if the player wants to take a piece aka jump over a piece
	if(redOrBlack == 'b') {
		//explanation for Hein. for black moving two diagonally to take a piece: the target row should be 2 more than the piece row and target column should be two more than the piece column and there should be a red piece one diagonally
		if(whereToMove[0] == (pieceToMove[0] + 2) && whereToMove[1] == (pieceToMove[1] + 2) && Board[pieceToMove[0] + 1][pieceToMove[1] + 1] == 'r') return true;
		//explanation for Hein. the other case is when the target column is two less than the piece column. This is also valid
		if(whereToMove[0] == (pieceToMove[0] + 2) && whereToMove[1] == (pieceToMove[1] - 2) && Board[pieceToMove[0] + 1][pieceToMove[1] - 1] == 'r') return true;
	}
	else if (redOrBlack == 'r') {
		if(whereToMove[0] == (pieceToMove[0] - 2) && whereToMove[1] == (pieceToMove[1] + 2) && Board[pieceToMove[0] - 1][pieceToMove[1] + 1] == 'b') return true;
		if(whereToMove[0] == (pieceToMove[0] - 2) && whereToMove[1] == (pieceToMove[1] - 2) && Board[pieceToMove[0] - 1][pieceToMove[1] - 1] == 'b') return true;
	}

	return false;

}

void Checkers::play() {
	int turn = 0;

	while (true) {
		char player;
		int XPieceToMove, YPieceToMove, XWhereToMove, YWhereToMove; 
		printBoard();
		
		if (turn % 2 == 0) { 
			cout << "\nBlack's "; 
			player = 'b';
		}
		else {
			cout << "\nRed's ";
			player = 'r';
		}
		cout << "turn. " << endl;

		cout << "Enter the checker you want to move in the format \"X Y\": ";
		cin >> XPieceToMove >> YPieceToMove;

		//checks to see if in bounds and if there is a piece at the position the player has entered
		while (XPieceToMove < 0 || YPieceToMove < 0 || XPieceToMove > 7 || YPieceToMove > 7 || Board[XPieceToMove][YPieceToMove] != player) {
			cout << "Invalid. Please try again." << endl;
			cout << "Enter the checker you want to move in the format \"X Y\": ";
			cin >> XPieceToMove >> YPieceToMove;
		} 

		cout << "Enter where you want to move your piece in the format \"X Y\": ";
		cin >> XWhereToMove >> YWhereToMove;

		int piecePosition[2] = {XPieceToMove, YPieceToMove};
		int movePosition[2] = {XWhereToMove, YWhereToMove};
		while(!isValidMove(piecePosition, movePosition, player)) {
			cout << "Invalid move. Please try again." << endl; //should we change this error message to something less broad?
			cout << "Enter where you want to move your piece in the format \"X Y\": ";
			cin >> movePosition[0] >> movePosition[1];
		}

		//implementing the changes to the board after the move is confirmed to be valid
		if(player == 'b') {
			Board[XPieceToMove][YPieceToMove] = ' ';
			Board[movePosition[0]][movePosition[1]] = 'b';

			//if they want to take a piece we also need to remove that piece
			if(XPieceToMove + 2 == movePosition[0]) {
				if(YPieceToMove + 2 == movePosition[1]) Board[XPieceToMove + 1][YPieceToMove + 1] = ' ';
				else if(YPieceToMove - 2 == movePosition[1]) Board[XPieceToMove + 1][YPieceToMove - 1] = ' ';
			}
		}
		else if(player == 'r') {
			Board[XPieceToMove][YPieceToMove] = ' ';
			Board[movePosition[0]][movePosition[1]] = 'r';

			//if they want to take a piece we also need to remove that piece
			if(XPieceToMove - 2 == movePosition[0]) {
				if(YPieceToMove + 2 == movePosition[1]) Board[XPieceToMove - 1][YPieceToMove + 1] = ' ';
				else if(YPieceToMove - 2 == movePosition[1]) Board[XPieceToMove - 1][YPieceToMove - 1] = ' ';
			}
		}

		
		turn++;
	}

}
int main() {

	Checkers check;
	check.play();
	
}
