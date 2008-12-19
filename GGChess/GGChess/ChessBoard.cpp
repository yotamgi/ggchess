
#include "ChessBoard.h"

#include "EdgeDetectionEffect.h"


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
emphasised(false),
inMotion(false)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

ChessObject::ChessObject(d3d::IEffectMesh* effect_obj,
						 ID3DXMesh *object, 
						 std::vector<D3DMATERIAL9> materials, 
						 std::vector<IDirect3DTexture9*> texs, 
						 std::vector<D3DXMATRIX> transform,
						 std::vector<int> subsets,
						 IDirect3DDevice9* device,			
						 ChessPart type):
m_effect_obj(effect_obj),
m_objMesh(object),
m_objMtrls(materials),
m_objTexs(texs),
m_objTransform(transform),
m_objSubsets(subsets),
m_device(device),
m_type(type),
emphasised(false),
inMotion(false)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void ChessObject::setMovement(const D3DXVECTOR3& to) {
	D3DXVECTOR3 delta = to - m_pos;
	float length = D3DXVec3Length(&delta);
	inMotion = true;
	finalPos = to;
	maxSpeed = length;
	startAcc = length;
	stopAcc =  length*1.3f;
	speed = 0.0f;
}

void ChessObject::draw(float timeDelta) {

	update(timeDelta);

	// calulate the per-object transform
	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, m_pos.x, m_pos.y, m_pos.z);

	// for every subset
	std::vector<int>::const_iterator iter = m_objSubsets.begin();
	for(; iter != m_objSubsets.end(); iter++)
	{
		// calculate the per-subset transform
		D3DXMATRIX finalTrans = m_objTransform[*iter] * trans;

		// set the device param.
		m_device->SetTransform(D3DTS_WORLD, &finalTrans);
		m_device->SetMaterial( &m_objMtrls[*iter] );
		m_device->SetTexture(0, m_objTexs[*iter]);

		// draw!
		m_objMesh->DrawSubset(*iter);
	}
}
void ChessObject::drawWithEffect(float timeDelta)
{

	update(timeDelta);

	// calulate the per-object transform
	D3DXMATRIX trans;
	D3DXMatrixTranslation(&trans, m_pos.x, m_pos.y, m_pos.z);

	// for every subset
	std::vector<int>::const_iterator iter = m_objSubsets.begin();
	for(; iter != m_objSubsets.end(); iter++)
	{
		// calculate the per-subset transform
		D3DXMATRIX finalTrans = m_objTransform[*iter] * trans;
		m_device->SetTransform(D3DTS_WORLD, &finalTrans);

		// draw!
		m_effect_obj->drawSubset(*iter);
	}
}


