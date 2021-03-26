#version 450 core

/*
Tessellation Control Shader (TCS) takes input from the VS (vertex shader) and is responsible for 2 tasks:
determining the tessellation levels to be sent to the TE (Tessellation Engine) and generating data to be passed to the TES.

            v1
            /\
           /  \
       e0 /    \ e2
         /      \
        /________\
      v2    e1    v0

TRIANGLES:
    gl_TessLevelInner:
        [0]: # of subdivisions along the edges

    gl_TessLevelOuter:
        [0]: edge 0-2
        [1]: edge 1-2
        [2]: edge 0-1


            e3
      v2 ________ v1
        |        |
     e0 |        | e2
        |        |
        |________|
      v3    e1    v0

QUADS:
    gl_TessLevelInner:
        [0]: horizontal tessellation
        [1]: vertical tessellation

    gl_TessLevelOuter:
        [0]: edge 0-3
        [1]: edge 2-3
        [2]: edge 1-2
        [3]: edge 0-1
*/

// define the number of CPs in the output patch
layout (vertices = 3) out;

in vec3 TCS_ModelPos[];
in vec3 TCS_Normal[];
in vec2 TCS_TexCoords[];

out vec3 TES_ModelPos[];
out vec3 TES_Normal[];
out vec2 TES_TexCoords[];

uniform bool riemannSphere;
uniform vec3 eyePos;
uniform mat4 model;

float GetTessLevel(float, float);
float level(vec4, vec4);

void main()
{
    
	// Set the control points of the output patch
    TES_ModelPos[gl_InvocationID] = TCS_ModelPos[gl_InvocationID];
    TES_Normal[gl_InvocationID] = TCS_Normal[gl_InvocationID];
    TES_TexCoords[gl_InvocationID] = TCS_TexCoords[gl_InvocationID];
    
    // Calculate the distance from the camera to the three control points
    float EyeToVertexDistance0 = distance(eyePos, vec3(model * vec4(TES_ModelPos[0], 1.0)));
    float EyeToVertexDistance1 = distance(eyePos, vec3(model * vec4(TES_ModelPos[1], 1.0)));
    float EyeToVertexDistance2 = distance(eyePos, vec3(model * vec4(TES_ModelPos[2], 1.0)));

    // Calculate the tessellation levels
    // To fix stitching, the point used is the opposite midpoint of the vertex
    gl_TessLevelOuter[0] = GetTessLevel(EyeToVertexDistance1, EyeToVertexDistance2);
    gl_TessLevelOuter[1] = GetTessLevel(EyeToVertexDistance2, EyeToVertexDistance0);
    gl_TessLevelOuter[2] = GetTessLevel(EyeToVertexDistance0, EyeToVertexDistance1);
    gl_TessLevelInner[0] = gl_TessLevelOuter[2];
}

float GetTessLevel(float Distance0, float Distance1)
{
    if (!riemannSphere)
        return 1;

    float AvgDistance = (Distance0 + Distance1) / 2.0;

    float maxDist = 3;
    float minDist = 1.1;
    float maxTess = 16;
    float minTess = 4;
    return int( (1 - (clamp(AvgDistance, minDist, maxDist) - minDist) / (maxDist - minDist)) * (maxTess - minTess) + minTess );

    //float AvgDistance = Distance0;
    /*
    if (AvgDistance <= 1.2)
        return 10.0;
    else if (AvgDistance <= 1.5)
        return 7.0;
    else
        return 3.0;
    */
}