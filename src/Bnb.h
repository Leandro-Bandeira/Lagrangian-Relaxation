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
    int branchStrategy();  
    void algorithm(Lagrange* lagrangeSolutionInit, double upper_bound);
    void fullNode(NodeInfoBnb* node);
    void dfs(int v, NodeInfoBnb* node);
    void initDfs();
    static int idGeneral;
    static int t;
private:
    std::vector < std::vector<double>>*costMatrix;
    std::vector< std::vector<int>>*matrixAdj;
    std::vector <std::vector < int>>listAdj;
    std::vector < int > PE; /*Vector utilizado para a busca em profundidade*/
    std::vector <int> v_fathers;
    std::vector < int > PS;
    std::vector < int > visitedVertices;
    std::list<NodeInfoBnb*>tree;
};









#endif

