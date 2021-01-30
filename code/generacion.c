#include "generacion.h"

int globales;
int NTabs = 0;
int NTabsProg = 0;
int NTabsMain = 0;

int NUMERO_ETIQUETA_ACTUAL = -1;
int NUMERO_VARIABLE_ACTUAL = -1;

int EN_PROCEDIMIENTO = 0;
int TABULAR = 1;
int DENTRADA = 0;

char PARAMETROS_ACTUALES[255];

FILE *SALIDA_CODIGO_PRINCIPAL = NULL;
FILE *SALIDA_CODIGO_SUBPROGRAMAS = NULL;
FILE *SALIDA_ACTUAL = NULL;

char * llamada_lista;

DescriptorDeInstrControl TE[MAX_ENTRADAS];
int TOPE_PILA = -1;

char* salida;
 
/**
  * Genera nombres a partir de base y numero, obteniendo su declaración.
  */
char * generarIdentificador(char *base, int numero){
    char *ident =(char*) malloc(sizeof(char) * 256);
    sprintf(ident, "%s%d", base, numero);
    return ident;
}

/**
  * Genera nombres de variables temporales, obteniendo su declaración.
  */
char *generarVariable(){
    NUMERO_VARIABLE_ACTUAL++;
    return generarIdentificador("temp", NUMERO_VARIABLE_ACTUAL);
}

/**
  * Genera nombres de etiquetas, obteniendo su declaración.
  */
char *generarEtiqueta(){
    NUMERO_ETIQUETA_ACTUAL++;
    return generarIdentificador("etiqueta", NUMERO_ETIQUETA_ACTUAL);
}

/**
  * Incluimos las cabeceras que necesitemos
  */
void codigoCabeceraMain(FILE *f){
    fprintf(f, "#include <stdio.h>\n");
    fprintf(f, "#include \"bool.c\"\n");
    fprintf(f, "#include \"dec_dat.c\"\n");
    fprintf(f, "#include \"dec_fun.c\"\n");
    fprintf(f, "\n");
}

/**
  * Escribir tabulaciones
  */
void tabular(FILE *f){
    int i;
    for(i=0; i<NTabs; i++){
        fprintf(f, "\t");
    }
}
/**
  * Abrimos un bloque al comienzo de la traducción, y lo cerramos al final de esta.
  * Esto permite optimizar la memoria requerida en ejecución por las variables temporales introducidas, ya que se liberaría la memoria de las variables temporales una vez finalizado el bloque.
  */
void abrirBloque(FILE *f){
    tabular(f);
    NTabs++;
    fprintf(f, "{\n");
}

void cerrarBloque(FILE *f){
    NTabs--;
    tabular(f); 
    fprintf(f, "}\n");
}


/*------------------TIPOS--------------------*/
int cTipo (tDato tipo, FILE *f){
    int correcto = 1;
    
    if(TABULAR)
        tabular(f);
        
    switch(tipo){
        case ENTERO:
            fprintf(f, "int ");
            break;
        case BOOLEANO:
            fprintf(f, "bool ");
            break;
        case REAL:
            fprintf(f, "float ");
            break;
        case CARACTER:
            fprintf(f, "char ");
            break;
        case STRING:
            fprintf(f, "char *");
            break;
        case LISTA:
            fprintf(f, "lista ");
            break;
        default:
            correcto = 0;
            break;
  }
  return correcto;
}

/*-----------------------PROGRAMA------------------------------*/
void codigoAbrirDescriptores(){
  SALIDA_CODIGO_PRINCIPAL = fopen("codigo.c", "w");
  SALIDA_CODIGO_SUBPROGRAMAS = fopen("dec_fun.c", "w");
  SALIDA_ACTUAL = SALIDA_CODIGO_PRINCIPAL;
}

void codigoCerrarDescriptores(){
    fclose(SALIDA_CODIGO_PRINCIPAL);
    fclose(SALIDA_CODIGO_SUBPROGRAMAS);
}

