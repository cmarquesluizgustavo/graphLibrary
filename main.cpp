#include <bits/stdc++.h>
#include "graph.h"
using namespace std;


int main(int argc, const char** argv){
	string filePath = argv[1];
	string representation = argv[2];
	string algorithm = argv[3];
	int selectedVertice = 1;
	int targetVertice  = 1;
	if (algorithm != "diameter"){
		string temp_selectedVertice = argv[4];
		selectedVertice = atoi(temp_selectedVertice.c_str());
	}
	if (algorithm == "distance"){
		string temp_targetVertice = argv[5];
		targetVertice = atoi(temp_targetVertice.c_str());
	}
  if (representation == "matrix"){
  	Matrix Adjacency_Matrix = Matrix(filePath, algorithm, selectedVertice, targetVertice);
	}
  else if (representation == "list"){
  	List Adjacency_List = List(filePath, algorithm, selectedVertice, targetVertice);
  }
	else{
  	cout << "Formato inválido.";
	}
  
  return 0;
}
