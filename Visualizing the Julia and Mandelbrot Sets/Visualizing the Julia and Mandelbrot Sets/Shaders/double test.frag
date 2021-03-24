#version 450 core

//#pragma optionNV(fastmath off)
//#pragma optionNV(fastprecision off)
//#pragma optimize (off)

out vec4 FragColor;

in vec2 TexCoords;
in vec2 FragPos;
//in vec4 FragPos_ED;

uniform float zoom;
uniform vec2 radius;
uniform vec2 center;

uniform vec2 zoom_ED;
uniform vec4 radius_ED;
uniform vec4 center_ED;

uniform float time;
uniform vec2 julia;
uniform int maxIterations;
uniform float maxDistance;
uniform float distFineness;
uniform float bailout;
uniform bool useDistance;

vec3 Julia();

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float PHI = 1.61803398874989484820459;

float gold_noise(in vec2 xy, in float seed){
       return fract(tan(distance(xy*PHI, xy)*seed)*xy.x);
}

vec2 ED_set(float a);
int ED_compare(vec2 dsa, vec2 dsb);
vec2 ED_add(vec2 dsa, vec2 dsb);
vec2 ED_sub(vec2 dsa, vec2 dsb);
vec2 ED_mul(vec2 dsa, vec2 dsb);
vec2 ED_div(vec2 dsa, vec2 dsb);

void main()
{
    //FragColor = vec4(0.5);
    //FragColor = vec4(result, 1.0);
    //FragColor = vec4(vec3(abs(norm.x), abs(norm.y), abs(norm.z)), 1.0);
    //FragColor = vec4(abs(eyePos), 1.0);

    FragColor = vec4(Julia(), 1.0);
    //FragColor = vec4(result * Julia(), 1.0);
    //FragColor = vec4(juliaColor, 1.0);
    //FragColor = vec4(result * juliaColor, 1.0);



    /*
    vec2 pos = ((gl_FragCoord.xy - .5) / (1000 - 1) - .5) * 2;

    //bool useDouble = zoom > 14;
    //bool useDouble = true;
    bool useDouble = TexCoords.x > .5;

    int iter = 0;
    
    // Initialize image center
    //vec2 center = vec2(-0.104826, 0.928356);
    //vec2 z = radius*FragPos/pow(2,zoom) + center;
    //dvec2 zd = FragPos;
    //dvec4 zq = dvec4(qs_set(FragPos.x), qs_set(FragPos.y));

     // Emulated double precision
    vec2 zm = ED_set(1 / pow(2,zoom));
    vec4 c = center_ED;
    //vec4 c = vec4(-0.311184, 0, 1.17568, 0);
    //vec4 c = vec4(-0.104826, 0, 0.928356, 0);
    vec4 r = radius_ED;
    //vec4 r = vec4(.000002,0,.000002,0);
    //vec4 p = vec4(ED_set(FragPos.x), ED_set(FragPos.y));
    vec4 p = vec4(ED_set(pos.x), ED_set(pos.y));
    vec4 z_ED = vec4(ED_add(ED_mul(ED_mul(r.xy,p.xy), zm), c.xy), ED_add(ED_mul(ED_mul(r.zw,p.zw), zm), c.zw));
    //vec4 z_ED = vec4(ED_add(ED_mul(r.xy,p.xy), c.xy), ED_add(ED_mul(r.zw,p.zw), c.zw));
    //vec4 z_ED = vec4(ED_mul(r.xy,p.xy), ED_mul(r.zw,p.zw));

    if (TexCoords.x > .5)
        FragColor = vec4(vec3(rand(z_ED.xy),rand(z_ED.zw), 1.0), 1.0);
        //FragColor = vec4(vec3(gold_noise(z_ED.xy, z_ED.x),gold_noise(z_ED.zw, z_ED.z), 1.0), 1.0);
    else
        FragColor = vec4(Julia(), 1.0);
    */
}


vec2 ED_set(float a)
{
    vec2 z;
    z.x = a;
    z.y = 0.0;
    return z;
}

int ED_compare(vec2 dsa, vec2 dsb)
{
    if (dsa.x < dsb.x)
        return -1;
    else if (dsa.x == dsb.x) 
    {
        if (dsa.y < dsb.y)
            return -1;
        else if (dsa.y == dsb.y)
            return 0;
        else
            return 1;
    }
    else
        return 1;
}

#define VAR_ZERO1 min (1.0, time)  // following iq's suggestion
#define VAR_ZERO2 min (1.0, time-.1)
#define VAR_ZERO3 min (1.0, time-.3)

