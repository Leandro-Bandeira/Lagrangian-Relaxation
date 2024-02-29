#include "tree.h"





Tree::Tree(int qVertices){

	this->nodes = new std::vector < std::pair<int,int> > (qVertices);
}


int Tree::getFather(int vertice){

	return (this->nodes->at(vertice).first);
}

void Tree::setFather(int vertice, int father){

	this->nodes->at(vertice).first = father;
}


int Tree::getRank(int vertice){

	return (this->nodes->at(vertice).second);
}
 
void Tree::setRank(int vertice, int rank){
	
	this->nodes->at(vertice).second = rank;

}
int Tree::getSize(){

	return this->nodes->size();
}
