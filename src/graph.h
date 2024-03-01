#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include "tree.h"



/* CLasse responsável por realizar operações sob o grafo*/
class Graph{
    public:
        Graph(std::vector < std::vector<int>>*matrixAdj);
        void calculateRates();
        std::vector < int > * getRates();

    private:
        std::vector < int > rates; // Grau de cada vértice da matriz
        std::vector < std::vector < int>>* matrixAdj;
      
};












#endif