#version 450 core

#define M_PI 3.1415926535897932384626433832795

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

out vec4 FragColor;

in vec2 TexCoords;
in vec2 FragPos;

uniform float time;
uniform float zoom;
uniform vec2 radius;
uniform vec2 center;

uniform int fractalType;
uniform int orbitTrap;
uniform int exteriorColoring;
uniform int interiorColoring;

uniform float foldCount;
uniform float foldAngle;
uniform vec2 foldOffset;

uniform int maxIterations;
uniform float maxDistance;
uniform float distFineness;
uniform float power;
uniform float c_power;
uniform vec4 bailout;
uniform bool useDistance;
uniform bool isConjugate;

vec3 Mandelbrot();

void main()
{
    FragColor = vec4(Mandelbrot(), 1.0);
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
    float x = dot(b,b);
    return vec2((a.x*b.x + a.y*b.y) / x, (a.y*b.x - a.x*b.y) / x);
}

vec2 c_2(vec2 c)
{
    return vec2(c.x*c.x - c.y*c.y, 2*c.x*c.y);
}

vec2 c_pow(vec2 z, float a)
{
    if (z.x == 0 && z.y == 0)
        return z;
	if (a == 1)
        return z;
	if (a == 2)
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
    return isnan(r) || isnan(theta) ? z : pow(r, a) * vec2(cos(theta * a), sin(theta * a));
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

    // Translate
    new_z += foldOffset;
    // Rotate (constant)
    new_z = RotateZ(new_z, foldAngle);
    // Base fold
    new_z.y = abs(new_z.y);

    int intCount = int(foldCount);

    for (int i = 0; i < foldCount - 1; i++)
    {
        // Rotate
        new_z = RotateZ(new_z, -M_PI / foldCount);
        //new_z = RotateZ(new_z, -M_PI / intCount);
        // Fold
        new_z.y = abs(new_z.y);

    }

    // Unrotate (both the constant and folding angles)
    new_z = RotateZ(new_z, -foldAngle + (foldCount-1)*M_PI/foldCount);
    //new_z = RotateZ(new_z, -foldAngle + (intCount-1)*M_PI/intCount);
    //new_z = RotateZ(new_z, -foldAngle + (intCount-1)*M_PI/foldCount);
    //new_z = RotateZ(new_z, -foldAngle + (foldCount-1)*M_PI/intCount);
    // Untranslate
    new_z -= foldOffset;

    return new_z;
}

bool Bounded(int type, vec2 z)
{
    switch (type)
    {
        case TRAP_CUSTOM:
            //return (sin(z.y) < sin(bailout.x));
            return cos(z.x + time) + sin(z.y + time) < bailout.x;
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

    if (isConjugate)
        z.y = -z.y;

    z = FoldZ(z);

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
            new_z = c_pow(z, power+1) + c_pow(c, c_power);
            break;
    }

    return new_z;
}

vec2 MandelbrotLoop(vec2 c, int maxIteration, inout int iter, bool use_bailout)
{
    vec2 z = ComputeFractal(fractalType, (fractalType == FRAC_LAMBDA) ? vec2(1.0/power,0) : vec2(0), c);
    vec2 pz = z;
    
    for (iter = 0; iter < maxIteration && (!use_bailout || Bounded(orbitTrap, z)); ++iter)
    {
        z = ComputeFractal(fractalType, z, c);
        
        if (z == pz || IsBad(z))
        {
            z = pz;
            iter = maxIteration;

            break;
        }
        
        pz = z;
    }
    
    return z;
}

