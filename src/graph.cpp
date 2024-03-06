#include "graph.h"


Graph::Graph(std::vector <std::vector<int>>* matrixAdj){
    this->matrixAdj = matrixAdj;
}

void Graph::calculateRates(){
    
    int qEdges = matrixAdj->size();
    this->rates.resize(qEdges, 0);

    /* A ideia é percorrer apenas o triangulo superior
    toda vez que haver a aresta i-j aumentamos em o grau daquele vértice
    o indice do vector indica o grau daquele vértice.
    Devemos aumentar tanto no indice i como no indice j, por exemplo,
    1 2 3
    2 3
    Acima está a linha 0 e a linha 1, perceba que o vértice 0 se conecta com o 1
    mas o vértice 0 não aparece na linha 1, mas temos que aumentar o grau dos dois*/
    for(int  i = 0; i < qEdges; i++){
        for(int j = i + 1; j < qEdges; j++){

            if((*this->matrixAdj)[i][j]){
                this->rates[i] += 1;
                this->rates[j] += 1;
            }
        }
    }
  
}

std::vector <int> * Graph::getRates(){
    return &(this->rates);
}