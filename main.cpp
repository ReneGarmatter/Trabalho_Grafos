#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <fstream>

#include "json.hpp"
using json = nlohmann::json;

using namespace std;

struct datacenter{
    string nome;
    int precoEnergia;
    int poderProcessamento;
};
struct task{
    string nome;
    int tamanho;
};

int main(int argc, char** argv){
    
    // leitor json

    ifstream arquivoEntrada("dados.json");
    json dados;
    arquivoEntrada >> dados;

    int totalDatacenters = dados["datacenters"].size();
    int totalTasks = dados["tasks"].size();

    // criação do vetor de datacenters
    vector<datacenter> vetorDatacenters = vector<datacenter>(totalDatacenters);
    for(int i=0; i<totalDatacenters; i++){
        json datacenterJson = dados["datacenters"][i];
        datacenter d;
        d.nome = datacenterJson["nome"];
        d.precoEnergia = datacenterJson["precoEnergia"];
        d.poderProcessamento = datacenterJson["poderProcessamento"];
        vetorDatacenters[i] = d;
    }

    // criação da fila de tasks
    queue<task> filaTasks;
    for(int i=0; i<totalTasks; i++){
        json taskJson = dados["tasks"][i];
        task t;
        t.nome = taskJson["nome"];
        t.tamanho = taskJson["tamanho"];
        filaTasks.push(t);
    }
    
    //loop principal
    while(!filaTasks.empty()){
        // pega tasks da fila de acordo com o total de datacenters
        vector<task> vetorTasks = vector<task>(totalDatacenters);
        for(int i=0; i<totalDatacenters;  i++){
            vetorTasks[i] = filaTasks.front();
            filaTasks.pop();
        }

        //calcula peso de cada aresta
        vector<vector<int>> custoTempoExecucao(totalDatacenters, vector<int>(totalTasks));
        for(int i=0; i<totalDatacenters; i++){
            // obs: total datacenters = total tasks
            datacenter d = vetorDatacenters[i];

            for(int j=0; j<totalTasks; j++){
                task t = vetorTasks[j];
                custoTempoExecucao[i][j] = (t.tamanho/d.poderProcessamento) * d.precoEnergia;
                cout << custoTempoExecucao[i][j] << " "; // debug
            }
            cout << "\n";// debug
        }

        // algoritmo hungaro começa aqui

    }
    
    return 0;
}