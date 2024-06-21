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


/* Realiza a leitura de uma matriz de custos */
std::vector < std::vector < double > > * readInstance(char* instanceName){
	
	std::string line;
	std::string value;
	
	std::fstream* fstreamFile = new std::fstream(instanceName, std::ios::in);

	std::vector < std::vector <double>  > * costs = new std::vector < std::vector < double > >();

	while(std::getline(*fstreamFile, line)){
		std::vector < double > costsLine;
		std::stringstream dataLine(line);
		costsLine.clear();

		while(std::getline(dataLine, value, ' ')){
			
			std::string::size_type sz;

			costsLine.push_back(std::stod(value, &sz));
		}

		costs->push_back(costsLine);

	}
	
	fstreamFile->close();
  delete fstreamFile;
	return costs;


}

	
int main(int argc, char** argv){
	
	if(argc < 3){

		std::cout << "Digite ./main -nome instancia- -upper_bound-\n";
		exit(1);
	}

  std::cout << argv[1] << "\n";
  std::cout << "Welcome to lagrange Relaxation algorithm insert:\n"; 
  std::cout << "0: branching BFS\n";
  std::cout << "1: branching DFS\n";
  int bMethod = 0;
  std::cin >> bMethod;
  const int branchingMethod = bMethod;

	std::vector < std::vector < double > >* costs = readInstance(argv[1]);
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
