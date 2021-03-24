


#include "Plot.h"


const char* FracToStr(int frac)
{
	switch (frac)
	{
		case 0:
			return "FRAC_CUSTOM";
		case 1:
			return "FRAC_MANDELBROT";
		case 2:
			return "FRAC_LAMBDA";
	}

	return "ERROR: INVALID FRACTAL TYPE";
}

const char* TrapToStr(int trap)
{
	switch (trap)
	{
		case 0:
			return "TRAP_CUSTOM";
		case 1:
			return "TRAP_CIRCLE";
		case 2:
			return "TRAP_SQUARE";
		case 3:
			return "TRAP_RECTANGLE";
		case 4:
			return "TRAPREAL";
		case 5:
			return "TRAP_IMAG";
		case 6:
			return "TRAP_POINT";
		case 7:
			return "TRAP_LINE";
		case 8:
			return "TRAP_CROSS";
	}

	return "ERROR: INVALID ORBIT TRAP";
}

const char* ColToStr(int col)
{
	switch (col)
	{
		case 0:
			return "COL_BLACK";
		case 1:
			return "COL_WHITE";
		case 2:
			return "COL_CLASSIC";
		case 3:
			return "COL_SMOOTH";
		case 4:
			return "COL_DOMAIN_SIMPLE";
		case 5:
			return "COL_DOMAIN_NORMAL";
		case 6:
			return "COL_DOMAIN_BRIGHTNESS";
		case 7:
			return "COL_DOMAIN_BRIGHT_RINGS";
		case 8:
			return "COL_DOMAIN_BRIGHT_RINGS_SMOOTH";
		case 9:
			return "COL_DOMAIN_DARK_RINGS";
		case 10:
			return "COL_DOMAIN_BRIGHT_DARK_SMOOTH";
	}

	return "ERROR: INVALID COLORING SCHEME";
}