#include <iostream>
#include "kruskal.h"
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <sstream>

std::vector < std::vector < int > > * leitorInstancia(char* instanciaName){
	
	std::string linha;
	std::string valor;
	
	std::fstream* arquivo = new std::fstream(instanciaName, std::ios::in);



	std::vector < std::vector <int>  > * grafo = new std::vector < std::vector < int > >();

	while(std::getline(*arquivo, linha)){
		std::vector < int > arestaValores;
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
bool not_violation(std::vector<float>* weight_restr){
	int sumWeight = 0;

	for(int i = 0; i < weight_restr->size(); i++){
		
		sumWeight += weight_restr->at(i);
	}
	if(sumWeight == 0){
		return 1;
	}
	return 0;
}

void print_grafo(std::vector < std::vector < int >>* grafo){
	int size = grafo->size();

	for(int i = 0; i < size; i++){

		std::vector < int > vertice = grafo->at(i);
		
		for(int i = 0; i < vertice.size(); i++){
				
			std::cout << vertice[i] << " ";
		}
		std::cout << std::endl;
	
	}
}
int main(int argc, char** argv){
	
	if(argc < 2){

		std::cout << "Digite ./main -nome instancia-\n";
		exit(1);
	}

	std::vector < std::vector < int > >* grafo = leitorInstancia(argv[1]);
	
	
	
	int qVertices = grafo->size();
	/*
	int qVertices = grafo->size();
	std::cout << qVertices << std::endl;
	Tree* tree = new Tree(qVertices);
	Kruskal kruskal(tree, grafo);
	
	kruskal.algorithm();
	std::cout << "O resultado eh: " << kruskal.result << std::endl;
	*/
	float upper_bound = 148; // Iniciando com um valor aleatorio para teste
	std::vector < float > weight_restr(qVertices, 0); // Inicializando o vetor de peso das restrições para cada vértice como nulo
	std::vector < float > gradiente(qVertices, 0); // Inicializando o vetor de peso das restrições para cada vértice como nulo
	float epslon = 1;
	float epslon_min = 0.5;
	float w_ot = 0;
	int k_max = 10;
	int k = 0;


	do{
		/* Primeiro temos que alterar a matriz de peso de acordo com o vetor de pesos */

		/* Só precisamos alterar o triangulo superior*/
		for(int i = 0; i < qVertices; i++){
			
			for(int j = i + 1; j < qVertices; j++){
				(grafo->at(i))[j] = (grafo->at(i))[j] - weight_restr[i] - weight_restr[j];
			}
		}

		/* Printando nova matriz para debug */
		print_grafo(grafo);
		/* Inicializando o algoritmo de kruskal */
		Tree* tree = new Tree(qVertices);
		Kruskal kruskal(tree, grafo);
		kruskal.algorithm();
		float w = kruskal.result;
		std::cout << "Valor do kruskal: " << w << std::endl;
		std::vector < std::pair<int,int>>* graus = kruskal.getGraus();
		if(w > w_ot){
			w_ot = w;
			std::cout << "Graus de cada vertice: ";
			for(int i = 0; i < qVertices; i++){
				std::cout << graus->at(i).second << " ";
			}
			std::cout << std::endl;
			/* Reformulando os valores do peso */

			std::cout << "Vetor gradiente: ";
			for(int i = 0; i < qVertices; i++){
				gradiente[i] = 2 - graus->at(i).second;
				std::cout << gradiente[i] << " ";
			}
			std::cout << std::endl;

			/* Calculando o produto interno bruto do gradiente*/
			float PI = 0;

			for(int i = 0; i < qVertices; i++){
				PI += (gradiente[i] * gradiente[i]);
			}
			std::cout << "Produto interno bruto: " << PI << std::endl;
			std::cout << std::endl;

			float value = (epslon * (upper_bound - w)) / PI;
			
			std::vector < float > weight_temp;
			std::cout << "Pesos temporarios: ";
			for(int i = 0; i < qVertices; i++){
				weight_temp.push_back(value* gradiente[i]);
				std::cout << weight_temp[i] << " ";
			}
			std::cout << std::endl;
			for(int i = 0; i < qVertices; i++){
				weight_restr[i] += weight_temp[i];
			}
			std::cout << "Novos valores de peso: ";
			for(int i = 0; i < qVertices; i++){
				std::cout << weight_restr[i] << " ";
			}
			std::cout << std::endl;
			k = 0;
			getchar();
			continue;
		}else{
			k += 1;
			if (k > k_max){
				k = 0;
				epslon /= 2;
			}
		}
		getchar();

	}while(epslon > epslon_min or not_violation(&weight_restr));
	
	return 0;
}
