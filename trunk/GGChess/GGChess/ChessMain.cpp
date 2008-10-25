//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: cameraApp.cpp
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Demonstrates using the Camera class.
//         
//////////////////////////////////////////////////////////////////////////////////////////////////

#include <sstream>

#include "common/d3dUtility.h"
#include "common/camera.h"
#include "common/D3dTextConsole.h"

#include "BoardModelDescs.h"
#include "ChessBoard.h"
#include "ChessLogic.h"
#include "IterableChess.h"
#include "ChessGame.h"
//
// Globals
//

IDirect3DDevice9* Device = 0; 

const int Width  = 1680;
const int Height = 1050;

ChessLogic* boardLogic;

ChessGame* game;

//
// For tests only!
//
static void printMoves(std::vector<ChessMove> moves, D3dTextConsole* textConsole) {

	std::vector<ChessMove>::iterator iter = moves.begin();
	for (; iter!= moves.end(); iter++) {
		std::stringstream s;
		s << iter->from_y << ", " << iter->from_x << " -> " << 
			iter->to_y << ", " << iter->to_x << std::endl;
		std::string text;
		
		textConsole->writeText(s.str());
	}

}

static void makeEmptyState(ChessPart state[CHESS_DIMENTION_Y][CHESS_DIMENTION_X]) {
	for (int y=0; y<CHESS_DIMENTION_Y; y++) {
		for (int x=0; x<CHESS_DIMENTION_X; x++) {
			state[y][x] = ChessPart(W, EMPTY);
		}
	}

}

void testIterableChess(IDirect3DDevice9* device) {
	D3dTextConsole* textConsole = D3dTextConsole::getTextOutputObject();
	ChessPart state[CHESS_DIMENTION_Y][CHESS_DIMENTION_X];

	IterableChess ic;

	//
	// The Pawn
	//
	makeEmptyState(state);
	state[1][3] = ChessPart(W, PAWN);
	state[2][4] = ChessPart(B, PAWN);

	ic.setState(state);
	std::vector<ChessMove> moves = ic.getPartMoves(1, 3, W);
	textConsole->writeText("pawn:\n");
	printMoves(moves, textConsole);

	//
	// The Bishops
	//
	makeEmptyState(state);
	state[3][3] = ChessPart(W, BISHOP);
	state[1][1] = ChessPart(B, BISHOP);
	state[6][6] = ChessPart(W, BISHOP);

	ic.setState(state);
	moves = ic.getPartMoves(3, 3, W);
	textConsole->writeText("bishops:\n");
	printMoves(moves, textConsole);

	//
	// The Rook
	//
	makeEmptyState(state);
	state[4][4] = ChessPart(B, ROOK);

	ic.setState(state);
	moves = ic.getPartMoves(4, 4, B);
	textConsole->writeText("rook:\n");
	printMoves(moves, textConsole);

	
	//
	// The Queen
	//
	makeEmptyState(state);
	state[0][0] = ChessPart(B, QUEEN);

	ic.setState(state);
	moves = ic.getPartMoves(0, 0, B);
	textConsole->writeText("queen:\n");
	printMoves(moves, textConsole);

	//
	// The King
	//
	makeEmptyState(state);
	state[0][7] = ChessPart(B, KING);

	ic.setState(state);
	moves = ic.getPartMoves(0, 7, B);
	textConsole->writeText("king:\n");
	printMoves(moves, textConsole);

	//
	// The Knight
	//
	makeEmptyState(state);
	state[0][0] = ChessPart(B, KNIGHT);
	state[1][2] = ChessPart(W, KING);
	state[2][1] = ChessPart(B, KING);

	ic.setState(state);
	moves = ic.getPartMoves(0, 0, B);
	textConsole->writeText("Knight:\n");
	printMoves(moves, textConsole);

}

void testChessLogic(ChessLogic *boardLogic) {

	boardLogic->init();
	ChessMove a = ChessMove( 0, 2, 3, 5 );
	if((boardLogic->makeMove(a, W))) {
		MessageBoxA(0, "Failed at move 1", "test failed.", MB_OK);
	}

	a = ChessMove( 1, 3, 2, 3 );
	if(!(boardLogic->makeMove(a, W))) {
		MessageBoxA(0, "Failed at move 2", "test failed.", MB_OK);
	}
	a = ChessMove( 0, 2, 3, 5 );
	if(!(boardLogic->makeMove(a, W))) {
		MessageBoxA(0, "Failed at move 3", "test failed.", MB_OK);
	}
	a = ChessMove( 3, 5, 3, 6 );
	if((boardLogic->makeMove(a, W))) {
		MessageBoxA(0, "Failed at move 4", "test failed.", MB_OK);
	}
	a = ChessMove( 3, 5, 6, 2 );
	if(!(boardLogic->makeMove(a, W))) {
		MessageBoxA(0, "Failed at move 5", "test failed.", MB_OK);
	}
}

//
// Framework functions
//
bool Setup()
{
	//
	// Loading the Things
	//
	BoardDesc bd;
	createRegualrBoardDesc(bd);
	ChessBoard* board;
	board = new ChessBoard(Device, bd);
	game = new ChessGame(board, Device);
	game->init();

	ChessGame::ChessUserPlayer* black = new ChessGame::ChessUserPlayer;
	ChessGame::ChessUserPlayer* white = new ChessGame::ChessUserPlayer;
	black->init(B);
	white->init(W);
	game->SetBlackPlayer(black);
	game->SetWhitePlayer(white);
	game->startGame();


//	testIterableChess(Device);	
//	testChessLogic(boardLogic);
	
	//
	// Set projection matrix.
	//

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
			&proj,
			D3DX_PI * 0.25f, // 45 - degree
			(float)Width / (float)Height,
			1.0f,
			1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	return true;
}

void Cleanup()
{
}

bool Display(float timeDelta)
{

	if( Device )
	{
		//
		// Render
		//

		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		Device->BeginScene();

		game->draw(timeDelta);

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
	return true;
}

//
// WndProc
//
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
		
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
			::DestroyWindow(hwnd);
		break;
	}

	if (Device) {
		D3dTextConsole* textConsole = D3dTextConsole::getTextOutputObject();
		textConsole->ProccessWinMessage(msg, wParam, lParam);
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

//
// WinMain
//
int WINAPI WinMain(HINSTANCE hinstance,
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	if(!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBoxA(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}
		
	if(!Setup())
	{
		::MessageBoxA(0, "Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop( Display );

	Cleanup();

	Device->Release();

	return 0;
}
