#ifndef __CHESS_BOARD__
#define __CHESS_BOARD__

#include <string>
#include "common/d3dUtility.h"
#include "common/EffectMesh.h"
#include "BoardModelDescs.h"
#include "common/D3dTextConsole.h"

class ChessObject {
public:
	ChessObject(ID3DXMesh* object,
		std::vector<D3DMATERIAL9> materials, 
		std::vector<IDirect3DTexture9*> texs,
		std::vector<D3DXMATRIX> transform,
		std::vector<int> subsets,
		IDirect3DDevice9* device,
		ChessPart type = ChessPart(W, EMPTY));

	ChessObject(d3d::IEffectMesh* effect_obj,
		ID3DXMesh* object,
		std::vector<D3DMATERIAL9> materials, 
		std::vector<IDirect3DTexture9*> texs,
		std::vector<D3DXMATRIX> transform,
		std::vector<int> subsets,
		IDirect3DDevice9* device,
		ChessPart type = ChessPart(W, EMPTY));

	void draw(float timeDelta);
	void drawWithEffect(float timeDelta);

	ChessPart getType() const { return m_type; }

	void setPostion(const D3DXVECTOR3& pos) {m_pos = pos; }
	D3DXVECTOR3 getPostion() const { return m_pos; }

	void setMovement(const D3DXVECTOR3& to);

	void emphasise(bool e) const { emphasised = e; } 
	bool isEmphasised() const { return emphasised; }

private:

	void update(float timeDelta);

	ID3DXMesh*						m_objMesh;
	std::vector<D3DMATERIAL9>		m_objMtrls;
	std::vector<IDirect3DTexture9*> m_objTexs;
	std::vector<D3DXMATRIX>			m_objTransform;
	std::vector<int>				m_objSubsets;
	d3d::IEffectMesh*				m_effect_obj;

	// The motion vars
	bool		inMotion;
	D3DXVECTOR3 finalPos;
	float maxSpeed, startAcc, stopAcc, speed;

	mutable bool emphasised; // this var can be changed even if the object is const.

	/**
	 * The whole object position.
	 */
	D3DXVECTOR3 m_pos;
	
	ChessPart m_type;

	IDirect3DDevice9* m_device;
};


class ChessBoard {
public:
	ChessBoard(IDirect3DDevice9* device, const BoardDesc &boardDesc);
	~ChessBoard();

	void init();

	void draw(float timeDelta) const ;

	/**
	 * Sets the board to a specific state.
	 * No Animation will be used to set it.
	 */
	void setGameState (const ChessPart state[CHESS_DIMENTION_Y][CHESS_DIMENTION_X]);
	void fillGameState(ChessPart state[CHESS_DIMENTION_Y][CHESS_DIMENTION_X]) const;

	/** 
	 * makes Move.
	 * might use some animation...
	 */
	void makeMove(int from_y, int from_x, int to_y, int to_x);

	/**
	 * Enlights the (x,y) part if exists
	 */
	void emphasys(int y, int x, bool e) const;

	D3DXPLANE getBoardPlane() const { return m_desc.boardPlane; }
	
private:

	/**
	 * Helper Function.
	 * Draws a vector of subsets from the mesh m_chessMesh.
	 */
	void drawSubsets(std::vector<int> subsets);

	/** The Chess Objects. */
	ChessObject* m_pawns[PAWN_NUMBER*2];
	ChessObject* m_bishops[BISHOP_NUMBER*2];
	ChessObject* m_knights[KNIGHT_NUMBER*2];
	ChessObject* m_rooks[ROOK_NUMBER*2];
	ChessObject* m_kings[KING_NUMBER*2];
	ChessObject* m_queens[QUEEN_NUMBER*2];
	ChessObject* m_board;

	/** The parts in a board' like  array; */ 
	ChessObject* m_partsOnBoard[CHESS_DIMENTION_Y][CHESS_DIMENTION_X];

	/** Update The parts pos according to m_partsOnBoard. */
	void updatePartsPos();

	/** The main Mesh with its corresponding textures and material. */
	ID3DXMesh*						m_chessMesh;
	std::vector<D3DMATERIAL9>		m_chessMtrls;
	std::vector<IDirect3DTexture9*> m_chessTexs;
	d3d::IEffectMesh*				m_effChessMesh;

	/** The Board Description struct, filled with data. */
	const BoardDesc m_desc;

	D3DXHANDLE m_RegTechHandle, m_EmphTechHandle;
	D3DXHANDLE m_WorldMatrixHandle;
	D3DXHANDLE m_ViewMatrixHandle;
	D3DXHANDLE m_ProjMatrixHandle;
	D3DXHANDLE m_TexHandle;
	
	/** The Direct3D 9.0c Device */
	IDirect3DDevice9* m_device;
};

#endif // __CHESS_BOARD__
