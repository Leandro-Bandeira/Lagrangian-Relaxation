#include "Data.h"
#include <iostream>
#include <chrono>
#include <iostream>
#include "Lagrange.h"
#include <iterator>
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include "graph.h"
#include <limits>
#include <list>
#include <iterator>
#include "Bnb.h"
#include "Kruskal.h"


typedef struct{
	int vertice;
	double valueAresta;
}tNode;


	
int main(int argc, char** argv){
	
	if(argc < 3){

		std::cout << "Digite ./main -nome instancia- -upper_bound-\n";
		exit(1);
	}
	
	
  auto data = Data(argc, argv[1]);
  data.read();
  size_t n = data.getDimension();

  double cost = 0.0;
  for (size_t i = 1; i < n; i++) {
      cost += data.getDistance(i, i+1);
  }
  cost += data.getDistance(n, 1);
 
  std::cout << "Welcome to lagrange Relaxation algorithm insert:\n"; 
  std::cout << "0: branching BFS\n";
  std::cout << "1: branching DFS\n";
  int bMethod = 0;
  std::cin >> bMethod;
  const int branchingMethod = bMethod;
  
  double** costsMatrix = data.getMatrixCost();
	std::vector < std::vector < double > >* costs = new std::vector<std::vector<double>>;
  
  for(int i = 0; i < n; i++){
    std::vector < double > costsLine;
    for(int j = 0; j < n; j++){
      costsLine.push_back(costsMatrix[i][j]);
    }
    costs->push_back(costsLine);
    costsLine.clear();
  }
	double upper_bound = std::stoi(argv[2]) + 1; // Upper_bound dado por alguma heuristica conhecida
	Lagrange lagrange(costs);
  auto inicio = std::chrono::high_resolution_clock::now();
	double lower_bound = lagrange.algorithm(upper_bound);

  std::vector < std::vector <int>>*lagrangeMatrix = lagrange.getLagrangeMatrix();
  std::vector < std::vector <double>>*lagrangeCosts = lagrange.getLagrangeCosts();

  Bnb bnb;
  bnb.algorithm(&lagrange,upper_bound, branchingMethod);
  auto fim = std::chrono::high_resolution_clock::now() - inicio;
  long long ms = std::chrono::duration_cast<std::chrono::seconds>(fim).count();
  std::cout << "time: " << ms << "\n";
  delete costs;
  return 0;
}

