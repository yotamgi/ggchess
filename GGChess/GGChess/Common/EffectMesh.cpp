#include "EffectMesh.h"

using namespace d3d;


StdEffectMesh::StdEffectMesh(ID3DXMesh* mesh, std::string effectFileName,
							 std::vector<IDirect3DTexture9*> texs):
m_mesh(mesh), 
m_texs(texs),
m_effectFileName(effectFileName),
useTexure(true)
{}


StdEffectMesh::StdEffectMesh(ID3DXMesh* mesh, std::string effectFileName):
m_mesh(mesh), 
m_effectFileName(effectFileName),
useTexure(false)
{}

bool StdEffectMesh::init(IDirect3DDevice9* device) {
	m_device = device;

	//
	// Load The Effect Stuff
	//

	// create the effect
	ID3DXBuffer* errorBuffer;
	HRESULT hr = D3DXCreateEffectFromFileA(m_device, m_effectFileName.c_str(), 0, 0, 
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
	m_WorldMatrixHandle = m_effect->GetParameterByName(0, "WorldMatrix");
	m_ViewMatrixHandle  = m_effect->GetParameterByName(0, "ViewMatrix");
	m_ProjMatrixHandle  = m_effect->GetParameterByName(0, "ProjMatrix");
	m_TexHandle			= m_effect->GetParameterByName(0, "Tex");
	m_TechHandle		= m_effect->GetTechnique(0); // load the first technique

	// return false if one of the handlers is NULL
	if (!m_WorldMatrixHandle | !m_ViewMatrixHandle  | !m_ProjMatrixHandle  | 
		!m_TexHandle | !m_TechHandle) 
	{
			return false;
	} else return true;
}

bool StdEffectMesh::setTechnique(std::string technique) {

	D3DXHANDLE tmp = m_effect->GetTechniqueByName(technique.c_str());
	if (!tmp) {
		return false;
	} else {
		m_TechHandle = tmp;
		return true;	
	}
}

bool StdEffectMesh::drawSubset(int subset) const {

	D3DXMATRIX view, proj, world;

	// get the stuff from the render states:
	m_device->GetTransform(D3DTS_VIEW, &view);
	m_device->GetTransform(D3DTS_PROJECTION, &proj);
	m_device->GetTransform(D3DTS_WORLD, &world);

	// set the parameters
	m_effect->SetMatrix(m_ViewMatrixHandle, &view);
	m_effect->SetMatrix(m_ProjMatrixHandle, &proj);
	m_effect->SetMatrix(m_WorldMatrixHandle, &world);

	if (useTexure) { 

		// set the texure for the subset
		m_effect->SetTechnique(m_TechHandle);
		m_effect->SetTexture(m_TexHandle, m_texs[subset]);

		UINT passes;
		m_effect->Begin(&passes, 0);

		for (UINT i=0; i<passes; i++) {
			m_effect->BeginPass(i);
			m_mesh->DrawSubset(subset);
			m_effect->EndPass();
		}

		m_effect->End();			

	} else {
		
		UINT passes;
		m_effect->Begin(&passes, 0);

		for (UINT i=0; i<passes; i++) {
			m_effect->BeginPass(i);
			m_mesh->DrawSubset(subset);
			m_effect->EndPass();
		}

		m_effect->End();
	}

	return true;
}
