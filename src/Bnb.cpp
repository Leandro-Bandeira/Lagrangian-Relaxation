#include "Bnb.h"
#include <time.h>
#include <iostream>
#include <algorithm>


Bnb::Bnb(){
  this->idGeneral = 0;
}

/* Retorna o indice do nó dado pela estratégia de branching
 * BFS = 0 (sempre será escolhido o primeiro nó)
 * DFS = 1 (sempre será escolhido o ultimo nó)
 */
int Bnb::branchStrategy(){
  int size = 2;
  std::srand(time(0));

  return std::rand() % size > 0 ? this->tree.size() - 1: 0; 
  
}



void Bnb::fullNode(NodeInfoBnb* node, Lagrange* lagrangeSolution){
  node->lagrangeSolution = lagrangeSolution;
  node->lower_bound = lagrangeSolution->getBestLowerBound();
  std::vector < std::vector <int >> *matrixAdj = lagrangeSolution->getLagrangeMatrix();
  std::vector < int > rates(matrixAdj->size(), 0);

  for(int i = 0; i < matrixAdj->size(); i++){
    for(int j = i + 1; j < matrixAdj->at(i).size(); j++){
      if(matrixAdj->at(i)[j] == 1){
        rates[i] += 1;
        rates[j] += 1;
      }
    }
  }

  int indexNodeMoreRate = std::distance(rates.begin(), std::max_element(rates.begin(), rates.end()));
  node->nodeChosen = indexNodeMoreRate;
  node->id = idGeneral;
  idGeneral+=1;
}

void Bnb::algorithm(Lagrange* lagrangeSolutionInit, double upper_bound){
  NodeInfoBnb root; /* Nó raiz = será dado pela resolução da relaxacao */

  this->tree.push_back(&root);
  
  /*Bnb algorithm */
  while(!this->tree.empty()){
    int indexNode = branchStrategy();
    /* Retorna o nó dependendo do método utilizado */
    NodeInfoBnb* node = (indexNode == 0 ? this->tree.front() : this->tree.back());
    fullNode(node, lagrangeSolutionInit);

    /* Avança o iterator para a posição do nó escolhido para depois apagá-lo */ 
    std::list < NodeInfoBnb*>::iterator init = this->tree.begin();
    advance(init, indexNode);
    
    if(node->lower_bound >= upper_bound){
      this->tree.erase(init);
      continue;
    }

    if(node->feasible){

    }
    else{
      
    }
     

  }
}
