#include "graph.h"
using namespace std;


List::List(string path, string algorithm, int selectedVertice, int targetVertice){
    ifstream rfile;
    rfile.open(path);
    if (!rfile){
      cout << "Arquivo não encontrado";
      return;
    }
    rfile >> vertices;
    m_pList = new ListNode*[vertices + 1]();

    edges = 0;
    
    string s;

    while (getline(rfile, s)){
        if (!s.empty()){
            istringstream tmp(s);
            int v1, v2;
            tmp >> v1 >> v2;
            if (v1<= vertices && v2 <= vertices){
                this->addAresta(v1, v2);
                this->addAresta(v2, v1);
            }
        }
    }
    Graph::vector_degree = new int[vertices + 1]();
    this->Degree();
    Graph::mergeSort(Graph::vector_degree, 1, vertices);
	
  Graph::Infos();
    visited = new int[vertices + 1](); 
    
    FComponentes_conexas();

    ofstream myInfoFile;
    myInfoFile.open(m_savePath + "/info.txt");
    myInfoFile << "Vertices = " << vertices << endl
    << "Arestas = " << (edges) << endl
    << "Grau maximo = " << gmax << endl
    << "Grau minimo = " << gmin << endl
    << "Mediana = " << gmed << endl
    << "Quantidade de componentes conexas: " << m_numero_de_componentes_conexas << endl;
    
    int k = m_i;

    for (int i = 1; i <= m_numero_de_componentes_conexas; i++){
      myInfoFile << "Numero de vertices da componete conexa: " << m_tamanho_das_componentes_conexas[i]
        << endl << "Vetores dessa componente conexa: ";

      for (int j = 1; j <= vertices + 1; j++){
        if (m_nodes_componentes_conexas[i][j] == 1) myInfoFile << j << ", ";
      }
      myInfoFile << "\n\n";
    }

    myInfoFile << endl << endl;
    
    visited = new int[vertices + 1](); 

    cout << "Arquivo salvo em " << m_savePath << "/info.txt" << endl;

    if (algorithm == "BFS") BFS_list(selectedVertice);
	  
    if (algorithm == "DFS") DFS_list(selectedVertice);
	
    if (algorithm == "diameter") Diameter();

	if (algorithm == "distance") BFS_list(selectedVertice, targetVertice);
}

void List::addAresta(int de, int para){
  ListNode* no = new ListNode;
  no->vertice = para;
  if (m_pList[de] != NULL){
    m_pList[de]->pPrev = no;
  }
  no->pNext = m_pList[de];
  no->pPrev = NULL;
  this->m_pList[de] = no;
  edges++;
}

void List::Degree(){
  int degree = 0;
  ListNode* temp;
  for (int i = 1; i <= vertices; i++){
    for (ListNode* neigh = m_pList[i]; neigh != NULL;){
      temp = neigh;
      neigh = neigh->pNext;
      degree++;
    }
    Graph::vector_degree[i] = degree;
    degree = 0;
  }
  return;
}

List::~List(){
    ListNode* temp;
    for (int i = 1; i <= vertices; i++){
        for (ListNode* neigh = m_pList[i]; neigh != NULL;){
            temp = neigh;
            neigh = neigh->pNext;
            delete temp;}
    }
    delete[] m_pList;
}

void List::BFS_list(int s, int targetVertice){
    while (!fqueue.empty()){
      fqueue.pop();
    }

    Tree* raiz = new Tree;
    raiz->vertice = s;
    raiz->nivel = 0;
    raiz->pai = NULL;
    
    ofstream myBFSFile;
    myBFSFile.open(m_savePath + "/lista_BFS.txt");
    myBFSFile << "Vertice: " << raiz->vertice << " --- Nivel: " << raiz->nivel << " --- Pai: x " << endl;
    
    visited[s] = 1;
    visitedInThisExecution[s] = 1;
    fqueue.push(raiz);
    
    while (!fqueue.empty()){
        Tree* v = fqueue.front();       
        fqueue.pop();                         
        m_tamanho_da_componente_conexa++;

        int w;
        for (ListNode* neigh = m_pList[v->vertice]; neigh != NULL;){
            w = neigh->vertice;
            neigh = neigh->pNext;
            if (visited[w] == 0){
              visited[w] = 1;
              visitedInThisExecution[w] = 1;
              Tree* x = this->Parentesco(v, w);
              myBFSFile << "Vertice: " << x->vertice << " --- Nivel: " << x->nivel
              << " --- Pai: " << v->vertice << endl;
              fqueue.push(x);
              if (x -> nivel > maxLevel){
                maxLevel = x -> nivel;
              }
			        if (targetVertice != 0){
				        if (x->vertice == targetVertice){
					        cout << "Distance between then is " << x->nivel << "\n";
				        }
			        }
            }
        }
    }
    return;
}

void List::DFS_list(int s){
	while (!pstack.empty()){
        pstack.pop();}
    Tree* raiz = new Tree;
    raiz->vertice = s;
    raiz->nivel = 0;
    raiz->pai = NULL;
    
    ofstream myDFSFile;
    myDFSFile.open(m_savePath + "/lista_DFS.txt");
    pstack.push(raiz);
    while (!pstack.empty()){
        Tree* u = pstack.top();
        pstack.pop();
        if (visited[u->vertice] == 0){
            myDFSFile << "Vertice: " << u->vertice << " --- Nivel: " << u->nivel;
            if (!(u == raiz)) myDFSFile << "--- Pai: " << u->pai->vertice << endl;
            else myDFSFile << "--- Pai: x " << endl;
            visited[u->vertice] = 1;
            for (ListNode* neigh = m_pList[u->vertice]; neigh != NULL;){
                int v;
                v = neigh->vertice;
                neigh = neigh->pNext;
                Tree* x = this->Parentesco(u, v);
                pstack.push(x);}
        }
    }
    return;
}

Tree* List::Parentesco(Tree* v, int w){
  Tree* filho = new Tree;
  filho->vertice = w;
  int nivel_pai = v->nivel;
  filho->nivel = nivel_pai + 1;
  filho->pai = v;
  return filho;
}

void List::FComponentes_conexas(){
  m_componentes_conexos = new int[vertices + 1];
  visitedInThisExecution = new int[vertices + 1];
  m_nodes_componentes_conexas = new int*[vertices + 1];
  m_tamanho_das_componentes_conexas = new int[vertices + 1];
  m_i = 1;
  for (int i = 1; i <= vertices; i++){
    if (visited[i] == 0){
      m_numero_de_componentes_conexas++;
      m_tamanho_da_componente_conexa = 0;
      BFS_list(i);
      m_tamanho_das_componentes_conexas[m_i] = m_tamanho_da_componente_conexa;
      m_componentes_conexos[i] = m_tamanho_da_componente_conexa;
      m_nodes_componentes_conexas[m_i] = visitedInThisExecution;
      visitedInThisExecution = new int[vertices + 1];
      m_i++;
    }
  }
  m_i = 1;
  return;
}

void List::Diameter(){
  maxLevel = 0;
  for (int i = 1; i <= vertices; i++){
    BFS_list(i);
  }
	cout << maxLevel << endl;
  return;
}
