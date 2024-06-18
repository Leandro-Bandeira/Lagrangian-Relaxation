#ifndef BNB_H
#define BNB_H


#include <vector>
#include <list>
#include "Lagrange.h"


typedef struct{
  std::vector < std::pair<int,int>> forbbiden_arcs; /* Arcos proibidos */
  int nodeChosen; /* Nó escolhido (aquele com maior grau) */ 
  double lower_bound; /* Custo da solução no algoritmo hungaro */
  bool feasible; /*Indica a viabilidade da solução */
  Lagrange* lagrangeSolution;
  int id;
}NodeInfoBnb;



class Bnb{
  public:
    Bnb();
    void algorithm(Lagrange* lagrangeSolutionInit, double upper_bound, const int branchingStrategy);
    void fullNode(NodeInfoBnb* node);
    bool viabilityChecker();
    void deleteNode(std::list<NodeInfoBnb*>::iterator& posIterator);
    static int idGeneral;
    static int t;
private:
    std::vector < std::vector<double>>*costMatrix;
    std::vector< std::vector<int>>*matrixAdj;
    std::vector <std::vector < int>>listAdj;
    std::list<NodeInfoBnb*>tree;
};









#endif