float FFAdd (float a, float b) 
{
    //return mix (a, a + b, (b != 0.) ? 1. : VAR_ZERO1);
    return mix (a, a + b, min (1.0, time));
}

float FFSub (float a, float b)
{
    //return mix (a, a - b, (b != 0.) ? 1. : VAR_ZERO2);
    return mix (a, a - b, min (1.0, time-.1));
}

float FFSub2 (float a, float b)
{
    //return mix (a, a - b, (b != 0.) ? 1. : VAR_ZERO3);
    return mix (a, a - b, min (1.0, time-.3));
}

vec2 ED_add (vec2 va, vec2 vb)
{
    vec2 vc;
    float t1, t2, e;

    /*
    t1 = va.x + vb.x;
    e = FFSub (t1, va.x);
    t2 = FFAdd (FFSub (vb.x, e), va.x - FFSub2 (t1, e)) + va.y + vb.y;
    vc.x = t1 + t2;
    vc.y = t2 - FFSub (vc.x, t1);
    */
    t1 = va.x + vb.x;
    e = t1 - va.x;
    t2 = ((vb.x - e) + (va.x - (t1 - e))) + va.y + vb.y;
    vc.x = t1 + t2;
    vc.y = t2 - (vc.x - t1);


    return vc;
}

vec2 ED_sub (vec2 va, vec2 vb)
{
    vec2 vc;
    float t1, t2, e;
    /*
    t1 = va.x - vb.x;
    e = FFSub (t1, va.x);
    t2 = FFAdd (FFSub (- vb.x, e), va.x - FFSub2 (t1, e)) + va.y - vb.y;
    vc.x = t1 - t2;
    vc.y = t2 - FFSub (vc.x, t1);
    */
    t1 = va.x - vb.x;
    e = t1 - va.x;
    t2 = ((-vb.x - e) + (va.x - (t1 - e))) + va.y - vb.y;
    vc.x = t1 - t2;
    vc.y = t2 - (vc.x - t1);


    return vc;
}

#define DSPLIT 8193.

vec2 ED_mul (vec2 va, vec2 vb)
{
    vec2 vc;
    float ma, mb, sa1, sa2, sb1, sb2, c1, c2, t, t1, t2, e;
    ma = va.x * DSPLIT;
    mb = vb.x * DSPLIT;


    //sa1 = FFSub (ma, FFSub2 (ma, va.x));
    //sb1 = FFSub (mb, FFSub2 (mb, vb.x));
    
    sa1 = ma - (ma - va.x);
    sb1 = mb - (mb - vb.x);


    sa2 = va.x - sa1;
    sb2 = vb.x - sb1;
    c1 = va.x * vb.x;
    c2 = va.x * vb.y + va.y * vb.x;
    t = (((sa1 * sb1 - c1) + sa1 * sb2) + sa2 * sb1) + sa2 * sb2;
    t1 = c1 + c2;

    /*
    e = FFSub (t1, c1);
    t2 = FFAdd (FFSub (c2, e), c1 - FFSub2 (t1, e)) + t + va.y * vb.y;
    vc.x = t1 + t2;
    vc.y = t2 - FFSub (vc.x, t1);
    */
    e = t1 - c1;
    t2 = va.y * vb.y + ((c2 - e) + (c1 - (t1 - e))) + c2;
    vc.x = t1 + t2;
    vc.y = t2 - (vc.x - t1);
    
    return vc;
}
/**/

vec2 ED_div(vec2 dsa, vec2 dsb)
{
    vec2 dsc;
    float c11, c21, c2, e, s1, s2, t1, t2, t11, t12, t21, t22;
    float a1, a2, b1, b2, cona, conb, split = 8193.;

    s1 = dsa.x / dsb.x;

    cona = s1 * split;
    conb = dsb.x * split;
    a1 = cona - (cona - s1);
    b1 = conb - (conb - dsb.x);
    a2 = s1 - a1;
    b2 = dsb.x - b1;

    c11 = s1 * dsb.x;
    c21 = (((a1 * b1 - c11) + a1 * b2) + a2 * b1) + a2 * b2;
    c2 = s1 * dsb.y;

    t1 = c11 + c2;
    e = t1 - c11;
    t2 = ((c2 - e) + (c11 - (t1 - e))) + c21;

    t12 = t1 + t2;
    t22 = t2 - (t12 - t1);

    t11 = dsa[0] - t12;
    e = t11 - dsa[0];
    t21 = ((-t12 - e) + (dsa.x - (t11 - e))) + dsa.y - t22;

    s2 = (t11 + t21) / dsb.x;

    dsc.x = s1 + s2;
    dsc.y = s2 - (dsc.x - s1);

    return dsc;
}

