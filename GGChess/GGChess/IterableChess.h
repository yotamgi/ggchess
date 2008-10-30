#ifndef __ITERABLE_CHESS_H__
#define __ITERABLE_CHESS_H__

#include <vector>
#include "ChessDefines.h"

class IterableChess {
public:

	void setState(const ChessPart state[8][8]);
	bool makeMove(const ChessMove& m);

	std::vector<ChessMove> getAllMoves(ChessColor player) const;

	std::vector<ChessMove> getPartMoves(int posx, int posy, 
										ChessColor player) const;

private:

	void pushPartMoves(int posx, int posy, 
						ChessColor player, std::vector<ChessMove>& moves) const;

	void pushPawnMoves(int posx, int posy,
						ChessColor player, std::vector<ChessMove>& moves) const;

	void pushBishopMoves(int posx, int posy,
						ChessColor player, std::vector<ChessMove>& moves) const;

	void pushRookMoves(int posx, int posy,
						ChessColor player, std::vector<ChessMove>& moves) const;

	void pushQueenMoves(int posx, int posy,
						ChessColor player, std::vector<ChessMove>& moves) const;

	void pushKingMoves(int posx, int posy,
						ChessColor player, std::vector<ChessMove>& moves) const;

	void pushKnightMoves(int posx, int posy,
						ChessColor player, std::vector<ChessMove>& moves) const;

	ChessPart m_state[CHESS_DIMENTION_Y][CHESS_DIMENTION_X];

};
#endif // __ITERABLE_CHESS_H__