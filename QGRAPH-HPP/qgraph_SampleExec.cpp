// Author: Jordan Randleman -- qgraph_SampleExec.cpp -- "qgraph.hpp" Sample Execution

#include <iostream>
#include <iomanip>
#include "qgraph.hpp"

/* 
 * "qgraph.hpp" PUBLIC CONTENTS:
 *
 * TYPE ALIASES (4):
 *   (0) qnum         // NUMERIC (ie any point, total # of points, etc)
 *   (1) qnums        // NUMERIC VECTOR (ie coordinates, list of adjacent points, etc)
 *   (2) qnum_matrix  // NUMERIC VECTOR MATRIX (ie inner cubes, list of adjacent coords, etc)
 *   (3) coord_matrix // NUMERIC VECTOR 3D MATRIX (ie coord adjacency matrix, inner cubes in coords, etc)
 *
 *
 * NON MEMBER FUNCTIONS (3):
 *   (0) qnum        convert_coord_to_point(qnums)  // converts coords to a point
 *   (1) qnums       shortest_path(qnum, qnum)      // returns shortest path btwn given pts
 *   (2) qnum_matrix shortest_path(qnums, qnums)    // returns shortest path's coords btwn given coords
 * 
 *
 * MEMBERS OF Hypercube (19): 
 *   (00) qnum dimension                            // member dictating Hypercube's dimension
 *   (01) qnum total_points()                       // returns total points in 'dimension'
 * 
 *   (02) Hypercube()                               // defaults 'dimension = 0'
 *   (03) Hypercube(qnum)                           // given dimension
 *   (04) Hypercube(Hypercube)                      // given another hypercube
 * 
 *   (05) qnums       convert_point_to_coord(qnum)  // converts point to its coords in 'dimension' space
 *   (06) qnums       adjacent_points(qnum)         // returns list of points adjacent to given pt in 'dimension'
 *   (07) qnums       adjacent_points(qnums)        // returns list of points adjacent to given coord in 'dimension'
 *   (08) qnum_matrix adjacent_coords(qnum)         // returns list of pt-coords adjacent to given pt in 'dimension'
 *   (09) qnum_matrix adjacent_coords(qnums)        // returns list of pt-coords adjacent to given coord in 'dimension'
 * 
 *   (10) qnum_matrix  point_adjacency_matrix()     // returns adjacency list matrix for all points in 'dimension'
 *   (11) coord_matrix coord_adjacency_matrix()     // returns adjacency list matrix for all pt-coords in 'dimension'
 *   (12) qnum         total_inner_cubes(qnum)      // returns # of given sub-dimension cubes w/in 'dimension'
 *   (13) qnum         total_inner_cubes(Hypercube) // returns # of given Hypercubes w/in 'dimension'
 *   (14) qnum         total_inner_cubes()          // returns # of all nested Hypercubes in 'dimension'
 * 
 *   (15) qnum_matrix  inner_cubes(qnum)            // given dimension n, returns matrix of all n cubes' pts in 'dimension'
 *   (16) qnum_matrix  inner_cubes(Hypercube)       // given Hypercube of dimension n, returns matrix of all n cubes' pts in 'dimension'
 *   (17) coord_matrix inner_cubes_coord(qnum)      // given dimension n, returns matrix of all n cubes' pt-coords in 'dimension'
 *   (18) coord_matrix inner_cubes_coord(Hypercube) // given Hypercube of dimension n, returns matrix of all n cubes' pt-coords in 'dimension'
 */

/******************************************************************************
* MAIN EXECUTION
******************************************************************************/

