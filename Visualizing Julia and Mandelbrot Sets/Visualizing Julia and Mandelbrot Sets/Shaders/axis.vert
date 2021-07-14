#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;

out vec3 color;
  
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * model * vec4(aPosition, 1.0f);
	color = aColor;
}



// DIVERGENCE NEEdS TO BE UPDATED WITH DELTA_T, GIVEN THAT THE VELOCITY_PIXEL WE USE IN ADVECTION MAY NOT BE NEXT TO IT
// BASICALLY ANYTIME YOU CALL ADJACENT_TEXELS

/*
  // Given an velocity texture and a time delta, advect the
  // quantities in the input texture into the output texture
      uniform float deltaT;
      uniform sampler2D inputTexture;
      uniform sampler2D velocity;
      varying vec2 textureCoord;
      
      void main() {
        vec2 u = texture2D(velocity, textureCoord).xy;
       
        vec2 pastCoord = fract(textureCoord - (0.5 * deltaT * u));
        gl_FragColor = texture2D(inputTexture, pastCoord);
      }

  // Apply a "splat" of change to a given place with a given
  // blob radius. The effect of the splat has an exponential falloff.
      uniform vec4 change;
      uniform vec2 center;
      uniform float radius;
      uniform sampler2D inputTex;
      
      varying vec2 textureCoord;
      
      void main() {
        float dx = center.x - textureCoord.x;
        float dy = center.y - textureCoord.y;
        vec4 cur = texture2D(inputTex, textureCoord);
        gl_FragColor = cur + change * exp(-(dx * dx + dy * dy) / radius);
      }

  // Calculate the divergence of the advected velocity field, and multiply by
  // (2 * epsilon * rho / deltaT).
      uniform float deltaT;         // Time between steps \n\
      uniform float rho;            // Density \n\
      uniform float epsilon;        // Distance between grid units \n\
      uniform sampler2D velocity;   // Advected velocity field, u_a \n\
      
      varying vec2 textureCoord;
      
      vec2 u(vec2 coord) {
        return texture2D(velocity, fract(coord)).xy;
      }
      
      void main() {
        gl_FragColor = vec4((-2.0 * epsilon * rho / deltaT) * (
          (u(textureCoord + vec2(epsilon, 0)).x -
           u(textureCoord - vec2(epsilon, 0)).x)
          +
          (u(textureCoord + vec2(0, epsilon)).y -
           u(textureCoord - vec2(0, epsilon)).y)
        ), 0.0, 0.0, 1.0);
      }

  // Perform a single iteration of the Jacobi method in order to solve for
  // pressure.
      uniform float epsilon;        // Distance between grid units \n\
      uniform sampler2D divergence; // Divergence field of advected velocity, d \n\
      uniform sampler2D pressure;   // Pressure field from previous iteration, p^(k-1) \n\
      
      varying vec2 textureCoord;
      
      float d(vec2 coord) {
        return texture2D(divergence, fract(coord)).x;
      }
     
      float p(vec2 coord) {
        return texture2D(pressure, fract(coord)).x;
      }
      
      void main() {
        gl_FragColor = vec4(0.25 * (
          d(textureCoord)
          + p(textureCoord + vec2(2.0 * epsilon, 0.0))
          + p(textureCoord - vec2(2.0 * epsilon, 0.0))
          + p(textureCoord + vec2(0.0, 2.0 * epsilon))
          + p(textureCoord - vec2(0.0, 2.0 * epsilon))
        ), 0.0, 0.0, 1.0);
      }

  // Subtract the pressure gradient times a constant from the advected velocity
  // field.
      uniform float deltaT;         // Time between steps \n\
      uniform float rho;            // Density \n\
      uniform float epsilon;        // Distance between grid units \n\
      uniform sampler2D velocity;   // Advected velocity field, u_a \n\
      uniform sampler2D pressure;   // Solved pressure field \n\
      
      varying vec2 textureCoord;
      
      float p(vec2 coord) {
        return texture2D(pressure, fract(coord)).x;
      }
      
      void main() {
        vec2 u_a = texture2D(velocity, textureCoord).xy;
        
        float diff_p_x = (p(textureCoord + vec2(epsilon, 0.0)) -
                          p(textureCoord - vec2(epsilon, 0.0)));
        float u_x = u_a.x - deltaT/(2.0 * rho * epsilon) * diff_p_x;
        
        float diff_p_y = (p(textureCoord + vec2(0.0, epsilon)) -
                          p(textureCoord - vec2(0.0, epsilon)));
        float u_y = u_a.y - deltaT/(2.0 * rho * epsilon) * diff_p_y;
        
        gl_FragColor = vec4(u_x, u_y, 0.0, 0.0);
      }

    if (options.advectV) {
      // Advect the velocity texture through itself, leaving the result in
      // textures.velocity0
      velocity1.drawTo(velocity0);
      textures.swap(velocity0, velocity1);
    }

    if (options.applyPressure) {
      // Calculate the divergence, leaving the result in textures.divergence
      calcDivergence(textures.velocity0);

      // Calculate the pressure, leaving the result in textures.pressure0
      int JACOBI_ITERATIONS = 10;

      for (int i = 0; i < JACOBI_ITERATIONS; i++) {
        pressure1.drawTo(textures.divergence, textures.pressure0);
        textures.swap(pressure0, pressure1);
      }

      // Subtract the pressure gradient from the advected velocity texture,
      // leaving the result in textures.velocity0
      velocity1(textures.velocity0, textures.pressure0);
      textures.swap(velocity0, velocity1);
    }

    // Advect the color field, leaving the result in textures.color0
    color1.drawTo(textures.color0, textures.velocity0);
    textures.swap(color0, color1);

    if (ev.dragging) {
      velocity1.drawTo(
        addSplat(
          textures.velocity0,
          [10.0 * ev.deltaX / WIDTH, -10.0 * ev.deltaY / HEIGHT, 0.0, 0.0],
          [ev.offsetX / WIDTH, 1.0 - ev.offsetY / HEIGHT],
          0.01
        );
      textures.swap(velocity0, velocity1);
    }
*/