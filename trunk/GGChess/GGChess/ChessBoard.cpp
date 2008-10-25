
#include "ChessBoard.h"

//////////////////////////////////////////////////////////////////////////
// The Chess Object Class
//

ChessObject::ChessObject(ID3DXMesh *object, 
						 std::vector<D3DMATERIAL9> materials, 
						 std::vector<IDirect3DTexture9*> texs, 
						 std::vector<D3DXMATRIX> transform,
						 std::vector<int> subsets,
						 IDirect3DDevice9* device,
						 ChessPart type):
m_objMesh(object),
m_objMtrls(materials),
m_objTexs(texs),
m_objTransform(transform),
m_objSubsets(subsets),
m_device(device),
m_type(type),
emphasised(false)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void ChessObject::draw() const {
	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, m_pos.x, m_pos.y, m_pos.z);

	std::vector<int>::const_iterator iter = m_objSubsets.begin();

	for(; iter != m_objSubsets.end(); iter++)
	{
		D3DXMATRIX finalTrans = m_objTransform[*iter] * trans;

		D3DXMATRIX i;
		D3DXMatrixIdentity(&i);
		m_device->SetTransform(D3DTS_WORLD, &i);
		m_device->SetTransform(D3DTS_WORLD, &finalTrans);
		m_device->SetMaterial( &m_objMtrls[*iter] );
		m_device->SetTexture(0, m_objTexs[*iter]);
		m_objMesh->DrawSubset(*iter);
	}
}

//////////////////////////////////////////////////////////////////////////
// The ChessBoard Class
//

ChessBoard::ChessBoard(IDirect3DDevice9* device, const BoardDesc &boardDesc):
m_chessMesh(NULL), 
m_desc(boardDesc),
m_device(device) {}

ChessBoard::~ChessBoard() {
	m_chessMesh->Release();
}

