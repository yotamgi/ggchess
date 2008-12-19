#ifndef __EDGE_DETECTION_EFFECT_H__
#define __EDGE_DETECTION_EFFECT_H__

#include <d3dx9.h>
#include "common\\EffectMesh.h"

struct EdgeDetectVertex {

	// position
	D3DXVECTOR3 pos;

	// normal 2
	D3DXVECTOR3 n;

	// normal 1
	D3DXVECTOR3 n1;

	// normal 0
	D3DXVECTOR3 n2;
};

struct MeshVertex
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
};


class EdgeDetectionEffect : public d3d::IEffectMesh {
public:
	EdgeDetectionEffect(ID3DXMesh* mesh, ID3DXBuffer* meshAdjBuff);
	bool init(IDirect3DDevice9* device);

	bool setTechnique(std::string technique) {return false; }

	bool drawSubset(int subset) const;

private:
	// helper funcs
	void fillVertexBuffer();
	void fillIndexBuffer();
	
	void getFaceNormals(ID3DXMesh*   mesh,
					ID3DXBuffer* adjBuffer,
					D3DXVECTOR3* currentFaceNormal,
					D3DXVECTOR3  adjFaceNormals[3],
					DWORD        faceIndex);

	void getFaceNormal(	ID3DXMesh* mesh,
					DWORD faceIndex,
					D3DXVECTOR3* faceNormal);


	IDirect3DDevice9* m_device;

	// the poligon info
	IDirect3DVertexBuffer9* m_vertices;
	IDirect3DIndexBuffer9*  m_indices;
	DWORD m_VerticesNum;
	DWORD m_FacesNum;
	IDirect3DVertexDeclaration9* m_decl;

	// The effect stuff
	ID3DXEffect*	m_effect;
	D3DXHANDLE		m_TechHandle;
	D3DXHANDLE		m_WVMatrixHandle;
	D3DXHANDLE		m_ProjMatrixHandle;
	
	// unused after initializing
	ID3DXMesh*		m_mesh;
	ID3DXBuffer*	m_adjBuff;
};


#endif // __EDGE_DETECTION_EFFECT_H__