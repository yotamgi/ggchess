#include "ComputerChessPlayer.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////
// smartState funcs
//
SmartChessState::SmartChessState(ChessPart state[CHESS_DIMENTION_Y][CHESS_DIMENTION_X])
{
	for (int y=0; y<CHESS_DIMENTION_Y; y++) {
		for (int x=0; x<CHESS_DIMENTION_X;x++) {
			partsOnBoard[y][x] = NULL;
		}
	}
	// The constant part values
	int i;
	for (i=0 ; i<8 ; i++) parts[i] = playerPartInfo(ChessPart(W, PAWN),   1.0f);
	for (i=8 ; i<10; i++) parts[i] = playerPartInfo(ChessPart(W, BISHOP), 3.0f);
	for (i=10; i<12; i++) parts[i] = playerPartInfo(ChessPart(W, KNIGHT), 3.0f);
	for (i=12; i<14; i++) parts[i] = playerPartInfo(ChessPart(W, ROOK),   5.0f);
	for (i=14; i<15; i++) parts[i] = playerPartInfo(ChessPart(W, QUEEN),  10.0f);
	for (i=15; i<16; i++) parts[i] = playerPartInfo(ChessPart(W, QUEEN),  10.0f);

	for (i=16; i<24; i++) parts[i] = playerPartInfo(ChessPart(B, PAWN),   1.0f);
	for (i=24; i<26; i++) parts[i] = playerPartInfo(ChessPart(B, BISHOP), 3.0f);
	for (i=26; i<28; i++) parts[i] = playerPartInfo(ChessPart(B, KNIGHT), 3.0f);
	for (i=28; i<30; i++) parts[i] = playerPartInfo(ChessPart(B, ROOK),   5.0f);
	for (i=30; i<31; i++) parts[i] = playerPartInfo(ChessPart(B, QUEEN),  10.0f);
	for (i=31; i<32; i++) parts[i] = playerPartInfo(ChessPart(B, QUEEN),  10.0f);

	// make the current state
	int wPawnIndex=0 , wBishopIndex=8 , wKnightIndex=10, wRookIndex=12, wQueenIndex=14,wKingIndex=15; 
	int bPawnIndex=16, bBishopIndex=24, bKnightIndex=26, bRookIndex=28, bQueenIndex=30,bKingIndex=31;

	for (int y=0; y<CHESS_DIMENTION_Y; y++) {
		for (int x=0; x<CHESS_DIMENTION_X;x++) {
			if (state[y][x] == ChessPart(W,PAWN))   {  partsOnBoard[y][x] =&parts[wPawnIndex];   parts[wPawnIndex++].setPos(y,x);}
			if (state[y][x] == ChessPart(W,BISHOP)) {  partsOnBoard[y][x] =&parts[wBishopIndex]; parts[wBishopIndex++].setPos(y,x);}
			if (state[y][x] == ChessPart(W,KNIGHT)) {  partsOnBoard[y][x] =&parts[wKnightIndex]; parts[wKnightIndex++].setPos(y,x);}
			if (state[y][x] == ChessPart(W,ROOK))   {  partsOnBoard[y][x] =&parts[wRookIndex];   parts[wRookIndex++].setPos(y,x);}
			if (state[y][x] == ChessPart(W,QUEEN))  {  partsOnBoard[y][x] =&parts[wQueenIndex];  parts[wQueenIndex++].setPos(y,x);}
			if (state[y][x] == ChessPart(W,KING))   {  partsOnBoard[y][x] =&parts[wKingIndex];   parts[wKingIndex++].setPos(y,x);}

			if (state[y][x] == ChessPart(B,PAWN))   {  partsOnBoard[y][x] =&parts[bPawnIndex];   parts[bPawnIndex++].setPos(y,x);}
			if (state[y][x] == ChessPart(B,BISHOP)) {  partsOnBoard[y][x] =&parts[bBishopIndex]; parts[bBishopIndex++].setPos(y,x);}
			if (state[y][x] == ChessPart(B,KNIGHT)) {  partsOnBoard[y][x] =&parts[bKnightIndex]; parts[bKnightIndex++].setPos(y,x);}
			if (state[y][x] == ChessPart(B,ROOK))   {  partsOnBoard[y][x] =&parts[bRookIndex];	 parts[bRookIndex++].setPos(y,x);}
			if (state[y][x] == ChessPart(B,QUEEN))  {  partsOnBoard[y][x] =&parts[bQueenIndex];	 parts[bQueenIndex++].setPos(y,x);}
			if (state[y][x] == ChessPart(B,KING))   {  partsOnBoard[y][x] =&parts[bKingIndex];	 parts[bKingIndex++].setPos(y,x);}
		}
	}
	// calculate the initial mark.
	combinedBlackMark = 0.0f;
	combinedWhiteMark = 0.0f;
	for (int i=0; i<32; i++) {
		if (parts[i].alive) {
			if (parts[i].part.color == W)
				combinedWhiteMark += parts[i].importance;
			else if (parts[i].part.color == B)
				combinedBlackMark += parts[i].importance;
		}
	}
}

