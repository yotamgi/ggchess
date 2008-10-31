#ifndef __CHESS_LOGIC_H__
#define __CHESS_LOGIC_H__

#include "ChessBoard.h"
#include "ChessDefines.h"
#include "IterableChess.h"

class ChessLogic {
public:
	ChessLogic(ChessBoard* board);
	~ChessLogic();
	
	void init();
	void draw(float timeDelta) const;

	bool makeMove(ChessMove move, ChessColor player);

	void fillGameState(ChessPart state[CHESS_DIMENTION_Y][CHESS_DIMENTION_X]) const;

	void emphasys(int y, int x, bool e) const { m_board->emphasys(y, x, e); }

	const ChessBoard* getBoard() const { return m_board;}

private:
	
	ChessBoard*	m_board;
	IterableChess m_ic;

};

#endif // __CHESS_LOGIC_H__
