#version 450 core

#define M_PI 3.1415926535897932384626433832795
#define NR_POINT_LIGHTS 4

#define FRAC_CUSTOM						0
#define FRAC_MANDELBROT					1
#define FRAC_LAMBDA						2

#define TRAP_CUSTOM						0
#define TRAP_CIRCLE						1
#define TRAP_SQUARE						2
#define TRAP_RECTANGLE					3
#define TRAP_REAL						4
#define TRAP_IMAG						5
#define TRAP_POINT						6
#define TRAP_LINE						7
#define TRAP_CROSS						8

#define COL_CUSTOM						0
#define COL_BLACK						1
#define COL_WHITE						2
#define COL_CLASSIC                     3
#define COL_SMOOTH                      4
#define COL_DOMAIN_SIMPLE               5
#define COL_DOMAIN_NORMAL               6
#define COL_DOMAIN_BRIGHTNESS           7
#define COL_DOMAIN_BRIGHT_RINGS         8
#define COL_DOMAIN_BRIGHT_RINGS_SMOOTH  9
#define COL_DOMAIN_DARK_RINGS           10
#define COL_DOMAIN_BRIGHT_DARK_SMOOTH   11

/*
Tessellation Evaluation Shader is the last link in the process, taking each
new vertex created by the tessellation and giving it a position within the worldS.
*/

layout(triangles, equal_spacing, ccw) in;


in vec3 TES_ModelPos[];
in vec3 TES_Normal[];
in vec2 TES_TexCoords[];

out vec3 FragPosModel;
out vec3 FragPosWorld;
out vec3 Normal;
out vec2 TexCoords;

out vec3 juliaColor;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform float zoom;
uniform vec2 radius;
uniform vec2 center;
uniform float rollAngle;

uniform int fractalType;
uniform int orbitTrap;
uniform int exteriorColoring;
uniform int interiorColoring;

uniform float foldCount;
uniform float foldAngle;
uniform vec2 foldOffset;

uniform vec2 julia;
uniform int maxIterations;
uniform float maxDistance;
uniform float distFineness;
uniform float power;
uniform float c_power;
uniform vec4 bailout;
uniform bool useDistance;
uniform bool isConjugate;

uniform bool riemannSphere;
uniform float terrain;


vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2);
vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2);
float Julia();


void main()
{
    // Interpolate the attributes of the output vertex using the barycentric coordinates
    FragPosModel = normalize(interpolate3D(TES_ModelPos[0], TES_ModelPos[1], TES_ModelPos[2]));
    //Normal = mat3(transpose(inverse(model))) * interpolate3D(TES_Normal[0], TES_Normal[1], TES_Normal[2]);
    Normal = interpolate3D(TES_Normal[0], TES_Normal[1], TES_Normal[2]);
    TexCoords = interpolate2D(TES_TexCoords[0], TES_TexCoords[1], TES_TexCoords[2]);

    
    if (riemannSphere)
    {
        if (terrain != 0)
        {
            float scale = terrain;

            FragPosModel += Julia() * Normal * scale;
        }
        
        FragPosWorld = vec3(model * vec4(FragPosModel, 1.0));
        gl_Position = projection * view * vec4(FragPosWorld, 1.0);
    }
    else
    {
        gl_Position = vec4(FragPosModel, 1.0);

	    float a = rollAngle * M_PI / 180;
	    float s = sin(a);
	    float c = cos(a);
	    vec2 newPos = vec2(FragPosModel.x*c - FragPosModel.y*s, FragPosModel.y*c + FragPosModel.x*s);

        //FragPos = aPos.xy;
        FragPosModel = vec3(radius*FragPosModel.xy/zoom, 0.0);
	    FragPosModel = vec3(center + vec2(FragPosModel.x*c - FragPosModel.y*s, FragPosModel.y*c + FragPosModel.x*s), 0.0);
    }
}


vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

bool IsBad(vec2 z)
{
    return isnan(z.x) || isnan(z.y) || isinf(z.x) || isinf(z.y) || isinf(length(z));
}

vec2 c_mul(vec2 a, vec2 b)
{
    return vec2(a.x*b.x - a.y*b.y, a.x*b.y + a.y*b.x);
}

