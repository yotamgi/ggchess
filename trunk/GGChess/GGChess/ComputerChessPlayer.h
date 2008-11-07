#ifndef  __COMPUTER_CHESS_PLAYER__
#define  __COMPUTER_CHESS_PLAYER__

#include <vector>
#include "Common/D3dTextConsole.h"
#include "ChessPlayer.h"
#include "IterableChess.h"
#include "ChessDefines.h"


struct playerPartInfo {
	playerPartInfo(){}
	playerPartInfo(ChessPart _part, float _importance) 
	{	part = _part; importance = _importance; alive = false; }

	void setPos(int _y, int _x) { y=_y; x=_x; alive = true; }

	ChessPart	part;
	bool alive;
	int y, x;
	float importance;
};

//////////////////////////////////////////////////////////////////////////
// SmartChessState class
//
class SmartChessState {
public:
	SmartChessState(ChessPart state[CHESS_DIMENTION_Y][CHESS_DIMENTION_X]);

	/***
	 * makes a move in the state.
	 * works in O(1).
	 */
	void makeMove(const ChessMove &m, bool record = true);

	/***
	 * comes back one move.
	 * operates in O(makeMove)
	 */
	void undoMove();

	/***
	 * returnes the part in pos y,x.
	 * works in O(1).
	 */
	const playerPartInfo& getPart(int y, int x) const { return *partsOnBoard[y][x]; }

	/***
	 * returns all the parts as a array of length 32.
	 * works in O(1).
	 */
	const playerPartInfo* const getParts() const {return parts;}

	/***
	 * returns the combined mark of the parts, according to their importance
	 * operates in O(1).
	 */
	float getPartsMark(ChessColor color) {
		if (color == B) return combinedBlackMark;
		else return combinedWhiteMark;
	}

	/**
	 * returns the asked playerPartInfo of the asked part.
	 * operates in O(1).
	 */
	const playerPartInfo& getKing(ChessColor color) {
		if (color == W) return parts[15];
		else return parts[31];
	}

	/***
	 * Verifies that the state is the same as the sent state.
	 * Used for debugging.
	 */
	bool verifyState(ChessPart state[CHESS_DIMENTION_Y][CHESS_DIMENTION_X]) const;

private:

	float combinedWhiteMark;
	float combinedBlackMark;

	struct undoableMove {
		ChessMove move;
		playerPartInfo* died;
	};

	std::vector<undoableMove> movesStack;

	playerPartInfo  parts[32];
	playerPartInfo* partsOnBoard[CHESS_DIMENTION_Y][CHESS_DIMENTION_X];

};

//////////////////////////////////////////////////////////////////////////
// ComputerChessPlayer class
//
class ComputerChessPlayer : public ChessPlayer {
public:
	ComputerChessPlayer() {}

	virtual ChessMove play(const ChessLogic& board);

	virtual void illigalMove();
private:

	ChessMove findBestMove(IterableChess& chess, SmartChessState& smartState);

	float markState(IterableChess& chess, SmartChessState& state) const;	
};


#endif // __COMPUTER_CHESS_PLAYER__

