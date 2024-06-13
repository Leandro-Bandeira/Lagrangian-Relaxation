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

std::vector < std::vector < double > > * leitorInstancia(char* instanciaName){
	
	std::string linha;
	std::string valor;
	
	std::fstream* arquivo = new std::fstream(instanciaName, std::ios::in);



	std::vector < std::vector <double>  > * costs = new std::vector < std::vector < double > >();

	while(std::getline(*arquivo, linha)){
		std::vector < double > arestaValores;
		std::stringstream dadosLinha(linha);
		arestaValores.clear();

		while(std::getline(dadosLinha, valor, ' ')){
			
			std::string::size_type sz;

			arestaValores.push_back(std::stod(valor, &sz));
		}

		costs->push_back(arestaValores);

	}
	
	arquivo->close();
  delete arquivo;
	return costs;


}

	
int main(int argc, char** argv){
	
	if(argc < 3){

		std::cout << "Digite ./main -nome instancia- -upper_bound-\n";
		exit(1);
	}


	std::vector < std::vector < double > >* costs = leitorInstancia(argv[1]);
	double upper_bound = std::stoi(argv[2]); // Upper_bound dado por alguma heuristica conhecida
	Lagrange lagrange(costs);
	double lower_bound = lagrange.algorithm(upper_bound);
  std::cout << "here" << std::endl; 	
  std::vector < std::vector <int>>*lagrangeMatrix = lagrange.getLagrangeMatrix();
  std::vector < std::vector <double>>*lagrangeCosts = lagrange.getLagrangeCosts();
  std::cout << "lagrangeMatrix best solution: " << lagrangeMatrix->size() << "\n";
  for(int i = 0; i < lagrangeMatrix->size(); i++){
    for(int j = 0; j < lagrangeMatrix->at(i).size(); j++){
      std::cout << lagrangeMatrix->at(i)[j] <<  " ";
    }
    std::cout << "\n";
  }
  std::cout << "lagranteCosts best solution: \n";
  for(int i = 0; i < lagrangeCosts->size(); i++){
    for(int j = 0; j < lagrangeCosts->at(i).size(); j++){
      std::cout << lagrangeCosts->at(i)[j] << " ";
    }
    std::cout << "\n";
  }
  /* Após resolver o dual lagrangiano, vamos utilizar o BNB */
  Bnb bnb;
  bnb.algorithm(&lagrange,upper_bound);

  return 0;
}