dvec2 qs_set(double a)
{
    dvec2 z;
    z.x = a;
    z.y = 0.0;
    return z;
}

double ds_compare(dvec2 dsa, dvec2 dsb)
{
    if (dsa.x < dsb.x)
        return -1.;
    else if (dsa.x == dsb.x) 
    {
        if (dsa.y < dsb.y)
            return -1.;
        else if (dsa.y == dsb.y)
            return 0.;
        else
            return 1.;
    }
    else
        return 1.;
}

dvec2 ds_add (dvec2 dsa, dvec2 dsb)
{
    dvec2 dsc;
    double t1, t2, e;

    t1 = dsa.x + dsb.x;
    e = t1 - dsa.x;
    t2 = ((dsb.x - e) + (dsa.x - (t1 - e))) + dsa.y + dsb.y;

    dsc.x = t1 + t2;
    dsc.y = t2 - (dsc.x - t1);
    return dsc;
}

dvec2 ds_sub (dvec2 dsa, dvec2 dsb)
{
    dvec2 dsc;
    double e, t1, t2;

     t1 = dsa.x - dsb.x;
     e = t1 - dsa.x;
     t2 = ((-dsb.x - e) + (dsa.x - (t1 - e))) + dsa.y - dsb.y;

     dsc.x = t1 + t2;
     dsc.y = t2 - (dsc.x - t1);
     return dsc;
}

dvec2 ds_mul (dvec2 dsa, dvec2 dsb)
{
    dvec2 dsc;
    double c11, c21, c2, e, t1, t2;
    double a1, a2, b1, b2, cona, conb, split = 8193.;

    cona = dsa.x * split;
    conb = dsb.x * split;
    a1 = cona - (cona - dsa.x);
    b1 = conb - (conb - dsb.x);
    a2 = dsa.x - a1;
    b2 = dsb.x - b1;

    c11 = dsa.x * dsb.x;
    c21 = a2 * b2 + (a2 * b1 + (a1 * b2 + (a1 * b1 - c11)));

    c2 = dsa.x * dsb.y + dsa.y * dsb.x;

    t1 = c11 + c2;
    e = t1 - c11;
    t2 = dsa.y * dsb.y + ((c2 - e) + (c11 - (t1 - e))) + c21;

    dsc.x = t1 + t2;
    dsc.y = t2 - (dsc.x - t1);

    return dsc;
}

dvec2 ds_div(dvec2 dsa, dvec2 dsb)
{
    dvec2 dsc;
    double c11, c21, c2, e, s1, s2, t1, t2, t11, t12, t21, t22;
    double a1, a2, b1, b2, cona, conb, split = 8193.;

    s1 = dsa.x / dsb.x;

    cona = s1 * split;
    conb = dsb.x * split;
    a1 = cona - (cona - s1);
    b1 = conb - (conb - dsb.x);
    a2 = s1 - a1;
    b2 = dsb.x - b1;

    c11 = s1 * dsb.x;
    c21 = (((a1 * b1 - c11) + a1 * b2) + a2 * b1) + a2 * b2;
    c2 = s1 * dsb.y;

    t1 = c11 + c2;
    e = t1 - c11;
    t2 = ((c2 - e) + (c11 - (t1 - e))) + c21;

    t12 = t1 + t2;
    t22 = t2 - (t12 - t1);

    t11 = dsa[0] - t12;
    e = t11 - dsa[0];
    t21 = ((-t12 - e) + (dsa.x - (t11 - e))) + dsa.y - t22;

    s2 = (t11 + t21) / dsb.x;

    dsc.x = s1 + s2;
    dsc.y = s2 - (dsc.x - s1);

    return dsc;
}

vec2 c_2(vec2 c)
{
    return vec2(c.x*c.x - c.y*c.y, 2*c.x*c.y);
}

vec4 ED_2(vec4 c)
{
    return vec4(ED_sub(ED_mul(c.xy,c.xy), ED_mul(c.zw,c.zw)), ED_mul(ED_mul(vec2(2,0),c.xy),c.zw));
}

dvec2 dc_2(dvec2 c)
{
    return dvec2(c.x*c.x - c.y*c.y, 2*c.x*c.y);
}

dvec4 ds_2(dvec4 c)
{
    dvec2 r = ds_sub(ds_mul(c.xy,c.xy), ds_mul(c.zw,c.zw));
    dvec2 i = ds_mul(dvec2(2,0), ds_mul(c.xy,c.zw));

    return dvec4(r,i);
}