void ChessObject::update(float timeDelta) {

	if (inMotion) {
		D3DXVECTOR3 delta = finalPos - m_pos, direction;
		D3DXVec3Normalize(&direction, &delta);

		// update the speed:
		if(D3DXVec3Length(&delta) < (maxSpeed*maxSpeed/stopAcc)*0.49f) {
			//speed -= timeDelta*stopAcc;
			speed = D3DXVec3Length(&delta);
		}else if (speed < maxSpeed) speed += timeDelta*startAcc;

		// update the position
		m_pos += timeDelta*direction*speed;
		if (D3DXVec3Length(&delta) <= 0.2f)
			inMotion = false;		
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

	ID3DXBuffer* adjBuff;

	//D3DXCreateSphere(m_device, 100.0f, 100, 40, &m_chessMesh, &adjBuff);
	//D3DXCreateTeapot(m_device,  &m_chessMesh, &adjBuff);

	
	d3d::LoadX(m_device, m_desc.XFileName.c_str(), &m_chessMesh, 
										   &m_chessMtrls, 
										   &m_chessTexs, &adjBuff);

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
	}//*/

	//m_effChessMesh = new d3d::StdEffectMesh(m_chessMesh, 
	//										"..\\effect.fx",
	//										m_chessTexs);
	m_effChessMesh = new EdgeDetectionEffect(m_chessMesh, adjBuff);

	if (!m_effChessMesh->init(m_device))  exit(1);


	// installize the parts
	for (int i=0; i<PAWN_NUMBER; i++) {
		m_pawns[i] = new ChessObject(m_effChessMesh, m_chessMesh, m_chessMtrls, m_chessTexs,
			m_desc.subsetsTransform, m_desc.PawnSubsets[i], m_device, ChessPart(W,PAWN));
	}
	for (int i=PAWN_NUMBER; i<PAWN_NUMBER*2; i++) {
		m_pawns[i] = new ChessObject(m_effChessMesh, m_chessMesh, m_chessMtrls, m_chessTexs,
			m_desc.subsetsTransform, m_desc.PawnSubsets[i], m_device,ChessPart(B,PAWN));
	}
	for (int i=0; i<BISHOP_NUMBER; i++) {
		m_bishops[i] = new ChessObject(m_effChessMesh, m_chessMesh, m_chessMtrls, m_chessTexs,
			m_desc.subsetsTransform, m_desc.BishopSubsets[i], m_device, ChessPart(W,BISHOP));
	}
	for (int i=BISHOP_NUMBER; i<BISHOP_NUMBER*2; i++) {
		m_bishops[i] = new ChessObject(m_effChessMesh, m_chessMesh, m_chessMtrls, m_chessTexs,
			m_desc.subsetsTransform, m_desc.BishopSubsets[i], m_device, ChessPart(B,BISHOP));
	}
	for (int i=0; i<KNIGHT_NUMBER; i++) {
		m_knights[i] = new ChessObject(m_effChessMesh, m_chessMesh, m_chessMtrls, m_chessTexs,
			m_desc.subsetsTransform, m_desc.KnightSubsets[i], m_device, ChessPart(W,KNIGHT));
	}
	for (int i=KNIGHT_NUMBER; i<KNIGHT_NUMBER*2; i++) {
		m_knights[i] = new ChessObject(m_effChessMesh, m_chessMesh, m_chessMtrls, m_chessTexs,
			m_desc.subsetsTransform, m_desc.KnightSubsets[i], m_device, ChessPart(B,KNIGHT));
	}
	for (int i=0; i<ROOK_NUMBER; i++) {
		m_rooks[i] = new ChessObject(m_effChessMesh, m_chessMesh, m_chessMtrls, m_chessTexs,
			m_desc.subsetsTransform, m_desc.RookSubsets[i], m_device, ChessPart(W,ROOK));
	}
	for (int i=ROOK_NUMBER; i<ROOK_NUMBER*2; i++) {
		m_rooks[i] = new ChessObject(m_effChessMesh, m_chessMesh, m_chessMtrls, m_chessTexs,
			m_desc.subsetsTransform, m_desc.RookSubsets[i], m_device, ChessPart(B,ROOK));
	}
	for (int i=0; i<KING_NUMBER; i++) {
		m_kings[i] = new ChessObject(m_effChessMesh, m_chessMesh, m_chessMtrls, m_chessTexs,
			m_desc.subsetsTransform, m_desc.KingSubsets[i], m_device, ChessPart(W,KING));
	}
	for (int i=KING_NUMBER; i<KING_NUMBER*2; i++) {
		m_kings[i] = new ChessObject(m_effChessMesh, m_chessMesh, m_chessMtrls, m_chessTexs,
			m_desc.subsetsTransform, m_desc.KingSubsets[i], m_device, ChessPart(B,KING));
	}
	for (int i=0; i<QUEEN_NUMBER; i++) {
		m_queens[i] = new ChessObject(m_effChessMesh, m_chessMesh, m_chessMtrls, m_chessTexs,
			m_desc.subsetsTransform, m_desc.QueenSubsets[i], m_device, ChessPart(W,QUEEN));
	}
	for (int i=QUEEN_NUMBER; i<QUEEN_NUMBER*2; i++) {
		m_queens[i] = new ChessObject(m_effChessMesh, m_chessMesh, m_chessMtrls, m_chessTexs,
			m_desc.subsetsTransform, m_desc.QueenSubsets[i], m_device, ChessPart(B,QUEEN));
	}

	m_board = new ChessObject(m_effChessMesh, m_chessMesh, m_chessMtrls, m_chessTexs,
			m_desc.subsetsTransform, m_desc.BoardSubsets, m_device);
}

void ChessBoard::emphasys(int y, int x, bool e) const {
	if (m_partsOnBoard[y][x]) {
		m_partsOnBoard[y][x]->emphasise(e);
	}
}

void ChessBoard::draw(float timeDelta) const {

	m_effChessMesh->setTechnique("SimpleVsTechnique");

	m_board->drawWithEffect(timeDelta);

	// draw only the parts that are on the board
	for (int x=0; x<CHESS_DIMENTION_X;x++) {
		for (int y=0; y<CHESS_DIMENTION_Y;y++) {
			if( m_partsOnBoard[y][x])  {

				if (m_partsOnBoard[y][x]->isEmphasised()) 
					m_effChessMesh->setTechnique("EmphasizedFixedPipeline");
				else m_effChessMesh->setTechnique("FixedPipeline");

				m_partsOnBoard[y][x]->drawWithEffect(timeDelta); 
			}
		}
	}
	//m_effChessMesh->drawSubset(12);
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
	m_partsOnBoard[to_y][to_x]->setMovement(
		float(to_x)*m_desc.right + float(to_y)*m_desc.forward);
	//updatePartsPos();
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

