// Author: Jordan Randleman -- qgraph.hpp -- Q-Graph Analysis Library

// Somehow, Hypercubes keep popping up in my code. 
// This may be a gotta-hammer-issa-nail situation, but regardless this 
// header aims to consolidate common fcns I've developed for them thusfar.

#ifndef QGRAPH_HPP_
#define QGRAPH_HPP_
#include <vector>
#include <algorithm>

/*
 * OPERATING DEFINITION:
 * Hypercube/Q-Graph of Nth Dimension:
 *   => A set of 2^N distinct vertices
 *   => Each w/ an undirected edge to N other vertices in the set
 */


// Note that in the diagram below, converting the
// labels into their binary yields 2 interesting 
// properties:
//   (1) Adjacent vertices to point P are all the
//       possible gray-codes formed from the 1st
//       D bits in P
//   (2) The binary for each point corresponds to
//       its unit-cube coordinate in Dth 
//       dimensional space

/************************************************
 * Labeled Points on Hypercube of Dimension D=3 *
 *                                              *
 *      6 ---------- 7       110----------111   *
 *     /|           /|       /|           /|    *
 *    / |          / |      / |          / |    *
 *   4 ---------- 5  |    100----------101 |    *
 *   |  |         |  |     |  |         |  |    *
 *   |  2 --------|- 3     | 010--------|-011   *
 *   | /          | /      | /          | /     *
 *   |/           |/       |/           |/      *
 *   0 ---------- 1       000----------001      *
 *                                              *
 ***********************************************/


/* 
 * PUBLIC CONTENTS:
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
* PRIVATE COMPUTATIONAL HELPER FUNCTIONS --- USERS DISREGARD
******************************************************************************/

// Rather surprisingly, this returns n's factorial
static unsigned long long factorial(const unsigned long long n) {
  return (!n) ? 1 : (n * factorial(n - 1));
}


// Insertion, Erasure, & Search fcn templates for Sorted & Unique Vectors
template<typename T>
static void sorted_insert(std::vector<T>&v, const T elt) {
  // "lower_bound" performs binary search
  auto position = std::lower_bound(v.begin(), v.end(), elt);
  if(position == v.end() || elt < *position)
    v.insert(position, elt);
}

template<typename T>
static void sorted_erase(std::vector<T>&v, const T elt) {
  // "lower_bound" performs binary search
  auto position = std::lower_bound(v.begin(), v.end(), elt);
  if(position != v.end() && *position == elt)
    v.erase(position);
}

template<typename T>
static bool sorted_find(const std::vector<T>&v, const T elt) {
  // "lower_bound" performs binary search
  auto position = std::lower_bound(v.begin(), v.end(), elt);
  return (position != v.end() && *position == elt);
}


// Returns set matrix of combinations size "n" w/in iterator range [begin,end)
static void get_n_combinations_in_range(const unsigned long long n, 
const std::vector<unsigned long long>::iterator begin, 
const std::vector<unsigned long long>::iterator end, 
std::vector<unsigned long long> seen, 
std::vector<std::vector<unsigned long long>> &combo_matrix) {
  std::vector<unsigned long long>::iterator temp, it = begin;
  while(it != end) {
    sorted_insert(seen, *it);
    temp = it++;
    switch(n) {
      case 1:  sorted_insert(combo_matrix, seen); break;
      default: get_n_combinations_in_range(n-1, it, end, seen, combo_matrix);
    } 
    sorted_erase(seen, *temp);
  }
}


// Prefix the shortest vector w/ 0's until of equal length w/ the other vector
static void prefix_shortest_vector_with_zeros(
std::vector<unsigned long long>&a,
std::vector<unsigned long long>&b) {
  auto &min_vec = (a.size() < b.size()) ? a : b;
  const auto &max_vec = (a.size() > b.size()) ? a : b;
  const unsigned long long length_difference = max_vec.size() - min_vec.size();
  // Add 0s to the front of the coordinate & append the shortest coordinate
  std::vector<unsigned long long> zero_prefixed_vec(length_difference, 0);
  zero_prefixed_vec.insert(zero_prefixed_vec.end(), min_vec.begin(), min_vec.end());
  min_vec = zero_prefixed_vec;
}


/******************************************************************************
* PUBLIC "qgraph" NAMESPACE FOR "HYPERCUBE" STRUCT & TO PERFORM COMPUTATIONS
******************************************************************************/

// Public UI Object for Qgraph Computations
namespace qgraph {

  /******************************************************************************
  * NAMESPACE TYPE ALIASES
  ******************************************************************************/

  using qnum = unsigned long long; // NUMERIC VALUE USED FOR ALL OF "qgraph" OPERATIONS
  using qnums = std::vector<qnum>;
  using qnum_matrix = std::vector<qnums>;
  using coord_matrix = std::vector<qnum_matrix>;

