#include <iostream>
#include "kruskal.h"
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

typedef struct{
	int vertice;
	double valueAresta;
}tNode;


typedef struct{
  std::vector < std::pair<int,int>> forbbiden_arcs; /* Arcos proibidos */
  double lower_bound; /* Custo da solução no algoritmo hungaro */ 
  int nodeChosen; /* Nó escolhido (aquele com maior grau) */ 
  bool feasible; /*Indica a viabilidade da solução */ 

}NodeInfoBnb;



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




/* Retorna o indice do nó dado pela estratégia de branching
 * BFS = 0 (sempre será escolhido o primeiro nó)
 * DFS = 1 (sempre será escolhido o ultimo nó)
 */
int branchStrategy(std::list<NodeInfoBnb*>*tree){
  int size = 2;
  std::srand(time(0));

  return std::rand() % size > 0 ? tree->size() - 1: 0; 
  
}


	
int main(int argc, char** argv){
	
	if(argc < 3){

		std::cout << "Digite ./main -nome instancia- -upper_bound-\n";
		exit(1);
	}


	std::vector < std::vector < double > >* costs = leitorInstancia(argv[1]);
	double upper_bound = std::stoi(argv[2]); // Upper_bound dado por alguma heuristica conhecida
  	std::vector < std::vector <int>>lagrangeMatrix;
	Lagrange lagrange(costs);
	double lower_bound = lagrange.algorithm(upper_bound);
  	

  /* Após resolver o dual lagrangiano, vamos utilizar o BNB */
  NodeInfoBnb root; /* Nó raiz = será dado pela resolução da relaxacao */


  std::list<NodeInfoBnb*>tree;
  tree.push_back(&root);

  /*Bnb algorithm */ 
  while(!tree.empty()){
    int indexNode = branchStrategy(&tree);
    /* Retorna o nó dependendo do método utilizado */
    NodeInfoBnb* node = indexNode == 0 ? tree.front() : tree.back();
    
    /* Avança o iterator para a posição do nó escolhido para depois apagá-lo */ 
    std::list < NodeInfoBnb*>::iterator init = tree.begin();
    advance(init, indexNode);

    if(node->lower_bound > upper_bound){
      tree.erase(init);
      continue;
    }
     

  }


  return 0;
}
