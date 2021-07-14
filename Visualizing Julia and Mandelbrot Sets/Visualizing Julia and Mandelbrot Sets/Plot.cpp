


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

Triangle::Triangle()
{
	this->vertices[0] = glm::vec3(0);
	this->vertices[1] = glm::vec3(0);
	this->vertices[2] = glm::vec3(0);

	center = glm::vec3(0);
	normal = glm::vec3(0);
}
Triangle::Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	this->vertices[0] = a;
	this->vertices[1] = b;
	this->vertices[2] = c;

	center = (a + b + c) / 3.f;

	normal = glm::normalize(glm::cross(vertices[2] - vertices[0], vertices[1] - vertices[0]));
}

IcoSphere::IcoSphere(int subdivisions, float radius, TextureType textureType)
{
	/*
	 * ICOSPHERE GENERATION
	*/
	count = 20 * pow(4, subdivisions);
	std::vector<Triangle> faces(count);

	float s = 1;
	float t = (float)((1.0 + sqrt(5.0)) / 2.0);

	/*
	 *				Mapping of Faces and Verticies of Base Icosahedron (Subdivision 0)
	 *
	 *															   0       0       0       0       0
	 *	         /\      /\      /\      /\      /\			       /\      /\      /\      /\      /\
	 *	        /  \    /  \    /  \    /  \    /  \		      /  \    /  \    /  \    /  \    /  \
	 *	     4 / 0  \  / 1  \  / 2  \  / 3  \  / 4  \ 0		     /    \  /    \  /    \  /    \  /    \
	 *	      /______\/______\/______\/______\/______\		    /______\/______\/______\/______\/______\
	 *	     /\      /\      /\      /\      /\      /		   /\ 5    /\ 4    /\ 3    /\ 2    /\ 1    / 5
	 *	    /  \ 5  /  \ 6  /  \ 7  /  \ 8  /  \ 9  / 10	  /  \    /  \    /  \    /  \    /  \    /
	 *	 9 / 10 \  / 11 \  / 12 \  / 13 \  / 14 \  /		 /    \  /    \  /    \  /    \  /    \  /
	 *	  /______\/______\/______\/______\/______\/			/______\/______\/______\/______\/______\/
	 *	  \      /\      /\      /\      /\      /			\ 6    /\ 7    /\ 8    /\ 9    /\ 10   / 6
	 *	19 \ 15 /  \ 16 /  \ 17 /  \ 18 /  \ 19 / 15		 \    /  \    /  \    /  \    /  \    /
	 *	    \  /    \  /    \  /    \  /    \  /			  \  /    \  /    \  /    \  /    \  /
	 *	     \/      \/      \/      \/      \/				   \/      \/      \/      \/      \/
	 *														   11      11      11      11      11
	 *
	 *
	 *		                  Individual Face Mappings
	 *	              v0					              /\
	 *	              /\					             /  \
	 *	__________   /  \   __________		            /    \
	 *	\        /  / s0 \  \        /		           / adj1 \
	 *	 \ adj2 /  /______\  \ adj0 /		          /________\
	 *	  \    /  /\      /\  \    /		    v1 ________________ v2
	 *	   \  /  /  \ s3 /  \  \  /			       \      /\      /
	 *	    \/  / s2 \  / s1 \  \/			    /\  \ s1 /  \ s2 /  /\
	 *	       /______\/______\				   /  \  \  / s3 \  /  /  \
	 *	     v2   __________   v1			  /    \  \/______\/  /    \
	 *	          \        /				 / adj0 \  \      /  / adj2 \
	 *	           \ adj1 /					/________\  \ s0 /  /________\
	 *	            \    /					             \  /
	 *	             \  /					              \/
	 *	              \/					              v0
	 *
	 */

	 // vertices on the icosahedron
	glm::vec3 v0 = radius * glm::normalize(glm::vec3(s, t, 0));   // 0
	glm::vec3 v1 = radius * glm::normalize(glm::vec3(0, s, -t));   // 1
	glm::vec3 v2 = radius * glm::normalize(glm::vec3(t, 0, -s));   // 2
	glm::vec3 v3 = radius * glm::normalize(glm::vec3(t, 0, s));   // 3
	glm::vec3 v4 = radius * glm::normalize(glm::vec3(0, s, t));   // 4
	glm::vec3 v5 = radius * glm::normalize(glm::vec3(-s, t, 0));   // 5
	glm::vec3 v6 = radius * glm::normalize(glm::vec3(-t, 0, -s));   // 6
	glm::vec3 v7 = radius * glm::normalize(glm::vec3(-t, 0, s));   // 7
	glm::vec3 v8 = radius * glm::normalize(glm::vec3(0, -s, t));   // 8
	glm::vec3 v9 = radius * glm::normalize(glm::vec3(s, -t, 0));   // 9
	glm::vec3 v10 = radius * glm::normalize(glm::vec3(0, -s, -t));   // 10
	glm::vec3 v11 = radius * glm::normalize(glm::vec3(-s, -t, 0));   // 11

	// Rotate points so the icosahedron is vertical (to fix texturing issues)
	float rotateAngle = atan(1.0f / t);
	glm::mat4 rot = glm::mat4(1);
	rot = glm::rotate(rot, rotateAngle, glm::vec3(0, 0, 1));
	{
		v0 = rot * glm::vec4(v0, 1);
		v1 = rot * glm::vec4(v1, 1);
		v2 = rot * glm::vec4(v2, 1);
		v3 = rot * glm::vec4(v3, 1);
		v4 = rot * glm::vec4(v4, 1);
		v5 = rot * glm::vec4(v5, 1);
		v6 = rot * glm::vec4(v6, 1);
		v7 = rot * glm::vec4(v7, 1);
		v8 = rot * glm::vec4(v8, 1);
		v9 = rot * glm::vec4(v9, 1);
		v10 = rot * glm::vec4(v10, 1);
		v11 = rot * glm::vec4(v11, 1);
	}

	// All arranged counter-clockwise
	{
		/*
		// five faces around vertex 1
		faces.push_back(Triangle(v0, v4, v5));
		faces.push_back(Triangle(v0, v3, v4));
		faces.push_back(Triangle(v0, v2, v3));
		faces.push_back(Triangle(v0, v1, v2));
		faces.push_back(Triangle(v0, v5, v1));
		// adjacent faces to the previous 5
		faces.push_back(Triangle(v7, v5, v4));
		faces.push_back(Triangle(v8, v4, v3));
		faces.push_back(Triangle(v9, v3, v2));
		faces.push_back(Triangle(v10, v2, v1));
		faces.push_back(Triangle(v6, v1, v5));
		// adjacent faces to the last 5
		faces.push_back(Triangle(v5, v7, v6));
		faces.push_back(Triangle(v4, v8, v7));
		faces.push_back(Triangle(v3, v9, v8));
		faces.push_back(Triangle(v2, v10, v9));
		faces.push_back(Triangle(v1, v6, v10));
		// five faces around vertex 12
		faces.push_back(Triangle(v11, v6, v7));
		faces.push_back(Triangle(v11, v7, v8));
		faces.push_back(Triangle(v11, v8, v9));
		faces.push_back(Triangle(v11, v9, v10));
		faces.push_back(Triangle(v11, v10, v6));
		*/
		// five faces around vertex 1
		faces[0] = Triangle(v0, v4, v5);
		faces[1] = Triangle(v0, v3, v4);
		faces[2] = Triangle(v0, v2, v3);
		faces[3] = Triangle(v0, v1, v2);
		faces[4] = Triangle(v0, v5, v1);
		// adjacent faces to the previous 5
		faces[5] = Triangle(v7, v5, v4);
		faces[6] = Triangle(v8, v4, v3);
		faces[7] = Triangle(v9, v3, v2);
		faces[8] = Triangle(v10, v2, v1);
		faces[9] = Triangle(v6, v1, v5);
		// adjacent faces to the last 5
		faces[10] = Triangle(v5, v7, v6);
		faces[11] = Triangle(v4, v8, v7);
		faces[12] = Triangle(v3, v9, v8);
		faces[13] = Triangle(v2, v10, v9);
		faces[14] = Triangle(v1, v6, v10);
		// five faces around vertex 12
		faces[15] = Triangle(v11, v6, v7);
		faces[16] = Triangle(v11, v7, v8);
		faces[17] = Triangle(v11, v8, v9);
		faces[18] = Triangle(v11, v9, v10);
		faces[19] = Triangle(v11, v10, v6);
	}

	// subdivide triangles
	for (int i = 0; i < subdivisions; i++)
	{
		int currentFaceNum = 20 * pow(4, i);
		int nextFaceNum = currentFaceNum * 4;

		std::vector<Triangle> subdivisions;

		// Subdivide each triangle
		for (int j = 0; j < currentFaceNum; j++)
		{
			/*
			 *	              v0				    0		  1 ________ 2
			 *	              /\				    /\			\      /
			 *	             /  \				   /  \			 \    /
			 *	            / s0 \				  /    \		  \  /
			 *	        m2 /______\ m0			 /______\		   \/
			 *	          /\      /\			2        1		    0
			 *	         /  \ s3 /  \
			 *	        / s2 \  / s1 \
			 *	       /______\/______\
			 *	     v2       m1       v1
			 */

			 // Get midpoints
			glm::vec3 m0 = (faces[j].vertices[0] + faces[j].vertices[1]) / 2.f;
			glm::vec3 m1 = (faces[j].vertices[1] + faces[j].vertices[2]) / 2.f;
			glm::vec3 m2 = (faces[j].vertices[2] + faces[j].vertices[0]) / 2.f;

			if (radius > 0)
			{
				m0 = radius * glm::normalize(m0);
				m1 = radius * glm::normalize(m1);
				m2 = radius * glm::normalize(m2);
			}

			subdivisions.push_back(Triangle(faces[j].vertices[0], m0, m2));	// top
			subdivisions.push_back(Triangle(m0, faces[j].vertices[1], m1));	// bottom-right
			subdivisions.push_back(Triangle(m2, m1, faces[j].vertices[2]));	// bottom-left
			subdivisions.push_back(Triangle(m1, m2, m0));			// middle
		}

		// Replace parent faces with child faces
		faces.clear();
		faces = subdivisions;
	}

	std::cout << "\nNumber of faces (icosahedron): " << count << "\n";

	// done, now add triangles to mesh
	vertices = new TexturedVertex[count *= 3];
	int index = 0;

	for (int i = 0; i < faces.size(); i++)
	{
		glm::vec3 v0 = faces[i].vertices[0];
		glm::vec3 v1 = faces[i].vertices[1];
		glm::vec3 v2 = faces[i].vertices[2];

		glm::vec2 uvA;
		glm::vec2 uvB;
		glm::vec2 uvC;

		// Get texture coordinates
		switch (textureType)
		{
		case SPHERICAL:
			uvA = GetSphereCoord(v0);
			uvB = GetSphereCoord(v1);
			uvC = GetSphereCoord(v2);

			break;
		case RIEMANN:
			uvA = GetSphereCoord2(v0);
			uvB = GetSphereCoord2(v1);
			uvC = GetSphereCoord2(v2);

			break;
		}

		// Handle poles
		if (uvA.y == 0 && uvB.x != 0)
		{
			//uvA.x = uvC.x + (uvB.x - uvC.x) / 2;
			uvA.x = uvC.x;
		}

		// Handle case where the texture wraps around on face
		if ((fmax(fmax(uvA.x, uvB.x), uvC.x) - fmin(fmin(uvA.x, uvB.x), uvC.x) > .5))
		{
			if (uvA.x <= .5)
				uvA.x++;
			if (uvB.x <= .5)
				uvB.x++;
			if (uvC.x <= .5)
				uvC.x++;
		}

		//vertices[index++] = TexturedVertex(v0, faces[i].normal, uvA);
		//vertices[index++] = TexturedVertex(v1, faces[i].normal, uvB);
		//vertices[index++] = TexturedVertex(v2, faces[i].normal, uvC);
		vertices[index++] = TexturedVertex(v0, glm::normalize(v0), uvA);
		vertices[index++] = TexturedVertex(v1, glm::normalize(v1), uvB);
		vertices[index++] = TexturedVertex(v2, glm::normalize(v2), uvC);
	}
}
IcoSphere::~IcoSphere()
{
	delete[] vertices;
}

// https://dreamstatecoding.blogspot.com/search?q=texturedvertex
glm::vec2 IcoSphere::GetSphereCoord(glm::vec3 v) const
{
	glm::vec2 uv;
	double len = glm::length(v);

	uv.y = acos(v.y / len) / M_PI;
	uv.x = -(atan2(v.z, v.x) / M_PI + 1.0f) * 0.5 + 1;

	return uv;
}

glm::vec2 IcoSphere::GetSphereCoord2(glm::vec3 v) const
{
	glm::vec2 uv;
	float r = glm::length(v);

	float theta = atan2(v.z, v.x);
	float phi = acos(-v.y / r);

	phi /= abs(cos(fmod(theta, M_PI) / 4));

	uv.x = phi * cos(theta);
	uv.y = phi * sin(theta);

	uv.x = (uv.x - -M_PI) / (M_PI - -M_PI) * (1 - 0) + 0;
	uv.y = (uv.y - -M_PI) / (M_PI - -M_PI) * (1 - 0) + 0;

	return uv;
}

int IcoSphere::GetSize()
{
	return count * sizeof(TexturedVertex);
}

int IcoSphere::GetVertexCount()
{
	return count;
}