void ChessBoard::init() {
	
	d3d::LoadX(m_device, m_desc.XFileName.c_str(), &m_chessMesh, 
										   &m_chessMtrls, 
										   &m_chessTexs);
	std::vector<IDirect3DTexture9*> textures;
	for (unsigned int i=0; i<m_desc.TexturesFileNames.size(); i++){
		textures.push_back(NULL);
		D3DXCreateTextureFromFileA(m_device, m_desc.TexturesFileNames[i].c_str()
														, &textures[i]);
	}

	for (unsigned int i=0; i< m_desc.overriteTexs.size(); i++) {
		if (m_desc.overriteTexs[i] == true) {
			m_chessTexs[i] = textures[m_desc.subsetTexsIndex[i]];
		}
	}	

	for (int x=0; x<CHESS_DIMENTION_X;x++) {
		for (int y=0; y<CHESS_DIMENTION_Y;y++) {
			m_partsOnBoard[y][x] = NULL;
		}
	}

	// installize the parts
	for (int i=0; i<PAWN_NUMBER; i++) {
		m_pawns[i] = new ChessObject(m_chessMesh, m_chessMtrls, m_chessTexs,
			m_desc.subsetsTransform, m_desc.PawnSubsets[i], m_device, ChessPart(W,PAWN));
	}
	for (int i=PAWN_NUMBER; i<PAWN_NUMBER*2; i++) {
		m_pawns[i] = new ChessObject(m_chessMesh, m_chessMtrls, m_chessTexs,
			m_desc.subsetsTransform, m_desc.PawnSubsets[i], m_device,ChessPart(B,PAWN));
	}
	for (int i=0; i<BISHOP_NUMBER; i++) {
		m_bishops[i] = new ChessObject(m_chessMesh, m_chessMtrls, m_chessTexs,
			m_desc.subsetsTransform, m_desc.BishopSubsets[i], m_device, ChessPart(W,BISHOP));
	}
	for (int i=BISHOP_NUMBER; i<BISHOP_NUMBER*2; i++) {
		m_bishops[i] = new ChessObject(m_chessMesh, m_chessMtrls, m_chessTexs,
			m_desc.subsetsTransform, m_desc.BishopSubsets[i], m_device, ChessPart(B,BISHOP));
	}
	for (int i=0; i<KNIGHT_NUMBER; i++) {
		m_knights[i] = new ChessObject(m_chessMesh, m_chessMtrls, m_chessTexs,
			m_desc.subsetsTransform, m_desc.KnightSubsets[i], m_device, ChessPart(W,KNIGHT));
	}
	for (int i=KNIGHT_NUMBER; i<KNIGHT_NUMBER*2; i++) {
		m_knights[i] = new ChessObject(m_chessMesh, m_chessMtrls, m_chessTexs,
			m_desc.subsetsTransform, m_desc.KnightSubsets[i], m_device, ChessPart(B,KNIGHT));
	}
	for (int i=0; i<ROOK_NUMBER; i++) {
		m_rooks[i] = new ChessObject(m_chessMesh, m_chessMtrls, m_chessTexs,
			m_desc.subsetsTransform, m_desc.RookSubsets[i], m_device, ChessPart(W,ROOK));
	}
	for (int i=ROOK_NUMBER; i<ROOK_NUMBER*2; i++) {
		m_rooks[i] = new ChessObject(m_chessMesh, m_chessMtrls, m_chessTexs,
			m_desc.subsetsTransform, m_desc.RookSubsets[i], m_device, ChessPart(B,ROOK));
	}
	for (int i=0; i<KING_NUMBER; i++) {
		m_kings[i] = new ChessObject(m_chessMesh, m_chessMtrls, m_chessTexs,
			m_desc.subsetsTransform, m_desc.KingSubsets[i], m_device, ChessPart(W,KING));
	}
	for (int i=KING_NUMBER; i<KING_NUMBER*2; i++) {
		m_kings[i] = new ChessObject(m_chessMesh, m_chessMtrls, m_chessTexs,
			m_desc.subsetsTransform, m_desc.KingSubsets[i], m_device, ChessPart(B,KING));
	}
	for (int i=0; i<QUEEN_NUMBER; i++) {
		m_queens[i] = new ChessObject(m_chessMesh, m_chessMtrls, m_chessTexs,
			m_desc.subsetsTransform, m_desc.QueenSubsets[i], m_device, ChessPart(W,QUEEN));
	}
	for (int i=QUEEN_NUMBER; i<QUEEN_NUMBER*2; i++) {
		m_queens[i] = new ChessObject(m_chessMesh, m_chessMtrls, m_chessTexs,
			m_desc.subsetsTransform, m_desc.QueenSubsets[i], m_device, ChessPart(B,QUEEN));
	}

	m_board = new ChessObject(m_chessMesh, m_chessMtrls, m_chessTexs,
			m_desc.subsetsTransform, m_desc.BoardSubsets, m_device);

	//
	// Set the light
	//
	D3DXVECTOR3 pos(50.0f, 100.0f, 100.0f);
	D3DXVECTOR3 dir(-5.0f, -10.0f, -10.0f);
	D3DXCOLOR col = d3d::WHITE*0.75f;//(1.0f, 1.0f, 1.0f, 10.0f);

	::ZeroMemory(&normalLight, sizeof(normalLight));

	normalLight.Type      = D3DLIGHT_SPOT;
	normalLight.Ambient   = col* 0.4f;
	normalLight.Diffuse   = col;
	normalLight.Specular  = d3d::BLUE * 0.07f + col*0.4f;
	normalLight.Position  = pos;
	normalLight.Direction = dir;
	normalLight.Range        = 1000.0f;
	normalLight.Falloff      = 1.0f;
	normalLight.Attenuation0 = 1.0f;
	normalLight.Attenuation1 = 0.0f;
	normalLight.Attenuation2 = 0.0f;
	normalLight.Theta        = 1.5f;
	normalLight.Phi          = 2.5f;

	col = d3d::WHITE*0.7f + d3d::YELLOW * 0.3f;
	enhancedLight = d3d::InitDirectionalLight(&dir, &col);

	m_device->LightEnable(0, true);
	m_device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	m_device->SetRenderState(D3DRS_SPECULARENABLE, true);

}

void ChessBoard::emphasys(int y, int x, bool e) const {
	if (m_partsOnBoard[y][x]) {
		m_partsOnBoard[y][x]->emphasise(e);
	}
}

void ChessBoard::draw(float timeDelta) const {

	m_device->SetLight(0, &normalLight);
	m_board->draw();

	// draw only the parts that are on the board
	for (int x=0; x<CHESS_DIMENTION_X;x++) {
		for (int y=0; y<CHESS_DIMENTION_Y;y++) {
			if( m_partsOnBoard[y][x])  {
				if (m_partsOnBoard[y][x]->isEmphasised()) 
					m_device->SetLight(0, &enhancedLight);
				else m_device->SetLight(0, &normalLight);
				
				m_partsOnBoard[y][x]->draw();
			}
		}
	}
}

void ChessBoard::drawSubsets(std::vector<int> subsets) {

	std::vector<int>::const_iterator iter = subsets.begin();

	for(; iter != subsets.end(); iter++)
	{
		//m_device->SetTransform(D3DTS_WORLD, &m_desc.subsetsTransform[*iter]);
		m_device->SetMaterial( &m_chessMtrls[69] );
		m_device->SetTexture(0, m_chessTexs[*iter]);
		m_chessMesh->DrawSubset(*iter);
	}
}

