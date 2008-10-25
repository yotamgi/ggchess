#ifndef __CHESSS_DEFINES_H__
#define __CHESSS_DEFINES_H__


const int CHESS_DIMENTION_X = 8;
const int CHESS_DIMENTION_Y = 8;

// global chess constants
const int PAWN_NUMBER	=	8;
const int BISHOP_NUMBER =	2;
const int KNIGHT_NUMBER =	2;
const int ROOK_NUMBER	=	2;
const int KING_NUMBER	=	1;
const int QUEEN_NUMBER	=	1;
const int PARTS_TOTAL	=	26; // per oponent

struct ChessMove {
	ChessMove(){}
	ChessMove(int _from_y, int _from_x, int _to_y, int _to_x) {
		from_x = _from_x;	from_y = _from_y; 
		to_x = _to_x;		to_y = _to_y;
	}
	bool operator == (ChessMove r) {
		return ((r.from_x == from_x) && (r.from_y == from_y) && 
				(r.to_x == to_x) && (r.to_y == to_y));
	}
	int from_x;
	int from_y;
	int to_x;
	int to_y;
};

enum ChessType {
	EMPTY, PAWN, BISHOP, KNIGHT, ROOK, KING, QUEEN 
};
enum ChessColor {  B, W  };

inline ChessColor OtherColor(ChessColor c) {
	if (c == W) return B;
	else return W;
}

struct ChessPart {
	ChessPart() {}
	ChessPart(ChessColor c, ChessType t) { color = c; type = t; }

	bool operator == (const ChessPart right) const {
		return ((right.color == color) && (right.type == type));
	}

	ChessColor color; // irrelevant when the type is EMPTY.
	ChessType type;
};


const ChessPart InitialChessState[8][8] = {
{   ChessPart(W,ROOK),	ChessPart(W,KNIGHT),ChessPart(W,BISHOP),ChessPart(W,KING),	ChessPart(W,QUEEN),	ChessPart(W,BISHOP),ChessPart(W,KNIGHT),ChessPart(W,ROOK) },
{	ChessPart(W,PAWN),	ChessPart(W,PAWN),	ChessPart(W,PAWN),	ChessPart(W,PAWN),	ChessPart(W,PAWN),	ChessPart(W,PAWN),	ChessPart(W,PAWN),	ChessPart(W,PAWN) },
{	ChessPart(W,EMPTY),	ChessPart(W,EMPTY),	ChessPart(W,EMPTY),	ChessPart(W,EMPTY),	ChessPart(W,EMPTY),	ChessPart(W,EMPTY),	ChessPart(W,EMPTY),	ChessPart(W,EMPTY)},
{	ChessPart(W,EMPTY),	ChessPart(W,EMPTY),	ChessPart(W,EMPTY),	ChessPart(W,EMPTY),	ChessPart(W,EMPTY),	ChessPart(W,EMPTY),	ChessPart(W,EMPTY),	ChessPart(W,EMPTY)},
{	ChessPart(W,EMPTY),	ChessPart(W,EMPTY),	ChessPart(W,EMPTY),	ChessPart(W,EMPTY),	ChessPart(W,EMPTY),	ChessPart(W,EMPTY),	ChessPart(W,EMPTY),	ChessPart(W,EMPTY)},
{	ChessPart(W,EMPTY),	ChessPart(W,EMPTY),	ChessPart(W,EMPTY),	ChessPart(W,EMPTY),	ChessPart(W,EMPTY),	ChessPart(W,EMPTY),	ChessPart(W,EMPTY),	ChessPart(W,EMPTY)},
{	ChessPart(B,PAWN),	ChessPart(B,PAWN),	ChessPart(B,PAWN),	ChessPart(B,PAWN),	ChessPart(B,PAWN),	ChessPart(B,PAWN),	ChessPart(B,PAWN),	ChessPart(B,PAWN) },
{   ChessPart(B,ROOK),	ChessPart(B,KNIGHT),ChessPart(B,BISHOP),ChessPart(B,KING),	ChessPart(B,QUEEN),	ChessPart(B,BISHOP),ChessPart(B,KNIGHT),ChessPart(B,ROOK) },
};


#endif //__CHESSS_DEFINES_H__