/*-----------------------VARIABLES---------------------------*/
int NoParamFormal(char * variable, int i){
    while(i>=0 && (TS[i].entrada != FUNCION)){
        if( TS[i].entrada==PAR_FORMAL &&
           strcmp(TS[i].lexema, variable) == 0)
           return 0;
        i--;
    }
    
    return 1;
}
void codigoDeclaracionVariables(FILE *f, int globales){
    int i = TOPE-1;

    while (i >= 0 && (TS[i].entrada == VARIABLE)){
        if(NoParamFormal(TS[i].lexema, i-1)){
            cTipo(TS[i].tipoDato, f);
            fprintf(f, "%s;\n", TS[i].lexema);
        }
        i--;
    }
    
    fprintf(f, "\n");
		
    if(globales){
        fprintf(f, "int main()\n");
        abrirBloque(f);
        
        codigoDeclaracionVariablesExtern();
    }
    
    i = TOPE-1;
    while (i >= 0 && (TS[i].entrada == VARIABLE)){
        if(TS[i].tipoDato == LISTA){
            if(NoParamFormal(TS[i].lexema, i-1)){
                tabular(f);
	            escribir(llamada_lista, SALIDA_ACTUAL);
            }
        }
        i--;
    }
    fprintf(f, "\n");
    
    //if(tipoGlobal == LISTA){
        
	//}
}

void codigoDeclaracionVariablesExtern(){
    int i = TOPE-1;

    while (i >= 0 && (TS[i].entrada == VARIABLE)){
        if(NoParamFormal(TS[i].lexema, i-1)){
            fprintf(SALIDA_CODIGO_SUBPROGRAMAS, "extern ");
            cTipo(TS[i].tipoDato, SALIDA_CODIGO_SUBPROGRAMAS);
            fprintf(SALIDA_CODIGO_SUBPROGRAMAS, "%s;\n", TS[i].lexema);
        }
        i--;
    }
    
    fprintf(SALIDA_CODIGO_SUBPROGRAMAS, "\n");
}


/*----------------------SENTENCIAS---------------------------*/
void codigoSentenciaAsignacion(atributos destino, atributos origen, FILE *f){
    tabular(f);
    fprintf(f, "%s = %s;\n", destino.lexema, origen.lexema);
}



/*----------------------EXPRESIONES--------------------------*/
/**
  * OPUNARIO
  */
char *codigoOpUna(atributos op, atributos exp, FILE *f){
    char *var = generarVariable();
  
    switch(op.atrib){
        case 0: // !
            cTipo(exp.tipo, f);
            fprintf(f, "%s = !%s;\n", var, exp.lexema);
            break;
        case 1: // ? -> elemento actual
            cTipo(exp.tipo_lista, f);
            if(op.tipo_lista == ENTERO)
                llamada_lista = escribirFuncionLista("elementoI");
            else if(op.tipo_lista == REAL)
                llamada_lista = escribirFuncionLista("elementoF");
            else
                llamada_lista = escribirFuncionLista("elementoC");
                
            llamada_lista = escribirParamListaID(llamada_lista, exp.lexema, 1);
            fprintf(f, "%s = ", var);
            escribir(llamada_lista, SALIDA_ACTUAL);
            break;
        case 2: // # -> longitud
            cTipo(ENTERO, f);
            llamada_lista = escribirFuncionLista("longitud");
                
            llamada_lista = escribirParamListaID(llamada_lista, exp.lexema, 1);
            fprintf(f, "%s = ", var);
            escribir(llamada_lista, SALIDA_ACTUAL);
            break;
    }        
    return var;
}

/**
  * SUMRES UNARIO
  */
char *codigoSumResUnario(atributos op, atributos exp, FILE *f){
    char *var = generarVariable();
    cTipo(exp.tipo, f);

    switch(op.atrib){
        case 0:
            fprintf(f, "%s = %s;\n", var, exp.lexema);
            break;
        case 1:
            fprintf(f, "%s = - %s;\n", var, exp.lexema);
            break;
    }       
    return var;
}

/**
  * OPOR
  */
char *codigoOpOr(atributos exp1, atributos exp2, FILE *f){
    char *var = generarVariable();
    cTipo(exp1.tipo, f);

    fprintf(f, "%s = %s || %s;\n", var, exp1.lexema, exp2.lexema);
    return var;
}

/**
  * OPAND
  */
char *codigoOpAnd(atributos exp1, atributos exp2, FILE *f){
    char *var = generarVariable();
    cTipo(exp1.tipo, f);

    fprintf(f, "%s = %s && %s;\n", var, exp1.lexema, exp2.lexema);
    return var;
}
  
/**
  * OPRELACION
  */
