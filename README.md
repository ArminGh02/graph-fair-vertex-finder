
# Graph Fair Vertex Finder

- [About](#about)
- [Definition of Fair Vertex](#definition-of-fair-vertex)
- [Example](#example)
- [Compile](#compile)
- [To Do](#to-do)

## About
This was my final project for Data Structures and Algorithms course at Amirkabir University.
It calculates the fair vertex (vertices) in a non-negative weighted undirected graph given some source vertices.

## Definition of Fair Vertex
A fair vertex in a graph with a set of source vertices is a vertex with minimum fair score.

Consider the set of source vertices as: `{A, B, C, ...}`

The fair score for vertex `V` is calculated using below formula:

```
FairScore(V) = |Distance(A, V) - Distance(B, V)| + |Distance(A, V) - Distance(C, V)| + |Distance(B, V) - Distance(C, V)| + ...
```

## Example
Input graph should be of this form:
```
<number of vertices> <number of edges>
<space-separated list of vertices numbers>
<first vertex of edge> <second vertex of edge> <weight of edge>
...
```
![Example](/screenshot/screenshot.png "Example")

## Compile
Simply run:
```
g++ main.cpp -I ".\lib\fmt-8.1.1\include\"
```

## To Do
I implemented a minimal **[STL-like hash map](hash_map.hpp)** data structure that I wish to extend.
If you are a C++ fan, feel free to contribute.
