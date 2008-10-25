#ifndef __CHESS_GAME___
#define __CHESS_GAME___

#include <d3d9.h>
#include <process.h>

#include "Common/camera.h"
#include "common/D3dTextConsole.h"
#include "ChessBoard.h"
#include "ChessLogic.h"
#include "ChessPlayer.h"

/************************************************************************/
/* The Chess Game class.                                                */
/* Is responsible for making the game with the players.                 */
/* Is using a separate thread for the players.                          */
/************************************************************************/
class ChessGame {
public:
	ChessGame(ChessBoard* board, IDirect3DDevice9* device);
	~ChessGame();

	void init();

	void draw(float timeDelta);

	void SetBlackPlayer(ChessPlayer* b) { m_blackPlayer = b; }
	void SetWhitePlayer(ChessPlayer* w) { m_whitePlayer = w; }

	void startGame();

	/************************************************************************/
	/*  The User Player.                                                    */
	/************************************************************************/
	class ChessUserPlayer : public ChessPlayer {
	public:
		ChessUserPlayer();

		virtual ChessMove play(const ChessLogic& board);

		virtual void illigalMove();

		static void mouseMovedEvent(WPARAM wp, LPARAM lp, void* pplayer);
		static void mouseClickEvent(WPARAM wp, LPARAM lp, void* pplayer);
		static void calcPartByMouseCoord( 
			int x, int y, ChessUserPlayer* player, int* partx, int* party);
		
	private:
		const ChessLogic* m_game;

		volatile bool listening;
		int ChosenX, ChosenY;

		// temporary vars.
		int tmpPrevX;
		int tmpPrevY;

	};

private:

	void UpdateCamera( float timeDelta );

	ChessPlayer *m_blackPlayer, *m_whitePlayer;

	ChessLogic* m_boardLogic;

	Camera *TheCamera;

	friend void __cdecl makeGameSequence(void* pgame);

	static IDirect3DDevice9* m_device;
};


void __cdecl makeGameSequence(void* pgame);

#endif // __CHESS_GAME___