  /******************************************************************************
  * COORDINATE TO POINT CONVERTER (BCD DECODER)
  ******************************************************************************/

  // Given a Point's coordinates, returns the Point's Numeric Label.
  // Converting a point to a coord requires a "Hypercube" object's method.
  qnum convert_coord_to_point(const qnums coords) {
    qnum P = 0;
    for(qnum coord : coords) P = (P << 1) | coord;
    return P;
  }

  /******************************************************************************
  * SHORTEST DISTANCE BETWEEN 2 POINTS OR 2 COORDINATES
  ******************************************************************************/

  // Returns the shortest path of points from point a to point b.
  // Since adjacent points are labeled s.t. their binary follows gray code, the
  // shortest path is simply found by flipping each bit until the points are =
  qnums shortest_path(const qnum a, const qnum b) {
    qnums path;
    qnum current_point = a;
    const qnum gray_code_difference = a ^ b;
    for(qnum bit = (8*sizeof(qnum))-1; bit >= 0; --bit) {
      if((gray_code_difference >> bit) & 1) { // if bits are unequal
        current_point ^= (1<<bit);            // flip bit
        path.push_back(current_point);
      }
      if(!bit) return path;
    }
    return path;
  }


  // Returns the shortest path of coordinates from coords a to coords b.
  qnum_matrix shortest_path(qnums a, qnums b) {
    if(a.size() != b.size()) prefix_shortest_vector_with_zeros(a,b);
    qnum_matrix path;
    qnums current_coord(a);
    const qnum n = a.size();

    for(qnum i = 0; i < n; ++i)
      if(a[i] != b[i]) {
        current_coord[i] = current_coord[i] ? 0 : 1;
        path.push_back(current_coord);
      }
    return path;
  }


  /******************************************************************************
  * HYPERCUBE COMPUTATION STRUCTURE
  ******************************************************************************/

  // Structure to interact w/ hypercubes
  struct Hypercube {
  private: /****** PRIVATE METHOD PROTOTYPES – IMPLEMENTATION AFTER STRUCT ******/
    void erase_seen_point_P_from_adjMatrix(const qnum,qnum_matrix&)const;
    void erase_seen_points(const qnums&,qnum_matrix&)const;
    void register_and_derive_shared_point_adjacency_lists(qnums&,qnum_matrix&,const qnums&,const qnum_matrix&)const;
    void derive_shared_cube_points(const qnum,const qnum,qnums&,const qnum_matrix&)const;
    void derive_all_n_cubes_containing_point_P(const qnum,const qnum,qnum_matrix&,qnum_matrix&)const;

  public:
    /******************************************************************************
    * HYPERCUBE'S DIMENSION & TOTAL POINTS: DIMENSION MAY BE CHANGED AT WILL
    ******************************************************************************/

    qnum dimension = 0;
    constexpr qnum total_points() const {return 1 << dimension;}

    /******************************************************************************
    * CONSTRUCTORS
    ******************************************************************************/

    Hypercube() = default;
    Hypercube(const qnum n) : dimension(n) {}
    Hypercube(const Hypercube &h) : dimension(h.dimension) {}

    /******************************************************************************
    * A SINGLE POINT'S COORDINATE OR ADJACENCY LIST W/IN THIS HYPERCUBE
    ******************************************************************************/

    // Returns vector of point P's coordinates in this Hypercube's dimensional space
    // IE: in a 3D cube, point #3 in the above diagram has coordinates: 
    //     (x,y,z) = (0,1,1) => 3 in binary
    qnums convert_point_to_coord(const qnum P) const {
      if(!dimension || P > total_points()) return qnums{};
      qnums P_coordinates;
      for(qnum shift = dimension-1; shift >= 0; --shift) {
        P_coordinates.push_back((P>>shift)&1);
        if(!shift) return P_coordinates;
      }
      return P_coordinates;
    }


    // Returns vector of points connected to P in this dimension
    qnums adjacent_points(const qnum P) const {
      if(P > total_points()) return qnums{};
      qnums points_connected_to_P;
      for(qnum n = 0; n < dimension; ++n)
        sorted_insert(points_connected_to_P, P^(1<<n)); 
      return points_connected_to_P;
    }


    // Returns vector of coordinates for points connected to P in this dimension.
    // NOTE: each coord is also a vector, hence this returns a MATRIX.
    qnum_matrix adjacent_coords(const qnum P) const {
      if(P > total_points()) return qnum_matrix{};
      qnum_matrix coords_of_points_connected_to_P;
      const qnums P_adjacency_list(adjacent_points(P));

      for(auto point : P_adjacency_list) 
        coords_of_points_connected_to_P.push_back(convert_point_to_coord(point));
      return coords_of_points_connected_to_P;
    }

