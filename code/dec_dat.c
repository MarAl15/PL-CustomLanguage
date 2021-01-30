#include <stdlib.h>
#include <stdio.h>

typedef struct{
    int entero;
    float real;
    char caracter;
    bool booleano;
}nodo;

typedef struct{
    nodo dato[1000];
    int tam;
    int cursor;
    int tipo;
}lista;

void imprimir(lista *l){
    int i;
    char * type;
    
    printf("[ ");
    if(l->tipo == 2)
        for(i=0; i<l->tam; i++)
            printf("%f ", l->dato[i].real);
    else if(l->tipo == 3)
        for(i=0; i<l->tam; i++)
            printf("%c ", l->dato[i].caracter);
    else
        for(i=0; i<l->tam; i++)
            printf("%d ", l->dato[i].entero);
        
    printf("]\n");
}

void lista_ini(lista *l, int tipo){
    l->tam = 0;
    l->cursor = 0;
    l->tipo = tipo;
}

lista copiar(lista *l, int tam, int cursor, int tipo){
    int i;
    lista nueva;
    
    nueva.tam = tam;
    nueva.cursor = cursor;
    nueva.tipo = tipo;
    
    int min = l->tam;
    if(min > tam)
        min = tam;
        
    for(i=0; i<min; i++){
        nueva.dato[i] = l->dato[i]; 
    }
    
    return nueva;
}

// Avanzar: avanzar el cursor en una posición
lista avanzar(lista *l){
    return copiar(l, l->tam, l->cursor+1, l->tipo);  
}

// Retroceder: retrocede el cursor en una posición
lista retroceder(lista *l){
    return copiar(l, l->tam, l->cursor-1, l->tipo);  
}

// Cursor al comienzo de la lista: lleva el cursor al comienzo de la lista
lista inicio(lista *l){
    return copiar(l, l->tam, 0, l->tipo);  
}

// Longitud: devuelve el número de elementos de l
int longitud(lista *l){
    return l->tam;
}

// Elemento posición: devuelve el elemento de la posición x
int getI(lista *l, int x){
    return l->dato[x].entero;
}

int getF(lista *l, int x){
    return l->dato[x].real;
}

int getC(lista *l, int x){
    return l->dato[x].caracter;
}

// Elemento actual: devuelve el elemento actual de la lista
int elementoI(lista *l){
    return getI(l, l->cursor);
}

float elementoF(lista *l){
    return getF(l, l->cursor);
}

char elementoC(lista *l){
    return getC(l, l->cursor);
}

// Añadir elemento en una posición: devuelve una copia de l con x añadido en la posición z
void copiarParte(lista *nueva, lista *l, int inicio, int inicio_lista, int fin){
    int i, j;
    
    for(j=inicio, i=inicio_lista; j<fin; i++, j++){
        nueva->dato[j] = l->dato[i];
    }
}

lista setI(lista *l, int x, int z){
    lista nueva;
    nueva.tam = l->tam+1;
    nueva.cursor = l->cursor;
    nueva.tipo = l->tipo; 
    
    copiarParte(&nueva, l, 0, 0, z);
    nueva.dato[z].entero = x;
    copiarParte(&nueva, l, z+1, z, nueva.tam);
    return nueva;
}

lista setF(lista *l, float x, int z){
    lista nueva;
    nueva.tam = l->tam+1;
    nueva.cursor = l->cursor;
    nueva.tipo = l->tipo; 
    
    copiarParte(&nueva, l, 0, 0, z);
    nueva.dato[z].real = x;
    copiarParte(&nueva, l, z, z-1, nueva.tam);
    
    return nueva;
}

lista setC(lista *l, float x, int z){
    lista nueva;
    nueva.tam = l->tam+1;
    nueva.cursor = l->cursor;
    nueva.tipo = l->tipo; 
    
    copiarParte(&nueva, l, 0, 0, z);
    nueva.dato[z].caracter = x;
    copiarParte(&nueva, l, z+1, z, nueva.tam);
    
    return nueva;
}

