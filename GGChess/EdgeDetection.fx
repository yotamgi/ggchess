// effect file for ggchess
//
// Can work with both the DirectX 9.0c fixed pipeline and with shaders
// and complicated effects.
//


//////////////////////////////////////////////////////////////////////////////////////
// Globals.
// Should be filled by DirectX application.
//

extern matrix WorldViewMatrix;
extern matrix ProjMatrix;


static vector White = {1.0f, 1.0f, 1.0f, 1.0f};

//////////////////////////////////////////////////////////////////////////////////////
// Vertex Shaders
//
 
//
// Edge Detection shader
//

struct ED_INPUT {
    vector position    : POSITION;
    vector normal      : NORMAL0;
    vector faceNormal1 : NORMAL1; 
    vector faceNormal2 : NORMAL2;
};

struct ED_OUTPUT {
    vector position : POSITION;
    vector diffuse  : COLOR;
};

ED_OUTPUT EdgeDetection(ED_INPUT input) {
    // zero out each member in output
    ED_OUTPUT output = (ED_OUTPUT)0;

    // transform position to view space
    input.position = mul(input.position, WorldViewMatrix);

    // Compute a vector in the direction of the vertex
    // from the eye.  Recall the eye is at the origin
    // in view space - eye is just camera position.
    vector eyeToVertex = input.position;

    // transform normals to view space.  Set w
    // components to zero since we're transforming vectors.
    // Assume there are no scalings in the world
    // matrix as well.
    input.normal.w      = 0.0f;
    input.faceNormal1.w = 0.0f;
    input.faceNormal2.w = 0.0f;

    input.normal      = mul(input.normal,      WorldViewMatrix);
    input.faceNormal1 = mul(input.faceNormal1, WorldViewMatrix);
    input.faceNormal2 = mul(input.faceNormal2, WorldViewMatrix);

    // compute the cosine of the angles between
    // the eyeToVertex vector and the face normals.
    float dot0 = dot(eyeToVertex, input.faceNormal1);
    float dot1 = dot(eyeToVertex, input.faceNormal2);

    // if cosines are different signs (positive/negative)
    // than we are on a silhouette edge.  Do the signs
    // differ?
    if( (dot0 * dot1) < 0.0f )
    {
         // yes, then this vertex is on a silhouette edge, 
         // offset the vertex position by some scalar in the
         // direction of the vertex normal.
         input.position += 0.2f * input.normal;
    }

    // transform to homogeneous clip space
    output.position = mul(input.position, ProjMatrix);

    // set outline color
    output.diffuse = White;
    
    return output;
 
}


//////////////////////////////////////////////////////////////////////////////////////
// The Techniques
//

technique EdgeDetectionTechnique 
{
	pass P0 {
	
		CullMode = none;
	
		// no shaders
		VertexShader = compile vs_1_1 EdgeDetection();
		PixelShader  = null;
	}
}