    /******************************************************************************
    * ADJACENCY LIST MATRIX FOR ALL POINTS (OR COORDINATES) W/IN THIS HYPERCUBE
    ******************************************************************************/

    // Returns MxN matrix of Hypercube's point adjacency lists:
    // => M = point 
    // => matrix[M] = vector of points adjacent to M
    qnum_matrix point_adjacency_matrix() const {
      qnum_matrix point_adjacency_lists;
      for(qnum P = 0; P < total_points(); ++P)
        point_adjacency_lists.push_back(adjacent_points(P));
      return point_adjacency_lists;
    }
    

    // Identical functionality to the above method, EXCEPT instead of returning
    // a 2D matrix of points, returns a 3D matrix of their coordinates.
    coord_matrix coord_adjacency_matrix() const {
      coord_matrix point_adjacency_coords_lists;
      for(qnum P = 0; P < total_points(); ++P)
        point_adjacency_coords_lists.push_back(adjacent_coords(P));
      return point_adjacency_coords_lists;
    }

    /******************************************************************************
    * NESTED CUBE MAGNITUDE ANALYSIS
    ******************************************************************************/

    // Total n-cubes w/in N: (N! * 2^(N - n)) / (n! * (N - n)!)
    // Given dimension number
    qnum total_inner_cubes(const qnum n) const {
      const qnum N = dimension;
      return (n > N) ? 0 : 
             (n == N) ? 1 :
             (factorial(N) * (1 << (N-n))) / (factorial(n) * factorial(N-n));
    }


    // Returns # of all possible inner cubes, excluding itself
    qnum total_inner_cubes() const {
      qnum total_cubes = 0;
      for(qnum inner_dim = 0; inner_dim < dimension; ++inner_dim)
        total_cubes += total_inner_cubes(inner_dim);
      return total_cubes;
    }

    /******************************************************************************
    * NESTED CUBE INSTANCES DERIVATION
    ******************************************************************************/

    // Returns vector of all n-cubes w/in this cube.
    // NOTE: An n-cube is a vector of points composing the cube, hence this
    //       returns a MATRIX of points.
    qnum_matrix inner_cubes(const qnum n) const {
      if(n > dimension) return qnum_matrix{};
      qnum_matrix inner_cube_matrix;
      // 0th cubes are just points
      if(!n) { 
        for(qnum P = 0; P < total_points(); ++P) 
          inner_cube_matrix.push_back({P});
        return inner_cube_matrix;
      }
      // Derive nth sub-cubes from this cube's point adjacency list matrix.
      qnum_matrix adj_matrix = point_adjacency_matrix();
      for(qnum P = 0; P < total_points(); ++P)
        if(adj_matrix[P].size() >= n) {
          derive_all_n_cubes_containing_point_P(P,n,inner_cube_matrix,adj_matrix);
          erase_seen_point_P_from_adjMatrix(P, adj_matrix);
        }
      return inner_cube_matrix;
    }


    // Returns vector of all n-cubes' coordinates w/in this cube.
    // NOTE: identical to the above method, BUT coords are themselves vectors
    //       hence this method returns a 3D qnum matrix.
    coord_matrix inner_cubes_coord(const qnum n) const {
      if(n > dimension) return coord_matrix{};
      qnum_matrix inner_cube_matrix = inner_cubes(n);
      coord_matrix inner_cube_coord_matrix;
      
      for(auto cube : inner_cube_matrix) {
        inner_cube_coord_matrix.push_back({});
        const qnum length = inner_cube_coord_matrix.size()-1;
        for(auto point : cube)
          inner_cube_coord_matrix[length].push_back(convert_point_to_coord(point));
      }
      return inner_cube_coord_matrix;
    }

    /******************************************************************************
    * POINT-OPERATION METHOD COMPOSITIONS, IF GIVEN A POINT'S COORDINATES INSTEAD
    ******************************************************************************/

    // These operate precisely as do their above 'const qnum' ARG counterparts,
    // save for they 1st convert a point's coordinates to its numeric label
    // if so given.

    qnum total_inner_cubes(const qnums coord) const {
      return total_inner_cubes(convert_coord_to_point(coord));
    }
    qnums adjacent_points(const qnums coord) const {
      return adjacent_points(convert_coord_to_point(coord));
    }
    qnum_matrix adjacent_coords(const qnums coord) const {
      return adjacent_coords(convert_coord_to_point(coord));
    }

    /******************************************************************************
    * METHOD EQUIVALENCIES, GIVEN HYPERCUBE STRUCT INSTEAD OF DIMENSION
    ******************************************************************************/

