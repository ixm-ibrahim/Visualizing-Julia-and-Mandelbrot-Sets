# Visualizing-Julia-and-Mandelbrot-Sets

See https://ixm-ibrahim.com/explanations/project-visualizing-julia-and-mandelbrot-sets for an extended gallery.
See https://fractalforums.org/fractal-mathematics-and-new-theories/28/correction-for-the-infinite-burning-ship-fractal-algorithm/4112 for an explanation on how "folds" work.

***CONTROLS:***

  **WASD, QE**: camera movement (for 3D mode, press right-control to move the fractal center)  
  **Mouse**: camera movement  
  **Scroll**: zoom  
  **T**: reset all values  

  **IJKL**: movement along the complex plane  
  **O, U**: zoom in and out on the complex plane  
  
  **Grave Accent**: display all values to the console  
   
  **Left, Right, Down Arrow Keys**: decrease, increase, or stop changing the animation  
  **Up Arrow Key**: speed time up  
  **Right Alt with Up Arrow Key**: reverse time speed increase  
  
  **Enter**: cycle through the available fractals  
  **Backslash**: cycle through available orbit traps  
  **Backspace**: toggle centering on first critical point of julia set while animating  
  
  **1**: change the maximum iterations  
  **2**: change the bailout radius  
  **3**: change the maximum distance for distance estimation  
  **4**: change the fineness for distance estimation  
  **5**: change the power applied to z  
  **6**: change the power applied to c  
  **7**: change the folding rotation  
  **8**: change the number of folds (1 for Perpendicular Burning Ship, 2 for regular Burning Ship)  
  **9**: change the real component of the folding offset  
  **0**: change the imaginary component of the folding offset  
  
  **Left Shift**: increase change factor  
  **Left Control**: decrease change factor  
  **Left Alt**: negate change factor  
  
  **-**: toggle smooth coloring  
  **+**: toggle complex conjugate  
  **Tab**: toggle julia set  
  **Space**: toggle 3D mode (projecting fractal onto the Riemann Sphere)  
  **Slash**: toggle lighting in 3D mode  
  **Period**: toggle terrain height scale in 3D mode  
  **Comma**: toggle terrain color in 3D mode  


**Known Bugs**: 2D mode is scaled in (this is due to a bug in the tessellation shaders), and terrain color is not correct (I think tessellation is the issue here as well)  
