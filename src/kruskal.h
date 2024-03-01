#ifndef KRUSKAL_H
#define KRUSKAL_H

#include "tree.h"
#include <queue>


class Kruskal{
	public:
		Kruskal(Tree* tree, std::vector< std::vector<double>>*grafo);
		void makeSet();
		int findSet(int vertice);
		int setUnion(int vertice_a, int vertice_b);
		void algorithm();
		std::vector < std::vector <int>>* getMatrizAdj();
		void setMatrizAdj();
		std::vector < Aresta > * getEdges();
		double result;
	private:
		Tree* tree;
		std::vector <std::vector <int>> matrizAdj;
		std::vector < std::vector < double>>*grafo;
		std::vector < Aresta > arestasIn;
};






#endif
