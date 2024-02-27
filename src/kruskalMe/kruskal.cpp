#include "kruskal.h"


Kruskal::Kruskal(Tree* tree,std::vector<std::vector<float>>*grafo ){

	this->tree = tree;
	this->grafo = grafo;
	this->result = 0;

	int qVertices = this->tree->getSize();
	std::vector < int > matriz(qVertices,0);

	for(int i = 0; i < qVertices; i++){
		this->matrizAdj.push_back(matriz);
	}
}


void Kruskal::makeSet(){

	int qVertices = this->tree->getSize();
	
	// Seta o pai de cada nó inicialmente como sendo ele próprio
	// OU seja, cada nó é um nó raiz inicialmente
	/* Vamos começar em 1 pois estamos ignorando o nó zero*/
	for(int i = 1; i < qVertices; i++){
		
			this->tree->setFather(i, i);
			this->tree->setRank(i, 0);
	}
}


// A função findSet é uma função recursiva que retorna o pai de um elemento
// vamos subindo a arvore recursivamente, então seu caso base é quando
// encontramos a raiz, ou seja, o pai do nó é o próprio nó pai[x] = x
// e quando estivermos voltando na recursividade
// devemos atualizar os valores do pai de todos os outros nós para o nó raiz
// esse metodo anteriormente descrito é o pathcompression
int Kruskal::findSet(int vertice){
	int i = vertice;

	if (this->tree->getFather(i) == i){
			
		return i;
	}	
	else{ 
		  
		int father = this->tree->getFather(i);
		int raiz = findSet(father);
		this->tree->setFather(i, raiz);
		return raiz;
	} 
}

// Na função setUnion temos que encontrar a raiz de ambos os vertices
// Dito isso temos que chamar a função find-set para cada vertice e então
// verificar o rank de cada raiz, a raiz de rank menor terá o pai como sendo
// a raiz de rank maior para construir arvores mais balanceadas
int Kruskal::setUnion(int vertice_a, int vertice_b){
	
	int raiz_a = findSet(vertice_a);
	int raiz_b = findSet(vertice_b);
	if (raiz_a == raiz_b){
		return 0;
	}
	int rank_a = this->tree->getRank(raiz_a);
	int rank_b = this->tree->getRank(raiz_b);
	

	
	int vertice_maiorRank = rank_a <= rank_b ? raiz_b : raiz_a;
	int vertice_menorRank = rank_a <= rank_b ? raiz_a : raiz_b;
		

	this->tree->setFather(vertice_menorRank, vertice_maiorRank);
	
	
	int newRank = 0;
	if(this->tree->getRank(vertice_maiorRank) == 0){
		newRank += 1;
	}
	else{
		newRank = this->tree->getRank(vertice_maiorRank) + this->tree->getRank(vertice_menorRank);
	}
	

	this->tree->setRank(vertice_maiorRank, newRank);
	return 1;

}

/* Função responsável por calcular o grau de cada vértice
* Percorre o vetor de arestas incluida e verifica se o vertice está nela, caso sim
* incremeta o grau em 1 */
void Kruskal::calculate_grau(){
	
	int qVertices = this->tree->getSize();

	for(int  i = 0; i < qVertices; i++){
		
		int g = 0;
		for(Aresta  aresta : arestasIn){
			
			if(aresta.vertice_A == i or aresta.vertice_B == i){
				g++;
			}
		}
		this->graus.push_back(std::make_pair(i, g));
	}
}
std::vector < std::pair <int,int>>* Kruskal::getGraus(){

	return &(this->graus);

}

std::vector < std::vector <int >> * Kruskal::getMatrizAdj(){

	return &(this->matrizAdj);
}


void Kruskal::setMatrizAdj(){
	int qVertices = this->tree->getSize();
	

	for(int i = 0; i < qVertices; i++){
		for(Aresta aresta: arestasIn){
			this->matrizAdj[aresta.vertice_A][aresta.vertice_B] = this->matrizAdj[aresta.vertice_B][aresta.vertice_A] = 1;			
		}
	}
}
void Kruskal::algorithm(){
		
	makeSet();

	
		/* Função lambda responsável pela comparação em ordem não decrescente	*/
	auto comp = [](const Aresta& a, const Aresta& b){

		return a.peso >= b.peso;
	};

	//decltype pega a função sem reavaliar seu valor
	std::priority_queue< Aresta, std::vector<Aresta>,decltype(comp) > arestasOrdenadas(comp);
	/* Percorrendo as arestas	*/
	int qVertices = this->tree->getSize();
	for(int i = 1; i < qVertices; i++){
		
		int col = this->grafo->at(i).size();

		for(int j = 1; j < col; j++){
		
			/* Cria-se a aresta e então adiciona na priority_queue	*/
			Aresta aresta;
			aresta.vertice_A = i;
			aresta.vertice_B = j;
			aresta.peso = (this->grafo->at(i))[j];
			if (aresta.peso == 0)
				continue;
			
			arestasOrdenadas.push(aresta);
		}

	}
	
	
	while(!arestasOrdenadas.empty()){
		Aresta aresta = arestasOrdenadas.top();
		
		int vertice_a = aresta.vertice_A;
		int vertice_b = aresta.vertice_B;
		
		int resultUnion = setUnion(vertice_a, vertice_b);
		if (resultUnion){
			this->arestasIn.push_back(aresta);
			result += aresta.peso;
		}
		
		arestasOrdenadas.pop();

	}
	
	std::cout << "Arestas Utilizadas" << std::endl;
	for(int  i = 0; i < this->arestasIn.size(); i++){
		std::cout << this->arestasIn[i].vertice_A << "--" << this->arestasIn[i].vertice_B << std::endl;
	}
	
	calculate_grau();
	setMatrizAdj();


}