char *codigoOpRelacion(atributos exp1, atributos op, atributos exp2, FILE *f){
    char *var = generarVariable();
    cTipo(BOOLEANO, f);

    switch (op.atrib){
        case 0:
            fprintf(f, "%s = %s == %s;\n", var, exp1.lexema, exp2.lexema);
            break;
        case 1:
            fprintf(f, "%s = %s != %s;\n", var, exp1.lexema, exp2.lexema);
            break;
        case 2:
            fprintf(f, "%s = %s < %s;\n", var, exp1.lexema, exp2.lexema);
            break;
        case 3:
            fprintf(f, "%s = %s <= %s;\n", var, exp1.lexema, exp2.lexema);
            break;
        case 4:
            fprintf(f, "%s = %s > %s;\n", var, exp1.lexema, exp2.lexema);
            break;
        case 5:
            fprintf(f, "%s = %s >= %s;\n", var, exp1.lexema, exp2.lexema);
            break;
  }
  return var;
}

/**
  * SUMRES BINARIO 
  */
char *codigoOpSumResBinario(atributos exp1, atributos op, atributos exp2, FILE *f){
    char *var = generarVariable();
    
    if(exp1.tipo == LISTA){
        cTipo(LISTA, f);
        
        switch (op.atrib){
            case 0:
                if(exp2.tipo == ENTERO)
                    llamada_lista = escribirFuncionLista("sumaI");
                else
                    llamada_lista = escribirFuncionLista("sumaF");
                break;
            case 1:
                if(exp2.tipo == ENTERO)
                    llamada_lista = escribirFuncionLista("restaI");
                else
                    llamada_lista = escribirFuncionLista("restaF");
                break;
        }
        llamada_lista = escribirParamListaID(llamada_lista, exp1.lexema, 0);
        llamada_lista = escribirParamListaS(llamada_lista, exp2.lexema, 1);
        fprintf(f, "%s = ", var);
        escribir(llamada_lista, SALIDA_ACTUAL);
    }
    else if(exp2.tipo == LISTA){
        cTipo(LISTA, f);
        
        switch (op.atrib){
            case 0:
                if(exp2.tipo == ENTERO)
                    llamada_lista = escribirFuncionLista("sumaI");
                else
                    llamada_lista = escribirFuncionLista("sumaF");
                break;
            case 1:
                if(exp2.tipo == ENTERO)
                    llamada_lista = escribirFuncionLista("restaI");
                else
                    llamada_lista = escribirFuncionLista("restaF");
                break;
        }
        llamada_lista = escribirParamListaID(llamada_lista, exp2.lexema, 0);
        llamada_lista = escribirParamListaS(llamada_lista, exp1.lexema, 1);
        fprintf(f, "%s = ", var);
        escribir(llamada_lista, SALIDA_ACTUAL);
    }
    else{
        if(exp1.tipo == REAL || exp2.tipo == REAL)
            cTipo(REAL, f);
        else
            cTipo(exp1.tipo, f);
        
        switch (op.atrib){
            case 0:
                fprintf(f, "%s = %s + %s;\n", var, exp1.lexema, exp2.lexema);
                break;
            case 1:
                fprintf(f, "%s = %s - %s;\n", var, exp1.lexema, exp2.lexema);
                break;
        }
    }
    
    return var;
}
  
/**
  * OPMULT 
  */
char *codigoOpMult(atributos exp1, atributos exp2, FILE *f){
    char *var = generarVariable();

    if(exp1.tipo == LISTA){
        cTipo(LISTA, f);
        
        if(exp2.tipo == ENTERO)
            llamada_lista = escribirFuncionLista("productoI");
        else
            llamada_lista = escribirFuncionLista("productoF");
            
        llamada_lista = escribirParamListaID(llamada_lista, exp1.lexema, 0);
        llamada_lista = escribirParamListaS(llamada_lista, exp2.lexema, 1);
        fprintf(f, "%s = ", var);
        escribir(llamada_lista, SALIDA_ACTUAL);
    }
    else if(exp2.tipo == LISTA){
        cTipo(LISTA, f);
        
        if(exp1.tipo == ENTERO)
            llamada_lista = escribirFuncionLista("productoI");
        else
            llamada_lista = escribirFuncionLista("productoF");
            
        llamada_lista = escribirParamListaID(llamada_lista, exp2.lexema, 0);
        llamada_lista = escribirParamListaS(llamada_lista, exp1.lexema, 1);
        fprintf(f, "%s = ", var);
        escribir(llamada_lista, SALIDA_ACTUAL);
    }
    else{
        if(exp1.tipo == REAL || exp2.tipo == REAL)
            cTipo(REAL, f);
        else
            cTipo(exp1.tipo, f);
        
        fprintf(f, "%s = %s * %s;\n", var, exp1.lexema, exp2.lexema);
    }
    
    return var;
}

