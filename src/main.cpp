#include <iostream>
#include "kruskal.h"
#include <iterator>
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include "graph.h"
#include <limits>
#include "hungarian.h"
#include <list>
#include <iterator>

typedef struct{
	int vertice;
	double valueAresta;
}tNode;


typedef struct{
  std::vector < std::pair<int,int>> forbbiden_arcs; /* Arcos proibidos */
  double lower_bound; /* Custo da solução no algoritmo hungaro */ 
  int nodeChosen; /* Nó escolhido (aquele com maior grau) */ 
  bool feasible; /*Indica a viabilidade da solução */ 

}NodeInfoBnb;



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
  delete arquivo;
	return grafo;


}


/*Retorna o vértice de maior grau da solução obtida no APTSP */
int solve_hungarian(std::vector < std::vector < double >> *costMatrix, NodeInfoBnb* node){
  
  int n = costMatrix->size();
  int size = n - 1;
  double **cost = new double*[size];
  /* Inicializamos coluna e linha como tendendo ao infinito para desconsiderar o nó zero no assignment problem */
  for (int i = 0; i < size; i++){
    cost[i] = new double[size];
    for (int j = 0; j < size; j++){
      cost[i][j] = (*costMatrix)[i  + 1][j+1];
      if(i == j)
        cost[i][j] = 99999999; 
    }
  }
  
  for(int i = 0; i < node->forbbiden_arcs.size(); i++){
    cost[node->forbbiden_arcs[i].first][node->forbbiden_arcs[i].second] = 99999999; 
  }
  hungarian_problem_t p;
  int mode = HUNGARIAN_MODE_MINIMIZE_COST;
  hungarian_init(&p, cost, size , size, mode); // Carregando o problema
  
  std::cout << "Hungarian Cost Matrix:\n";
  hungarian_print_costmatrix(&p); 
  node->lower_bound = hungarian_solve(&p);
  
  int rows = p.num_rows;
  int cols = p.num_cols;
 
  /* Calculo do grau de cada solução */
  std::vector < int > rates(n,0);
  
  std::cout << "Hungarian Solution: " << node->lower_bound << std::endl;
  hungarian_print_assignment(&p);
  for(int i = 0; i < n; i++){

    for(int j = i + 1; j < n; j++){
    
      
      if(p.assignment[i][j] == 1){
        rates[i] += 1;
        rates[j] += 1;
      }
    }
  }
  
  std::vector<int>::iterator VerticeMoreRate = std::max_element(rates.begin(), rates.end());
  int indexVerticeMoreRate = std::distance(rates.begin(), VerticeMoreRate);
  
  std::cout << "Vertice Rates: ";
  for(int i = 0; i < rates.size(); i++){
    std::cout << rates[i] << " ";
  }
  std::cout << std::endl;
  
  std::vector<std::vector<int>> listAdj(n);

  for(int i = 0; i < n; i++){
    std::cout << "listAdj[" << i << "]: ";
    for(int j = 0; j < n; j++){
      if(p.assignment[i][j] == 1){
        listAdj[i].push_back(j);
        std::cout << j << " ";
      }
    }
    std::cout << std::endl;
  }

  return (indexVerticeMoreRate + 1);

}


/* Verifica se os pesos são todos nulos*/
bool not_violation(std::vector<int>* subgradient){
	int sum = 0;

	for(int i = 0; i < subgradient->size(); i++){
		
		sum += std::abs(subgradient->at(i));
	}
	return sum;
}



/* Retorna o indice do nó dado pela estratégia de branching
 * BFS = 0 (sempre será escolhido o primeiro nó)
 * DFS = 1 (sempre será escolhido o ultimo nó)
 */
int branchStrategy(std::list<NodeInfoBnb*>*tree){
  int size = 2;
  std::srand(time(0));

  return std::rand() % size > 0 ? tree->size() - 1: 0; 
  
}

