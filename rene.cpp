#include <bits/stdc++.h>
#include <fstream>

#include "json.hpp"
using json = nlohmann::json;

using namespace std;

struct datacenter
{
    string nome;
    long precoEnergia;
    long poderProcessamento;
};
struct task
{
    string nome;
    long tamanho;
};

long totalDatacenters;
long totalTasks;

void algoritmoHungaro(vector<vector<long>> custo, long totalDatacenters,
                      vector<long> &pareamentoDatacenterTask, vector<long> &pareamentoTaskDatacenter)
{

    vector<long> potencialDatacenter = vector<long>(totalDatacenters, 0);
    vector<long> potencialTask = vector<long>(totalDatacenters, 0);

    // Inicializa potenciais das tarefas com o mínimo de cada coluna
    for (long j = 0; j < totalDatacenters; j++) {
        potencialTask[j] = custo[0][j];
        for (long i = 1; i < totalDatacenters; i++) {
            if (custo[i][j] < potencialTask[j]) {
                potencialTask[j] = custo[i][j];
            }
        }
    }

    /**
     * Algoritmo hungaro começa aqui (esse mantém a arvore).
     * Sempre que o algorimo encontra um pareamento , 
     * a cardinalidade de M aumenta em 1, por isso so precisamos desse for
     * como condição de parada (pareamos de datacenter em datacenter)
     */
    for (long i = 0; i < totalDatacenters; ++i) {

        vector<bool> visitadoDatacenter(totalDatacenters, false);
        vector<bool> visitadoTarefa(totalDatacenters, false);

        vector<long> predecessorTarefa(totalDatacenters, -1);  // tarefa -> datacenter
        vector<long> predecessorDatacenter(totalDatacenters, -1);  // datacenter -> tarefa
    
        if(pareamentoDatacenterTask[i] != -1){ // checamos se o datacenter i já esta pareado
            continue; // continua para o proximo caso verdadeiro
        }
        vector<long> nivelAtual; 
        nivelAtual.push_back(i); // i é o datacenter de inicio da BFS

        visitadoDatacenter[i] = true;
        long nivel = 0; // nivel atual da arvore
        bool caminhoEncontrado = false;
        long tarefaLivre = -1; // guarda a tarefa livre encontrada

        while(!caminhoEncontrado){
            // inicio BFS
            while(!nivelAtual.empty() && !caminhoEncontrado){ // condição de parada da BFS
                vector<long> proximoNivel;
    
                if(nivel % 2 == 0){ // se nivel atual é par então o nivel atual contem datacenters e o proximo contem tarefas
    
                    for(long datacenter : nivelAtual){ // para cada datacenter do nivel atual

                        for(long tarefa=0; tarefa<totalDatacenters; tarefa++){ // visitamos cada tarefa visinha do datacenter
                            
                            // se a aresta de datacenter com tarefa é apertada
                            if(custo[datacenter][tarefa] - potencialTask[tarefa] - potencialDatacenter[datacenter] == 0
                            && !visitadoTarefa[tarefa]){ // e tarefa não foi visitada
    
                                visitadoTarefa[tarefa] = true; // visitamos tarefa
                                predecessorTarefa[tarefa] = datacenter; // registramos predecessor
                                
                                if(pareamentoTaskDatacenter[tarefa] == -1){ // se tarefa não esta emparelhada achamos caminho de aumento
                                    caminhoEncontrado = true;
                                    tarefaLivre = tarefa;
                                    break;
                                }
                                else{ // se não continua BFS
                                    // adicionamos no proximo nivel
                                    proximoNivel.push_back(tarefa);
                                }
                            }
                        }
                        if(caminhoEncontrado) break;
                    }
                }
                else if(nivel % 2 == 1){// se nivel atual é impar então o nivel atual contem tarefas e o proximo contem datacenters
                    
                    for(long tarefa : nivelAtual){ // faça para cada tarefa do nivel atual
                        
                        if(pareamentoTaskDatacenter[tarefa] != -1){ // se tarefa esta emparelhada
                            long datacenter = pareamentoTaskDatacenter[tarefa];
                            
                            if(!visitadoDatacenter[datacenter]) {
                                visitadoDatacenter[datacenter] = true; // visitamos datacenter
                                predecessorDatacenter[datacenter] = tarefa; // registramos predecessor
                                // continua a BFS
                                // adicionamos no proximo nivel
                                proximoNivel.push_back(datacenter);
                            }
                        }
                    }
                }
    
                nivelAtual = proximoNivel;
                nivel++;
            }
        
            if(caminhoEncontrado){ // aumenta o caminho
                long tarefaAtual = tarefaLivre;
                
                // percorre
                while(tarefaAtual != -1) {
                    long datacenterAtual = predecessorTarefa[tarefaAtual];
                    
                    if(datacenterAtual != -1) {
                        long tarefaAnterior = predecessorDatacenter[datacenterAtual];
                        
                        // Inverte o emparelhamento
                        pareamentoDatacenterTask[datacenterAtual] = tarefaAtual;
                        pareamentoTaskDatacenter[tarefaAtual] = datacenterAtual;
                        
                        tarefaAtual = tarefaAnterior;
                    } else {
                        break;
                    }
                }
            }
            else { // atualiza potenciais
                // Calcula delta: menor custo reduzido entre S e (Tarefas - N(S))
                // S = datacenters visitados (níveis pares)
                // N(S) = tarefas visitadas (níveis ímpares)
                long delta = LONG_MAX;
                for(long d = 0; d < totalDatacenters; d++) {
                    if(visitadoDatacenter[d]) {
                        for(long t = 0; t < totalDatacenters; t++) {
                            if(!visitadoTarefa[t]) {
                                long custoReduzido = custo[d][t] - potencialDatacenter[d] - potencialTask[t];
                                if(custoReduzido < delta) {
                                    delta = custoReduzido;
                                }
                            }
                        }
                    }
                }
                
                if(delta == LONG_MAX) break;  // não deveria acontecer em um problema válido
                
                // Atualiza potenciais
                // Para datacenters em S (visitados): aumenta o potencial
                for(long d = 0; d < totalDatacenters; d++) {
                    if(visitadoDatacenter[d]) {
                        potencialDatacenter[d] += delta;
                    }
                }
                
                // Para tarefas em N(S) (visitadas): diminui o potencial
                for(long t = 0; t < totalDatacenters; t++) {
                    if(visitadoTarefa[t]) {
                        potencialTask[t] -= delta;
                    }
                }
                
                // Prepara para continuar a BFS
                // Reinsere os datacenters visitados no nível atual
                // (eles estão nos níveis pares e precisam ser re-explorados
                //  pois novas arestas de custo zero podem ter surgido)
                nivelAtual.clear();
                for(long d = 0; d < totalDatacenters; d++) {
                    if(visitadoDatacenter[d]) {
                        nivelAtual.push_back(d);
                    }
                }
                // Reinicia o nível para 0 para re-explorar a partir dos datacenters
                nivel = 0;
            }
        }
    }
}

