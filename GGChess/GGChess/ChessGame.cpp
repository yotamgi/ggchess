#include "ChessGame.h"

IDirect3DDevice9* ChessGame::m_device;

ChessGame::ChessGame(ChessBoard* board, IDirect3DDevice9* device)
{
	m_boardLogic = new ChessLogic(board);
	m_device = device;
}

ChessGame::~ChessGame() {
	delete m_boardLogic;
}

void ChessGame::startGame() {
	// start the game sequnce in a new thread.
	_beginthread(makeGameSequence, 0, (void*)this);
}

void ChessGame::init() {

	//
	// setting the board and the logic
	//
	m_boardLogic->init();

	//
	// setting the console
	//
	D3dTextConsole* textConsole = D3dTextConsole::getTextOutputObject();
	textConsole->init(m_device);
	textConsole->setFont("Times New Roman", 15);

	//
	// Setting the Camera
	//
	TheCamera = new Camera(Camera::AIRCRAFT);
	D3DXVECTOR3 Pos(0.0f, 120.0f, -250.0f);
	TheCamera->setPosition(&Pos);
	TheCamera->pitch(0.3f);
	
}

void ChessGame::draw(float timeDelta) {

	// Update the camera.
	UpdateCamera(timeDelta);

	// Update the view matrix representing the cameras 
	// new position/orientation.
	D3DXMATRIX V;
	TheCamera->getViewMatrix(&V);
	m_device->SetTransform(D3DTS_VIEW, &V);

	//
	// Render
	//

	m_boardLogic->draw(timeDelta);

	D3DXMATRIX idtt;
	D3DXMatrixIdentity(&idtt);
	m_device->SetTransform(D3DTS_WORLD, &idtt);

	//
	// The Text Output
	// 
	D3dTextConsole* textConsole = D3dTextConsole::getTextOutputObject();
	textConsole->draw(timeDelta);
}

void ChessGame::UpdateCamera( float timeDelta )
{
	if( ::GetAsyncKeyState('W') & 0x8000f )
		TheCamera->walk(40.0f * timeDelta);

	if( ::GetAsyncKeyState('S') & 0x8000f )
		TheCamera->walk(-40.0f * timeDelta);

	if( ::GetAsyncKeyState('A') & 0x8000f )
		TheCamera->strafe(-40.0f * timeDelta);

	if( ::GetAsyncKeyState('D') & 0x8000f )
		TheCamera->strafe(40.0f * timeDelta);

	if( ::GetAsyncKeyState('R') & 0x8000f )
		TheCamera->fly(40.0f * timeDelta);

	if( ::GetAsyncKeyState('F') & 0x8000f )
		TheCamera->fly(-40.0f * timeDelta);

	if( ::GetAsyncKeyState(VK_UP) & 0x8000f )
		TheCamera->pitch(1.0f * timeDelta);

	if( ::GetAsyncKeyState(VK_DOWN) & 0x8000f )
		TheCamera->pitch(-1.0f * timeDelta);

	if( ::GetAsyncKeyState(VK_LEFT) & 0x8000f )
		TheCamera->yaw(-1.0f * timeDelta);

	if( ::GetAsyncKeyState(VK_RIGHT) & 0x8000f )
		TheCamera->yaw(1.0f * timeDelta);

	if( ::GetAsyncKeyState('N') & 0x8000f )
		TheCamera->roll(1.0f * timeDelta);

	if( ::GetAsyncKeyState('M') & 0x8000f )
		TheCamera->roll(-1.0f * timeDelta);
}


void __cdecl makeGameSequence(void* pgame){
	ChessGame* game = reinterpret_cast<ChessGame*>(pgame);

	D3dTextConsole *console = D3dTextConsole::getTextOutputObject();

	ChessMove move;
	while (true){
		// Whitey's turn
		console->stream() << "White's Turn:" << std::endl;
		move = game->m_whitePlayer->play(*game->m_boardLogic);
		while (game->m_boardLogic->makeMove(move, W) == false) {
			game->m_whitePlayer->illigalMove();
			move = game->m_whitePlayer->play(*game->m_boardLogic);
		}

		// Black's's turn
		console->stream() << "Black's Turn:" << std::endl;
		move = game->m_blackPlayer->play(*game->m_boardLogic);
		while (game->m_boardLogic->makeMove(move, B) == false) {
			game->m_blackPlayer->illigalMove();
			move = game->m_blackPlayer->play(*game->m_boardLogic);
		}
	}
}

