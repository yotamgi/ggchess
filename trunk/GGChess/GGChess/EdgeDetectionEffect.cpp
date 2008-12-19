
#include "EdgeDetectionEffect.h"

//
// EdgeDetectionEffect fields implementation
//


EdgeDetectionEffect::EdgeDetectionEffect(ID3DXMesh* mesh, 
										 ID3DXBuffer* meshAdjBuff):
m_mesh(mesh),
m_adjBuff(meshAdjBuff)
{}


bool EdgeDetectionEffect::init(IDirect3DDevice9* device){
	m_device = device;

	fillVertexBuffer();
	fillIndexBuffer();

	//
	// create the vertexDeclaration
	//

	HRESULT hr = 0;

	D3DVERTEXELEMENT9 decl[] = 
	{
		// offsets in bytes
		{0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
		{0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   1},
		{0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   2},
		D3DDECL_END()
	};

	hr = m_device->CreateVertexDeclaration(decl, &m_decl);
	if(FAILED(hr))
	{
		::MessageBoxA(0, "CreateVertexDeclaration() - FAILED", 0, 0);
		return false;
	}

	//
	// Load The Effect Stuff
	//

	// create the effect
	ID3DXBuffer* errorBuffer;
	hr = D3DXCreateEffectFromFileA(m_device, "../EdgeDetection.fx", 0, 0, 
										D3DXSHADER_DEBUG, 0, &m_effect, &errorBuffer);
	if (errorBuffer) {
		MessageBoxA(0, (char*)errorBuffer->GetBufferPointer(), "Effect Error", MB_OK);
		return false;
	}
	if ( FAILED(hr) ) {
		MessageBoxA(0, "D3DXCreateEffectFromFile - faild.", "Fatel Error", MB_OK);
		return false;
	}

	// create the handles
	m_WVMatrixHandle = m_effect->GetParameterByName(0, "WorldViewMatrix");
	m_ProjMatrixHandle  = m_effect->GetParameterByName(0, "ProjMatrix");
	m_TechHandle		= m_effect->GetTechnique(0); // load the first technique

	// return false if one of the handlers is NULL
	if (!m_WVMatrixHandle | !m_ProjMatrixHandle  | !m_TechHandle)  {
		return false;
	} else return true;
}


bool EdgeDetectionEffect::drawSubset(int subset) const  {
	//
	// find the subset vertices.
	//
	DWORD tableSize=1;
	m_mesh->GetAttributeTable(NULL, &tableSize);
	D3DXATTRIBUTERANGE* table = new D3DXATTRIBUTERANGE[tableSize];
	m_mesh->GetAttributeTable(table, &tableSize);

	//
	// calculate the transform and set the effect global params
	//
	D3DXMATRIX view, proj, world;

	// get the stuff from the render states:
	m_device->GetTransform(D3DTS_VIEW, &view);
	m_device->GetTransform(D3DTS_PROJECTION, &proj);
	m_device->GetTransform(D3DTS_WORLD, &world);

	D3DXMATRIX worldView = world*view;

	// set the parameters
	m_effect->SetMatrix(m_WVMatrixHandle, &worldView);
	m_effect->SetMatrix(m_ProjMatrixHandle, &proj);

	//
	// draw with the effect
	//
	m_device->SetVertexDeclaration(m_decl);
	m_device->SetStreamSource(0, m_vertices, 0, sizeof(EdgeDetectVertex));
	m_device->SetIndices(m_indices);

	// set the technique for the subset
	m_effect->SetTechnique(m_TechHandle);

	UINT passes;
	m_effect->Begin(&passes, 0);

	m_effect->BeginPass(0);

	m_device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST, table[subset].FaceStart*12, 0,
		m_VerticesNum, 0, table[subset].FaceCount*6);

	m_effect->EndPass();

	m_effect->End();			

	return true;
}


