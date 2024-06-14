#include "Lagrange.h"
#include "graph.h"
#include <algorithm>
#include "Kruskal.h"

/* Inicializa o método lagrange com a matriz de custos iniciais */
Lagrange::Lagrange(std::vector <std::vector<double>>* costs){

    this->initCosts = costs;

};



/* Verifica se os pesos são todos nulos*/
bool not_violation(std::vector<int>* subgradient){
	int sum = 0;

	for(int i = 0; i < subgradient->size(); i++){
		
		sum += std::abs(subgradient->at(i));
	}
	return sum;
}



double Lagrange::algorithm(double upper_bound){
  std::vector < std::vector < double >> costsDual = *this->initCosts;
	std::vector < std::vector < double > > costsOriginal = *this->initCosts;
	/* Configuração da relaxação */	
	int qVertices = this->initCosts->size();
	
  std::vector < double > harsh(qVertices, 0); // Vetor penalizador
	std::vector < int > subgradient(qVertices, 0); // Vetor subgradient
	double epslon = 1;
	double epslon_min = 0.0005;
	double w_ot = 0;
	int k_max = 30;
	int k = 0;
	double step = 1.0;

	/***************************/
  int globalVertice_a, globalVertice_b;
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
    
    /*
    std::cout << "PENALIZADORES: ";
    for(int i = 0; i < harsh.size(); i++){
      std::cout << harsh[i] << " ";
    }
    std::cout << std::endl;
    */ 
		/* Primeiro temos que alterar a matriz de peso de acordo com o vetor de pesos */
		/* Inicialmente vamos alterar a matriz toda, depois otimizados para utilizar apenas o triangulo superior
			No primeiro loop esse vetor é nulo então o this->initCosts de peso é o mesmo*/
		
	  //std::cout << "COSTS DUAL\n";
    for(int i = 0; i < qVertices; i++){
			
			for(int j = 0; j < qVertices; j++){

				costsDual[i][j] = costsOriginal[i][j]- harsh[i] - harsh[j];
        if(i == j){
          costsDual[i][j] = 99999999;
        }

			}
		}
    /*
	  for(int i = 0; i < qVertices; i++){
      for(int j = 0; j < qVertices; j++){
        std::cout << costsDual[i][j] << " ";
      }
    std::cout << "\n";
    }	
    */ 
		
    Kruskal kruskal(costsDual);
    kruskal.MST(qVertices);
    std::vector<std::pair<int,int>> edges = kruskal.getEdges();
    

        /* Nós ordenados de acordo com sua distancia em relação ao nó zero*/
		std::vector < std::pair<int, double> > nodesSortedByEdge0;
		

		/* Adiciona todos os vértices e suas respectivas distancias ao nó zero	*/
		for(int j = 1; j < qVertices; j++){
			nodesSortedByEdge0.push_back(std::make_pair(j, costsDual[0][j]));
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

    edges.push_back(std::make_pair(0, vertice_a));
    edges.push_back(std::make_pair(0, vertice_b));
    double w = 0;
    for(int i = 0; i < edges.size(); i++){
      w += costsDual[edges[i].first][edges[i].second];
      //std::cout << edges[i].first   << " " << edges[i].second   << "\n";
      
    }
    //getchar();
    std::vector < std::vector <int>> matrizAdj;

    for(int i = 0; i < qVertices; i++){
      matrizAdj.push_back(std::vector<int>(qVertices,0));
    }

    for(int i = 0; i < edges.size(); i++){
      matrizAdj[edges[i].first][edges[i].second] = 1;
      matrizAdj[edges[i].second][edges[i].first] = 1;
    }
    
    /*
	  std::cout << "MATRIZ DE ADJENCENCIA:\n"; 
    for(int i = 0; i < matrizAdj.size(); i++){
      for(int j = 0; j < matrizAdj[i].size(); j++){
        std::cout << matrizAdj[i][j] << " ";
      }
    std::cout << "\n";

    }
    */
    //getchar();
		Graph graphOperation = Graph(&matrizAdj);
		graphOperation.calculateRates();
		std::vector< int> * rates = graphOperation.getRates();
		
    

		for(int i = 0; i < qVertices; i++){
			w += 2 * harsh[i];
		}

    //std::cout << w << std::endl;

		double PI = 0;
    
    //std::cout << "subgradiente: ";
		/* Calculando o vetor subsubgradient*/
		for(int i = 0; i < qVertices; i++){
			subgradient[i] = 2 - (*rates)[i];
			PI += (subgradient[i] * subgradient[i]);
		}
    //for(int i = 0; i < subgradient.size(); i++){
    //std::cout << subgradient[i] << " ";
    //}
    //std::cout << "\n";
    //std::cout << "PI: " << PI << std::endl;
    
    step = epslon * ((upper_bound - w) / PI);
		//step = ((epslon * (upper_bound - w))) / PI;
    //std::cout << "STEP: " << step << std::endl;

		/* Altera o vetor de pesos */
		for(int i = 0; i < qVertices; i++){
			harsh[i] += (step * subgradient[i]);
		}
    
   // getchar();

    //std::cout << "-------------------------------------\n";
   	if(w >= w_ot){	
			w_ot = w;
			//std::cout << "Novo valor do lower bound: " << w_ot << std::endl;
      lagrangeMatrix = matrizAdj;
      lagrangeCosts = costsOriginal;
       /* Salva os dois vertices */
      verticesChoosen.first = vertice_a;
      verticesChoosen.second = vertice_b;
      /* Calculo do vértice de maior grau */
      /* Para isso vamos 

      /* Quando estamos trabalhando com numeros flutuantes as vezes eles podem dar valores diferentes mesmo sendo iguais
       * Por exemplo : 0.3 * 3 + 0.1 deveria dar 1 e ser igual 1 porem isso não é verdade devido a precisao dos valores */ 
      if(!not_violation(&subgradient) or std::abs(upper_bound - w_ot) < 1e-9){
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
  this->best_lower_bound = w_ot;
  //getchar();
  return w_ot;
}
