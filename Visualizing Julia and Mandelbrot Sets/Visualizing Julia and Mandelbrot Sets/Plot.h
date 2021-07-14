


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
	bool useRiemannSphere;
	bool useLighting;
	bool useTerrainColor;
	float terrain;
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
		this->zoom = -1;
		this->useDistance = false;
		this->isConjugate = false;
		this->isJulia = false;
		this->isJuliaCentered = false;
		this->isBuddhabrot = false;
		this->useRiemannSphere = false;
		this->useLighting = false;
		this->useTerrainColor = false;
		this->terrain = 0;
		this->julia = glm::vec2(0, 0);
		this->center = glm::vec2(0, 0);
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


enum TextureType
{
	NONE = 0, SPHERICAL, RIEMANN
};

class Triangle
{
	public:
		glm::vec3 vertices[3];
		glm::vec3 center;
		glm::vec3 normal;

		Triangle();
		Triangle(glm::vec3, glm::vec3, glm::vec3);
};

class IcoSphere
{
	private:
		int count;

	public:
		TexturedVertex* vertices;

		IcoSphere();
		IcoSphere(int, float, TextureType = SPHERICAL);
		~IcoSphere();

		glm::vec2 GetSphereCoord(glm::vec3 v) const;
		glm::vec2 GetSphereCoord2(glm::vec3 v) const;

		int GetSize();
		int GetVertexCount();
};


#endif