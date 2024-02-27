#ifndef TREE_H
#define TREE_H



#include <vector>
#include <iostream>

typedef struct Element{
	
	int vertice_A;
	int vertice_B;
	float peso;

}Aresta;

class Tree{
	public:
		Tree(int qVertices);
		int getFather(int vertice);
		void setFather(int vertice, int father);
		int getSize();
		int getRank(int vertice);
		void setRank(int vertice, int rank);

	private:
		// O nó é um vector de pares onde o first indica o pai dele
		// e o second indica o rank
		std::vector < std::pair<int,int> > * nodes;

};







#endif
