#ifndef LAGRANGE_H
#define LAGRANGE_H

#include <vector>

class Lagrange{
    public:
        Lagrange(std::vector<std::vector<double>>* costs);
        double algorithm(double upper_bound);
        std::vector < std::vector<int>>* getLagrangeMatrix(){return &this->lagrangeMatrix;};
        std::vector < std::vector<double>>* getLagrangeCosts(){return &this->lagrangeCosts;};
        double getBestLowerBound(){return this->best_lower_bound;};
    private:
        std::vector<std::vector<int>> lagrangeMatrix;
        std::vector<std::vector<double>> lagrangeCosts;
        std::vector<std::vector<double>>* initCosts;
        double best_lower_bound;
        int nodeMoreRateBest; // Vértice de maior grau da solução
};
















#endif