int main(int argc, char **argv)
{

    // leitor json

    ifstream arquivoEntrada("dadosTesteEstresse.json");
    json dados;
    arquivoEntrada >> dados;

    totalDatacenters = dados["datacenters"].size();
    totalTasks = dados["tasks"].size();

    // criação do vetor de datacenters
    vector<datacenter> vetorDatacenters = vector<datacenter>(totalDatacenters);
    for (long i = 0; i < totalDatacenters; i++)
    {
        json datacenterJson = dados["datacenters"][i];
        datacenter d;
        d.nome = datacenterJson["nome"];
        d.precoEnergia = datacenterJson["precoEnergia"];
        d.poderProcessamento = datacenterJson["poderProcessamento"];
        vetorDatacenters[i] = d;
    }

    // criação da fila de tasks
    queue<task> filaTasks;
    for (long i = 0; i < totalTasks; i++)
    {
        json taskJson = dados["tasks"][i];
        task t;
        t.nome = taskJson["nome"];
        t.tamanho = taskJson["tamanho"];
        filaTasks.push(t);
    }

    long numTarefasProcessadas = 0;
    
    // loop principal
    while (!filaTasks.empty())
    {
        long tarefasNoLote = min(totalDatacenters, (long)filaTasks.size());
        
        // pega tasks da fila de acordo com o total de datacenters
        vector<task> vetorTasks;
        for (long i = 0; i < tarefasNoLote; i++)
        {
            vetorTasks.push_back(filaTasks.front());
            filaTasks.pop();
        }

        // Completa com tarefas filler
        while (vetorTasks.size() < totalDatacenters)
        {
            task tFiller;
            tFiller.nome = "tFiller";
            tFiller.tamanho = LONG_MAX / 1000; // evita overflow
            vetorTasks.push_back(tFiller);
        }

        // calcula peso de cada aresta
        vector<vector<long>> custo = vector<vector<long>>(totalDatacenters, vector<long>(totalDatacenters));
        for (long i = 0; i < totalDatacenters; i++)
        {
            // obs: total datacenters = total tasks
            // linha = datacenter, coluna = tarefa
            datacenter d = vetorDatacenters[i];

            for (long j = 0; j < totalDatacenters; j++)
            {
                task t = vetorTasks[j];
                if(t.nome == "tFiller") {
                    custo[i][j] = LONG_MAX / 1000;
                } else {
                    custo[i][j] = (t.tamanho / d.poderProcessamento) * d.precoEnergia;
                }
                //cout << custo[i][j] << " "; // debug
            }
            //cout << "\n"; // debug
        }

        vector<long> pareamentoDatacenterTask(totalDatacenters, -1); // datacenter -> task
        vector<long> pareamentoTaskDatacenter(totalDatacenters, -1); // task -> datacenter

        algoritmoHungaro(custo, totalDatacenters, pareamentoDatacenterTask, pareamentoTaskDatacenter);

        // Exibe o emparelhamento (apenas tarefas reais)
        cout << "\n=== Emparelhamento do lote ===" << endl;
        for (long i = 0; i < totalDatacenters; i++) {
            long j = pareamentoDatacenterTask[i];
            if (j != -1 && vetorTasks[j].nome != "tFiller") {
                cout << "Datacenter " << vetorDatacenters[i].nome 
                     << " -> Task " << vetorTasks[j].nome << endl;
            }
        }
        
        numTarefasProcessadas += tarefasNoLote;
    }

    return 0;
}