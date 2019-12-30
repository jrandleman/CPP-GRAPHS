# QGRAPH-HPP
## _Hypercube Analysis For Dimensions <= # bits in `unsigned long long`!_
### _Compile >= C++17!_
----------------------

## General Information For [`qgraph.hpp`](https://github.com/jrandleman/CPP-GRAPHS/blob/master/QGRAPH-HPP/qgraph.hpp):
### Library's 4 Predefined Types in Namespace `qgraph`:
1) `qnum`: `unsigned long long` (_for **ALL** numerics in `qgraph`_)</br>
2) `qnums`: `vector<qnum>`</br>
3) `qnum_matrix`: `vector<qnums>`</br>
4) `coord_matrix`: `vector<qnum_matrix>`</br>

### Refer to [`qgraph_SampleExec.cpp`](https://github.com/jrandleman/CPP-GRAPHS/blob/master/QGRAPH-HPP/qgraph_SampleExec.cpp) for a demo!

----------------------

## 3 Non-Member Functions:
* ***NOTE:** "coord" in this README is short for "coordinate"!*</br>
1) `qnum` `convert_coord_to_point`(`qnums`) => _Converts coords to a point_
2) `qnums` `shortest_path`(`qnum`, `qnum`) => _Returns shortest path btwn given pts_
3) `qnum_matrix` `shortest_path`(`qnums`, `qnums`) => _Returns shortest path's coords btwn given coords_

----------------------

## 19 Members of the `qgraph::Hypercube` Struct:
* ***NOTE:** Further manipulates fcns returned by `SOP_str` & `POS_str`!*
1) `qnum` `dimension` => _Member dicating Hypercube's dimension_
2) `qnum` `total_points`() => _Returns total points in 'dimension'_
------
3) `Hypercube`() => _Defaults 'dimension = 0'_
4) `Hypercube`(`qnums`) => _Given dimension_
5) `Hypercube`(`Hypercube`) => _Given another hypercube_
------
6) `qnums` `convert_point_to_coord`(`qnum`) => _Converts point to its coords in 'dimension' space_
7) `qnums` `adjacent_points`(`qnum`) => _Returns list of points adjacent to given pt in 'dimension'_
8) `qnums` `adjacent_points`(`qnums`) => _Returns list of points adjacent to given coord in 'dimension'_
9) `qnum_matrix` `adjacent_coords`(`qnum`) => _Returns list of pt-coords adjacent to given pt in 'dimension'_
10) `qnum_matrix` `adjacent_coords`(`qnums`) => _Returns list of pt-coords adjacent to given coord in 'dimension'_
------
11) `qnum_matrix` `point_adjacency_matrix`() => _Returns adjacency list matrix for all points in 'dimension'_
12) `coord_matrix` `coord_adjacency_matrix`() => _Returns adjacency list matrix for all pt-coords in 'dimension'_
13) `qnum` `total_inner_cubes`(`qnum`) => _Returns # of given sub-dimension cubes w/in 'dimension'_
14) `qnum` `total_inner_cubes`(`Hypercube`) => _Returns # of given Hypercubes w/in 'dimension'_
15) `qnum` `total_inner_cubes`() => _Returns # of all nested Hypercubes in 'dimension'_
------
16) `qnum_matrix` `inner_cubes`(`qnum`) => _Given dimension n, returns matrix of all n cubes' pts in 'dimension'_
17) `qnum_matrix` `inner_cubes`(`Hypercube`) => _Given H-cube, returns matrix of all its pts in 'dimension'_
18) `coord_matrix` `inner_cubes_coord`(`qnum`) => _Given dim n, returns matrix of all n cubes' pt-coords in 'dimension'_
19) `coord_matrix` `inner_cubes_coord`(`Hypercube`) => _Given H-cube, returns matrix of all it's pt-coords in 'dimension'_
