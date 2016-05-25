#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define POBLACION 10
#define LONG_COD 7
//#define LIMITE -5.12
#define PROB_CRUCE 0.3
#define PROB_MUTACION 0.001
//#define INTERVALO 10.24/pow(2,LONG_COD/2)



typedef struct {
    int tree[LONG_COD];
    int fit;
} Individuo;


Individuo generarIndividuo (void);
Individuo * generarPoblacion (void);
Individuo * seleccionTorneos(Individuo * pob);
void mutacionHijos (Individuo *);
void cruzarSeleccion (Individuo *);
Individuo elite(Individuo *);
void AG();


int generaRandom(){
	int random1 = rand() % 10000 + 1;

	return random1;
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
int fitness(int parent[],int n){
	int lvl=1;
	int fit=0;
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

/* PROC generarIndividuo (void)                         DEV (Individuo)
 * MODIFICA nada
 * EFECTO se encarga de generar un individuo utilizando valores aleatorios. primero crea
 *  la cadena de bits del genotipo utilizando la funcion generaBinario y luego evalua
 *  la aptitud del individuo utilizando la funcion decoder para decodificar el genotipo
 *  y la funcion fitness para obtener la aptitud.   */

Individuo generarIndividuo (void){
    Individuo ind;
    int i;

    for (i=0; i<LONG_COD; i++)
        ind.tree[i]=generaRandom();

    ind.fit = fitness(ind.tree,LONG_COD);

    return ind;
}

/* PROC generarPoblacion (void)                         DEV (Individuo *)
 * MODIFICA nada
 * EFECTO esta funcion genera una poblacion con la cantidad de individuos dada por la
 *  macro POBLACION. para generar cada individuo utiliza la funcion generarIndividuo()
 *  y una vez ha terminado el bucle, devuelve el puntero al primer individuo    */
Individuo * generarPoblacion(void)
{
    Individuo * poblacion;
    int i;

    poblacion = (Individuo *) malloc(sizeof(Individuo)*POBLACION);
    for(i=0;i<POBLACION;i++)
        poblacion[i] = generarIndividuo();

    return poblacion;
}

/* PROC seleccionTorneos (Individuo *)                  DEV (Individuo *)
 * MODIFICA nada
 * EFECTO se crea un nuevo vector de individuos que contendra a los individuos seleccionados
 *  para el cruce. la seleccion se hace por torneos por tanto cada individuo seleccionado
 *  sera el que tenga mejor aptitud de dos candidatos. el proceso se repite en POBLACION-1
 *  iteraciones, que es la cantidad de individuos que se deben seleccionar. la reserva de
 *  memoria se hace sobre POBLACION individuos dado que, como luego vamos a seleccionar el
 *  mejor de la poblacion anterior por elitismo, debemos dejar una plaza de la siguiente
 *  generacion libre. la seleccion es con repeticion */

Individuo * seleccionTorneos (Individuo * poblacion)
{
    Individuo candidato_a, candidato_b;
    int i;

    Individuo * seleccion = (Individuo *) malloc (sizeof(Individuo)*POBLACION);

    for (i=0; i<POBLACION-1; i++)
    {
        candidato_a = poblacion[(int) (((double) POBLACION)*rand()/(RAND_MAX+1.0))];
        candidato_b = poblacion[(int) (((double) POBLACION)*rand()/(RAND_MAX+1.0))];

        if (candidato_a.fit < candidato_b.fit)
            seleccion[i] = candidato_a;
        else
            seleccion[i] = candidato_b;
    }

    return seleccion;
}

/* PROC mutacionHijos (Individuo *)                         DEV (void)
 * MODIFICA (Individuo *)
 * EFECTO esta funcion aplica la mutacion segun la probabilidad dada por PROB_MUTACION.
 *  recibe un vector de individuos en el que debe ocurrir que los dos primeros sean
 *  los hijos correspondientes a un cruce. el genotipo de cada uno de los individuos
 *  se recorre por completo calculando la probabilidad de que el bit mute y cambiando
 *  si se diera el caso positivo    */

void mutacionHijos (Individuo * hijos)
{
    int i, j;

    for(i=0; i<2; i++)
        for(j=0; j<LONG_COD; j++)
            if ((double) rand()/(RAND_MAX+1.0) < PROB_MUTACION)
            {
                hijos[i].tree[j] = (hijos[i].tree[j]*generaRandom())/generaRandom();
            }
}

/* PROC cruzarSeleccion (Individuo *)                   DEV (void)
 * MODIFICA (Individuo *)
 * EFECTO esta funcion se encarga de cruzar los individuos seleccionados. la seleccion
 *  esta ordenada luego cruzamos los individuos seguidos de dos en dos. para cada una
 *  de las iteraciones se aplica la probabilidad de cruce. en caso de que se crucen
 *  los individuos se genera un punto de cruce y se intercambian las porciones del
 *  vector. luego se llama a la funcion de mutacion. en caso de que no haya cruce, los
 *  padres pasan directamente a la siguiente generacion */

void cruzarSeleccion (Individuo * seleccion)
{
    int i, j, punto, aux;
    double x, y;

    for(i=0; i<POBLACION-1; i+=2)
    {
        if((double) rand()/(RAND_MAX+1.0) < PROB_CRUCE)
        {
            //punto = (int) (((double) LONG_COD)*rand()/(RAND_MAX+1.0));
        	punto = (int) ((seleccion[i].fit)+1);

            for(j=punto; j<LONG_COD; j++)
            {
                aux=seleccion[i].tree[j];
                seleccion[i].tree[j]=seleccion[i+1].tree[j];
                seleccion[i+1].tree[j]=aux;
            }

            mutacionHijos(&seleccion[i]);


            seleccion[i].fit = fitness(seleccion[i].tree,LONG_COD);


            seleccion[i+1].fit = fitness(seleccion[i].tree,LONG_COD);
        }
    }
}

/* PROC elite (Individuo * poblacion)                   DEV (Individuo)
 * MODIFICA nada
 * EFECTO se trata de una funcion que devuelve el mejor individuo de una poblacion
 *  que se pasa como argumento. utiliza un individuo como comparador y devuelve
 *  el que para nuestro caso tiene el mejor fitness, es decir, aptitud mas baja */

Individuo elite (Individuo * poblacion)
{
    int i;
    Individuo best = poblacion[0];

    for(i=0; i<POBLACION; i++)
        if(best.fit > poblacion[i].fit)
            best = poblacion[i];

    return best;
}
void AG()
{
    Individuo * seleccion, * poblacion = generarPoblacion();
    Individuo best;
    int generacion = 0;

    do
    {
        seleccion = seleccionTorneos(poblacion);
        cruzarSeleccion(seleccion);
        seleccion[POBLACION-1] = elite(poblacion);
        free(poblacion);
        poblacion = seleccion;
        generacion++;
    } while ((elite(poblacion).fit != (LONG_COD-1)/2) or generacion!=10);

    best = elite(poblacion);
    free(poblacion);

    printf("*************************************\n");
    printf ("*          FIN DEL ALGORITMO        *\n");
    printf ("*************************************\n");
    //printf (" - El arreglo (%.5f)\n", best.tree);
    //printf (" - Su fenotipo es %.5f\n", best.fit);
    printf (" - Es la generacion numero %i\n", generacion);
    printf ("*************************************\n");
}

int main() {

    srand(time(0));
    AG();
    return 0;
}










