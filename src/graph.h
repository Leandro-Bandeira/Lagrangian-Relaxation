#ifndef GRAPH_H
#define GRAPH_H


class Graph{
    public:
        Graph(std::vector < std::vector < double >>* matrixP);
        void calculateRates();

    private:
        std::vector < int > rates; // Grau de cada vértice da matriz
        std::vector < std::vector < double>>* matrixP;
};












#endif