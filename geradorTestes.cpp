#include <bits/stdc++.h>
#include <fstream>
#include <random>
#include <iostream>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

int main(){

    json dados;
    random_device rd;
    mt19937 gen(rd()); 
    
    cout << "quantos datacenters gerar?" << "\n";
    int quantidadeDatacenters;
    cin >> quantidadeDatacenters;

    cout << "quantas tarefas gerar?" << "\n";
    int quantidadeTarefas;
    cin >> quantidadeTarefas;

    // preenche com datacenters aleatorios de um conjunto predefinido
    for(int i=0; i<quantidadeDatacenters; i++){
        string nome = "d" + to_string(i);
        dados["datacenters"][i]["nome"] = nome;

        random_device rd;
        mt19937 gen(rd()); 
        uniform_int_distribution<long> distPoder(10, 200);
        uniform_int_distribution<long> distEnergia(10, 100);

        dados["datacenters"][i]["precoEnergia"] = distEnergia(gen);
        dados["datacenters"][i]["poderProcessamento"] = distPoder(gen);
    }
    // preenche com tarefas aleatorias de um conjunto predefinido
    for(int i=0; i<quantidadeTarefas; i++){
        string nome = "t" + to_string(i);
        dados["tasks"][i]["nome"] = nome;

        random_device rd;
        mt19937 gen(rd()); 
        uniform_int_distribution<long> distTamanho(100, 3000);

        dados["tasks"][i]["tamanho"] = distTamanho(gen);
    }
    // cria arquivo de dados de teste

    
    ofstream arquivoSaida("dadosTesteEstresse.json");
    arquivoSaida << dados.dump(4);
    arquivoSaida.close();
    

    return 0;
}