/* Função responsável por calcular os arcos proibidos dado um determinado nó */
void addForbiddenArcsToNode(std::vector<std::vector<int>>* lagrangeMatrix, NodeInfoBnb* node){

  int n = lagrangeMatrix->size();
  std::vector < int > rates (n,0);
  for(int i = 1; i < n; i++){
    for(int j = i + 1; j < n - 1; j++){
      if((*lagrangeMatrix)[i][j] == 1){
        rates[i] += 1;
        rates[j] += 1;
      }
    }
  }
  std::vector<int>::iterator VerticeMoreRate = std::max_element(rates.begin(), rates.end());
  int indexVerticeMoreRate = std::distance(rates.begin(), VerticeMoreRate);

  for(int  j = 1; j < n; j++){
    if(lagrangeMatrix->at(indexVerticeMoreRate)[j] == 1){
      node->forbbiden_arcs.push_back((std::make_pair(indexVerticeMoreRate, j)));
      node->forbbiden_arcs.push_back((std::make_pair(j, indexVerticeMoreRate)));
    }
  }
}

int main(int argc, char** argv){
	
	if(argc < 3){

		std::cout << "Digite ./main -nome instancia- -upper_bound-\n";
		exit(1);
	}


	std::vector < std::vector < double > >* grafo = leitorInstancia(argv[1]);
  std::vector < std::vector < double >> grafoDual;
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
  std::vector < std::vector <int>>LagrangeMatrix;
  int vertice_a, vertice_b; /* vertices mais proximas do vertice zero */
	/* Como estamos resolvendo um problema relaxado
	Queremos encontrar o melhor lower bound possivel,
	pois como é um problema relaxado não é possivel ele ser melhor 
	do que uma soluçao do problema original. Desse modo, para o TSP
	é necessário achar 1-árvore geradora mínima que não inclua o nó zero
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
			
			for(int j = i + 1; j < qVertices; j++){
				(*grafo)[i][j] = grafoOriginal[i][j]- harsh[i] - harsh[j];
        (*grafo)[j][i] = (*grafo)[i][j];
			}
		}
		
		
		/* Inicializando o algoritmo de kruskal */
		Tree* tree = new Tree(qVertices);
		Kruskal kruskal(tree, grafo);
		kruskal.algorithm();
		double w = kruskal.result;
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

		vertice_a = nodesSortedByEdge0.back().first;
		nodesSortedByEdge0.pop_back();
		vertice_b = nodesSortedByEdge0.back().first;
		
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

      /* Quando estamos trabalhando com numeros flutuantes as vezes eles podem dar valores diferentes mesmo sendo iguais
       * Por exemplo : 0.3 * 3 + 0.1 deveria dar 1 e ser igual 1 porem isso não é verdade devido a precisao dos valores */ 
      if(!not_violation(&subgradient) or std::abs(upper_bound - w_ot) < 1e-9){
        LagrangeMatrix = *matrizAdj;
        grafoDual = *grafo;
        break;
      }
				
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


  /* Após resolver o dual lagrangiano, vamos utilizar o BNB */

  /* Desativa as arestas em relação ao nó zero */ 
  LagrangeMatrix[0][vertice_a] = LagrangeMatrix[vertice_a][0] = 0;
  LagrangeMatrix[0][vertice_b] = LagrangeMatrix[vertice_b][0] = 0;
  
  NodeInfoBnb root; /* Nó raiz = será dado pela resolução da relaxacao */

  addForbiddenArcsToNode(&LagrangeMatrix,&root); 
  int indexVerticeMoreRate = solve_hungarian(&grafoDual, &root);

  std::list<NodeInfoBnb*>tree;
  tree.push_back(&root);

  /*Bnb algorithm */ 
  while(!tree.empty()){
    int indexNode = branchStrategy(&tree);
    /* Retorna o nó dependendo do método utilizado */
    NodeInfoBnb* node = indexNode == 0 ? tree.front() : tree.back();
    
    /* Avança o iterator para a posição do nó escolhido para depois apagá-lo */ 
    std::list < NodeInfoBnb*>::iterator init = tree.begin();
    advance(init, indexNode);

    if(node->lower_bound > upper_bound){
      tree.erase(init);
      continue;
    }
     

  }


  return 0;
}