vec2 JuliaLoop(inout vec2 z, vec2 c, int maxIteration, inout int iter, bool use_bailout)
{   
    c = z;
    
    for (iter = 0; iter < maxIteration && (!use_bailout || (z.x * z.x + z.y * z.y < bailout)); ++iter)
        z = c_2(z) + c;

    return z;
}

float JuliaLoopDistance(inout vec2 z, vec2 c, int maxIteration, inout int iter, float dist, float fineness, bool use_bailout)
{
    //c = z;
    
    float z2 = z.x * z.x + z.y * z.y;
    float d2 = 1;

    for (iter = 0; iter < maxIteration && (!use_bailout || (z.x * z.x + z.y * z.y < bailout)); ++iter)
    {
        d2 *= 4.0 * z2;
        
        z = c_2(z) + c;

        z2 = z.x * z.x + z.y * z.y;
        
        if(z2 > dist)
            break;
    }
    
    float d = sqrt(z2 / d2) * log(z2);
    
	//return 1;
	//return sqrt(sqrt(d * pow(fineness, 2)));
	return sqrt(clamp(d * pow(fineness, 2) * pow(2,zoom), 0, 1));
}

dvec2 JuliaLoopD(inout dvec2 z, dvec2 c, int maxIteration, inout int iter, bool use_bailout)
{
    for (iter = 0; iter < maxIteration && (!use_bailout || (z.x * z.x + z.y * z.y < bailout)); ++iter)
        z = dc_2(z) + c;

    return z;
}

vec4 JuliaLoop_ED(inout vec4 z, vec4 c, int maxIteration, inout int iter, bool use_bailout)
{
    c = z;

    for (iter = 0; iter < maxIteration && (!use_bailout || (ED_compare(ED_add(ED_mul(z.xy, z.xy), ED_mul(z.zw, z.zw)), ED_set(bailout)) < 0)); ++iter)
    {
        z = ED_2(z);

        z = vec4(ED_add(z.xy,c.xy), ED_add(z.zw,c.zw));
    }

    return z;
}

/*
vec4 JuliaLoop_ED(inout vec4 z, vec4 c, int maxIteration, inout int iter, bool use_bailout)
{
    vec2 zx = z.xy;
    vec2 zy = z.zw;
    vec2 cx = c.xy;
    vec2 cy = c.zw;
    vec2 two = ED_set(2.0); 
  
    vec2 e_radius = ED_set(bailout);
    
    vec2 tmp;
  
    for(iter=0; iter<maxIteration; iter++)
    {
        tmp = zx;
        zx = ED_add(ED_sub(ED_mul(zx, zx), ED_mul(zy, zy)), cx);
        zy = ED_add(ED_mul(ED_mul(zy, tmp), two), cy);

        if( ED_compare(ED_add(ED_mul(zx, zx), ED_mul(zy, zy)), e_radius) > 0) 
	    {
	        break;
	    }
    }

    return vec4(zx,zy);
}
*/

double JuliaLoopDistanceD(inout dvec2 z, dvec2 c, int maxIteration, inout int iter, float dist, float fineness, bool use_bailout)
{
    double z2 = z.x * z.x + z.y * z.y;
    double d2 = 1;

    for (iter = 0; iter < maxIteration && (!use_bailout || (z.x * z.x + z.y * z.y < bailout)); ++iter)
    {
        d2 *= 4.0 * z2;
        
        z = dc_2(z) + c;

        z2 = z.x * z.x + z.y * z.y;
        
        if(z2 > dist)
            break;
    }
    
    double d = sqrt(z2 / d2) * log(float(z2));
    
	//return 1;
	//return sqrt(sqrt(d * pow(fineness, 2)));
	return sqrt(clamp(d * pow(fineness, 2) * pow(2,zoom), 0, 1));
}

