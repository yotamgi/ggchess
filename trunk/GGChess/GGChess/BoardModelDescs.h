#ifndef __COMMON_MODE_NOARD_DESCS__
#define __COMMON_MODE_NOARD_DESCS__

#include <string>
#include <vector>
#include <d3dx9.h>

#include "ChessDefines.h"

struct BoardDesc {
	std::string XFileName;

	// first white than black, and the order shold be from left to rite.
	std::vector<int> PawnSubsets[PAWN_NUMBER*2];
	std::vector<int> BishopSubsets[BISHOP_NUMBER*2];
	std::vector<int> KnightSubsets[KNIGHT_NUMBER*2];
	std::vector<int> RookSubsets[ROOK_NUMBER*2];
	std::vector<int> KingSubsets[KING_NUMBER*2];
	std::vector<int> QueenSubsets[QUEEN_NUMBER*2];
	std::vector<int> BoardSubsets;

	// Override Textures
	std::vector<bool> overriteTexs;
	std::vector<std::string> TexturesFileNames;
	std::vector<int> subsetTexsIndex;

	// The Pieces Tranformations;
	std::vector<D3DXMATRIX> subsetsTransform;

	// The Advancing Vectors:
	D3DXVECTOR3 right;
	D3DXVECTOR3 forward;

	// The plane where the board sits.
	D3DXPLANE boardPlane;

};

void createRegualrBoardDesc(BoardDesc& desc);


#endif // __COMMON_MODE_NOARD_DESCS__