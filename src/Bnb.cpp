#include "Bnb.h"
#include <cstdio>
#include <time.h>
#include <iostream>
#include <algorithm>


int Bnb::idGeneral = 0;
int Bnb::t = 0;


Bnb::Bnb(){
}

  
/* Uma solução viável é um ciclo hamiltoniano 
 * logo cada vértice possui apenas 2 vizinhos
 * caso contrário não é um ciclo hamiltoniano
 * e a solução é inviável */
bool Bnb::viabilityChecker(){
  
  bool feasible = true;
  const int sizeList = listAdj.size();

  int count = 0;
  for(int i = 0; i < sizeList; i++){
    const int sizeNeighbors = listAdj[i].size();
    if(sizeNeighbors == 2){
      count++;
    }
  }

  if (count == sizeList){
    feasible = true;
  }else{
    feasible = false;
  }
  return feasible;
  
}

/* Função responsável por preencher as informações do nó 
 * a partir da solução lagrangeana */ 

void Bnb::fullNode(NodeInfoBnb* node){
  Lagrange *lagrangeSolution = node->lagrangeSolution;
  node->lower_bound = lagrangeSolution->getBestLowerBound();
  std::vector < std::vector <int >> *matrixAdj = lagrangeSolution->getLagrangeMatrix();
  std::vector < int > rates(matrixAdj->size(), 0);
  
  std::vector < int > neighboors;
  this->listAdj.clear();
  for(int i = 0; i < matrixAdj->size(); i++){
    for(int j = 0; j < matrixAdj->size(); j++){
      if(matrixAdj->at(i)[j])
        neighboors.push_back(j);
    }
    listAdj.push_back(neighboors);
    neighboors.clear();
  } 
  for(int i = 0; i < matrixAdj->size(); i++){
    for(int j = i + 1; j < matrixAdj->at(i).size(); j++){
      if(matrixAdj->at(i)[j] == 1){
        rates[i] += 1;
        rates[j] += 1;
      }
    }
  }
   

  node->feasible = viabilityChecker();
  int indexNodeMoreRate = std::distance(rates.begin(), std::max_element(rates.begin(), rates.end()));
  node->nodeChosen = indexNodeMoreRate;
  node->id = Bnb::idGeneral;
  Bnb::idGeneral+=1;
}



void Bnb::deleteNode(std::list<NodeInfoBnb*>::iterator& postIterator){
  
  /* Primeiro vamos deletar todas as informações da solução lagrangeana */
  
  this->tree.erase(postIterator);
  //delete (*postIterator);
}




void Bnb::algorithm(Lagrange* lagrangeSolutionInit, double upper_bound_lagrange, const int branchingStrategy){
  NodeInfoBnb root; /* Nó raiz = será dado pela resolução da relaxacao */
  root.lagrangeSolution = lagrangeSolutionInit;
  fullNode(&root);
  this->tree.push_back(&root);
  int initBnb = 0;
  double upper_bound = root.lower_bound;
  int finded = 0;
  /*Bnb algorithm */
  while(!this->tree.empty()){

    int indexNode = 0;
    /* Avança o iterator para a posição do nó escolhido para depois apagá-lo */ 
    std::list < NodeInfoBnb*>::iterator init = this->tree.begin();

    if(branchingStrategy == 1){
      indexNode = this->tree.size() - 1;
      advance(init, indexNode);
    }
    /* Retorna o nó dependendo do método utilizado */
    NodeInfoBnb* node = (indexNode == 0 ? this->tree.front() : this->tree.back());
    if (initBnb != 0){
      fullNode(node);
    }

    if(node->lower_bound > upper_bound_lagrange - 1){
      deleteNode(init);
      continue;
    }
    

    if(node->feasible){
      upper_bound = std::max(upper_bound, node->lower_bound);
      if(finded == 0){
        std::cout << "found feasible: " << upper_bound << "\n";
        finded = 1;
      }
    }
    /* Para criar os filhos, as seguintes etapas são necessarias 
     * 1 - Armazenar a matriz de custos do pai
     * 2 - Saber quem são os arcos proibidos
     * 3 - Modificar a matriz de custos original com base nos arcos proibidos 
     * 4 - Calcular a solução de lagrange para o nó */
    else{
      std::vector<std::vector<double>> fatherCosts = *(node->lagrangeSolution->getLagrangeCosts());
      std::vector < int > neighborsForbidden = listAdj[node->nodeChosen];
      for(int i = 0 ; i < neighborsForbidden.size(); i++){
        NodeInfoBnb* n = new NodeInfoBnb;
        int nodeForbidden = neighborsForbidden[i];
        std::vector <std::vector <double>> currentCosts = fatherCosts;
        currentCosts[node->nodeChosen][nodeForbidden] = currentCosts[nodeForbidden][node->nodeChosen] = 99999999;
        Lagrange* currentLagrange = new Lagrange(&currentCosts);
        double valueChield = currentLagrange->algorithm(upper_bound_lagrange);
        n->lagrangeSolution = currentLagrange;
        tree.push_back(n);
      }
    }
    deleteNode(init);
    initBnb += 1;     
    
  }
  std::cout << "upper_bound: " << upper_bound << std::endl;
}