/**
  * OPBINARIO
  */
char *codigoOpBinario(atributos exp1, atributos op, atributos exp2, FILE *f){
    char *var = generarVariable();
    char *operador;
    
    //tabular(f);
    
    if(exp1.tipo == LISTA){
        cTipo(LISTA, f);
        switch (op.atrib){
            case 0: // / -> division
                if(exp2.tipo == ENTERO)
                    llamada_lista = escribirFuncionLista("divisionI");
                else
                    llamada_lista = escribirFuncionLista("divisionF");
                
                llamada_lista = escribirParamListaID(llamada_lista, exp1.lexema, 0);
                llamada_lista = escribirParamListaS(llamada_lista, exp2.lexema, 1);
                fprintf(f, "%s = ", var);
                escribir(llamada_lista, SALIDA_ACTUAL);
                break;
            case 1: // % -> borrar lista a partir de una posición
                llamada_lista = escribirFuncionLista("borrar");
                llamada_lista = escribirParamListaID(llamada_lista, exp1.lexema, 0);
                llamada_lista = escribirParamListaS(llamada_lista, exp2.lexema, 1);
                fprintf(f, "%s = ", var);
                escribir(llamada_lista, SALIDA_ACTUAL);
                break;
            case 2: // ** -> concatenar
                llamada_lista = escribirFuncionLista("concatenar");
                llamada_lista = escribirParamListaID(llamada_lista, exp1.lexema, 0);
                llamada_lista = escribirParamListaID(llamada_lista, exp2.lexema, 1);
                fprintf(f, "%s = ", var);
                escribir(llamada_lista, SALIDA_ACTUAL);
                break;   
        }
    }
    else{
        switch (op.atrib){
            case 0:
                operador = "/";
                break;
            case 1:
                operador = "%";
                break;
        }
    
        if(exp1.tipo == REAL || exp2.tipo == REAL)
            cTipo(REAL, f);
        else
            cTipo(exp1.tipo, f);
    
    //if(exp1.tipo == ENTERO || exp1.tipo!= REAL){
        fprintf(f, "%s = %s %s %s;\n", var, exp1.lexema, operador, exp2.lexema);
    }
    
    return var;
}


/**
  * ARROBA // @ -> elemento posicion
  */
char *codigoArroba(atributos exp1, atributos exp2, FILE *f){
    char *var = generarVariable();
    cTipo(exp1.tipo_lista, f);
    
    if(exp1.tipo_lista == ENTERO)
        llamada_lista = escribirFuncionLista("getI");
    else if(exp1.tipo_lista == REAL)
        llamada_lista = escribirFuncionLista("getF");
    else
        llamada_lista = escribirFuncionLista("getC");
                
    llamada_lista = escribirParamListaID(llamada_lista, exp1.lexema, 0);
    llamada_lista = escribirParamListaS(llamada_lista, exp2.lexema, 1);
    fprintf(f, "%s = ", var);
    escribir(llamada_lista, SALIDA_ACTUAL);
    
    return var;
}

/**
  * SEURO // -- -> borrar elemento en una posición
  */
char *codigoSeuro(atributos exp1, atributos exp2, FILE *f){
    char *var = generarVariable();
    cTipo(LISTA, f);

    llamada_lista = escribirFuncionLista("eliminar");
    llamada_lista = escribirParamListaID(llamada_lista, exp1.lexema, 0);
    llamada_lista = escribirParamListaS(llamada_lista, exp2.lexema, 1);
    fprintf(f, "%s = ", var);
    escribir(llamada_lista, SALIDA_ACTUAL);
    return var;
}

/**
  * TERNARIO -> añadir un elemento en una determinada posición
  */
char *codigoOpSumArroba(atributos exp1, atributos exp2, atributos exp3, FILE *f){
    char *var = generarVariable();
    cTipo(LISTA, f);

    if(exp1.tipo_lista == ENTERO)
        llamada_lista = escribirFuncionLista("setI");
    else if(exp1.tipo_lista == REAL)
        llamada_lista = escribirFuncionLista("setF");
    else
        llamada_lista = escribirFuncionLista("setC");
                
    llamada_lista = escribirParamListaID(llamada_lista, exp1.lexema, 0);
    llamada_lista = escribirParamListaS(llamada_lista, exp2.lexema, 0);
    llamada_lista = escribirParamListaS(llamada_lista, exp3.lexema, 1);
    fprintf(f, "%s = ", var);
    escribir(llamada_lista, SALIDA_ACTUAL);
    
    return var;
}

  
/**
  * Condicional IF
  */
