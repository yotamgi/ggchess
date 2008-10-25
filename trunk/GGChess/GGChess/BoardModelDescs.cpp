
#include "BoardModelDescs.h"

using namespace std;

void createRegualrBoardDesc(BoardDesc& desc) {

	desc.XFileName = "models\\XFormat\\newChess.x";

	//
	// The subsets
	//

	// The bishop subsets:
	desc.BishopSubsets[0].push_back(33);
	desc.BishopSubsets[0].push_back(34);
	desc.BishopSubsets[0].push_back(35);
	desc.BishopSubsets[1].push_back(46);
	desc.BishopSubsets[1].push_back(47);
	desc.BishopSubsets[1].push_back(48);
	desc.BishopSubsets[2].push_back(15);
	desc.BishopSubsets[2].push_back(16);
	desc.BishopSubsets[2].push_back(17);
	desc.BishopSubsets[3].push_back(0); 
	desc.BishopSubsets[3].push_back(1); 
	desc.BishopSubsets[3].push_back(2); 

	// The King subsets:
	desc.KingSubsets[0].push_back(36);
	desc.KingSubsets[0].push_back(37);
	desc.KingSubsets[0].push_back(38);
	desc.KingSubsets[1].push_back(3 );
	desc.KingSubsets[1].push_back(4 );
	desc.KingSubsets[1].push_back(5 );

	// The Queen subsets
	desc.QueenSubsets[0].push_back(39);
	desc.QueenSubsets[1].push_back(6);

	// The Pawn Subsets
	desc.PawnSubsets[0 ].push_back(60);
	desc.PawnSubsets[0 ].push_back(61);
	desc.PawnSubsets[1 ].push_back(58);
	desc.PawnSubsets[1 ].push_back(59);
	desc.PawnSubsets[2 ].push_back(54);
	desc.PawnSubsets[2 ].push_back(55);
	desc.PawnSubsets[3 ].push_back(52);
	desc.PawnSubsets[3 ].push_back(53);
	desc.PawnSubsets[4 ].push_back(40);
	desc.PawnSubsets[4 ].push_back(41);
	desc.PawnSubsets[5 ].push_back(56);
	desc.PawnSubsets[5 ].push_back(57);
	desc.PawnSubsets[6 ].push_back(12);
	desc.PawnSubsets[6 ].push_back(13);
	desc.PawnSubsets[7 ].push_back(67);
	desc.PawnSubsets[7 ].push_back(68);
	desc.PawnSubsets[8 ].push_back(27);
	desc.PawnSubsets[8 ].push_back(28);
	desc.PawnSubsets[9 ].push_back(21);
	desc.PawnSubsets[9 ].push_back(22);
	desc.PawnSubsets[10].push_back(25);
	desc.PawnSubsets[10].push_back(26);
	desc.PawnSubsets[11].push_back(23);
	desc.PawnSubsets[11].push_back(24);
	desc.PawnSubsets[12].push_back(7 );
	desc.PawnSubsets[12].push_back(8 );
	desc.PawnSubsets[13].push_back(29);
	desc.PawnSubsets[13].push_back(30);
	desc.PawnSubsets[14].push_back(31);
	desc.PawnSubsets[14].push_back(32);
	desc.PawnSubsets[15].push_back(65);
	desc.PawnSubsets[15].push_back(66);
	

	// The Knight subsets:
	desc.KnightSubsets[0].push_back(42);
	desc.KnightSubsets[0].push_back(43);
	desc.KnightSubsets[0].push_back(44);
	desc.KnightSubsets[1].push_back(49);
	desc.KnightSubsets[1].push_back(50);
	desc.KnightSubsets[1].push_back(51);
	desc.KnightSubsets[2].push_back(18);
	desc.KnightSubsets[2].push_back(19);
	desc.KnightSubsets[2].push_back(20);
	desc.KnightSubsets[3].push_back( 9);
	desc.KnightSubsets[3].push_back(10);
	desc.KnightSubsets[3].push_back(11);

	// The RookSubsets
	desc.RookSubsets[0].push_back(62);
	desc.RookSubsets[1].push_back(45);
	desc.RookSubsets[2].push_back(14);
	desc.RookSubsets[3].push_back(64);

	// The Board subsets
	desc.BoardSubsets.push_back(63);
	desc.BoardSubsets.push_back(69);

	//
	// The Texures
	// 

	// The textures
	desc.TexturesFileNames.push_back(string("models\\XFormat\\blackmarble.jpg"));
	desc.TexturesFileNames.push_back(string("models\\XFormat\\whitemarble.jpg"));
	desc.TexturesFileNames.push_back(string("models\\XFormat\\chessboard.jpg"));

	// the indices
	for (int i=0; i<=69; i++){
		desc.overriteTexs.push_back(false);
		desc.subsetTexsIndex.push_back(0);
	}
	for (int i=0; i<PAWN_NUMBER; i++) {
		vector<int>::iterator subs = desc.PawnSubsets[i].begin();
		for (;subs != desc.PawnSubsets[i].end(); subs++) {
			desc.overriteTexs[*subs] = true;
			desc.subsetTexsIndex[*subs] = 1;
		}
	}
	for (int i=0; i<KING_NUMBER; i++) {
		vector<int>::iterator subs = desc.KingSubsets[i].begin();
		for (;subs != desc.KingSubsets[i].end(); subs++) {
			desc.overriteTexs[*subs] = true;
			desc.subsetTexsIndex[*subs] = 1;
		}
	}
	for (int i=0; i<QUEEN_NUMBER; i++) {
		vector<int>::iterator subs = desc.QueenSubsets[i].begin();
		for (;subs != desc.QueenSubsets[i].end(); subs++) {
			desc.overriteTexs[*subs] = true;
			desc.subsetTexsIndex[*subs] = 1;
		}
	}
	for (int i=0; i<BISHOP_NUMBER; i++) {
		vector<int>::iterator subs = desc.BishopSubsets[i].begin();
		for (;subs != desc.BishopSubsets[i].end(); subs++) {
			desc.overriteTexs[*subs] = true;
			desc.subsetTexsIndex[*subs] = 1;
		}
	}
	for (int i=0; i<KNIGHT_NUMBER; i++) {
		vector<int>::iterator subs = desc.KnightSubsets[i].begin();
		for (;subs != desc.KnightSubsets[i].end(); subs++) {
			desc.overriteTexs[*subs] = true;
			desc.subsetTexsIndex[*subs] = 1;
		}
	}
	for (int i=0; i<ROOK_NUMBER; i++) {
		vector<int>::iterator subs = desc.RookSubsets[i].begin();
		for (;subs != desc.RookSubsets[i].end(); subs++) {
			desc.overriteTexs[*subs] = true;
			desc.subsetTexsIndex[*subs] = 1;
		}
	}
	for (int i=PAWN_NUMBER; i<PAWN_NUMBER*2; i++) {
		vector<int>::iterator subs = desc.PawnSubsets[i].begin();
		for (;subs != desc.PawnSubsets[i].end(); subs++) {
			desc.overriteTexs[*subs] = true;
			desc.subsetTexsIndex[*subs] = 0;
		}
	}
	for (int i=KING_NUMBER; i<KING_NUMBER*2; i++) {
		vector<int>::iterator subs = desc.KingSubsets[i].begin();
		for (;subs != desc.KingSubsets[i].end(); subs++) {
			desc.overriteTexs[*subs] = true;
			desc.subsetTexsIndex[*subs] = 0;
		}
	}
	for (int i=QUEEN_NUMBER; i<QUEEN_NUMBER*2; i++) {
		vector<int>::iterator subs = desc.QueenSubsets[i].begin();
		for (;subs != desc.QueenSubsets[i].end(); subs++) {
			desc.overriteTexs[*subs] = true;
			desc.subsetTexsIndex[*subs] = 0;
		}
	}
	for (int i=BISHOP_NUMBER; i<BISHOP_NUMBER*2; i++) {
		vector<int>::iterator subs = desc.BishopSubsets[i].begin();
		for (;subs != desc.BishopSubsets[i].end(); subs++) {
			desc.overriteTexs[*subs] = true;
			desc.subsetTexsIndex[*subs] = 0;
		}
	}
	for (int i=KNIGHT_NUMBER; i<KNIGHT_NUMBER*2; i++) {
		vector<int>::iterator subs = desc.KnightSubsets[i].begin();
		for (;subs != desc.KnightSubsets[i].end(); subs++) {
			desc.overriteTexs[*subs] = true;
			desc.subsetTexsIndex[*subs] = 0;
		}
	}
	for (int i=ROOK_NUMBER; i<ROOK_NUMBER*2; i++) {
		vector<int>::iterator subs = desc.RookSubsets[i].begin();
		for (;subs != desc.RookSubsets[i].end(); subs++) {
			desc.overriteTexs[*subs] = true;
			desc.subsetTexsIndex[*subs] = 0;
		}
	}
	desc.overriteTexs[63] = true;
	desc.subsetTexsIndex[63] = 2;

	for (int i=0; i<=69; i++) {
		D3DXMATRIX identity;
		D3DXMatrixIdentity(&identity);
		desc.subsetsTransform.push_back(identity);
	}

	//
	// Advancing Vectors
	// 

	desc.forward	= D3DXVECTOR3( 0.0f, 0.0f, 25.0f);
	desc.right		= D3DXVECTOR3(25.0f, 0.0f,  0.0f);

	//
	// The trasformations
	//
	
	// The bishops
	D3DXMatrixTranslation(&desc.subsetsTransform[15], -5.0 * 25.0f, 0.0f, -7.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[16], -5.0 * 25.0f, 0.0f, -7.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[17], -5.0 * 25.0f, 0.0f, -7.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[0 ], -2.0 * 25.0f, 0.0f, -7.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[1 ], -2.0 * 25.0f, 0.0f, -7.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[2 ], -2.0 * 25.0f, 0.0f, -7.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[33], -2.0 * 25.0f, 0.0f, -0.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[34], -2.0 * 25.0f, 0.0f, -0.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[35], -2.0 * 25.0f, 0.0f, -0.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[46], -5.0 * 25.0f, 0.0f, -0.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[47], -5.0 * 25.0f, 0.0f, -0.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[48], -5.0 * 25.0f, 0.0f, -0.0 * 25.0f);

	// The Pawns
	D3DXMatrixTranslation(&desc.subsetsTransform[27], -7.0 * 25.0f, 0.0f, -6.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[28], -7.0 * 25.0f, 0.0f, -6.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[21], -6.0 * 25.0f, 0.0f, -6.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[22], -6.0 * 25.0f, 0.0f, -6.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[25], -5.0 * 25.0f, 0.0f, -6.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[26], -5.0 * 25.0f, 0.0f, -6.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[23], -4.0 * 25.0f, 0.0f, -6.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[24], -4.0 * 25.0f, 0.0f, -6.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[7 ], -3.0 * 25.0f, 0.0f, -6.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[8 ], -3.0 * 25.0f, 0.0f, -6.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[29], -2.0 * 25.0f, 0.0f, -6.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[30], -2.0 * 25.0f, 0.0f, -6.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[31], -1.0 * 25.0f, 0.0f, -6.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[32], -1.0 * 25.0f, 0.0f, -6.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[65], -0.0 * 25.0f, 0.0f, -6.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[66], -0.0 * 25.0f, 0.0f, -6.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[60], -7.0 * 25.0f, 0.0f, -1.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[61], -7.0 * 25.0f, 0.0f, -1.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[58], -6.0 * 25.0f, 0.0f, -1.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[59], -6.0 * 25.0f, 0.0f, -1.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[54], -5.0 * 25.0f, 0.0f, -1.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[55], -5.0 * 25.0f, 0.0f, -1.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[52], -4.0 * 25.0f, 0.0f, -1.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[53], -4.0 * 25.0f, 0.0f, -1.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[40], -3.0 * 25.0f, 0.0f, -1.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[41], -3.0 * 25.0f, 0.0f, -1.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[56], -2.0 * 25.0f, 0.0f, -1.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[57], -2.0 * 25.0f, 0.0f, -1.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[12], -1.0 * 25.0f, 0.0f, -1.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[13], -1.0 * 25.0f, 0.0f, -1.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[67], -0.0 * 25.0f, 0.0f, -1.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[68], -0.0 * 25.0f, 0.0f, -1.0 * 25.0f);

	// The Rooks
	D3DXMatrixTranslation(&desc.subsetsTransform[14], -7.0 * 25.0f, 0.0f, -7.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[64], -0.0 * 25.0f, 0.0f, -7.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[62], -0.0 * 25.0f, 0.0f, -0.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[45], -7.0 * 25.0f, 0.0f, -0.0 * 25.0f);

	// The Kings
	D3DXMatrixTranslation(&desc.subsetsTransform[3 ], -3.0 * 25.0f, 0.0f, -7.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[4 ], -3.0 * 25.0f, 0.0f, -7.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[5 ], -3.0 * 25.0f, 0.0f, -7.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[36], -4.0 * 25.0f, 0.0f, -0.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[37], -4.0 * 25.0f, 0.0f, -0.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[38], -4.0 * 25.0f, 0.0f, -0.0 * 25.0f);

	// The Queens
	D3DXMatrixTranslation(&desc.subsetsTransform[ 6], -4.0 * 25.0f, 0.0f, -7.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[39], -3.0 * 25.0f, 0.0f, -0.0 * 25.0f);

	// The Knights
	D3DXMatrixTranslation(&desc.subsetsTransform[18], -6.0 * 25.0f, 0.0f, -7.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[19], -6.0 * 25.0f, 0.0f, -7.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[20], -6.0 * 25.0f, 0.0f, -7.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[ 9], -1.0 * 25.0f, 0.0f, -7.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[10], -1.0 * 25.0f, 0.0f, -7.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[11], -1.0 * 25.0f, 0.0f, -7.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[42], -1.0 * 25.0f, 0.0f, -0.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[43], -1.0 * 25.0f, 0.0f, -0.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[44], -1.0 * 25.0f, 0.0f, -0.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[49], -6.0 * 25.0f, 0.0f, -0.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[50], -6.0 * 25.0f, 0.0f, -0.0 * 25.0f);
	D3DXMatrixTranslation(&desc.subsetsTransform[51], -6.0 * 25.0f, 0.0f, -0.0 * 25.0f);

	// The board plane
	desc.boardPlane = D3DXPLANE(0.0f, 25.0f, 0.0f, -100.0f);
}

