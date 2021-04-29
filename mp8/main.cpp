#include <stdio.h>
#include <assert.h>
using namespace std;

#include "arg_parser.h"
#include "glCanvas.h"
#include "spline_parser.h"
#include "gl/GLUT.h"

// ====================================================================
// ====================================================================

int main(int argc, char *argv[]) {
  
  glutInit(&argc, argv);
  // parse the command line arguments & input file
  ArgParser *args = new ArgParser(argc,argv);
  SplineParser* splines = new SplineParser(args->input_file);

  // launch curve editor!
  

  // output as required
  splines->SaveBezier(args);
  splines->SaveBSpline(args);
  splines->SaveTriangles(args);

  if (args->gui) {
      GLCanvas glcanvas;
      glcanvas.initialize(args, splines);
      // this never returns...
  }
  // cleanup
  delete args;
  delete splines;
  return 0;
}

// ====================================================================
// ====================================================================



