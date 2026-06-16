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

vector<vector<int>> custo;

vector<int> potencialDatacenters;
vector<int> potencialTasks;



vector<int> recTasks;
vector<int> recDatacenters;
set<int> S;
set<int> NS;

set<pair<int,int>> emparelhamentoMínimo;
set<pair<int,bool>> verticesEmparelhados;

void construirCaminhoDeAumento(pair<int,bool> &verticeDeInicio,vector<int> &recTasks,vector<int> &recDatacenters,pair<int,bool> &verticeFinal){
    
    pair<int,bool> verticeAtual=verticeFinal;
    pair<int,bool> verticeSeguinte;
    int level=0;
    cout<<"Caminho de aumento encontrado"<<endl;
    if(verticeFinal.second){
        verticeSeguinte={recDatacenters[verticeFinal.first],false};
    }
    else{
        verticeSeguinte={recTasks[verticeFinal.first],true};
    }
    while (verticeSeguinte!=verticeDeInicio){

        verticesEmparelhados.insert(verticeAtual);
        verticesEmparelhados.insert(verticeSeguinte);

        if(verticeAtual.second){
            cout<<"Datacenter("<<verticeAtual.first<<") ";
        }
        else{
            cout<<"Task("<<verticeAtual.first<<") ";
        }

        if(level%2==0){
            if(verticeAtual.second){
                emparelhamentoMínimo.insert({verticeAtual.first,verticeSeguinte.first});
            }
            else{
                emparelhamentoMínimo.insert({verticeSeguinte.first,verticeAtual.first});
            }
        }
        else{
            if(verticeAtual.second){
                emparelhamentoMínimo.erase({verticeAtual.first,verticeSeguinte.first});
            }
            else{
                emparelhamentoMínimo.erase({verticeSeguinte.first,verticeAtual.first});
            }
        }
    
        verticeAtual=verticeSeguinte;
        if(verticeAtual.second){
            verticeSeguinte={recDatacenters[verticeAtual.first],false};
        }
        else{
            verticeSeguinte={recTasks[verticeAtual.first],true};
        }
        level++;
    }

    if(verticeAtual.second){
        cout<<"Datacenter("<<verticeAtual.first<<")"<<" Task("<<verticeSeguinte.first<<")"<<endl;
    }
    else{
        cout<<"Task("<<verticeAtual.first<<")"<<" DataCenter("<<verticeSeguinte.first<<")"<<endl;
    }

    verticesEmparelhados.insert(verticeAtual);
    verticesEmparelhados.insert(verticeSeguinte);
    if(level%2==0){
        if(verticeAtual.second){
            emparelhamentoMínimo.insert({verticeAtual.first,verticeSeguinte.first});
        }
        else{
            emparelhamentoMínimo.insert({verticeSeguinte.first,verticeAtual.first});
        }
    }
    else{
        if(verticeAtual.second){
            emparelhamentoMínimo.erase({verticeAtual.first,verticeSeguinte.first});
        }
        else{
            emparelhamentoMínimo.erase({verticeSeguinte.first,verticeAtual.first});
        }
    }
    
}

bool buscarCaminhoDeAumento(){
    recTasks= vector(totalDatacenters,-1);
    recDatacenters= vector(totalDatacenters,-1);
    S.clear();
    NS.clear();

    pair<int,bool> verticeDeInicio={0,true};
    while(verticesEmparelhados.count(verticeDeInicio)){    
        verticeDeInicio={verticeDeInicio.first+1,true};
    }

    if(verticeDeInicio.second){
        cout<<"começando do Datacenter "<< verticeDeInicio.first<<endl;
    }
    else{
        cout<<"começando da tarefa "<<verticeDeInicio.first<<endl;
    }

    queue<pair<int,bool>> fila;
    set<pair<int,bool>> visitados;
    fila.push(verticeDeInicio);
    visitados.insert(verticeDeInicio);
    S.insert(verticeDeInicio.first);
    
    while(!fila.empty()){
        pair<int,bool> visitando=fila.front();

        if(visitando.second){
            cout<<"Olhando Datacenter "<< visitando.first<<endl;
        }
        else{
            cout<<"Olhando tarefa "<<visitando.first<<endl;
        }
        
        if(visitando.second){
            for(int i=0;i<totalDatacenters;i++){
                if(!visitados.count({i,false})){
                    if(custo[visitando.first][i]-potencialDatacenters[visitando.first]-potencialTasks[i] == 0){                
                        visitados.insert({i,false});
                        NS.insert(i);
                        fila.push({i,false});
                        recTasks[i]=visitando.first;
                        if(!verticesEmparelhados.count({i,false})){
                            pair<int,bool> verticeFinal={i,false};
                            construirCaminhoDeAumento(verticeDeInicio,recTasks,recDatacenters,verticeFinal);
                            return true;
                        }
                    }
                }
            }
        }
        else{
            for(int i=0;i<totalDatacenters;i++){
                if(emparelhamentoMínimo.count({i,visitando.first}) && !visitados.count({i,true})){                
                    if(verticesEmparelhados.count({i,true})){
                        visitados.insert({i,true});
                        S.insert(i);
                        fila.push({i,true});
                        recDatacenters[i]=visitando.first;
                    }
                }
            }
        }
        fila.pop();
    }
    cout<<"Nenhum caminho de aumento encontrado"<<endl;
    return false;

}

void AtualizarPreços(){
    int delta=INT_MAX;
    for(auto p: S){
        for(int i=0;i<totalDatacenters;i++){
            if(!NS.count(i) && custo[p][i]-potencialTasks[i]-potencialDatacenters[p]<delta){
                delta=custo[p][i]-potencialTasks[i]-potencialDatacenters[p];
            }
        }
    }

    for(auto p: S){      
        potencialDatacenters[p]+=delta;
        cout<<"Preço do DataCenter "<<p<<" atualizado para "<<potencialDatacenters[p]<<endl;    
    }

    for(auto p: NS){
        potencialTasks[p]-=delta;
        cout<<"Preço da Tarefa "<<p<<" atualizado para "<<potencialTasks[p]<<endl;
    }
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
        // pega tasks da fila de acordo com o total de datacenters
        vetorTasks = vector<task>(totalDatacenters);
        for(int i=0; i<totalDatacenters;  i++){
            vetorTasks[i] = filaTasks.front();
            filaTasks.pop();
        }

        //calcula peso de cada aresta
        custo= vector<vector<int>> (totalDatacenters, vector<int>(totalTasks));
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

        // algoritmo hungaro começa aqui
        potencialDatacenters= vector(totalDatacenters,0);
        potencialTasks=vector(totalTasks,0);

        
        while(verticesEmparelhados.size()!=2*totalDatacenters){
            if(!buscarCaminhoDeAumento()){
                AtualizarPreços();
            }

        }
        
       
        
        cout<<"O melhor emparelhamento é:"<<endl;
        for(auto p: emparelhamentoMínimo){
            cout<<"Datacenter("<<p.first<<") - "<<" Task("<<p.second<<")"<<endl;
        }
    }
    
    return 0;
}