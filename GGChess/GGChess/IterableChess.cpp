
#include "IterableChess.h"

std::vector<ChessMove> IterableChess::getAllMoves(ChessColor player) const
{
	std::vector<ChessMove> ans;

	for (int x=0; x<CHESS_DIMENTION_X; x++) {
		for (int y=0; y<CHESS_DIMENTION_Y; y++) {
			pushPartMoves(y, x, player, ans);
		}
	}
	return ans;	
}

std::vector<ChessMove> IterableChess::getPartMoves(int y, int x, 
										ChessColor player) const 
{
	std::vector<ChessMove> moves;
	pushPartMoves(y, x, player, moves);
	return moves;
}

void IterableChess::pushPartMoves(int y, int x, 
						ChessColor player, std::vector<ChessMove>& moves) const
{

	if (m_state[y][x] == ChessPart(player,PAWN)) {
		pushPawnMoves(y, x, player, moves); 
	} else if (m_state[y][x] == ChessPart(player,BISHOP)) {
		pushBishopMoves(y, x, player, moves); 
	} else if (m_state[y][x] == ChessPart(player,ROOK)) {
		pushRookMoves(y, x, player, moves); 
	} else if (m_state[y][x] == ChessPart(player,QUEEN)) {
		pushQueenMoves(y, x, player, moves); 
	} else if (m_state[y][x] == ChessPart(player,KING)) {
		pushKingMoves(y, x, player, moves); 
	} else if (m_state[y][x] == ChessPart(player,KNIGHT)) {
		pushKnightMoves(y, x, player, moves); 
	}
}

void IterableChess::setState(const ChessPart state[CHESS_DIMENTION_Y][CHESS_DIMENTION_X])
{
	for (int y=0; y<CHESS_DIMENTION_Y; y++) {
		for (int x=0; x<CHESS_DIMENTION_X; x++) {
			m_state[y][x] = state[y][x];
		}
	}
}

bool IterableChess::makeMove(const ChessMove &m, bool record) {
	
	if (m_state[m.from_y][m.from_x].type != EMPTY) {

		// record the move
		if (record) {
			UndoableMove undoable;
			undoable.move = m;
			undoable.died = ChessPart(W, EMPTY);
			if (m_state[m.to_y][m.to_x].type != EMPTY) {
				undoable.died  = m_state[m.to_y][m.to_x];
				undoable.diedy = m.to_y;
				undoable.diedx = m.to_x;
			}
			movesStack.push_back(undoable);
		}

		m_state[m.to_y][m.to_x] = m_state[m.from_y][m.from_x] ;
		m_state[m.from_y][m.from_x] = ChessPart(W, EMPTY);
		return true;
	}
	else return false;
}

void IterableChess::undoMove() {
	UndoableMove lastMove = *movesStack.end();
	lastMove.move.reverse();
	makeMove(lastMove.move);

	if (lastMove.died.type != EMPTY) {
		m_state[lastMove.diedy][lastMove.diedx] = lastMove.died;
	}

	movesStack.pop_back();
}

/**
 * push the move to the vector only if the the target checker is empty.
 */
static void insert_move_if_empty(int from_y, int from_x, int to_y, int to_x,
			std::vector<ChessMove> &moves, const ChessPart state[8][8]) 

{
	if (state[to_y][to_x].type == EMPTY) {
		ChessMove m = ChessMove(from_y, from_x, to_y, to_x);
		moves.push_back(m);
	}
}


/**
 * Insert the move if the destination exists on board, empty, 
 * or filled with other color.
 */
static void insert_move_if_empty_exst_other(int fromy, int fromx, 
					int toy, int tox, ChessColor player, 
					std::vector<ChessMove> &moves, const ChessPart state[8][8])
{
	if ( (toy < CHESS_DIMENTION_Y && toy >= 0 && 
		  tox < CHESS_DIMENTION_X && tox >= 0) &&
		 (state[toy][tox].type == EMPTY || 
		 state[toy][tox].color == OtherColor(player)) ) 
	{
		moves.push_back(ChessMove(fromy, fromx, toy, tox));
	}
}

void IterableChess::pushPawnMoves(int posy, int posx,
					ChessColor player, std::vector<ChessMove>& moves) const 
{

	int begins_from_y;	// Where it begins
	int direction;		// 1 for forward, -1 for backward

	if (player == B) {
		begins_from_y = 6;	direction = -1;
	} else {
		begins_from_y = 1;	direction = 1;
	}
	
	if (posy == begins_from_y) {
		insert_move_if_empty(posy, posx, posy+2*direction, posx, moves, m_state);
	}
	if (posy + 1*direction < CHESS_DIMENTION_Y) {
		insert_move_if_empty(posy, posx, posy+1*direction, posx, moves, m_state);

		// check the possibility of eating
		if (posx >= 1) {
			if (m_state[posy+1*direction][posx+1].color == OtherColor(player) 
								&& m_state[posy+1*direction][posx+1].type != EMPTY) {
				moves.push_back(ChessMove(posy, posx, posy+1*direction, posx+1));
			}
		}
		if (posx < CHESS_DIMENTION_X-1) {
			if (m_state[posy+1*direction][posx-1].color == OtherColor(player)
								&& m_state[posy+1*direction][posx-1].type != EMPTY) {
				moves.push_back(ChessMove(posy, posx, posy+1*direction, posx-1));
			}
		}
	}
}

