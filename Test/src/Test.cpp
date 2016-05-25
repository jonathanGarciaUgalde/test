//============================================================================
// Name        : Test.cpp
// Author      : Edd
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

//For adding new line in a program
inline void newLine(){
    cout << "\n";
}


int eleva(int base, int expo){
	int x=1;
	for(int i=0;i<expo;i++){
		x=x*base;
	}
	return x;
}
//fitness indica que tan perfecto es el arbol
//el valor que regresa indica hasta que punto del array el arbol deja de ser perfecto
//por medio de la formula: fit*2, esto indica que los elemntos del array[0] hasta el array[fit*2]
//estan acomodados de la forma correcta, y que los valores del array[1+fit*2] hasta array[n]
//tiene que ser mutados
int fitness(int parent[],int n, int fit){
	int lvl=1;
	//cout<<parent[0]<<"Pos("<<0<<")";
	//newLine();
	for(int i=0;i<n;i++){
		//Comprueba que el nivel del arbol se encuentre lleno
		int n_elemntos=eleva(2,lvl);
		if((2*i+2==n_elemntos)&&(2*i+2<n)){
			//newLine();
			//cout<<"Nivel"<<lvl<<":";
			//newLine();
			lvl++;
		}
		if(2*i+2<n){
			//valida que el hijo izq sea menor que la raiz y que el hijo derecho sea mayor
			if(parent[i]>parent[2*i+1] && parent[i]<parent[2*i+2]){
				fit++;
				//cout<<parent[2*i+1]<<"Pos("<<2*i+1<<")";
				//cout<<parent[2*i+2]<<"Pos("<<2*i+2<<")";
			}
			//en el caso de no se cumpla la condicion indica hasta que punto no se cumple
			else return fit;

		}

	}
	return fit;
}

void print(int parent[],int n){
	int lvl=1;
	cout<<parent[0]<<"Pos("<<0<<")";
	newLine();
	for(int i=0;i<n;i++){
		int n_elemntos=eleva(2,lvl);
		if((2*i+2==n_elemntos)&&(2*i+2<n)){
			newLine();
			cout<<"Nivel"<<lvl<<":";
			newLine();
			lvl++;
		}
		if(2*i+2<n){
			cout<<parent[2*i+1]<<"Pos("<<2*i+1<<")";
			cout<<parent[2*i+2]<<"Pos("<<2*i+2<<")";
			}
	}
}


int main(){
	//cout << eleva(2,5) << endl; // prints test
	int parent[] =  {10,5,15,1,6,11,17};
	int n = sizeof parent / sizeof parent[0];
    //Node *root = createTree(parent, n);
    //cout << "Inorder Traversal of constructed tree\n";
    //inorder(root);

	cout<<"Fitness: "<<fitness(parent,n,0)<<endl;

	return 0;
}