vec2 c_div(vec2 a, vec2 b)
{
    return vec2(((a.x*b.x+a.y*b.y)/(b.x*b.x+b.y*b.y)),((a.y*b.x-a.x*b.y)/(b.x*b.x+b.y*b.y)));
}

vec2 c_2(vec2 c)
{
    return vec2(c.x*c.x - c.y*c.y, 2*c.x*c.y);
}

vec2 c_pow(vec2 z, float a)
{
	if (z.x == 0 && z.y == 0)
        return z;
	else if (a == 1)
        return z;
	else if (a == 2)
        return c_2(z);
    else if (int(a) == a)
    {
        vec2 original = z;
        float p = abs(a);

        for (p; p - 1 > 0; p--)
            z = c_mul(z, original);

        if (a < 0)
            return c_div(vec2(1,0), z);

        return z;
    }
    
    float r = length(z);
    float theta = atan(z.y, z.x);   
    return pow(r, a) * vec2(cos(theta * a), sin(theta * a));
}

vec2 c_sqrt(vec2 c)
{
    float r = length(c);
    float a = 1/sqrt(2);
    
    //return vec2(a * sqrt(r + c.x), sign(c.y) * a * sqrt(r - c.x));
    return .5 * sqrt(2) * vec2(sqrt(r + c.x), sign(c.y) * a * sqrt(r - c.x));
}

vec2 RotateZ(vec2 z, float a)
{
   return c_mul(z, vec2(cos(a), sin(a)));
}

vec2 FoldZ(vec2 z)
{
    if (foldCount == 0)
        return z;
    
    vec2 new_z = z;
    /*
    // Translate
    new_z += foldOffset;
    
    // Rotate
    //new_z = RotateZ(new_z, foldAngle);
    new_z = RotateZ(new_z, -foldAngle);
    // Initial Fold
    new_z.y = abs(new_z.y);

    
    for (int i = 0; i < foldCount - 1; i++)
    {
        // Rotate
        //new_z = c_mul(new_z, vec2(cos(foldAngle), sin(foldAngle)));
        //new_z = RotateZ(new_z, 2*M_PI / foldCount);
        new_z = RotateZ(new_z, M_PI / foldCount);
        //new_z = RotateZ(new_z, -M_PI / foldCount);
        // Fold
        new_z.y = abs(new_z.y);
    }
    
    //
    //new_z.y = -new_z.y;
    new_z = RotateZ(new_z, -M_PI / 2);
    new_z.y = abs(new_z.y);
    //new_z.x = abs(new_z.x);

    // Unrotate
    new_z = RotateZ(new_z, foldAngle + M_PI);
    //new_z = c_mul(new_z, vec2(cos(-foldAngle*count), sin(-foldAngle*count)));
    //new_z = RotateZ(new_z, foldAngle + M_PI);
    //new_z = RotateZ(new_z, -foldAngle - M_PI);
    //new_z = RotateZ(new_z, foldAngle);
    //new_z = RotateZ(new_z, -foldAngle);

    // Untranslate
    new_z -= foldOffset;
*/
    
    // 1
    /*
    new_z = RotateZ(new_z, foldAngle);
    new_z.y = abs(new_z.y);
    new_z = RotateZ(new_z, -foldAngle);
    */
    
    // 2
    /*
    new_z = RotateZ(new_z, foldAngle);

    new_z.y = abs(new_z.y);
    new_z = RotateZ(new_z, -M_PI / 2);
    new_z.y = abs(new_z.y);
    new_z = RotateZ(new_z, M_PI / 2);

    new_z = RotateZ(new_z, -foldAngle);
    */
    
    // 3
    /*
    new_z = RotateZ(new_z, foldAngle);

    new_z.y = abs(new_z.y);
    new_z = RotateZ(new_z, -M_PI / 3);
    new_z.y = abs(new_z.y);
    new_z = RotateZ(new_z, -M_PI / 3);
    new_z.y = abs(new_z.y);
    new_z = RotateZ(new_z, 2*M_PI / 3);

    new_z = RotateZ(new_z, -foldAngle);
    */


    // N

    bool doWeFoldFirst = foldCount < 0 || mod(foldCount,2) >= 1;

    // Rotate (constant)
    new_z = RotateZ(new_z, foldAngle);
    // Translate
    new_z += foldOffset;
    // Base fold
    if (doWeFoldFirst)
        new_z.y = abs(new_z.y);

    for (int i = 0; i < foldCount - 1 * int(doWeFoldFirst); i++)
    {
        // Rotate
        new_z = RotateZ(new_z, -M_PI / foldCount);
        // Fold
        new_z.y = abs(new_z.y);
    }

    // Untranslate
    new_z -= foldOffset;
    // Unrotate (both the constant and folding angles)
    new_z = RotateZ(new_z, -foldAngle + (foldCount-1)*M_PI/foldCount);

    return new_z;
}

