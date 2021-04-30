#include "Grafo.h"

using namespace std;

Matriz::Matriz(string path, string algorithm, int selectedVertice, int targetVertice){
	ifstream rfile;
    rfile.open(path);
    if (!rfile){
        cout << "Arquivo não encontrado";
        return;
    }
    
    rfile >> vertices;        
    this->construtor();       
    edges = 0; 
    Grafo::vector_grau = new int[vertices + 1](); 
    string s;
    while (getline(rfile, s)){
      if (s.empty() == false){
        istringstream tmp(s);
        int v1, v2;
        tmp >> v1 >> v2;
        this->addAresta(v1, v2);
        this->addAresta(v2, v1);
      }
    }
    
    visited = new int[vertices + 1](); 
    Grafo::mergeSort(Grafo::vector_grau, 1, vertices);
    Grafo::Infos();

     FComponentes_conexas();
    
    ofstream myInfoFile;
    myInfoFile.open(m_savePath + "/info.txt");
    myInfoFile << "Vertices = " << vertices << endl
    << "Arestas= " << (edges) << endl
    << "Grau maximo = " << gmax << endl
    << "Grau minimo = " << gmin << endl
    << "Mediana = " << gmed << endl
    << "Quantidade de componentes conexas: " << m_numero_de_componentes_conexas << endl;
    
    int k = m_i;

    for (int i = 1; i <= m_numero_de_componentes_conexas; i++){
      myInfoFile << "Numero de vertices da componete conexa: " << m_tamanho_das_componentes_conexas[i]
        << endl << "Vetores dessa componente conexa: ";

      for (int j = 1; j <= vertices; j++){
        if (m_nodes_componentes_conexas[i][j] == 1){
          myInfoFile << j << ", ";
        }
      }
      myInfoFile << "\n\n";
    }

    myInfoFile << endl << endl;
    
    cout << "Arquivo salvo em " << m_savePath << "/info.txt" << endl;

    visited = new int[vertices + 1]();
    
    if (algorithm == "BFS"){
        BFS_Matriz(selectedVertice);
    }
    if (algorithm == "DFS"){
        DFS_matriz(selectedVertice);
    }
    if (algorithm == "diameter"){
      Diameter();
    }
}

void Matriz::addAresta(int linha, int coluna){
    matriz[linha][coluna] = true;
    edges++;
    Grafo::vector_grau[linha]++;
    return;
}

void Matriz::construtor(){
    matriz = new bool*[vertices + 1];
    for (int i = 0; i < vertices + 1; ++i)
        matriz[i] = new bool[vertices];
    for (int i = 0; i <= vertices; ++i)
        for (int j = 0; j <= vertices; ++j)
            matriz[i][j] = false;
    return;
}

Matriz::~Matriz(){
    for (int i = 0; i <= vertices; ++i)
       delete[]matriz[i];
    delete[]matriz;
}

void Matriz::BFS_Matriz(int s){
    while (!fqueue.empty()){
        fqueue.pop();
    }
    Tree* raiz = new Tree;
    raiz->vertice = s;
    raiz->nivel = 0;
    raiz->pai = NULL;
    
    ofstream myBFSFile;
    myBFSFile.open(m_savePath + "/matriz_BFS.txt");
    myBFSFile << "Vertice: " << raiz->vertice << "--- Nivel: " << raiz->nivel
    << "--- Pai: x " << endl;
    
    visited[s] = 1;
    visitedInThisExecution[s] = 1;
    fqueue.push(raiz);
    
    while (!fqueue.empty()){
      Tree* v = fqueue.front();
      fqueue.pop();
      m_tamanho_da_componente_conexa++;

      for (int w = 1; w <= vertices; w++){
        if (matriz[v->vertice][w] == true)
          if (visited[w] == 0){
            visited[w] = 1;
            visitedInThisExecution[w] = 1;
            Tree* x = this->Parentesco(v, w);
            myBFSFile << "Vertice: " << x->vertice << "--- Nivel: " << x->nivel
            << "--- Pai: " << v->vertice << endl;
            fqueue.push(x);
            if (x -> nivel > maxLevel){
              maxLevel = x -> nivel;
            }
          }
      }
    }
}

void Matriz::DFS_matriz(int s){
    while (!pstack.empty()){
        pstack.pop();
    }
    Tree* raiz = new Tree;
    raiz->vertice = s;
    raiz->nivel = 0;
    raiz->pai = NULL;
    
    ofstream myDFSFile;
    myDFSFile.open(m_savePath + "/matriz_DFS.txt");
    
    pstack.push(raiz);
    while (!pstack.empty()){
        Tree* u = pstack.top();
        pstack.pop();

        if (visited[u->vertice] == 0){
            myDFSFile << "Vertice: " << u->vertice << "--- Nivel: " << u->nivel;
            if (!(u == raiz)) myDFSFile << "--- Pai: " << u->pai->vertice << endl;
            else myDFSFile << "--- Pai: x " << endl;
            visited[u->vertice] = 1;
            for (int w = 1; w <= vertices; w++){
                if (matriz[u->vertice][w] == true){
                    Tree* x = this->Parentesco(u, w);
                    pstack.push(x);
                }
            }  
        }
    }
    return;
}

Tree* Matriz::Parentesco(Tree* v, int w){
    Tree* filho = new Tree;
    filho->vertice = w;
    int nivel_pai = v->nivel;
    filho->nivel = nivel_pai + 1;
    filho->pai = v;
    return filho;
}

void Matriz::FComponentes_conexas(){
  visitedInThisExecution = new int[vertices + 1];
  m_nodes_componentes_conexas = new int*[vertices + 1];
  m_tamanho_das_componentes_conexas = new int[vertices + 1];
  m_i = 1;
  for (int i = 1; i <= vertices; i++){
    if (visited[i] == 0){
      m_numero_de_componentes_conexas++;
      m_tamanho_da_componente_conexa = 0;
      BFS_Matriz(i);
      m_tamanho_das_componentes_conexas[m_i] = m_tamanho_da_componente_conexa;
      m_nodes_componentes_conexas[m_i] = visitedInThisExecution;
      visitedInThisExecution = new int[vertices + 1];
      m_i++;
    }
  }
  m_i = 1;
  return;
}

void Matriz::Diameter(){
  maxLevel = 0;
  for (int i = 1; i <= vertices; i++){
    BFS_Matriz(i);
  }
	cout << maxLevel;
	cout << "\n";
  return;
}