#ifndef  __EFFECT_MESH__
#define __EFFECT_MESH__

#include <d3dx9.h>
#include <string>
#include <vector>

namespace d3d {

	/**
	 * An interface for effect meshes.
	 * 
	 * The setTechnique is optional and if not called, the first technique is the 
	 * default.
	 */ 
	class IEffectMesh  {
	public:

		virtual bool init(IDirect3DDevice9* device) =0;

		virtual bool setTechnique(std::string technique) =0;

		virtual bool drawSubset(int subset) const =0;

	};

	/**
	 * A Standart Effect Mesh.
	 * It applies some constraints on the effect file:
	 * 
	 *  1. It should have the global vars: WorldMatrix, ViewMatrix, ProjMatrix,
	 *	   while Tex is opional. if you specify a texure in the constor, then 
	 *     you must have it in your fx file and vice verssa.
	 * 	   The global variables should refer to their obvious meannin.
	 */
	class StdEffectMesh : public IEffectMesh {
	public:
		// construct with texture
		StdEffectMesh(ID3DXMesh* mesh, 
					  std::string effectFileName, 
					  std::vector<IDirect3DTexture9*> texs);

		// construct without
		StdEffectMesh(ID3DXMesh* mesh, 
					  std::string effectFileName);

		virtual bool init(IDirect3DDevice9* device);

		virtual bool setTechnique(std::string technique);

		virtual bool drawSubset(int subset) const;

	private:

		// The mesh
		ID3DXMesh*	 m_mesh;

		// The textures
		std::vector<IDirect3DTexture9*> m_texs;

		// The effect stuff
		ID3DXEffect*	m_effect;
		D3DXHANDLE		m_TechHandle;
		D3DXHANDLE		m_WorldMatrixHandle;
		D3DXHANDLE		m_ViewMatrixHandle;
		D3DXHANDLE		m_ProjMatrixHandle;
		D3DXHANDLE		m_TexHandle;

		std::string m_effectFileName;

		bool useTexure;

		// the Direct3D device
		IDirect3DDevice9* m_device;
	};

} // ended namespace d3d

#endif // __EFFECT_MESH__