float MandelbrotLoopDistance(inout vec2 c, int maxIteration, inout int iter, float dist, float fineness, bool use_bailout)
{
    vec2 z = ComputeFractal(fractalType, (fractalType == FRAC_LAMBDA) ? vec2(1.0/power,0) : vec2(0), c);
    vec2 pz = z;
    
    vec2 dz = vec2(1.0,0.0);
    float m2 = dot(z,z);
    float di =  1.0;

    for (iter = 0; iter < maxIteration && (!use_bailout || Bounded(orbitTrap, z)); ++iter)
    {
        dz = 2 * c_mul(z,dz) + vec2(1.0,0.0);
        
        z = ComputeFractal(fractalType, z, c);
        
        if (z == pz || IsBad(z))
        {
            z = pz;
            iter = maxIteration;

            break;
        }
        
        m2 = dot(z,z);
        
        if(m2 > dist)
        {
            di = 0;
            break;
        }
    }
    
    c = z;

    float d = sqrt(m2 / dot(dz,dz)) * .5 * log(m2);

    if(di > 0.5)
        d=0.0;
    
	//return d/maxDistance;
	//return sqrt(sqrt(d * pow(fineness, 2)));
	return sqrt(clamp(d * pow(fineness, 2) * zoom, 0, 1));
}

vec3 ColorFromHSV(vec3 color)
{
    float hi = mod(floor(color.x / 60.0), 6);
    float f = color.x / 60.0 - floor(color.x / 60.0);

    //value = value * 255;
    float v = color.z;
    float p = color.z * (1 - color.y);
    float q = color.z * (1 - f * color.y);
    float t = color.z * (1 - (1 - f) * color.y);

    if (hi == 0)
        return vec3(v, t, p);
    if (hi == 1)
        return vec3(q, v, p);
    if (hi == 2)
        return vec3(p, v, t);
    if (hi == 3)
        return vec3(p, q, v);
    if (hi == 4)
        return vec3(t, p, v);

    return vec3(v, p, q);
}

vec3 DomainColoring(int coloring, vec2 z, float dist, int iter)
{
    float t = time * 20;

    vec3 color;

    if (coloring == COL_CLASSIC || coloring == COL_SMOOTH)
        return ColorFromHSV(vec3(atan(FragPos.y, FragPos.x) * 180 / M_PI + t, 1, 1));

    float theta = sin(atan(float(z.y), float(z.x))) * 360 + t;
	float r = length(z);
        	
	if (z.x == 0 && z.y == 0)
	{
		theta = 0;
		r = 0;
	}
	else if (isnan(z.x) || isnan(z.y))
	{
		z = vec2(0);
		theta = 0;
		r = 0;
	}

    float twoPI = 2 * M_PI;
    float s = abs(sin(mod(r * twoPI, twoPI)));
	float b = sqrt(sqrt(abs(sin(mod(z.y * twoPI, twoPI)) * sin(mod(z.x * twoPI, twoPI)))));
	float b2 = .5 * ((1 - s) + b + sqrt(pow(1 - s - b, 2) + 0.01));

    switch (coloring)
    {
        case COL_DOMAIN_NORMAL:
            color = ColorFromHSV(vec3(theta, s, b2 > 1 ? 1 : b2));
            break;
        case COL_DOMAIN_BRIGHTNESS:
            color = ColorFromHSV(vec3(theta, 1, b2 > 1 ? 1 : b2));
            break;
        case COL_DOMAIN_BRIGHT_RINGS:
            color = ColorFromHSV(vec3(theta, s, 1));
            break;
        case COL_DOMAIN_BRIGHT_RINGS_SMOOTH:
            color = ColorFromHSV(vec3(theta, mod(r,1), 1));
            break;
        case COL_DOMAIN_DARK_RINGS:
            color = ColorFromHSV(vec3(theta, 1, s));
            break;
        case COL_DOMAIN_BRIGHT_DARK_SMOOTH:
            color = ColorFromHSV(vec3(theta, 1, mod(r,1)));
            break;
        default:
            //color = mix(outerColor1, outerColor2, theta);
            color = ColorFromHSV(vec3(theta, 1, 1));
            break;
    }
    
    return color;
}