int main() {

  using std::cout;
  using std::endl;

  // Entire Library In Namespace 'qgraph'
  using namespace qgraph; 

  // Print line lambda
  auto printl = [](auto data){cout << data << endl;};

  // Print spaced elts in container
  auto printlist = [](auto container) { 
    for(auto e : container) cout << e << " ";
  };

  /******************************************************************************
  * NON-MEMBER FUNCTIONS
  ******************************************************************************/

  cout << "\n---------------------\n"
       << "NON-MEMBER FUNCTIONS:"
       << "\n---------------------\n";


  // Getting the shortest path from 1 point to another
  qnum point1 = 0, point2 = 15;
  cout << "=> Shortest path from " << point1 << " to " << point2 << ": ";
  const qnums short_path = shortest_path(point1, point2);
  printlist(short_path);
  printl("");


  // Getting the shortest path from 1 coord to another
  const qnums point1_coords = {0,0,1,0}, point2_coords = {1,1,0,1};
  cout << "=> Shortest path from { "; 
  printlist(point1_coords);
  cout << "} to { ";
  printlist(point2_coords);
  cout << "}:\n    ";
  const qnum_matrix short_coord_path = shortest_path(point1_coords, point2_coords);
  for(auto coord : short_coord_path) {
    cout << "{ ";
    printlist(coord);
    cout << "} ";
  }
  printl("");
  

  // Converting a coordinate to a point:
  cout << "=> Coordinate { "; 
  printlist(short_coord_path[0]);
  cout << "} = Point " << convert_coord_to_point(short_coord_path[0]) << "\n";

  /******************************************************************************
  * HYPERCUBE MEMBER FUNCTIONS
  ******************************************************************************/

  cout << "\n---------------------------\n"
       << "HYPERCUBE MEMBER FUNCTIONS:"
       << "\n---------------------------\n";


  // Constructing a Hypercube, showing its total points, & changing its dimension:
  Hypercube hcube(3);
  cout << "=> Total Points in Cube of 3D Cube = " << hcube.total_points() << endl;
  hcube.dimension = 4;
  cout << "=> Total Points in Cube of 4D Cube = " << hcube.total_points() << endl;


  // Getting a point's coordinates w/in out hypercube's 'dimension' space 
  const qnum some_pt = 13;
  cout << "=> Coordinates for point " << some_pt 
       << " in Dimension " << hcube.dimension << " = { ";
  printlist(hcube.convert_point_to_coord(some_pt));
  printl("}");


  // Getting list of points adjacent to a point in 'dimension'
  printl("\nNOTE: Either of the following can also take a coordinate arg!");
  cout << "  => Points adjacent to Point " << some_pt << " include: ";
  printlist(hcube.adjacent_points(some_pt));
  printl("");


  // Getting list of point-coordinates adjacent to a point in 'dimension'
  cout << "  => Coordinates adjacent to Point " << some_pt << " include: ";
  const qnum_matrix some_pt_coords = hcube.adjacent_coords(some_pt);
  for(auto coord : some_pt_coords) {
    cout << "{ ";
    printlist(coord);
    cout << "} ";
  }
  printl("\n");


  // Retrieving the adjacency list matrix for every point in our hypercube
  const qnum_matrix hcube_points = hcube.point_adjacency_matrix();
  cout << "=> Adjacency list matrix for all points in our hypercube of dimension " 
       << hcube.dimension << ":\n";
  for(qnum point = 0; point < hcube.total_points(); ++point) {
    cout << "   " << std::setw(2) << point << ": ";
    printlist(hcube_points[point]);
    printl("");
  }
  printl("");


  // Retrieving the adjacency list matrix for every point coordinate in our hypercube
  const coord_matrix hcube_coords = hcube.coord_adjacency_matrix();
  cout << "=> Adjacency list matrix for all point coords in our hypercube of dimension " 
       << hcube.dimension << ":\n";
  for(qnum coords = 0; coords < hcube.total_points(); ++coords) {
    cout << "   " << std::setw(2) << coords << ": ";
    for(auto coordinate : hcube_coords[coords]) {
      cout << "{ ";
      printlist(coordinate);
      cout << "} ";
    }
    printl("");
  }
  printl("");


  // Getting the total instances of an nested dimension, its instance points,
  // & its intance coordinates:
  const qnum inner_cube = 3;
  cout << "=> Total instances of inner cube " << inner_cube 
       << " w/in dimension " << hcube.dimension << ": " 
       << hcube.total_inner_cubes(inner_cube) << endl;

  cout << "=> Points of each " << inner_cube 
       << "D instance w/in dimension " << hcube.dimension << ":\n";
  const qnum_matrix inner_cube_instances = hcube.inner_cubes(inner_cube);
  for(auto cube : inner_cube_instances) {
    cout << "   { ";
    printlist(cube);
    cout << "}\n";
  }
  printl("");

  cout << "=> Point Coordinates of each " << inner_cube 
       << "D instance w/in dimension " << hcube.dimension << ":\n";
  const coord_matrix inner_cube_instance_coords = hcube.inner_cubes_coord(inner_cube);
  for(auto cube_coords : inner_cube_instance_coords) {
    cout << "   { ";
    for(auto coord : cube_coords) {
      cout << "{ ";
      printlist(coord);
      cout << "} ";
    }
    cout << "}\n";
  }

  return 0;
}
