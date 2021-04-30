#include <bits/stdc++.h>
#ifndef Grafo_hp
#define Grafo_hp
using namespace std;

struct Tree{
  int vertice;
  int nivel;
  Tree* pai;
};
    
class Grafo{
public:
    Grafo();
    ~Grafo();
    
protected:
    int vertices;
    int edges;
    int gmax;
    int gmin;
    int gmed;
    int * vector_grau;
	  int maxLevel;
    int *visitedInThisExecution;
    int* *m_nodes_componentes_conexas;
    int *m_tamanho_das_componentes_conexas;

    float gmedio;

    string m_savePath = "output";

    void merge(int arr[], int a, int b, int c);
    void mergeSort(int arr[], int a, int c);
    void Infos();
};
#endif /* Grafo_hpp */

#ifndef List_h
#define List_h

struct ListNode{
    int vertice;
    float peso;
    ListNode* pPrev;
    ListNode* pNext;
};

class Lista :
	public Grafo{
		public:
    		Lista(string path, string algorithm, int selectedVertice, int targetVertice);
    		~Lista();
			protected:
    			stack<Tree*> pstack; //pilha p DFS
    			queue<Tree*> fqueue;   //fila p BFS
    			int *visited;   //vetor de marcação
    			void addAresta(int de, int para);   
    			ListNode** m_pLista;
    			void Grau();
    			void BFS_lista(int s, int targetVertice = 0);
    			void DFS_lista(int s);
				  void Diameter();
    			int m_tamanho_da_componente_conexa;
    			int m_numero_de_componentes_conexas = 0;
    			int* m_componentes_conexos;
    			int m_x;
    			int m_maior_tamanho = 0;
    			bool etapa;
    			int m_i;
    			Tree* Parentesco(Tree* v, int w);
    			void FComponentes_conexas();
    			std::chrono::time_point<std::chrono::system_clock> start, 	end;
};

#endif /* List_h */


#ifndef Matriz_hp
#define Matriz_hp
using namespace std::chrono;

class Matriz :
	public Grafo{
		public:
		    Matriz(string path, string algorithm, int selectedVertice, int targetVertice);
		    ~Matriz();
		
		private:
		    void construtor();
		    stack<Tree*> pstack;
		    bool** matriz;
		    void addAresta(int i, int j);
		    int * vector_grau;
		    queue<Tree*> fqueue;
		    int *visited;
		    void BFS_Matriz(int s);
		    void DFS_matriz(int s);
				void Diameter();

		    Tree * Parentesco(Tree * v, int w);
    		int m_tamanho_da_componente_conexa;
    		int m_numero_de_componentes_conexas = 0;
    		int* m_componentes_conexos;
    		int m_x;
    		int m_maior_tamanho = 0;
    		bool etapa;
    		int m_i;
        void FComponentes_conexas();
		    std::chrono::time_point<std::chrono::system_clock> start, end;  
	};
#endif /* Matriz_hp */

