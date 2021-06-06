#include <bits/stdc++.h>
#ifndef Graph_hp
#define Graph_hp
using namespace std;

struct Tree{
  int vertice;
  int nivel;
  Tree* pai;
};
    
class Graph{
public:
    Graph();
    ~Graph();
    
protected:
    int vertices;
    int edges;
    int gmax;
    int gmin;
    int gmed;
    int * vector_degree;
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
#endif /* graph_hpp */

#ifndef List_h
#define List_h

struct ListNode{
    int vertice;
    float peso;
    ListNode* pPrev;
    ListNode* pNext;
};

class List :
	public Graph{
		public:
    		List(string path, string algorithm, int selectedVertice, int targetVertice);
    		~List();
			protected:
    			stack<Tree*> pstack; //pilha p DFS
    			queue<Tree*> fqueue;   //fila p BFS
    			int *visited;   //vetor de marcação
    			void addAresta(int de, int para, float peso=1);   
    			ListNode** m_pList;
    			void Degree();
    			void BFS_list(int s, int targetVertice = 0);
    			void DFS_list(int s);
				void Diameter();
				float excentricidade;
				void Dijkstra(int s, int targetVertice = 0);
                void Excentricidade(int s);
				float* dijkstra_distancia;
				int* dijkstra_pai;
    			int* dijkstraConjuntoS;
				float* mstDistancia;
    			int* mstPai;
    			int* mstConjuntoS;
    			int m_tamanho_da_componente_conexa;
    			int m_numero_de_componentes_conexas = 0;
    			int* m_componentes_conexos;
    			int m_x;
    			int m_maior_tamanho = 0;
    			bool etapa;
    			int m_i;
    			Tree* Parentesco(Tree* v, int w);
    			void FComponentes_conexas();
				void MST (int s);
    			std::chrono::time_point<std::chrono::system_clock> start, 	end;
};

#endif /* List_h */


#ifndef Matrix_hp
#define Matrix_hp
using namespace std::chrono;

class Matrix :
	public Graph{
		public:
		    Matrix(string path, string algorithm, int selectedVertice, int targetVertice);
		    ~Matrix();
		
		private:
		    void construtor();
		    stack<Tree*> pstack;
		    bool** matrix;
		    void addAresta(int i, int j);
		    int * vector_degree;
		    queue<Tree*> fqueue;
		    int *visited;
		    void BFS_Matrix(int s);
		    void DFS_Matrix(int s);
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
#endif /* Matrix_hp */

#ifndef Heap_h
#define Heap_h

struct node {
private:
    node* prev;
    node* next;
    node* child;
    node* parent;
    int degree;
    bool marked;
    
public:
    float Value;
    int Vertice;
    friend class Heap;
    node* getPrev() { return prev; }
    node* getNext() { return next; }
    node* getChild() { return child; }
    node* getParent() { return parent; }
    int getValue() { return Value; }
    bool isMarked() { return marked; }
    bool hasChildren() { return child; }
    bool hasParent() { return parent; }
};

class Heap
{
protected:
    node* heap;
public:
    Heap();
    virtual ~Heap();
    node* insert(float value,int vertice);
    void merge(Heap& other);
    bool isEmpty();
    node* getMinimum();
    int removeMinimum();
    node* decreaseKey(node* n, float value);
    node* find(float value);
private:
    node * _empty();
    node* _singleton(float value, int vertice);
    node* _merge(node* a, node* b);
    void _deleteAll(node* n);
    void _addChild(node* parent, node* child);
    void _unMarkAndUnParentAll(node* n);
    node* _removeMinimum(node* n);
    node* _cut(node* heap, node* n);
    node* _decreaseKey(node* heap, node* n, float value);
    node* _find(node* heap, float value);
};
#endif /* Heap_h */
