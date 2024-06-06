#ifndef LAGRANGE_H
#define LAGRANGE_H

#include <vector>

class Lagrange{
    public:
        Lagrange(std::vector<std::vector<double>>* costs);
        double algorithm(double upper_bound);
    private:
        std::vector<std::vector<int>> lagrangeMatrix;
        std::vector<std::vector<double>>* initCosts;
};
















#endif