void codigoIF(int parte, char *varControl, FILE *f) {
    tabular(f);
    
    switch (parte){
        case 0:
            TOPE_PILA++;
            TE[TOPE_PILA].EtiquetaElse = generarEtiqueta("etiqueta_sino_if");
            TE[TOPE_PILA].EtiquetaSalida = generarEtiqueta("etiqueta_salida_if");
            TE[TOPE_PILA].NombreVarControl = varControl;
            fprintf(f, "if (!%s) goto %s;\n", TE[TOPE_PILA].NombreVarControl, TE[TOPE_PILA].EtiquetaElse);
            break;
        case 1:
            fprintf(f, "%s:;\n", TE[TOPE_PILA].EtiquetaElse);
            TOPE_PILA--;
            break;
        case 2:
            fprintf(f, "goto %s;\n", TE[TOPE_PILA].EtiquetaSalida);
            tabular(f);
            fprintf(f, "%s:;\n", TE[TOPE_PILA].EtiquetaElse);
            break;
        case 3:
            fprintf(f, "%s:;\n", TE[TOPE_PILA].EtiquetaSalida);
            TOPE_PILA--;
            break;
    }
}

/**
  * Bucle WHILE
  */
void codigoWHILE(int parte, char *varControl, FILE *f){
    tabular(f);
    switch (parte){
        case 0:
            TOPE_PILA++;
            TE[TOPE_PILA].EtiquetaEntrada = generarEtiqueta("etiqueta_entrada_while");
            TE[TOPE_PILA].EtiquetaSalida  = generarEtiqueta("etiqueta_salida_while");
            fprintf(f, "%s:;\n", TE[TOPE_PILA].EtiquetaEntrada);
            break;
        case 1:
            TE[TOPE_PILA].NombreVarControl = varControl;
            fprintf(f, "if (!%s) goto %s;\n", TE[TOPE_PILA].NombreVarControl, TE[TOPE_PILA].EtiquetaSalida);
        break;
        case 2:
            fprintf(f, "goto %s;\n", TE[TOPE_PILA].EtiquetaEntrada);
            tabular(f);
            fprintf(f, "%s:;\n", TE[TOPE_PILA].EtiquetaSalida);
            TOPE_PILA--;
            break;
    }
}
/**
  * Bucle REPEAT-UNTIL
  */
void codigoRepeatUntil(int parte, char *varControl, FILE *f){
    tabular(f);
    switch (parte){
        case 0:
            TOPE_PILA++;
            TE[TOPE_PILA].EtiquetaEntrada = generarEtiqueta("etiqueta_entrada_RepeatUntil");
            TE[TOPE_PILA].EtiquetaSalida  = generarEtiqueta("etiqueta_salida_RepeatUntil");
            fprintf(f, "%s:;\n", TE[TOPE_PILA].EtiquetaEntrada);
            
            /*TOPE++;
            TS[TOPE].EtiquetaEntrada = generarEtiqueta("etiqueta_entrada_RepeatUntil");
            TS[TOPE].EtiquetaSalida  = generarEtiqueta("etiqueta_salida_RepeatUntil");
            fprintf(f, "%s:;\n", TS[TOPE].EtiquetaEntrada);*/
            break;
        case 1:
            TE[TOPE_PILA].NombreVarControl = varControl;
            fprintf(f, "if (!%s) goto %s;\n", TE[TOPE_PILA].NombreVarControl, TE[TOPE_PILA].EtiquetaEntrada);
            TOPE_PILA--;
            /*TS[TOPE].NombreVarControl = varControl;
            fprintf(f, "if (!%s) goto %s;\n", TS[TOPE].NombreVarControl, TS[TOPE].EtiquetaEntrada);
            TOPE--;*/
            break;
    }
}

