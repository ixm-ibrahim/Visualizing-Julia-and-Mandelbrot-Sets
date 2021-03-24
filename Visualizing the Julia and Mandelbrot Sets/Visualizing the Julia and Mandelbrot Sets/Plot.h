


#ifndef PLOT
#define PLOT

#ifndef M_PI
	#define M_PI glm::pi<float>()
#endif

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

#define FRAC_CUSTOM						0
#define FRAC_MANDELBROT					1
#define FRAC_LAMBDA						2
#define FRAC_COUNT						3

#define TRAP_CUSTOM						0
#define TRAP_CIRCLE						1
#define TRAP_SQUARE						2
#define TRAP_RECTANGLE					3
#define TRAP_REAL						4
#define TRAP_IMAG						5
#define TRAP_POINT						6
#define TRAP_LINE						7
#define TRAP_CROSS						8
#define TRAP_COUNT						9

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
#define COL_COUNT						12

// point, line, and cross orbit traps
// julia centering for lambda


const char* FracToStr(int);
const char* TrapToStr(int);
const char* ColToStr(int);

enum Buddhabrot
{
	NORMAL = 1, INVERSE
};

struct RiemannMandelbrot
{
	double maxIterations;
	glm::vec4 bailout;
	double zoom;
	double maxDistance;
	double distFineness;
	float foldAngle;
	float foldCount;
	glm::vec2 foldOffset;
	bool useDistance;
	bool isConjugate;
	bool isJulia;
	bool isJuliaCentered;
	bool isBuddhabrot;
	glm::vec2 center;
	glm::vec2 julia;
	float power;
	float c_power;
	float radius;
	int fractalType;
	int orbitTrap;
	int exteriorColoring;
	int interiorColoring;
	Buddhabrot buddhabrot;

	void Init()
	{
		this->maxIterations = 99;
		this->bailout = glm::dvec4(4,0,0,0);
		this->maxDistance = 100;
		this->distFineness = 10;
		this->foldCount = 0;
		this->foldAngle = 0;
		this->foldOffset = glm::vec2(0,0);
		this->zoom = 0;
		this->useDistance = false;
		this->isConjugate = false;
		this->isJulia = false;
		this->isJuliaCentered = false;
		this->isBuddhabrot = false;
		this->julia = glm::vec2(0, 0);
		this->center = glm::vec2(0);
		this->power = 2;
		this->c_power = 1;
		this->radius = 2;
		this->fractalType = FRAC_MANDELBROT;
		this->orbitTrap = TRAP_CIRCLE;
		this->exteriorColoring = COL_SMOOTH;
		this->interiorColoring = COL_DOMAIN_SIMPLE;
		this->buddhabrot = Buddhabrot::NORMAL;
	}
};

struct TexturedVertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoordinates;

	TexturedVertex()
	{
		this->position = glm::vec3(0);
		this->normal = glm::vec3(0);
		this->textureCoordinates = glm::vec2(0);
	}
	TexturedVertex(glm::vec3 position)
	{
		this->position = position;
		this->normal = glm::vec3(0);
		this->textureCoordinates = glm::vec2(0);
	}
	TexturedVertex(glm::vec3 position, glm::vec3 normal)
	{
		this->position = position;
		this->normal = normal;
		this->textureCoordinates = glm::vec2(0);
	}
	TexturedVertex(glm::vec3 position, glm::vec3 normal, glm::vec2 textureCoordinates)
	{
		this->position = position;
		this->normal = normal;
		this->textureCoordinates = textureCoordinates;
	}

	static int GetSize()
	{
		return (3 + 3 + 2) * sizeof(float);
	}
};



	static glm::vec2 ED_set(float a)
	{
		glm::vec2 z;
		z.x = a;
		z.y = 0.0;
		return z;
	}

	static glm::vec2 ED_add(glm::vec2 dsa, glm::vec2 dsb)
	{
		glm::vec2 dsc;
		float t1, t2, e;

		t1 = dsa.x + dsb.x;
		e = t1 - dsa.x;
		t2 = ((dsb.x - e) + (dsa.x - (t1 - e))) + dsa.y + dsb.y;

		dsc.x = t1 + t2;
		dsc.y = t2 - (dsc.x - t1);
		return dsc;
	}

	static glm::vec2 ED_sub(glm::vec2 dsa, glm::vec2 dsb)
	{
		glm::vec2 dsc;
		float e, t1, t2;

		t1 = dsa.x - dsb.x;
		e = t1 - dsa.x;
		t2 = ((-dsb.x - e) + (dsa.x - (t1 - e))) + dsa.y - dsb.y;

		dsc.x = t1 + t2;
		dsc.y = t2 - (dsc.x - t1);
		return dsc;
	}

	static glm::vec2 ED_mul(glm::vec2 dsa, glm::vec2 dsb)
	{
		glm::vec2 dsc;
		float c11, c21, c2, e, t1, t2;
		float a1, a2, b1, b2, cona, conb, split = 8193.;

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

	static glm::vec2 ED_div(glm::vec2 dsa, glm::vec2 dsb)
	{
		glm::vec2 dsc;
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


#endif