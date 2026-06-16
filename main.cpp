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

set<pair<int,bool>> verticesEmparelhados;

vector<int> recTasks;
vector<int> recDatacenters;
set<pair<int,bool>> S;
set<pair<int,bool>> NS;

vector<pair<int,bool>> construirCaminhoDeAumento(pair<int,bool> &verticeDeInicio,vector<int> &recTasks,vector<int> &recDatacenters,pair<int,bool> &verticeFinal){
    vector<pair<int,bool>> caminhoDeAumento;
    caminhoDeAumento.push_back(verticeFinal);
    pair<int,bool> next;
    if(verticeFinal.second){
        next={recDatacenters[verticeFinal.first],false};
    }
    else{
        next={recTasks[verticeFinal.first],true};
    }
    while (next!=verticeDeInicio){
        if(verticeFinal.second){
            next={recDatacenters[verticeFinal.first],false};
        }
        else{
            next={recTasks[verticeFinal.first],true};
        }
        caminhoDeAumento.push_back(next);
    }
    caminhoDeAumento.push_back(verticeDeInicio);
    
    cout<<"Caminho de aumento encontrado"<<endl;
    for(auto p:caminhoDeAumento){
        if(p.second){
            cout<<"DataCenter "<<p.first<<" ";
        }
        else{
            cout<<"Tarefa "<<p.first<<" ";
        }
    }
    return caminhoDeAumento;
}

vector<pair<int,bool>> buscarCaminhoDeAumento(){
    recTasks= vector(totalDatacenters,0);
    recDatacenters= vector(totalDatacenters,0);
    S.clear();
    NS.clear();

    pair<int,bool> verticeDeInicio={0,true};
    while(verticesEmparelhados.count(verticeDeInicio)){
        if(verticeDeInicio.second){
            verticeDeInicio={verticeDeInicio.first,false};
        }
        else{
            verticeDeInicio={verticeDeInicio.first+1,true};
        }
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
    S.insert(verticeDeInicio);
    
    int level=0;
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
                        if(level%2==0){
                            visitados.insert({i,false});
                            S.insert({i,false});
                            fila.push({i,false});
                            recTasks[i]=visitando.first;
                            if(!verticesEmparelhados.count({i,false})){
                                pair<int,bool> verticeFinal={i,false};
                                return construirCaminhoDeAumento(verticeDeInicio,recTasks,recDatacenters,verticeFinal);
                            }
                        }
                        else if(verticesEmparelhados.count({i,false})){
                            visitados.insert({i,false});
                            NS.insert({i,false});
                            fila.push({i,false});
                            recTasks[i]=visitando.first;
                        }
                    }
                }
            }
        }
        else{
            for(int i=0;i<totalDatacenters;i++){
                if(!visitados.count({i,true})){
                    if(custo[i][visitando.first]-potencialDatacenters[i]-potencialTasks[visitando.first] == 0){                
                        if(level%2==0){
                            visitados.insert({i,true});
                            S.insert({i,true});
                            fila.push({i,true});
                            recDatacenters[i]=visitando.first;
                            if(!verticesEmparelhados.count({i,true})){
                                pair<int,bool> verticeFinal={i,true};
                                return construirCaminhoDeAumento(verticeDeInicio,recTasks,recDatacenters,verticeFinal);
                            }
                        }
                        else if(verticesEmparelhados.count({i,true})){
                            visitados.insert({i,true});
                            NS.insert({i,true});
                            fila.push({i,true});
                            recDatacenters[i]=visitando.first;
                        }
                    }
                }
            }
        }
        
        fila.pop();
        level++;
    }
    cout<<"Nenhum caminho de aumento encontrado"<<endl;
    return vector<pair<int,bool>>(0);

}

void AtualizarPreços(){
    int delta=INT_MAX;
    for(auto p: S){
        if(p.second){
            for(int i=0;i<totalDatacenters;i++){
                if(!NS.count({i,false}) && custo[p.first][i]-potencialTasks[i]-potencialDatacenters[p.first]<delta){
                    delta=custo[p.first][i]-potencialTasks[i]-potencialDatacenters[p.first];
                }
            }
        }
        else{
            for(int i=0;i<totalDatacenters;i++){
                if(!NS.count({i,true}) && custo[i][p.first]-potencialTasks[p.first]-potencialDatacenters[i]<delta){
                    delta=custo[i][p.first]-potencialTasks[p.first]-potencialDatacenters[i];
                }
            }
        }
    }

    for(auto p: S){
        if(p.second){
            potencialDatacenters[p.first]+=delta;
            cout<<"Preço do DataCenter "<<p.first<<" atualizado para "<<potencialDatacenters[p.first]<<endl;
        }
        else{
            potencialTasks[p.first]+=delta;
            cout<<"Preço da tarefa "<<p.first<<" atualizado para "<<potencialTasks[p.first]<<endl;
        }
    }

    for(auto p: NS){
        if(p.second){
            potencialDatacenters[p.first]-=delta;
            cout<<"Preço do DataCenter "<<p.first<<" atualizado para "<<potencialDatacenters[p.first]<<endl;
        }
        else{
            potencialTasks[p.first]-=delta;
            cout<<"Preço da tarefa "<<p.first<<" atualizado para "<<potencialTasks[p.first]<<endl;
        }
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

        set<pair<int,int>> emparelhamentoMínimo;
        vector<pair<int,bool>> caminhoDeAumento=buscarCaminhoDeAumento();
        if(caminhoDeAumento.size()==0){
            AtualizarPreços();
        }
        buscarCaminhoDeAumento();
    }
    
    return 0;
}