dvec4 JuliaLoopQ(inout dvec4 z, dvec4 c, int maxIteration, inout int iter, bool use_bailout)
{
    float derX = 1;
    float derY = 0;
    /*
    dvec2 q = ds_mul(ds_sub(z.xy,dvec2(.25,0)), ds_sub(z.xy,dvec2(.25,0)))+ds_mul(z.zw,z.zw);
    
    if (q*(q+(z.x-.25)) < .25*z.y*z.y ||   // Period 1
       (z.x+1)*(z.x+1)+z.y*z.y < .0625)     // Period 2
        iter = maxIteration;
    else*/
        for (iter = 0; iter < maxIteration && (!use_bailout || (ds_compare(ds_add(ds_mul(z.xy, z.xy), ds_mul(z.zw, z.zw)), dvec2(bailout*bailout,0)) < 0)); ++iter)
        {
            z = ds_2(z);
            dvec2 r = ds_add(z.xy,c.xy);
            dvec2 i = ds_add(z.zw,c.zw);
            z = dvec4(r,i);
        }

    return z;
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

vec3 Julia()
{
    vec2 pos = ((gl_FragCoord.xy - .5) / (1000 - 1) - .5) * 2;

    //bool useDouble = zoom > 14;
    //bool useDouble = true;
    bool useDouble = TexCoords.x > .5;

    int iter = 0;
    
    int maxIterations = 1500;
    //float zoom = 18.f;

    // Initialize image center
    vec2 center = vec2(-1.25067, 0.02012);
    vec2 z = radius*FragPos/pow(2,zoom) + center;
    dvec2 zd = FragPos;
    dvec4 zq = dvec4(qs_set(FragPos.x), qs_set(FragPos.y));

     // Emulated double precision
    vec2 zm = ED_set(1 / pow(2,zoom));
    //vec4 c = center_ED;
    //vec4 c = vec4(-0.311184, 0, 1.17568, 0);
    //vec4 c = vec4(-0.104826, 0, 0.928356, 0);
    vec4 c = vec4(-1.25067, 0, 0.02012, 0);
    vec4 r = radius_ED;
    //vec4 r = vec4(.000002,0,.000002,0);
    //vec4 p = vec4(ED_set(FragPos.x), ED_set(FragPos.y));
    vec4 p = vec4(ED_set(pos.x), ED_set(pos.y));
    vec4 z_ED = vec4(ED_add(ED_mul(ED_mul(r.xy,p.xy), zm), c.xy), ED_add(ED_mul(ED_mul(r.zw,p.zw), zm), c.zw));
    //vec4 z_ED = vec4(ED_add(ED_mul(r.xy,p.xy), c.xy), ED_add(ED_mul(r.zw,p.zw), c.zw));

    vec4 julia_ED = vec4(ED_set(julia.x), ED_set(julia.y));
    dvec4 juliaq = dvec4(qs_set(julia.x), qs_set(julia.y));

    // Distance estimation
    float dist = 0;

    // Compute Julia Set
    if (useDouble)
        if (useDistance)
            dist = float(JuliaLoopDistanceD(zd, dvec2(julia), maxIterations, iter, maxDistance, distFineness, true));
        else
            z_ED = JuliaLoop_ED(z_ED, julia_ED, maxIterations, iter, true);
            //zd = JuliaLoopD(zd, julia, maxIterations, iter, true);
            //zq = JuliaLoopQ(zq, juliaq, maxIterations, iter, true);
    else
        if (useDistance)
            dist = JuliaLoopDistance(z, julia, maxIterations, iter, maxDistance, distFineness, true);
        else
            z = JuliaLoop(z, julia, maxIterations, iter, true);

    // Decide the color based on the number of iterations
    vec3 color;
    if (iter >= maxIterations)
    {
        color = vec3(0);
        vec3 outerColor1 = vec3(0.13f, 0.94f, 0.13f);
        vec3 outerColor2 = vec3(0.0f, 0.47f, 0.95f);

        //float theta = sin(atan(float(z.y), float(z.x)));
        float theta = sin(atan(float(z_ED.z), float(z_ED.x)));

        //color = mix(outerColor1, outerColor2, theta);
        //color = ColorFromHSV(vec3(theta*360, 1, 1));
        //color = abs(normalize(FragPosModel));
        color = pow( vec3(dist), vec3(0.9,1.1,1.4) );
    }
    else
    {
        // Smooth coloring
        float mu;

        int i = 0;
        if (useDouble)
        {
            JuliaLoop_ED(z_ED, julia_ED, 5, i, true);
            //JuliaLoopD(zd, julia, 5, i, true);
            //JuliaLoopQ(zq, juliaq, 5, i, true);

            //mu = iter + 1 - (log2(log2(float(length(z_ED.xz)))));
            //mu = iter + 1 - (log2(log2(float(length(zd)))));
            mu = iter;
        }
        else
        {
            JuliaLoop(z, julia, 5, i, true);

            mu = iter + 1 - (log2(log2(length(z))));
        }

        vec3 muColor = vec3(sin(7 * (mu + time) / 17) * .5 + .5, sin(11 * (mu + time) / 29) * .5 + .5, sin(13 * (mu + time) / 41) * .5 + .5);

        // Distance estimation
        //color = pow( muColor, vec3(1,1,1) );
        //color = pow( vec3(dist), vec3(0.9,1.1,1.4) );
        if (useDistance)
            color = pow(dist,1) * muColor;
        else
            color = muColor;

        /*
        color = vec3(sin(7 * (iter) / 17), sin(11 * (iter) / 29), sin(13 * (iter) / 41));
        */
    }

    return color;
}