vec3 ExteriorColoring(vec2 z, float dist, int iter)
{
    vec3 color;
    
    switch (exteriorColoring)
    {
        case COL_CUSTOM:
            vec3 outerColor1 = vec3(0.13f, 0.94f, 0.13f);
            vec3 outerColor2 = vec3(0.0f, 0.47f, 0.95f);
            
            float theta = sin(atan(float(z.y), float(z.x)) + time)*.5 + .5;

            color = mix(outerColor1, outerColor2, theta);
            break;
        case COL_BLACK:
            color = vec3(0);
            break;
        case COL_WHITE:
            color = vec3(1);
            break;
        case COL_CLASSIC:
            color = vec3(sin(7 * (iter + time) / 17) * .5 + .5, sin(11 * (iter + time) / 29) * .5 + .5, sin(13 * (iter + time) / 41) * .5 + .5);
            break;
        case COL_SMOOTH:
            float mu;
        
            mu = iter;
            //mu = iter + 1 - (log2(log2(length(z))));
            //mu = iter - log2(log2(dot(z,z))) + 4.0;
            //mu = iter - log(log(dot(z,z))/(log(bailout)))/log(power);
            //mu = iter - (log(log(length(z))/log(bailout.x)) / (sign(power)*log(abs(power))));
            mu = iter - (log(log(length(z))/log(bailout.x)) / (sign(power)*log(abs(power) == 1 ? 1.0000001 : abs(power))));
            //mu = iter + 1 - (log(log(length(z))) / (sign(power)*log(abs(power) == 1 ? 1.0000001 : abs(power))));
            //mu = iter - (log(log(length(z))/log(bailout.x)));

            color = vec3(sin(7 * (mu + time) / 17) * .5 + .5, sin(11 * (mu + time) / 29) * .5 + .5, sin(13 * (mu + time) / 41) * .5 + .5);
            break;
        default:
            color = DomainColoring(exteriorColoring, z, dist, iter);
            break;
    }
    
    // Distance estimation
    if (useDistance)
        //color = pow( vec3(dist), vec3(0.9,1.1,1.4) );
        color *= pow(dist,1);
    
    return color;
}

vec3 InteriorColoring(vec2 z, float dist, int iter)
{
    vec3 color;
    //vec3 outerColor1 = vec3(0.13f, 0.94f, 0.13f);
    //vec3 outerColor2 = vec3(0.0f, 0.47f, 0.95f);

    switch (interiorColoring)
    {
        case COL_CUSTOM:
            vec3 outerColor1 = vec3(0.13f, 0.94f, 0.13f);
            vec3 outerColor2 = vec3(0.0f, 0.47f, 0.95f);
            
            float theta = sin(atan(float(z.y), float(z.x)) + time)*.5 + .5;

            color = mix(outerColor1, outerColor2, theta);
            break;
        case COL_BLACK:
            color = vec3(0);
            break;
        case COL_WHITE:
            color = vec3(1);
            break;
        default:
            color = DomainColoring(interiorColoring, z, dist, iter);
            break;
    }
    
    return color;
}

vec3 GetColor(vec2 z, float dist, int iter)
{
    // Decide the color based on the number of iterations
    vec3 color;
    if (iter >= maxIterations)
    {
        color = InteriorColoring(z, dist, iter);
    }
    else
    {
        color = ExteriorColoring(z, dist, iter);
    }
    
    return color;
}

vec3 Mandelbrot()
{
    //vec2 pos = ((gl_FragCoord.xy - .5) / (1000 - 1) - .5) * 2;

    int iter = 0;

    // Initialize image center
    vec2 z = FragPos;
    //vec2 z = radius*FragPos/zoom + center;

    // Distance estimation
    float dist = 0;

    // Compute Julia Set
    if (useDistance)
        dist = MandelbrotLoopDistance(z, maxIterations, iter, maxDistance, distFineness, false);
    else
        z = MandelbrotLoop(z, maxIterations, iter, true);

    return GetColor(z, dist, iter);
}