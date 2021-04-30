#include "Grafo.h"
Grafo::Grafo(){}
Grafo::~Grafo(){}

void Grafo::merge(int arr[], int first, int m, int last){
    int i, j, k;
    int size_1 = 1+ m-first;
    int size_2 = last-m;
    int *L = (int*)malloc(size_1 * sizeof(int));
    int *R = (int*)malloc(size_2 * sizeof(int));
    
    for (i = 0; i < size_1; i++)
        L[i] = arr[first + i];
    for (j = 0; j < size_2; j++)
        R[j] = arr[first + 1 + j];
    i = 0;
    j = 0;
    k = first;
    while (i<size_1 && j<size_2){
        if (L[i] <= R[j]){
            arr[k]=L[i];
            i++;
        }else{
          arr[k]=R[j];
          j++;}
        k++;}
    while (i<size_1){
      arr[k]=L[i];
      i++;
      k++;}
    while (j<size_2){
      arr[k] = R[j];
      j++;
      k++;}
}
void Grafo::mergeSort(int arr[], int left, int right){
    if (left < right){
        int m = left + (right-left)/2;
        mergeSort(arr, left, m);
        mergeSort(arr, m+1, left);
        merge(arr, left, m, right);}
}


void Grafo::Infos(){
	gmax = 0;
	gmin = 2147483647;
	for (int i = 1; i <= vertices; i++){
		if (vector_grau[i] > gmax){
			gmax = vector_grau[i];
		}
		if (vector_grau[i] < gmin){
			  gmin = vector_grau[i];};
		}
    if (vertices %2 != 0){
        gmed = vector_grau[vertices/2];
    }
	else{
      gmed = (vector_grau[vertices/2]+vector_grau[1+ vertices/2])/2;}
    edges = edges/2;  
    return;
}
