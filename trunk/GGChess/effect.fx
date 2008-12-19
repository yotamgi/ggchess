// effect file for ggchess
//
// Can work with both the DirectX 9.0c fixed pipeline and with shaders
// and complicated effects.
//


//////////////////////////////////////////////////////////////////////////////////////
// Globals.
// Should be filled by DirectX application.
//

matrix WorldMatrix;
matrix ViewMatrix;
matrix ProjMatrix;

texture Tex;

vector lightDir = { -1.0f, -1.0f, -1.0f, 0.0f };


//////////////////////////////////////////////////////////////////////////////////////
// Sampler
//
sampler S0 = sampler_state {
	Texture = (Tex);
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
};


//////////////////////////////////////////////////////////////////////////////////////
// Vertex Shaders
//
 
//
// simple vertex shader
//

struct SimpleVS_INPUT {
	vector position : POSITION;
	vector normal	: NORMAL;
	float2 texCoor	: TEXCOORD0;
};

struct SimpleVS_OUTPUT {
	vector position : POSITION;
	vector diffuse	: COLOR;
	float2 texCoor	: TEXCOORD;
};

SimpleVS_OUTPUT SimpleVS(SimpleVS_INPUT input) {

	SimpleVS_OUTPUT output;
	
	// calculate the multipication of all the transform functions
	matrix worldViewProj = mul(WorldMatrix, ViewMatrix);
	worldViewProj= mul(worldViewProj, ProjMatrix);
	
	// transform the vertex
	output.position = mul(input.position, worldViewProj);
	
	// calculate the diffse color
	vector stam = { 0.5f, 0.5f, 0.5f, 0.0f }; 
	output.diffuse = stam * dot(lightDir, -1*input.normal)/2.0f + stam/1.25;
	
	// pass the texure coordinates untouched
	output.texCoor = input.texCoor;
	
	return output;
}

//////////////////////////////////////////////////////////////////////////////////////
// The Techniques
//

//
// The Fixed Pipeline technique
// 

technique FixedPipeline 
{
	pass P0 {
	
		// no shaders
		pixelshader  = null;
		vertexshader = null;
		
		// normal settings
		fvf = XYZ | Normal | Tex1;
		Lighting			= true;
		NormalizeNormals	= true;
		SpecularEnable		= false;
		
		//
		// set Transformation states
		
		WorldTransform[0]	= (WorldMatrix);
		ViewTransform		= (ViewMatrix);
		ProjectionTransform	= (ProjMatrix);
		
		//
		// set the litghts
		
		LightType[0]      		= Spot;
		LightAmbient[0]   		= { 0.3f,  0.3f,  0.3f,  1.0f };
		LightDiffuse[0]   		= { 0.75f, 0.75f, 0.75f, 1.0f };
		LightSpecular[0]  		= { 0.3f,  0.3f,  0.37f, 1.0f };
		LightPosition[0]		= { 50.0f, 100.0f, 100.0f, 0.0f };
		LightDirection[0]		= { -1.0f, -2.0f,  -2.0f };
		LightRange[0]        	= 1000.0f;
		LightFalloff[0]      	= 1.0f;
		LightAttenuation0[0] 	= 1.0f;
		LightAttenuation1[0] 	= 0.0f;
		LightAttenuation2[0] 	= 0.0f;
		LightTheta[0]        	= 1.5f;
		LightPhi[0]          	= 2.5f;
		
		LightEnable[0] = true;
		
		//
		// set the material
		MaterialAmbient		= { 1.0f, 1.0f, 1.0f, 1.0f };
		MaterialDiffuse		= { 1.0f, 1.0f, 1.0f, 1.0f };
		MaterialEmissive	= { 0.0f, 0.0f, 0.0f, 0.0f };
		MaterialPower		= 1.0f;
		MaterialSpecular	= { 1.0f, 1.0f, 1.0f, 1.0f };
		
		//
		// Sampler
		
		Sampler[0] = (S0);	
	
	}
}

technique EmphasizedFixedPipeline 
{
	pass P0 {
	
		// no shaders
		pixelshader  = null;
		vertexshader = null;
		
		// normal settings
		fvf = XYZ | Normal | Tex1;
		Lighting			= true;
		NormalizeNormals	= true;
		SpecularEnable		= false;
		
		//
		// set Transformation states
		
		WorldTransform[0]	= (WorldMatrix);
		ViewTransform		= (ViewMatrix);
		ProjectionTransform	= (ProjMatrix);
		
		//
		// set the litghts
		
		LightType[0]      		= Directional;
		LightAmbient[0]   		= { 1.0f,  0.3f,  0.3f,  1.0f };
		LightDiffuse[0]   		= { 1.0f, 0.75f, 0.75f, 1.0f };
		LightSpecular[0]  		= { 1.0f,  0.3f,  1.0f, 1.0f };
		LightDirection[0]		= { 0.0f, -1.0f,  -0.0f };
		
		LightEnable[0] = true;
		
		//
		// set the material
		
		MaterialAmbient		= { 1.0f, 1.0f, 1.0f, 1.0f };
		MaterialDiffuse		= { 1.0f, 1.0f, 1.0f, 1.0f };
		MaterialEmissive	= { 0.0f, 0.0f, 0.0f, 0.0f };
		MaterialPower		= 1.0f;
		MaterialSpecular	= { 1.0f, 1.0f, 1.0f, 1.0f };
		
		//
		// Sampler
		
		Sampler[0] = (S0);		
	}
}

//
// simple vertexShader technique
//


technique SimpleVsTechnique 
{
	pass P0 {
	
		// no shaders
		VertexShader = compile vs_1_1 SimpleVS();
		PixelShader  = null;
		
		Sampler[0] = (S0);	
			
	}
}