void SmartChessState::makeMove(const ChessMove &m, bool record) {
	undoableMove undoable;
	undoable.move = m;
	undoable.died = NULL;

	// first of all, update the parts
	if (partsOnBoard[m.to_y][m.to_x] != NULL) {

		partsOnBoard[m.to_y][m.to_x]->alive = false;
		undoable.died = partsOnBoard[m.to_y][m.to_x];

		// update the importance
		if (partsOnBoard[m.to_y][m.to_x]->part.color == B )
			 combinedBlackMark -= partsOnBoard[m.to_y][m.to_x]->importance;
		else combinedWhiteMark -= partsOnBoard[m.to_y][m.to_x]->importance;
	}
	partsOnBoard[m.from_y][m.from_x]->y = m.to_y;
	partsOnBoard[m.from_y][m.from_x]->x = m.to_x;

	// than update the board
	partsOnBoard[m.to_y][m.to_x] = partsOnBoard[m.from_y][m.from_x];
	partsOnBoard[m.from_y][m.from_x] = NULL;

	if (record) {
		// insert the move into the moves Stack
		movesStack.push_back(undoable);
	}
}

void SmartChessState::undoMove() {
	undoableMove undoable = *(movesStack.end()-1);
	undoable.move.reverse();
	makeMove(undoable.move, false);

	// if a chess part dies in this move, make it alive again.
	if (undoable.died != NULL)	{
		undoable.died->alive = true;
		partsOnBoard[undoable.died->y][undoable.died->x] = undoable.died;

		// update the importance
		if (undoable.died->part.color == W)
			 combinedWhiteMark += undoable.died->importance;
		else combinedBlackMark += undoable.died->importance;
	}

	// finally, pop the move from the end
	movesStack.pop_back();
}

bool SmartChessState::verifyState(ChessPart** const state) const {
	for (int y=0; y<CHESS_DIMENTION_Y; y++) {
		for (int x=0;x<CHESS_DIMENTION_X; x++) { 
			if (partsOnBoard[y][x] == NULL) {
				if (state[y][x].type == EMPTY) 
					return false;
			}
			else if (state[y][x].type != partsOnBoard[y][x]->part.type)
				return false;
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
// ComputerChessPlayer funcs
//
ChessMove ComputerChessPlayer::play(const ChessLogic& board) {
	ChessPart state[8][8];
	board.fillGameState(state);

	IterableChess chess;
	chess.setState(state);
	SmartChessState smartState(state);

	return findBestMove(chess, smartState);
}

void ComputerChessPlayer::illigalMove() {
	MessageBoxA(0, "Computer made an illegal move.", "Unbelievable...", MB_OK);	
}

ChessMove ComputerChessPlayer::findBestMove(IterableChess& iterable, 
													SmartChessState& smartState)
{
	D3dTextConsole* console = D3dTextConsole::getTextOutputObject();
	float bestWorstStateMark = -101.0f;
	ChessMove correspondingMove;

	// for every move I can do:
	vector<ChessMove> myMoves= iterable.getAllMoves(m_color);
	vector<ChessMove>::iterator currMyMove = myMoves.begin();
	for (; currMyMove!=myMoves.end();currMyMove++) {
		iterable.makeMove(*currMyMove);
		smartState.makeMove(*currMyMove);

		float worstStateMark = 101.0f;

		// for every move he can do:
		vector<ChessMove> hisMoves = iterable.getAllMoves(OtherColor(m_color));
		vector<ChessMove>::iterator currHisMove = hisMoves.begin();
		for (; currHisMove!=hisMoves.end();currHisMove++) {

			// make the  move
			iterable.makeMove(*currHisMove);
			smartState.makeMove(*currHisMove);

			// mark it
			float stateMark = markState(iterable, smartState);

			// peek the worst state available,
			if (worstStateMark > stateMark) 
				worstStateMark = stateMark;

			// undo the move
			iterable.undoMove();
			smartState.undoMove();
		}
		if (bestWorstStateMark < worstStateMark) {
			stringstream ss;
			ss << worstStateMark << endl;
			console->writeText(ss.str());

			bestWorstStateMark = worstStateMark;
			correspondingMove = *currMyMove;
		}

		iterable.undoMove();
		smartState.undoMove();
/*		if (!smartState.verifyState(iterable.getState())) {
			console->writeText("NOT GOOD!");
		}*/
	}

	return correspondingMove;
}

float ComputerChessPlayer::markState(IterableChess& chess, SmartChessState& state) const
{
	// check if there is a matt going on:
 	playerPartInfo king = state.getKing(m_color);
 	if (king.alive == false)  
 		return -100.0f;
	playerPartInfo hisKing = state.getKing(OtherColor(m_color));
	if (hisKing.alive == false) 
		return 100.0f;

	float mark = state.getPartsMark(m_color) - state.getPartsMark(OtherColor(m_color));
	return mark;
}