void IterableChess::pushBishopMoves(int posy, int posx,
						ChessColor player, std::vector<ChessMove>& moves) const
{

	std::vector< std::pair<int, int> > dirs;
	dirs.push_back(std::pair<int, int>(1, 1)); dirs.push_back(std::pair<int, int>(1, -1));
	dirs.push_back(std::pair<int, int>(-1, 1));dirs.push_back(std::pair<int, int>(-1, -1));

	std::vector< std::pair<int, int> >::iterator iter = dirs.begin();;
	for (; iter != dirs.end(); iter++) {

		int desty = posy+iter->first, destx = posx+iter->second;
		while (destx < CHESS_DIMENTION_X && destx >= 0 && 
			   desty < CHESS_DIMENTION_Y && desty >= 0 && 
			   m_state[desty][destx].type == EMPTY)
		{
			moves.push_back(ChessMove(posy, posx, desty, destx));
			desty+=iter->first; 
			destx+=iter->second;
		}
		if (m_state[desty][destx].type != EMPTY && 
					m_state[desty][destx].color == OtherColor(player))
		{
			moves.push_back(ChessMove(posy, posx, desty, destx));
		}
	}
}

void IterableChess::pushRookMoves(int posy, int posx,
						ChessColor player, std::vector<ChessMove>& moves) const
{

	std::vector< std::pair<int, int> > dirs;
	dirs.push_back(std::pair<int, int>(1, 0)); dirs.push_back(std::pair<int, int>(-1, 0));
	dirs.push_back(std::pair<int, int>(0, 1)); dirs.push_back(std::pair<int, int>(0, -1));

	std::vector< std::pair<int, int> >::iterator iter = dirs.begin();;
	for (; iter != dirs.end(); iter++) {

		int desty = posy+iter->first, destx = posx+iter->second;
		while (destx < CHESS_DIMENTION_X && destx >= 0 && 
			   desty < CHESS_DIMENTION_Y && desty >= 0 && 
			   m_state[desty][destx].type == EMPTY)
		{
			moves.push_back(ChessMove(posy, posx, desty, destx));
			desty+=iter->first; 
			destx+=iter->second;
		}
		if (m_state[desty][destx].type != EMPTY && 
					m_state[desty][destx].color == OtherColor(player))
		{
			moves.push_back(ChessMove(posy, posx, desty, destx));
		}
	}
}

void IterableChess::pushQueenMoves(int posy, int posx,
						ChessColor player, std::vector<ChessMove>& moves) const
{	
	std::vector< std::pair<int, int> > dirs;
	dirs.push_back(std::pair<int, int>(1, 1)); dirs.push_back(std::pair<int, int>(1, -1));
	dirs.push_back(std::pair<int, int>(-1, 1));dirs.push_back(std::pair<int, int>(-1, -1));
	dirs.push_back(std::pair<int, int>(1, 0)); dirs.push_back(std::pair<int, int>(-1, 0));
	dirs.push_back(std::pair<int, int>(0, 1)); dirs.push_back(std::pair<int, int>(0, -1));

	std::vector< std::pair<int, int> >::iterator iter = dirs.begin();;
	for (; iter != dirs.end(); iter++) {

		int desty = posy+iter->first, destx = posx+iter->second;
		while (destx < CHESS_DIMENTION_X && destx >= 0 && 
			   desty < CHESS_DIMENTION_Y && desty >= 0 && 
			   m_state[desty][destx].type == EMPTY)
		{
			moves.push_back(ChessMove(posy, posx, desty, destx));
			desty+=iter->first; 
			destx+=iter->second;
		}
		if (m_state[desty][destx].type != EMPTY && 
					m_state[desty][destx].color == OtherColor(player))
		{
			moves.push_back(ChessMove(posy, posx, desty, destx));
		}
	}
}


void IterableChess::pushKingMoves(int posy, int posx,
							ChessColor player, std::vector<ChessMove>& moves) const
{

	std::vector< std::pair<int, int> > tos;
	tos.push_back(std::pair<int, int>(1, 1)); tos.push_back(std::pair<int, int>(1, -1));
	tos.push_back(std::pair<int, int>(-1, 1));tos.push_back(std::pair<int, int>(-1, -1));
	tos.push_back(std::pair<int, int>(1, 0)); tos.push_back(std::pair<int, int>(-1, 0));
	tos.push_back(std::pair<int, int>(0, 1)); tos.push_back(std::pair<int, int>(0, -1));

	std::vector< std::pair<int, int> >::iterator curr_to = tos.begin();;
	for (; curr_to != tos.end(); curr_to++) {
		insert_move_if_empty_exst_other(posy, posx, 
			curr_to->first+posy, curr_to->second+posx, player, moves, m_state);
	}
}

void IterableChess::pushKnightMoves(int posy, int posx,
							ChessColor player, std::vector<ChessMove>& moves) const
{	
	std::vector< std::pair<int, int> > tos;
	tos.push_back(std::pair<int, int>(2, 1));tos.push_back(std::pair<int, int>(2,-1));
	tos.push_back(std::pair<int, int>(1, 2));tos.push_back(std::pair<int, int>(1,-2));
	tos.push_back(std::pair<int, int>(-2,1));tos.push_back(std::pair<int, int>(-2,-1));
	tos.push_back(std::pair<int, int>(-1,2));tos.push_back(std::pair<int, int>(-1,-2));

	std::vector< std::pair<int, int> >::iterator curr_to = tos.begin();;
	for (; curr_to != tos.end(); curr_to++) {
		insert_move_if_empty_exst_other(posy, posx, 
			curr_to->first+posy, curr_to->second+posx, player, moves, m_state);
	}
}
