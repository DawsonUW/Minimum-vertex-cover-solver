# Minimum-vertex-cover-solver
- Goal: To install the **minimum** number of security cameras at intersections of roads.
    
- it is an optimization problem: **Minimum Vertex Cover** problem (a NP-hard:**non-deterministic polynomial-time hardness** problem).
    
- **Vertex Cover** of a graph is a set of vertices that includes **at least one endpoint of every edge** of the graph.
    
    - **Minimum Vertex Cover** (also minimum-sized vertex cover): a vertex cover of smallest possible number of vertices

|     |     |
| --- | --- |
| A1  | Given street info, find corresponding undirected **graph** (Python) |
| A2  | Given an undirected graph, and two vertices of graph;  find the shortest-path between these two vertices. (C++) |
| A3  | -  generate random input for assignment 1. (C++)<br>\- modify A1 (Python)  <br>\- modify A2 (C++)<br>\- IPC: link generated random input to assignment 1, and link output of assignment 1 to input of assignment 2 (C++) |
| A4  | \- use `MiniSat` as a library to solve the minimum vertex cover problem for the graph (C++)<br>NOTE:<br>\- SAT(Satisfiability) solver: an algorithm for establishing satisfiability. Given Boolean logic formula, return SAT if it finds a combination of variables that can satisfy it.<br>\- CNF: Conjunctive Normal Form. |
| A5  | \- implement two additional ways to solve minimum vertex cover problem for the graph (C++)<br>\- multithreading: 4 threads: I/O, 2 ways above, A4 (C++)<br> - result plotting (Python) |