// Borrar elemento en una posición: devuelve una copia de l con el elemento en la posición x borrado
lista eliminar(lista *l, int x){
    lista nueva;
    nueva.tam = l->tam-1;
    nueva.cursor = l->cursor;
    nueva.tipo = l->tipo; 
    
    copiarParte(&nueva, l, 0, 0, x);
    copiarParte(&nueva, l, x, x-1, nueva.tam);
    
    return nueva;
}

// Borrar lista a partir de una posición: devuelve una copia de l sin los elementos a partir de la posición x
lista borrar(lista *l, int x){
    lista nueva;
    nueva.tam = x;
    nueva.tipo = l->tipo;
    if(l->cursor >= x)
        nueva.cursor = x;
    else
        nueva.cursor = l->cursor;
        
    copiarParte(&nueva, l, 0, 0, x); 
    
    return nueva;  
}

// Concatenar listas: Añade los elementos de l2 y l1 y devuelve una copia
lista concatenar(lista *l1, lista *l2){
    lista nueva;
    nueva.tam = l1->tam + l2->tam;
    nueva.tipo = l1->tipo;
    nueva.cursor = l1->cursor;
    
    copiarParte(&nueva, l1, 0, 0, l1->tam);
    copiarParte(&nueva, l2, l1->tam, 0, nueva.tam);
    
    return nueva;
}

// Suma de x con cada elemento
lista sumaI(lista *l, int x){
    lista nueva;
    nueva.tam = l->tam;
    nueva.tipo = l->tipo;
    nueva.cursor = l->cursor;
    
    int i;
    
    for(i=0; i<nueva.tam; i++){
        nueva.dato[i].entero = l->dato[i].entero+x;    
    }
}

lista sumaF(lista *l, float x){
    lista nueva;
    nueva.tam = l->tam;
    nueva.tipo = l->tipo;
    nueva.cursor = l->cursor;
    
    int i;
    
    for(i=0; i<nueva.tam; i++){
        nueva.dato[i].real = l->dato[i].real+x;    
    }
}

// Resta de x con cada elemento
lista restaI(lista *l, int x){
    lista nueva;
    nueva.tam = l->tam;
    nueva.tipo = l->tipo;
    nueva.cursor = l->cursor;
    
    int i;
    
    for(i=0; i<nueva.tam; i++){
        nueva.dato[i].entero = l->dato[i].entero-x;    
    }
}

lista restaF(lista *l, float x){
    lista nueva;
    nueva.tam = l->tam;
    nueva.tipo = l->tipo;
    nueva.cursor = l->cursor;
    
    int i;
    
    for(i=0; i<nueva.tam; i++){
        nueva.dato[i].real = l->dato[i].real-x;    
    }
}

// Producto de x con cada elemento
lista productoI(lista *l, int x){
    lista nueva;
    nueva.tam = l->tam;
    nueva.tipo = l->tipo;
    nueva.cursor = l->cursor;
    
    int i;
    
    for(i=0; i<nueva.tam; i++){
        nueva.dato[i].entero = l->dato[i].entero*x;    
    }
}

lista productoF(lista *l, float x){
    lista nueva;
    nueva.tam = l->tam;
    nueva.tipo = l->tipo;
    nueva.cursor = l->cursor;
    
    int i;
    
    for(i=0; i<nueva.tam; i++){
        nueva.dato[i].real = l->dato[i].real*x;    
    }
}

// División de x con cada elemento
lista divisionI(lista *l, int x){
    lista nueva;
    nueva.tam = l->tam;
    nueva.tipo = l->tipo;
    nueva.cursor = l->cursor;
    
    int i;
    
    for(i=0; i<nueva.tam; i++){
        nueva.dato[i].entero = l->dato[i].entero/x;    
    }
}

lista divisionF(lista *l, float x){
    lista nueva;
    nueva.tam = l->tam;
    nueva.tipo = l->tipo;
    nueva.cursor = l->cursor;
    
    int i;
    
    for(i=0; i<nueva.tam; i++){
        nueva.dato[i].real = l->dato[i].real/x;    
    }
}

//void operator=