//////////////////////////////////////////////////////////////////////////

ChessGame::ChessUserPlayer::ChessUserPlayer():m_game(NULL),tmpPrevX(0),tmpPrevY(0) {}

void ChessGame::ChessUserPlayer::calcPartByMouseCoord( 
			int y, int x, ChessUserPlayer* player, int* party, int* partx)
{
	d3d::Ray ray = d3d::CaclulatePickingRay(m_device, x, y);
	D3DXPLANE boardPlane = player->m_game->getBoard()->getBoardPlane();

	// calculate the intersection:
	float t;		
	t = -1 *(boardPlane.a*ray.origin.x + boardPlane.b*ray.origin.y + boardPlane.c*ray.origin.z + boardPlane.d)/
		(boardPlane.a*ray.direction.x + boardPlane.b*ray.direction.y + boardPlane.c*ray.direction.z);
	D3DXVECTOR3 intersection = ray.origin + t*ray.direction;

	*partx = int((intersection.x + 25.0f*4) / 25.0f);
	*party = int((intersection.z + 25.0f*4) / 25.0f);
}

void ChessGame::ChessUserPlayer::mouseMovedEvent(WPARAM wp, LPARAM lp, void* pplayer) {

	ChessUserPlayer* player = (ChessUserPlayer*)pplayer;
	if(player->listening) {
		D3dTextConsole *console = D3dTextConsole::getTextOutputObject();

		int x = LOWORD(lp);
		int y = HIWORD(lp);
		int partx, party;
		calcPartByMouseCoord(y, x, player, &party, &partx);

		if (party < CHESS_DIMENTION_Y && party>= 0 && 
							partx< CHESS_DIMENTION_X && partx>=0 ) {

			// emphasys the chosen part
			player->m_game->emphasys(player->tmpPrevY, player->tmpPrevX, false);
			player->m_game->emphasys(party, partx, true);
			player->tmpPrevY = party;
			player->tmpPrevX = partx;
		}
	}
}
 void ChessGame::ChessUserPlayer::mouseClickEvent(WPARAM wp, LPARAM lp, void* pplayer) {
	ChessUserPlayer* player = (ChessUserPlayer*)pplayer;
	if(player->listening) {		
		D3dTextConsole *console = D3dTextConsole::getTextOutputObject();

		int x = LOWORD(lp);
		int y = HIWORD(lp);
		int partx, party;
		calcPartByMouseCoord(y, x, player, &party, &partx);

		if (party < CHESS_DIMENTION_Y && party>= 0 && 
				partx< CHESS_DIMENTION_X && partx>=0) {
			player->ChosenY = party;
			player->ChosenX = partx;
			player->listening = false;
		}
	}
}

ChessMove ChessGame::ChessUserPlayer::play(const ChessLogic& board)
{
	D3dTextConsole *console = D3dTextConsole::getTextOutputObject();

	ChessMove move;

	console->stream() << "Use the mouse or keyboard to enter your move" << std::endl;
	m_game = &board;
	console->RegisterEvent(WM_MOUSEMOVE, mouseMovedEvent, this);
	console->RegisterEvent(WM_LBUTTONDOWN, mouseClickEvent, this);

	// start the listening
	listening = true;
	while (listening) {}
	console->stream() << "You chose " << ChosenY << ", " << ChosenX << " ";;
	move.from_y = ChosenY; move.from_x = ChosenX;

	console->stream() << "Where to? ";
	m_game = &board;

	// start the listening
	listening = true;
	while (listening) {
		m_game->emphasys(move.from_y, move.from_x, true);
	}
	m_game->emphasys(move.from_y, move.from_x, false);
	console->stream() << "You chose " << ChosenY << ", " << ChosenX << std::endl;
	move.to_y = ChosenY; move.to_x = ChosenX;

	return move;
}


void ChessGame::ChessUserPlayer::illigalMove() {
	D3dTextConsole *console = D3dTextConsole::getTextOutputObject();

	console->stream() << "You entered an illegal chess move." << std::endl;
}