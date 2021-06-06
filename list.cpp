#include "graph.h"
#include <string>
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

	bool Weight = false;

    while (getline(rfile, s)){
      if (!s.empty()){
        istringstream tmp(s);
        int v1, v2;
        float peso;
        tmp >> v1 >> v2 >> peso;

		if (v1 <= vertices && v2 <= vertices){
			if (peso > 0.00001){
            this->addAresta(v1, v2, peso);
            this->addAresta(v2, v1, peso);
			    Weight = true;
			    //cout << peso << "\n";
			}
			else if (peso < 1e-20 && peso > 1e-43){
          this->addAresta(v1, v2);
          this->addAresta(v2, v1);
			  //cout << v1 << v2;
			}
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

	//cout << algorithm << Weight;

	if (algorithm == "distance"){
		if (Weight) Dijkstra(selectedVertice, targetVertice);
		else BFS_list(selectedVertice, targetVertice);
	}

	if (algorithm == "dijkstra") Dijkstra(selectedVertice);

	if (algorithm == "eccentricity") Excentricidade(selectedVertice);

	if (algorithm == "mst") MST(selectedVertice);

}

void List::addAresta(int de, int para, float peso){
  ListNode* no = new ListNode;
  no->vertice = para;
  if (m_pList[de] != NULL){
    m_pList[de]->pPrev = no;
  }
  no->pNext = m_pList[de];
  no->pPrev = NULL;
  no->peso = peso;
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


void List::Dijkstra(int s, int targetVertice){
    Heap Dijkstra_heap;
    dijkstra_distancia = new float[vertices + 1]();
    dijkstra_pai = new int[vertices + 1]();
    dijkstraConjuntoS = new int[vertices + 1]();
    node **find = new node*[vertices + 1]();
    float infinito = std::numeric_limits<float>::max();
    
    //inicializa vetor de distancias com todas celulas infinitas, menos para o vertice inicial que é 0
    for (int i = 1; i <= vertices; i++){
        if (i != s) dijkstra_distancia[i] = infinito;
        else Dijkstra_heap.insert(0, s);
    }
    
    while (Dijkstra_heap.isEmpty() == false){
        node* v = Dijkstra_heap.getMinimum();
        int vertice = v->Vertice;
        dijkstraConjuntoS[vertice] = 1;
        Dijkstra_heap.removeMinimum();

        for (ListNode* w = m_pList[vertice]; w != NULL;){
            if ((dijkstra_distancia[w->vertice] > dijkstra_distancia[vertice] + w->peso) && (dijkstraConjuntoS[w->vertice] == 0)){
                if ((dijkstra_distancia[w->vertice] = !infinito)){
                    find[w->vertice] = Dijkstra_heap.decreaseKey(find[w->vertice], dijkstra_distancia[vertice] + w->peso);
                    dijkstra_distancia[w->vertice] = dijkstra_distancia[vertice] + w->peso;
                    dijkstra_pai[w->vertice] = vertice;
                }

                else{
                    find[w->vertice]=Dijkstra_heap.insert(dijkstra_distancia[vertice] + w->peso, w->vertice);
                    dijkstra_distancia[w->vertice] = dijkstra_distancia[vertice] + w->peso;
                    dijkstra_pai[w->vertice] = vertice;
                }
            }
            w = w->pNext;
        }
    }
	
    ofstream myDijkstraFile;
    myDijkstraFile.open(m_savePath + "/lista_Dijskstra.txt");
    for (int i = 1; i <= vertices; i++){
        myDijkstraFile << "Vertice: " << i << ", Distancia: " << dijkstra_distancia[i] << ", Caminho: " << i <<", ";
        int x = i;
        while (dijkstra_pai[x] != 0){
            myDijkstraFile << dijkstra_pai[x] << ", ";
            x = dijkstra_pai[x];
        }	
        myDijkstraFile << endl;
    }
	if (targetVertice != 0){
		cout << "Vértice de interesse: " << targetVertice << ", Distancia: " << dijkstra_distancia[targetVertice] << ", Caminho: " << targetVertice <<", ";
        int x = targetVertice;
        while (dijkstra_pai[x] != 0){
            cout << dijkstra_pai[x] << ", ";
            x = dijkstra_pai[x];
        }
		cout << "\n";
	}
    
}


void List::Excentricidade(int s){

    excentricidade = 0.0;
    List::Dijkstra(s);
    for (int i = 1; i <= vertices; i++){
    	if (excentricidade < dijkstra_distancia[i]) excentricidade = dijkstra_distancia[i];
    }
    c



void List::MST(int s){
    //high_resolution_clock::time_point t1 = high_resolution_clock::now();
    
    
    Heap MST_heap;
    mstDistancia = new float[vertices + 1]();
    mstPai = new int[vertices + 1]();
    mstConjuntoS = new int[vertices + 1]();
    
    //define infinito
    float infinito = std::numeric_limits<float>::max();
    
    node **find = new node*[vertices + 1]();


    for (int i = 1; i <= vertices; i++){

        if (i != s) mstDistancia[i] = infinito;
        
        else MST_heap.insert(0, s);
    }
    
    while (MST_heap.isEmpty() == false){
        node* v = MST_heap.getMinimum();  //pega valor minimo do heap
        int vertice = v->Vertice;
        mstConjuntoS[vertice] = 1;
        
        MST_heap.removeMinimum();  // remove minimo do heap
        
        for (ListNode* w = m_pList[vertice]; w != NULL;){

            //percorre os vizinhos desse vertice tirado do heap
            if ((mstDistancia[w->vertice] > w->peso) && (mstConjuntoS[w->vertice] == 0)){
                //se esse vertice ainda nao foi adicionado no heap, adiciona e atualiza vetores auxiliares
                if ((mstDistancia[w->vertice] = !infinito)){
                    find[w->vertice] = MST_heap.decreaseKey(find[w->vertice],  w->peso);
                    mstDistancia[w->vertice] = w->peso;
                    mstPai[w->vertice] = vertice;
                }
                //se esse vertice ja foi adicionado no heap, atualiza e atualiza vetores auxiliares
                else{
                    find[w->vertice]=MST_heap.insert( w->peso, w->vertice);
                    mstDistancia[w->vertice] =  w->peso;
                    mstPai[w->vertice] = vertice;
                }
            }
            w = w->pNext;
        }
    }
    
    
    
    //imprime arquivo de saida
    ofstream myMSTFile;
    myMSTFile.open(m_savePath + "/lista_MST.txt");
    float peso_total = 0;
    for (int i = 1; i <= vertices; i++){
        peso_total += mstDistancia[i];
    }
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    
	//auto duration1 = duration_cast<microseconds>(t2 - t1).count();
    //cout << duration1<<endl <<peso_total;
    
    myMSTFile << "Peso total: " << peso_total <<endl ;
    for (int i = 1; i <= vertices; i++){
        myMSTFile << i << "   " << mstPai[i] << endl;
    }
    
}