void EdgeDetectionEffect::fillVertexBuffer() {

	DWORD MeshVertSize = m_mesh->GetNumBytesPerVertex();

	m_VerticesNum = m_mesh->GetNumFaces() * 3 * 4;
	m_device->CreateVertexBuffer(m_VerticesNum * sizeof(EdgeDetectVertex), 
								D3DUSAGE_WRITEONLY, 0, 
								D3DPOOL_MANAGED, 
								&m_vertices, 0); 

	
	// lock the mesh vertices and indices
	MeshVertex* mesh_vert;
	m_mesh->LockVertexBuffer(0, (void**)&mesh_vert);
	DWORD* mesh_indices;
	m_mesh->LockIndexBuffer(0, (void**)&mesh_indices);

	// lock the m_vertices indices
	EdgeDetectVertex* vertex;
	m_vertices->Lock(0, 0, (void**)&vertex, 0);

	// for every face
	for (unsigned int face=0; face < m_mesh->GetNumFaces(); face++) {
		D3DXVECTOR3 currFaceNurmal;
		D3DXVECTOR3 adjFaceNurmal[3];

		// calulate the normals
		getFaceNormals(m_mesh, m_adjBuff, &currFaceNurmal, adjFaceNurmal, face);	

		// get the indices for this face
		DWORD i0 = mesh_indices[face*3 + 0];
		DWORD i1 = mesh_indices[face*3 + 1];
		DWORD i2 = mesh_indices[face*3 + 2];

		// get the verices
		MeshVertex v0 = *(MeshVertex*)((BYTE*)mesh_vert + i0*MeshVertSize);
		MeshVertex v1 = *(MeshVertex*)((BYTE*)mesh_vert + i1*MeshVertSize);
		MeshVertex v2 = *(MeshVertex*)((BYTE*)mesh_vert + i2*MeshVertSize);

		// A2 and B2 are duplication of A and B
		EdgeDetectVertex A, B, A2, B2;
		
		// for the edge v0 -> v1
		A.pos	= v0.position;
		A.n		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		A.n1	= currFaceNurmal;
		A.n2	= adjFaceNurmal[0];

		B.pos	= v1.position;
		B.n		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		B.n1	= currFaceNurmal;
		B.n2	= adjFaceNurmal[0];
		
		A2 = A;	A2.n = v0.normal;
		B2 = B;	B2.n = v1.normal;
		*vertex = A;	vertex++;
		*vertex = B;	vertex++;
		*vertex = A2;	vertex++;
		*vertex = B2;	vertex++;


		// for the edge v1 -> v2
		A.pos	= v1.position;
		A.n		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		A.n1	= currFaceNurmal;
		A.n2	= adjFaceNurmal[1];

		B.pos	= v2.position;
		B.n		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		B.n1	= currFaceNurmal;
		B.n2	= adjFaceNurmal[1];
		
		A2 = A;	A2.n = v1.normal;
		B2 = B;	B2.n = v2.normal;
		*vertex = A;	vertex++;
		*vertex = B;	vertex++;
		*vertex = A2;	vertex++;
		*vertex = B2;	vertex++;

		// for the edge v0 -> v2
		A.pos	= v0.position;
		A.n		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		A.n1	= currFaceNurmal;
		A.n2	= adjFaceNurmal[2];

		B.pos	= v2.position;
		B.n		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		B.n1	= currFaceNurmal;
		B.n2	= adjFaceNurmal[2];
		
		A2 = A;	A2.n = v0.normal;
		B2 = B;	B2.n = v2.normal;
		*vertex = A;	vertex++;
		*vertex = B;	vertex++;
		*vertex = A2;	vertex++;
		*vertex = B2;	vertex++;

	}

	m_mesh->UnlockVertexBuffer();
	m_mesh->UnlockIndexBuffer();
	m_vertices->Unlock();


}

void EdgeDetectionEffect::fillIndexBuffer() {
	DWORD numEdges = m_mesh->GetNumFaces() * 3;

	m_FacesNum = numEdges * 2;
	
	m_device->CreateIndexBuffer(
		numEdges * 6 * sizeof(DWORD), // 2 triangles per edge
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,
		D3DPOOL_MANAGED,
		&m_indices,
		0);

	DWORD* indices = 0;

	m_indices->Lock(0, 0, (void**)&indices, 0);

	// 0        1
	// *--------*
	// |  edge  |
	// *--------*
	// 2        3

	for(UINT i = 0; i < numEdges; i++)
	{
		// Six indices to define the triangles of the edge,
		// so every edge we skip six entries in the 
		// index buffer.  Four vertices to define the edge,
		// so every edge we skip four entries in the 
		// vertex buffer.
		indices[i * 6]     = i * 4 + 0;
		indices[i * 6 + 1] = i * 4 + 1;
		indices[i * 6 + 2] = i * 4 + 2;
		indices[i * 6 + 3] = i * 4 + 1;
		indices[i * 6 + 4] = i * 4 + 3;
		indices[i * 6 + 5] = i * 4 + 2;
	}

	m_indices->Unlock();

}

void EdgeDetectionEffect::getFaceNormal(ID3DXMesh* mesh,
							DWORD faceIndex,
							D3DXVECTOR3* faceNormal)
{
	DWORD MeshVertSize = m_mesh->GetNumBytesPerVertex();

	MeshVertex* vertices = 0;
	mesh->LockVertexBuffer(0, (void**)&vertices);

	DWORD* indices = 0;
	mesh->LockIndexBuffer(0, (void**)&indices);

	// Get the triangle's indices
	DWORD i0 = indices[faceIndex * 3 + 0];
	DWORD i1 = indices[faceIndex * 3 + 1];
	DWORD i2 = indices[faceIndex * 3 + 2];

	// Now extract the triangles vertices positions
	D3DXVECTOR3 v0 = ((MeshVertex*)((BYTE*)vertices + i0*MeshVertSize))->position;
	D3DXVECTOR3 v1 = ((MeshVertex*)((BYTE*)vertices + i1*MeshVertSize))->position;
	D3DXVECTOR3 v2 = ((MeshVertex*)((BYTE*)vertices + i2*MeshVertSize))->position;

	// Compute face normal
	D3DXVECTOR3 edge0, edge1;
	edge0 = v1 - v0;
	edge1 = v2 - v0;
	D3DXVec3Cross(faceNormal, &edge0, &edge1);
	D3DXVec3Normalize(faceNormal, faceNormal);

	mesh->UnlockVertexBuffer();
	mesh->UnlockIndexBuffer();
}


