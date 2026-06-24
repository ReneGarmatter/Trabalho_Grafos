#include <bits/stdc++.h>
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

int totalDatacenters;
int totalTasks;

vector<datacenter> vetorDatacenters;
vector<task> vetorTasks;

bool buscaCaminhoDeAumento(vector<vector<int>> &m){ 

    // aplica bfs em um vertice desemparelhado de m
    // checa caminho de aumento
    // retorna true se acha e aumenta caminho em M, false caso contrario
}

void atualizaPotenciais(vector<vector<int>> custo,vector<int> &potencialDatacenters, vector<int> &potencialTasks){

    // calcula delta
    // faz conjuntos S e N(S)
    // aumenta S em delta e decresce N(S) em delta
}

vector<int> achaTarefasDesemparelhadas(vector<vector<int>> m){
    
    vector<int> debug;
    // to pensano como faz
    // acho que da pra fazer uma bfs ou ficar com seu vetor de emparelhados msm
    return debug;
}
int main(int argc, char** argv){ 
    
    // leitor json

    ifstream arquivoEntrada("dados.json");
    json dados;
    arquivoEntrada >> dados;

    totalDatacenters = dados["datacenters"].size();
    totalTasks = dados["tasks"].size();

    // criação do vetor de datacenters
    vetorDatacenters = vector<datacenter>(totalDatacenters);
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
        //TODO: adicionar vértices fillers caso tamanho da fila.tasks < totalDatacenters
        // pega tasks da fila de acordo com o total de datacenters
        vetorTasks = vector<task>(totalDatacenters);
        for(int i=0; i<totalDatacenters;  i++){
            vetorTasks[i] = filaTasks.front();
            filaTasks.pop();
        }
        

        //calcula peso de cada aresta
        vector<vector<int>> custo = vector<vector<int>> (totalDatacenters, vector<int>(totalTasks));
        for(int i=0; i<totalDatacenters; i++){
            // obs: total datacenters = total tasks
            datacenter d = vetorDatacenters[i];

            for(int j=0; j<totalTasks; j++){
                task t = vetorTasks[j];
                custo[i][j] = (t.tamanho/d.poderProcessamento) * d.precoEnergia;
                cout << custo[i][j] << " "; // debug
            }
            cout << "\n";// debug
        }

        //inicializa potenciais com valor 0
        vector<int> potencialDatacenters = vector<int>(totalDatacenters, 0);
        vector<int> potencialTasks = vector<int>(totalDatacenters, 0); //obs: processamos uma quantidade de tasks igual a quantidade de datacenters

        //inicializa matriz M de arestas apertadas
        // 0 = vertice não pertence a M
        // 1 = vertice pertence a M e não está emparelhado
        // 2 = vertice pertence a M e está emparelhado
        vector<vector<int>> m = vector<vector<int>>(totalDatacenters, vector<int>(totalDatacenters, 0));
      
        // algoritmo hungaro começa aqui
        
        while(achaTarefasDesemparelhadas(m).size() != 0){
            if(!buscaCaminhoDeAumento(m)){
                atualizaPotenciais(custo, potencialDatacenters, potencialTasks);
            }
        }

        }
    
    return 0;
}