void ChessBoard::setGameState(const ChessPart state[CHESS_DIMENTION_X][CHESS_DIMENTION_Y]) {
	int wPawnsIndex=0,	wBishopsIndex=0,	wKnightsIndex=0,
		wRooksIndex=0,	wKingsIndex=0,		wQueensIndex=0,
		bPawnsIndex=0,	bBishopsIndex=0,	bKnightsIndex=0,
		bRooksIndex=0,	bKingsIndex=0,		bQueensIndex=0;

	for (int x=0; x<CHESS_DIMENTION_X;x++) {
		for (int y=0; y<CHESS_DIMENTION_Y;y++) {
			m_partsOnBoard[y][x] = NULL;
		}
	}
	
	for (int y=0; y<CHESS_DIMENTION_Y;y++) {
		for (int x=0; x<CHESS_DIMENTION_X;x++) {

			if (state[y][x] == ChessPart(B,PAWN))
				m_partsOnBoard[y][x] = m_pawns[PAWN_NUMBER + wPawnsIndex++];			
			else if (state[y][x] == ChessPart(W,PAWN))	
				m_partsOnBoard[y][x] = m_pawns[bPawnsIndex++];					

			else if (state[y][x] == ChessPart(B,BISHOP)) 
				m_partsOnBoard[y][x] = m_bishops[BISHOP_NUMBER + wBishopsIndex++];	
			else if (state[y][x] == ChessPart(W,BISHOP)) 
				m_partsOnBoard[y][x] = m_bishops[bBishopsIndex++];					

			else if (state[y][x] == ChessPart(B,KNIGHT)) 
				m_partsOnBoard[y][x] = m_knights[KNIGHT_NUMBER + wKnightsIndex++];	
			else if (state[y][x] == ChessPart(W,KNIGHT)) 
				m_partsOnBoard[y][x] = m_knights[bKnightsIndex++];					

			else if (state[y][x] == ChessPart(B,ROOK)) 
				m_partsOnBoard[y][x] = m_rooks[ROOK_NUMBER + wRooksIndex++];		
			else if (state[y][x] == ChessPart(W,ROOK)) 
				m_partsOnBoard[y][x] = m_rooks[bRooksIndex++];						

			else if (state[y][x] == ChessPart(B,KING)) 
				m_partsOnBoard[y][x] = m_kings[KING_NUMBER + wKingsIndex++];		
			else if (state[y][x] == ChessPart(W,KING)) 
				m_partsOnBoard[y][x] = m_kings[bKingsIndex++];						
			
			else if (state[y][x] == ChessPart(B,QUEEN)) 
				m_partsOnBoard[y][x] = m_queens[QUEEN_NUMBER + wQueensIndex++];		
			else if (state[y][x] == ChessPart(W,QUEEN)) 
				m_partsOnBoard[y][x] = m_queens[bQueensIndex++];					
			
		}
	}
	updatePartsPos();
}

void ChessBoard::fillGameState(ChessPart state[CHESS_DIMENTION_X][CHESS_DIMENTION_Y]) const
{

	for (int x=0; x<CHESS_DIMENTION_X;x++) {
		for (int y=0; y<CHESS_DIMENTION_Y;y++) {
			if (m_partsOnBoard[y][x]) {
				state[y][x] = m_partsOnBoard[y][x]->getType();
			}  else {
				state[y][x] = ChessPart(W,EMPTY);
			}// if else ends
		} // for y ends
	} // for x ends
}

void ChessBoard::makeMove(int from_y, int from_x, int to_y, int to_x) {
	if (m_partsOnBoard[from_y][from_x] == NULL) {
		MessageBoxA(0, "No particle found. an exception shold be thrown but I'm"
			" too lazy to make one.", "BA'AL HABAIT HISHTAGE'A!!!", MB_OK);
		exit(0);
	} else {
		m_partsOnBoard[to_y][to_x] = m_partsOnBoard[from_y][from_x];
		m_partsOnBoard[from_y][from_x] = NULL;
	}
	updatePartsPos();
}

void ChessBoard::updatePartsPos() {

	for (int y=0; y<CHESS_DIMENTION_Y;y++) {
		for (int x=0; x<CHESS_DIMENTION_X;x++) {
			if (m_partsOnBoard[y][x]) {
				m_partsOnBoard[y][x]->setPostion(
					float(x)*m_desc.right + float(y)*m_desc.forward);
			}
		}
	}
}