void EdgeDetectionEffect::getFaceNormals(ID3DXMesh*   mesh,
								     ID3DXBuffer* adjBuffer,
									 D3DXVECTOR3* currentFaceNormal,
									 D3DXVECTOR3  adjFaceNormals[3],
									 DWORD        faceIndex)

{
	DWORD MeshVertSize = m_mesh->GetNumBytesPerVertex();
	MeshVertex* vertices = 0;
	mesh->LockVertexBuffer(0, (void**)&vertices);

	DWORD* indices = 0;
	mesh->LockIndexBuffer(0, (void**)&indices);

	DWORD* adj = (DWORD*)adjBuffer->GetBufferPointer();

	//
	// Get the face normal.
	getFaceNormal(mesh, faceIndex, currentFaceNormal);

	//
	// Get adjacent face indices
	DWORD faceIndex0 = adj[faceIndex * 3];
	DWORD faceIndex1 = adj[faceIndex * 3 + 1];
	DWORD faceIndex2 = adj[faceIndex * 3 + 2];

	//
	// Get adjacent face normals, if there is no adjacent face,
	// then set the adjacent face normal to the opposite of the
	// "currentFaceNormal".  Recall we do this because edges that
	// don't have an adjacent triangle are automatically considered
	// silhouette edges.  And in order to make that happen, we need
	// the current face normal and adjacent face normal to point
	// in the opposite direction.  Also, recall that an entry
	// in the adjacency buffer equal to -1 denotes that the edge
	// doesn't have an adjacent triangle.

	D3DXVECTOR3 faceNormal0, faceNormal1, faceNormal2;
    
	if( faceIndex0 != USHRT_MAX ) // is there an adjacent triangle?
	{
		DWORD i0 = indices[faceIndex0 * 3];
		DWORD i1 = indices[faceIndex0 * 3 + 1];
		DWORD i2 = indices[faceIndex0 * 3 + 2];

		D3DXVECTOR3 v0 = ((MeshVertex*)((BYTE*)vertices + i0*MeshVertSize))->position;
		D3DXVECTOR3 v1 = ((MeshVertex*)((BYTE*)vertices + i1*MeshVertSize))->position;
		D3DXVECTOR3 v2 = ((MeshVertex*)((BYTE*)vertices + i2*MeshVertSize))->position;

		D3DXVECTOR3 edge0 = v1 - v0;
		D3DXVECTOR3 edge1 = v2 - v0;
		D3DXVec3Cross(&faceNormal0, &edge0, &edge1);
		D3DXVec3Normalize(&faceNormal0, &faceNormal0);
	}
	else
	{
		faceNormal0 = -(*currentFaceNormal);
	}

	if( faceIndex1 != USHRT_MAX ) // is there an adjacent triangle?
	{
		DWORD i0 = indices[faceIndex1 * 3];
		DWORD i1 = indices[faceIndex1 * 3 + 1];
		DWORD i2 = indices[faceIndex1 * 3 + 2];

		D3DXVECTOR3 v0 = ((MeshVertex*)((BYTE*)vertices + i0*MeshVertSize))->position;
		D3DXVECTOR3 v1 = ((MeshVertex*)((BYTE*)vertices + i1*MeshVertSize))->position;
		D3DXVECTOR3 v2 = ((MeshVertex*)((BYTE*)vertices + i2*MeshVertSize))->position;

		D3DXVECTOR3 edge0 = v1 - v0;
		D3DXVECTOR3 edge1 = v2 - v0;
		D3DXVec3Cross(&faceNormal1, &edge0, &edge1);
		D3DXVec3Normalize(&faceNormal1, &faceNormal1);
	}
	else
	{
		faceNormal1 = -(*currentFaceNormal);
	}

	if( faceIndex2 != USHRT_MAX ) // is there an adjacent triangle?
	{
		DWORD i0 = indices[faceIndex2 * 3];
		DWORD i1 = indices[faceIndex2 * 3 + 1];
		DWORD i2 = indices[faceIndex2 * 3 + 2];

		D3DXVECTOR3 v0 = ((MeshVertex*)((BYTE*)vertices + i0*MeshVertSize))->position;
		D3DXVECTOR3 v1 = ((MeshVertex*)((BYTE*)vertices + i1*MeshVertSize))->position;
		D3DXVECTOR3 v2 = ((MeshVertex*)((BYTE*)vertices + i2*MeshVertSize))->position;

		D3DXVECTOR3 edge0 = v1 - v0;
		D3DXVECTOR3 edge1 = v2 - v0;
		D3DXVec3Cross(&faceNormal2, &edge0, &edge1);
		D3DXVec3Normalize(&faceNormal2, &faceNormal2);
	}
	else
	{
		faceNormal2 = -(*currentFaceNormal);
	}

	// save adjacent face normals
	adjFaceNormals[0] = faceNormal0;
	adjFaceNormals[1] = faceNormal1;
	adjFaceNormals[2] = faceNormal2;

	mesh->UnlockVertexBuffer();
	mesh->UnlockIndexBuffer();
}