    // Given another Hypercube struct
    qnum total_inner_cubes(const Hypercube &h) const { 
      return total_inner_cubes(h.dimension); 
    }
    // "inner_cubes" Given another Hypercube
    qnum_matrix inner_cubes(const Hypercube &h) const {
      return inner_cubes(h.dimension);
    }
    // "inner_cubes_coord" Given another Hypercube
    coord_matrix inner_cubes_coord(const Hypercube &h) const {
      return inner_cubes_coord(h.dimension);
    }
  };



  /******************************************************************************
  * PRIVATE NESTED CUBE INSTANCES HELPER FCNS --- USERS DISREGARD
  ******************************************************************************/

  // Rm row & instances of point "P" in "adj_matrix" (found all cubes containing it)
  void Hypercube::erase_seen_point_P_from_adjMatrix(const qnum P,qnum_matrix &adj_matrix)const {
    adj_matrix[P].clear();
    for(qnum row = 0; row < total_points(); ++row) 
      sorted_erase(adj_matrix[row], P);
  }


  // Rm points already seen in dimension (only searching for new pts)
  void Hypercube::erase_seen_points(const qnums &seen, 
  qnum_matrix &shared_point_adjacency_lists) const {
    for(auto point : seen) 
      for(qnums &adj_list : shared_point_adjacency_lists) 
        sorted_erase(adj_list, point);
  }


  // Clears & refills "shared_point_adjacency_lists" from "adj_matrix", & pushes points 
  // from "cube_points" to "seen_points" set
  void Hypercube::register_and_derive_shared_point_adjacency_lists(qnums &seen_points,
  qnum_matrix &shared_point_adjacency_lists, const qnums &cube_points, 
  const qnum_matrix &adj_matrix) const {
    shared_point_adjacency_lists.clear();
    for(auto point : cube_points) {
      shared_point_adjacency_lists.push_back(adj_matrix[point]);
      sorted_insert(seen_points, point);
    }
  }


  // If all rows in "shared_point_adjacency_lists" share "edge" distinct elts w/ "n-edge"
  // other rows, return set of these shared elts --- else, return empty set
  void Hypercube::derive_shared_cube_points(const qnum n,const qnum edge,
  qnums &cube_points,const qnum_matrix &shared_point_adjacency_lists) const {
    cube_points.clear(); // holds the next set of shared points (if exists)
    qnum alist_idx = 0, other_alist_idx = 0;
    qnum shared_point_count = 0, shared_with_count = 0;

    for(auto alist : shared_point_adjacency_lists) {
      shared_point_count = 0;
      for(auto point : alist) {

        shared_with_count = other_alist_idx = 0;
        for(auto other_alist : shared_point_adjacency_lists) {
          if(sorted_find(other_alist, point) && other_alist_idx != alist_idx)
            ++shared_with_count;
          ++other_alist_idx;
        }

        if(shared_with_count == (n - edge)) {
          sorted_insert(cube_points, point);
          ++shared_point_count;
        }
      }

      if(shared_point_count != edge) { cube_points.clear(); return; }
      ++alist_idx;
    }
  }


  // Get all n-cubes w/in "adj_matrix" that can be formed containing point P.
  void Hypercube::derive_all_n_cubes_containing_point_P(const qnum P,const qnum n,
  qnum_matrix &inner_cube_matrix, qnum_matrix &adj_matrix) const {
    // Get "n" length combinations of P's values to try and form cubes w/
    qnum_matrix potential_cube_points_set;
    get_n_combinations_in_range(n,adj_matrix[P].begin(),adj_matrix[P].end(),{},potential_cube_points_set);

    qnum_matrix shared_point_adjacency_lists;
    qnums seen_points;
    qnum edge = 0;

    // Get all n-cubes formed from point P connecting to combinations of 
    // "edge" points from its adjacency list
    // NOTE: "cube_points" = idx "keys" for "shared_point_adjacency_lists"'s rows
    for(auto cube_points : potential_cube_points_set) { 
      seen_points = {P};
      edge = n-1;
      
      // Get initial series of point adj lists & push them to the set of "seen_points"
      register_and_derive_shared_point_adjacency_lists(seen_points,shared_point_adjacency_lists,cube_points,adj_matrix);

      // Determine wether P & its adjacent "cube_points" form a sub-cube
      for(; edge > 0; --edge) {
        erase_seen_points(seen_points, shared_point_adjacency_lists);
        derive_shared_cube_points(n, edge, cube_points, shared_point_adjacency_lists);
        if(cube_points.empty()) break; 
        register_and_derive_shared_point_adjacency_lists(seen_points,shared_point_adjacency_lists,cube_points,adj_matrix);
      }

      // If the above loop was never broken, P & its "cube_points" indeed seeded an n-cube. 
      // This n-cube's points were all registered w/in "seen_points" while verifying its existence.
      if(!edge) sorted_insert(inner_cube_matrix, seen_points);
    }
  }
};
#endif
