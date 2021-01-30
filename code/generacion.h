#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "semantico.h"

extern DescriptorDeInstrControl TE[MAX_ENTRADAS];
extern int TOPE_PILA;
extern int globales;
extern entradaTS TS[MAX_ENTRADAS];

extern int NUMERO_ETIQUETA_ACTUAL;
extern int NUMERO_VARIABLE_ACTUAL;

extern int EN_PROCEDIMIENTO;
extern int TABULAR;
extern int DENTRADA;

extern char PARAMETROS_ACTUALES[255];
extern char* salida;

extern FILE *SALIDA_CODIGO_PRINCIPAL;
extern FILE *SALIDA_ACTUAL;
extern FILE *SALIDA_CODIGO_SUBPROGRAMAS;

extern char * llamada_lista;

char * generarIdentificador(char *base, int numero);
char *generarVariable();
char *generarEtiqueta();

void codigoCabeceraMain(FILE *f);
void tabular(FILE *f);
void abrirBloque(FILE *f);
void cerrarBloque(FILE *f);
int cTipo (tDato tipo, FILE *f);

void codigoAbrirDescriptores();
void codigoCerrarDescriptores();

void codigoDeclaracionVariables(FILE *f, int globales);
void codigoDeclaracionVariablesExtern();

void codigoSentenciaAsignacion(atributos destino, atributos origen, FILE *f);

char *codigoOpUna(atributos op, atributos exp, FILE *f);
char *codigoSumResUnario(atributos op, atributos exp, FILE *f);
char *codigoOpOr(atributos exp1, atributos exp2, FILE *f);
char *codigoOpAnd(atributos exp1, atributos exp2, FILE *f);
char *codigoOpRelacion(atributos exp1, atributos op, atributos exp2, FILE *f);
char *codigoOpSumResBinario(atributos exp1, atributos op, atributos exp2, FILE *f);
char *codigoOpMult(atributos exp1, atributos exp2, FILE *f);
char *codigoOpBinario(atributos exp1, atributos op, atributos exp2, FILE *f);
char *codigoSeuro(atributos exp1, atributos exp2, FILE *f);
char *codigoArroba(atributos exp1, atributos exp2, FILE *f);
char *codigoOpSumArroba(atributos exp1, atributos exp2, atributos exp3, FILE *f);

void codigoIF(int parte, char *varControl, FILE *f); 
void codigoWHILE(int parte, char *varControl, FILE *f);
void codigoRepeatUntil(int parte, char *varControl, FILE *f);

void codigoProcedimiento(int parte, char* nombre, int tipo);
//void codigoParametrosFormales(FILE *f);
int NoParamFormal(char * variable, int i);
void codigoParametro(atributos param, char* coma);



void escribirTipo(int tipo, FILE *f);
void escribirTipoSalida(int tipo, FILE *f);
char* codigoSentenciaEntradaTipo(atributos id, FILE *f, char* identificadores);
char* codigoSentenciaSalidaTipo(atributos expr, FILE *f, char* expresiones);
void codigoSentenciaEntrada(char * identificadores, FILE *f);
void codigoSentenciaSalida(char * expresiones, FILE *f);



char* escribirFuncionLista(char * funcion);
char* escribirParamListaS(char * funcion, char * parametro, int fin);
char* escribirParamListaI(char * funcion, int parametro, int fin);
char* escribirParamListaF(char * funcion, float parametro, int fin);
char* escribirParamListaID(char * funcion, char * id, int fin);
void escribir(char * texto, FILE *f);
