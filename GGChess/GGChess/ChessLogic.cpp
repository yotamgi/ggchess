
#include "ChessLogic.h"

ChessLogic::ChessLogic(ChessBoard* board):
m_board(board) {}

ChessLogic::~ChessLogic() {}

void ChessLogic::init() {

	// set the board to its initial state
	m_board->init();
	m_board->setGameState(InitialChessState);
	m_ic.setState(InitialChessState);
}

void ChessLogic::draw(float timeDelta) const {
	m_board->draw(timeDelta);
}

bool ChessLogic::makeMove(ChessMove move, ChessColor player) {

	ChessPart state[8][8];
	m_board->fillGameState(state);

	// make sure that the state is OK:
	if (state[move.from_y][move.from_x].color != player) {
		return false;
	}

	// check if this move is legal
	std::vector<ChessMove> possibleMoves = m_ic.getAllMoves(player);

	std::vector<ChessMove>::iterator iter = possibleMoves.begin();
	for (;iter != possibleMoves.end(); iter++) {
		if (*iter == move)  {
			m_board->makeMove(move.from_y, move.from_x, move.to_y, move.to_x);
			m_ic.makeMove(move);
			return true;
		} 
	}
	return false;
}

void ChessLogic::fillGameState(ChessPart state[CHESS_DIMENTION_Y][CHESS_DIMENTION_X]) const
{
	m_board->fillGameState(state);
}