# graphLibrary
A simple, but pratical graph (from graph theory, not like plots and charts) C++ Library

To test this code or run it stand alone, you must use the main.cpp. You must complie all codes together, and then run output compiled code
passing the arguments as follows: ./main <input_graph.txt> <matrix or list representation> <algorithm> <selected vertice - optional> <target vertice - optional>

The implemented algorithms (and how to write then on the code in parentheses) so far are:
Breadth-first search (BFS) - requires selected vertices to start the search
Depth-first search (DFS) - requires selected vertices to start the search
Diameter (diameter)
Distance (distance) - requires two vertices to have the distance measured

Examples:

// BFS using adjacency list, root -> node 1

./main graph_file.txt list BFS 1

=================================

// Distance between nodes 1 and 2:

./main graph_file.txt x distance 1 2