/*----------------------SUBPROGRAMAS--------------------------*/
void codigoProcedimiento(int parte, char* nombre, int tipo){
    switch (parte){
        case 0:
            if (EN_PROCEDIMIENTO == 0){
                NTabsMain = NTabs;
                NTabs = NTabsProg;
            }
            
            EN_PROCEDIMIENTO++;
            SALIDA_ACTUAL = SALIDA_CODIGO_SUBPROGRAMAS;
            cTipo(tipo, SALIDA_ACTUAL);
            fprintf(SALIDA_ACTUAL, "%s (", nombre);
            TABULAR = 0;
            break;
        case 1:
            fprintf(SALIDA_ACTUAL, ")\n");
            TABULAR = 1;
            break;
        case 2:
            EN_PROCEDIMIENTO--;
            if (EN_PROCEDIMIENTO == 0){
                SALIDA_ACTUAL = SALIDA_CODIGO_PRINCIPAL;
                NTabsProg = NTabs;
                NTabs = NTabsMain;
            }
            break;
    }
}

void codigoParametro(atributos param, char* coma){
    fprintf(SALIDA_ACTUAL, "%s", coma);
    cTipo(param.tipo, SALIDA_ACTUAL);
    fprintf(SALIDA_ACTUAL, "%s", param.lexema);
}


/*----------------------LECTURA/ESCRITURA--------------------------*/
void escribirTipo(int tipo, FILE *f){
    switch(tipo){
        case ENTERO:
            fprintf(f, "%%d");
            break;
        case REAL:
            fprintf(f, "%%f");
            break;
        case CARACTER:
            fprintf(f, "%%c");
            break;
        case BOOLEANO:
            fprintf(f, "%%d");
            break;
        case STRING:
            fprintf(f, "%%s");
            break;
    }
}
char* codigoSentenciaEntradaTipo(atributos id, FILE *f, char* identificadores){
    escribirTipo(id.tipo, f);
    
    char *lista =(char*) malloc(sizeof(char) * 256);
    
    if(strlen(identificadores)!=0){
        strcpy(lista, identificadores);
    } 
    
    strcat(lista, ", &");
    strcat(lista, id.lexema);
    
    return lista;
}

void codigoSentenciaEntrada(char * identificadores, FILE *f){
    fprintf(f, "\"");
    fprintf(f, "%s", identificadores);
    fprintf(f, ");\n");
}


void escribirTipoSalida(int tipo, FILE *f){
    if(salida == NULL)
        salida = (char*) malloc(sizeof(char) * 256);     
    
    switch(tipo){
        case ENTERO:
            strcat(salida, "%d");
            break;
        case REAL:
            strcat(salida, "%f");
            break;
        case CARACTER:
            strcat(salida, "%c");
            break;
        case BOOLEANO:
            strcat(salida, "%d");
            break;
        case STRING:
            strcat(salida, "%s");
            break;
    }
}

char* codigoSentenciaSalidaTipo(atributos expr, FILE *f, char* expresiones){
    escribirTipoSalida(expr.tipo, f);
    
    char *lista =(char*) malloc(sizeof(char) * 256);
    
    if(strlen(expresiones)!=0){
        strcpy(lista, expresiones);
    } 
    
    strcat(lista, ", ");
    strcat(lista, expr.lexema);
    
    return lista;
}

void codigoSentenciaSalida(char * expresiones, FILE *f){
    tabular(f);
    fprintf(f, "printf(\"");
    fprintf(f, "%s", salida);
    fprintf(f, "\"");
    fprintf(f, "%s", expresiones);
    fprintf(f, ");\n");
}



/*----------------------LISTA--------------------------*/
char* escribirFuncionLista(char * funcion){
    char * aux = strdup(funcion);
    strcat(aux, "(");
    
    return aux;
}

char* escribirParamListaS(char * funcion, char * parametro, int fin){
    char * aux = strdup(funcion);
    strcat(aux, parametro);
    
    if(fin)
        strcat(aux, ");\n");
    else
        strcat(aux, ",");
        
    return aux;
}

char* escribirParamListaI(char * funcion, int parametro, int fin){
    char * aux = strdup(funcion);
    char entero[1];
    sprintf(entero, "%d", parametro);
    strcat(aux, entero);
    
    if(fin)
        strcat(aux, ");\n");
    else
        strcat(aux, ",");
        
    return aux;
}

char* escribirParamListaF(char * funcion, float parametro, int fin){
    char * aux = strdup(funcion);
    char real[1];
    sprintf(real, "%f", parametro);
    strcat(aux, real);
    
    if(fin)
        strcat(aux, ");\n");
    else
        strcat(aux, ",");
        
    return aux;
}

char* escribirParamListaID(char * funcion, char * id, int fin){
    char * ident = strdup("&");
	strcat(ident, id);
	
	return escribirParamListaS(funcion, ident, fin);
}

void escribir(char * texto, FILE *f){
   fprintf(f, "%s", texto); 
}



