#include <iostream>
#include "kruskal.h"
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include "graph.h"

typedef struct{
	int vertice;
	double valueAresta;
}tNode;

std::vector < std::vector < double > > * leitorInstancia(char* instanciaName){
	
	std::string linha;
	std::string valor;
	
	std::fstream* arquivo = new std::fstream(instanciaName, std::ios::in);



	std::vector < std::vector <double>  > * grafo = new std::vector < std::vector < double > >();

	while(std::getline(*arquivo, linha)){
		std::vector < double > arestaValores;
		std::stringstream dadosLinha(linha);
		arestaValores.clear();

		while(std::getline(dadosLinha, valor, ' ')){
			
			std::string::size_type sz;

			arestaValores.push_back(std::stod(valor, &sz));
		}

		grafo->push_back(arestaValores);

	}
	
	arquivo->close();
	return grafo;


}

/* Verifica se os pesos são todos nulos*/
bool not_violation(std::vector<int>* subgradient){
	int sum = 0;

	for(int i = 0; i < subgradient->size(); i++){
		
		sum += std::abs(subgradient->at(i));
	}
	return sum;
}



int main(int argc, char** argv){
	
	if(argc < 3){

		std::cout << "Digite ./main -nome instancia- -upper_bound-\n";
		exit(1);
	}


	std::vector < std::vector < double > >* grafo = leitorInstancia(argv[1]);
	std::vector < std::vector < double > > grafoOriginal = *grafo;
	
	
	
	

	/* Configuração da relaxação */	
	int qVertices = grafo->size();
	double upper_bound = std::stoi(argv[2]); // Upper_bound dado por alguma heuristica conhecida
	std::vector < double > harsh(qVertices, 0); // Vetor penalizador
	std::vector < int > subgradient(qVertices, 0); // Vetor subgradient
	double epslon = 1;
	double epslon_min = 0.0005;
	double w_ot = 0;
	int k_max = 30;
	int k = 0;
	double step = 1.0;
	

	/* Como estamos resolvendo um problema relaxado
	Queremos encontrar o melhor lower bound possivel,
	pois como é um problema relaxado não é possivel ele ser melhor 
	do que uma soluçao do problema original. Desse modo, para o TSP
	é necessário achar uma árvore geradora mínima que não inclua o nó zero
	Se acharmos uma solução em que os vértices não violem a restrição relaxada,
	temos então a solução ótima para o problema relaxado como também para o problema original.
	Os step do algoritmo são o seguinte, resolvemos o problema relaxado sem o nó zero
	achado sua solução ótima (w), adiciona o nó zero nas duas melhores arestas, soma-se o valor do w com as duas novas arestas
	então adiciona a w o valor dos penalizadores
	Apos isso calcula o vetor subgradiente, que é dado por 2 - grau de cada vértice
	calcula o passo e então muda o valor dos penalizadores, se acharmos um w melhor que o w_ot, atualizamos e resetamos k*/
	do{
		/* Primeiro temos que alterar a matriz de peso de acordo com o vetor de pesos */
		/* Inicialmente vamos alterar a matriz toda, depois otimizados para utilizar apenas o triangulo superior
			No primeiro loop esse vetor é nulo então o grafo de peso é o mesmo*/
		for(int i = 0; i < qVertices; i++){
			
			for(int j = 0; j < qVertices; j++){
				if(i == j)
					continue;
				(*grafo)[i][j] = grafoOriginal[i][j]- harsh[i] - harsh[j];
			}	
		}
		
		
		//std::cout << "Grafo Atualizado" << std::endl;
		/* Printando nova matriz para debug */
		
		
		/* Inicializando o algoritmo de kruskal */
		Tree* tree = new Tree(qVertices);
		Kruskal kruskal(tree, grafo);
		kruskal.algorithm();
		double w = kruskal.result;
		//std::cout << "Valor do kruskal: " << w << std::endl;
		std::vector < std::vector < int > >* matrizAdj = kruskal.getMatrizAdj();
		/*Fim do algoritmo de kruskal */
		
	
		/* Nós ordenados de acordo com sua distancia em relação ao nó zero*/
		std::vector < std::pair<int, double> > nodesSortedByEdge0;
		

		/* Adiciona todos os vértices e suas respectivas distancias ao nó zero	*/
		for(int j = 1; j < qVertices; j++){
			nodesSortedByEdge0.push_back(std::make_pair(j, grafo->at(0)[j]));
		}

		/* A função indica se a deve ser ordenado antes de b ou não,
		se for true, a será ordenado antes de b, caso não será b
		então se a for menor ou igual a b, a será ordenado antes de b
		Logo estamos organizando em ordem descrecente, para podermos pegar os vértices em o(1)*/
		sort(nodesSortedByEdge0.begin(), nodesSortedByEdge0.end(), [](const std::pair<int,double>&a, std::pair<int,double>const &b)
		{
				return a.second >= b.second;
		});

		int vertice_a = nodesSortedByEdge0.back().first;
		nodesSortedByEdge0.pop_back();
		int vertice_b = nodesSortedByEdge0.back().first;
		
		/* Ativando as arestas */
		(*matrizAdj)[0][vertice_a] = (*matrizAdj)[0][vertice_b] = 1;
		(*matrizAdj)[vertice_a][0] = (*matrizAdj)[vertice_b][0] = 1;


		Graph graphOperation = Graph(matrizAdj);
		graphOperation.calculateRates();
		std::vector< int> * rates = graphOperation.getRates();
		
		w += grafo->at(0)[vertice_a] + grafo->at(0)[vertice_b];


		for(int i = 0; i < qVertices; i++){
			w += 2 * harsh[i];
		}

		double PI = 0;
		/* Calculando o vetor subsubgradient*/
		for(int i = 0; i < qVertices; i++){
			subgradient[i] = 2 - (*rates)[i];
			PI += (subgradient[i] * subgradient[i]);
		}

		step = ((epslon * (upper_bound - w))) / PI;

		/* Altera o vetor de pesos */
		for(int i = 0; i < qVertices; i++){
			harsh[i] += (step * subgradient[i]);
		}


		if(w > w_ot){	
			w_ot = w;
			std::cout << "Novo valor do lower bound: " << w_ot << std::endl;
			if(!not_violation(&subgradient) or w_ot == upper_bound)
				break;
			k = 0;

		}else{
			k += 1;
			if (k >= k_max){
				k = 0;
				epslon /= 2;

			}
			
		}
		
	}while(epslon > epslon_min);
	std::cout << "Lower_bound: " << w_ot << std::endl;
	return 0;
}
