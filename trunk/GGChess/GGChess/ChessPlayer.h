#ifndef __GAME_PLAYER__
#define __GAME_PLAYER__

#include "ChessDefines.h"
#include "ChessLogic.h"

class ChessPlayer {
public:

	virtual ~ChessPlayer() {}

	virtual void init(ChessColor color) { m_color = color; };

	virtual ChessMove play(const ChessLogic& board) =0;

	virtual void illigalMove() =0;

protected:
	ChessColor m_color;

};


#endif // __GAME_PLAYER__