bool Bounded(int type, vec2 z)
{
    switch (type)
    {
        case TRAP_CUSTOM:
            return (sin(z.y) < sin(bailout.x));
        case TRAP_SQUARE:
            return abs(z.x) < bailout.x && abs(z.y) < bailout.x;
        case TRAP_RECTANGLE:
            return abs(z.x) < bailout.x && abs(z.y) < bailout.y;
        case TRAP_REAL:
            return abs(z.x) < bailout.x;
        case TRAP_IMAG:
            return abs(z.y) < bailout.x;
        default:
            return z.x*z.x + z.y*z.y < bailout.x*bailout.x;
    }

    return false;
}

vec2 ComputeFractal(int type, vec2 z, vec2 c)
{
    vec2 new_z;
    
    if (fractalType != FRAC_CUSTOM)
    {
        z = FoldZ(z);

        if (isConjugate)
            z.y = -z.y;
    }

    switch (fractalType)
    {
        case FRAC_MANDELBROT:
            new_z = c_pow(z, power) + c_pow(c, c_power);
            break;
        case FRAC_LAMBDA:
            //z = c_pow(z, power);
            //new_z = c_mul(c_pow(c, c_power), c_mul(z, vec2(1,0) - z));
            new_z = c_mul(c_pow(c, c_power), z - c_pow(z, power));
            break;
        default:
            //new_z = c_pow(z, power) + c_pow(c, c_power);
            new_z = c_2(z) + c;
            break;
    }

    return new_z;
}

float JuliaLoopDistance(inout vec2 z, vec2 c, int maxIteration, inout int iter, float dist, float fineness, float riemannAdjustment, bool use_bailout)
{
    z = ComputeFractal(fractalType, (fractalType == FRAC_LAMBDA) ? vec2(1.0/power,0) : vec2(0), z);
    vec2 pz = z;

    float z2 = z.x * z.x + z.y * z.y;
    float d2 = 1;

    for (iter = 0; iter < maxIteration && (!use_bailout || Bounded(orbitTrap, z)); ++iter)
    {
        d2 *= 4.0 * z2;
        
        z = ComputeFractal(fractalType, z, c);
        
        if (z == pz || IsBad(z))
        {
            z = pz;
            iter = maxIteration;

            break;
        }
        
        z2 = z.x * z.x + z.y * z.y;
        
        if(z2 > dist)
            break;
    }
    
    float d = sqrt(z2 / d2) * .5 * log(z2);
    
	//return 1;
	//return sqrt(sqrt(d * pow(fineness, 2)));
	return sqrt(clamp(d * pow(fineness, 2) * zoom / riemannAdjustment, 0, 1));
}

float Julia()
{
    int iter = 0;
    
    // Riemann projection
    vec3 pos = normalize(FragPosModel);
    
    float tmp = (1 + (pos.y + 1)/(1 - pos.y)) / 2.0 / zoom;
    float r = pos.x*tmp;
    float i = pos.z*tmp;
    
    // Initialize image center
    vec2 z = vec2(r + center.x, i + center.y);

    // Distance estimation
    float dist = 0;

    // Compute Julia Set
    dist = JuliaLoopDistance(z, julia, maxIterations, iter, maxDistance, distFineness, tmp, false);

    float height;
    if (iter >= maxIterations)
    {
        height = 0;
        juliaColor = vec3(1,0,0);
    }
    else
    {
        height = 1 - pow(dist,1);
        juliaColor = 1 - vec3(height);
    }

    return height;
}