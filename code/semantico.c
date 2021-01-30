#include "semantico.h"

entradaTS TS[MAX_ENTRADAS];
long int TOPE = 0;
int declarVar = 0;
int declarPar = 0;
int subProg = 0;
tDato tipoGlobal = DESC;
tDato tipoGlobalLista = DESC;
int numParam = 0;
int funcionActual = -1;

int ibloque = 1; /////


/**
 * Indica que si siendo listas las dos entradaTS tienen el mismo tamaño.
 */
int igualTam(atributos e1, atributos e2) {
	return (e1.tam==e2.tam);
}

/**
 * Almacena en la variable global tipo el tipo de la variable
 */
int asignaTipoGlobal(atributos elem) {
	tipoGlobal = elem.tipo;
}

/**
 * Almacena en la variable global tipo de lista el tipo de la variable lista
 */
int asignaTipoGlobalLista(atributos elem) {
	tipoGlobalLista = elem.tipo_lista;
}

/*------------  Funciones de manejo de la Tabla de Simbolos  ------------------------*/
/**
  * Inserta una entrada a la tabla de símbolos 
  **/
int tsAddEntrada(entradaTS ent) {
	if(TOPE < MAX_ENTRADAS) {
		TS[TOPE].entrada=ent.entrada;
		TS[TOPE].lexema=ent.lexema;
		TS[TOPE].tipoDato=ent.tipoDato;
		TS[TOPE].tipoDatoLista=ent.tipoDatoLista;
		TS[TOPE].nParam=ent.nParam;
		TOPE++;
		//imprimeTS();
		return 1;
	} else {
		printf("ERROR: PILA LLENA");
		return 0;
	}
};

/**
  * Elimina el elemento TOPE de tabla de símbolos 
  **/
int tsDelEntrada() {
	// Si la pila no está vacía. 
	if(TOPE > 0){
		TOPE--;
		return 1;
	}else{
		printf("ERROR: PILA VACIA");
		return 0;
	}
};

/**
 * Elimina de la tabla de símbolos todas las entradas hasta la última marca de inicio de bloque incluida esta
 **/
void tsVaciarEntradas() {
	while (TS[TOPE-1].entrada != MARCA && TOPE > 0) { 
		TOPE--; 
	} 
	if (TS[TOPE-1].entrada == MARCA) { 
		TOPE--; 
	}
}

/**
  * Busca un identificador en la TS para comprobar que ha sido declarado
  **/
int tsBuscarIdent(atributos elem) {

	int i = TOPE - 1;
	int encontrado = 0;
	
	while (i > 0 && !encontrado){ // && TS[i].entrada != MARCA) {
		if (TS[i].entrada == VARIABLE && strcmp(elem.lexema, TS[i].lexema) == 0) {
			encontrado = 1;
		} else{
			i--;
		}
	}
	
	if(!encontrado) {
		//printf("(Linea %d) Error semantico: Identificador no declarado, %s\n", linea_actual, elem.lexema);
		return -1;
	} else {
		return i;
	}
}

/**
  * Busca una entrada dado su nombre :
  *     Si la encuentra devuelve el indice donde se encuentra la entrada
  *     Si no la encuentra devuelve -1
  **/
int tsBuscarFuncion(atributos elem) {
	int i = TOPE - 1;
	int encontrado = 0;
	
	while (i > 0 && !encontrado){// && TS[i].entrada != MARCA) {
		if (TS[i].entrada == FUNCION && strcmp(elem.lexema, TS[i].lexema) == 0) {
			encontrado = 1;
		} else{
			i--;
		}
	}
	
	if(!encontrado) {
		printf("(Linea %d) Error semantico: Funcion no declarada, %s\n", linea_actual, elem.lexema);
		return -1;
	} else {
		return i;
	}
}

/**
 * Inserta un nuevo identificador en la tabla de símbolos
 **/
void tsInsertaIdent(atributos elem){
	//Se comprueba que no hemos llegado al tope de la pila, que el identificador no esta ya 
	//declarado previamente y que estamos declarando variables
	int j = TOPE-1;
	int encontrado = 0;
   
	if(j >= 0 && declarVar == 1) {
		//Se busca la marca de comienzo de bloue
		while((TS[j].entrada != MARCA) && (j >= 0) && !encontrado) {
			if(strcmp(TS[j].lexema, elem.lexema) != 0) {
				j--;
			}
			else {
				encontrado = 1;
				printf("(Linea %d) Error semantico: Identificador duplicado, %s\n", linea_actual, elem.lexema);
	 		}
		}

		//Si no hemos encontrado el identificador lo insertamos en la TS
		if(!encontrado) {
			entradaTS nuevaEntrada;
			nuevaEntrada.entrada = VARIABLE;
			nuevaEntrada.lexema = elem.lexema;
			//Asignamos el tipo desde la variable global
			nuevaEntrada.tipoDato = tipoGlobal;
			nuevaEntrada.tipoDatoLista = tipoGlobalLista;
			nuevaEntrada.nParam = 0;
			tsAddEntrada(nuevaEntrada);
		}
	}
};

/**
 * Inserta una marca de comienzo de un bloque
 **/
void tsInsertaMarca(){
	entradaTS entradaInicioBloque;

	entradaInicioBloque.entrada = MARCA;
	entradaInicioBloque.lexema = "{";
	entradaInicioBloque.tipoDato = DESC;
	entradaInicioBloque.tipoDatoLista = DESC;
	entradaInicioBloque.nParam = 0;
	tsAddEntrada(entradaInicioBloque);
	
	//Se insertan los parametros formales de la funcion como variables locales a esta
	if(subProg == 1){
		int j = TOPE - 2, marca = 0, funcion = 0;

		while(j > 0 && TS[j].entrada == PAR_FORMAL){
			if(TS[j].entrada == PAR_FORMAL) {
				entradaTS nuevaEntrada;
				nuevaEntrada.entrada = VARIABLE;
				nuevaEntrada.lexema = TS[j].lexema;
				nuevaEntrada.tipoDato = TS[j].tipoDato;
				nuevaEntrada.tipoDatoLista = TS[j].tipoDatoLista;
				nuevaEntrada.nParam = TS[j].nParam;
				tsAddEntrada(nuevaEntrada);
			}
			j--;
		}
	}
	
}

/**
 * Inserta una entrada de subprograma en la tabla de simbolos
 **/
void tsInsertaSubprog(atributos elem) {
	entradaTS entradaSubProg;

	entradaSubProg.entrada = FUNCION;
	entradaSubProg.lexema = elem.lexema;
	entradaSubProg.nParam = 0;
	entradaSubProg.tipoDato = tipoGlobal;
	entradaSubProg.tipoDatoLista = tipoGlobalLista;
	//entradaSubProg.tipoDato = elem.tipo;
	//entradaSubProg.tipoDatoLista = elem.tipo_lista;

	funcionActual = TOPE;
	tsAddEntrada(entradaSubProg);
}

/**
 * Inserta una entrada de parámetro formal de un subprograma en la tabla de símbolos
 **/
void tsInsertaParamFormal(atributos elem) {
	int j = TOPE - 1, encontrado = 0;

	while((j != funcionActual)  && (!encontrado) ) {
		if(strcmp(TS[j].lexema, elem.lexema) != 0) {
			j--;
		}
		else {
			encontrado = 1;
			printf("(Linea %d) Error semantico: Parametro duplicado: %s\n", linea_actual, elem.lexema);
 		}
	}
	if(!encontrado) {
		entradaTS nuevaEntrada;
		nuevaEntrada.entrada = PAR_FORMAL;
		nuevaEntrada.lexema = elem.lexema;
		//Asignamos el tipo desde la variable global
		nuevaEntrada.tipoDato = tipoGlobal;
		nuevaEntrada.tipoDatoLista = tipoGlobalLista;
		nuevaEntrada.nParam = 0;
		tsAddEntrada(nuevaEntrada);
	}
	
}

/**
 * Actualiza el número de parámetros de la función que estamos declarando
 **/
void tsActualizaNparam(atributos elem) {
	TS[funcionActual].nParam = numParam;
}




/*------------  Funciones para las comprobaciones semánticas ------------------------*/

/**
 * Busca la entrada de tipo FUNCION más próxima desde el tope de la tabla de símbolos
 * y devuelve el índice
 **/
int tsGetFuncionProxima() {
	int i = TOPE - 1;
	int encontrado = 0;
	
	while (i > 0) {
		if (TS[i].entrada == MARCA) {
			break;
		} else {
			i--;
		}
	}
	
	while (i > 0 && !encontrado) {
		if (TS[i].entrada == FUNCION) {
			encontrado = 1;
		} else {
			i--;
		}
	}
	
	if(!encontrado) {
		return -1;
	} else {
		return i;
	}
}

/**
 * Comprobacion semántica de la sentencia de retorno.
 * Comprueba que el tipo de expresión es el mismo que el de la función
 * donde se encuentra
 **/
void tsCompruebaRetorno(atributos expresion, atributos* retorno) {

	int indice = tsGetFuncionProxima();
	
	if (indice > -1) {
		if (expresion.tipo != TS[indice].tipoDato) {
			printf("(Linea %d) Error semantico: La expresion de la sentencia retorno no es del tipo que devuelve la funcion.\n",linea_actual);
			return;
		}
		
		retorno->tipo = expresion.tipo;
	}
	else {
		printf("(Linea %d) Error semantico: La sentencia retorno no se encuentra declarada dentro de ninguna funcion.\n", linea_actual);
		return;
	}
}

/**
 * Busca el identificador en la tabla de símbolos y lo rellena en el atributo de salida
 */
void tsGetIdent(atributos identificador, atributos* res) {
	int indice = tsBuscarIdent (identificador);
	if(indice==-1) {
		printf("(Linea %d) Error semantico: No se ha encontrado el identificador %s.\n", linea_actual, identificador.lexema);
	}
	else {
		res->lexema = strdup(TS[indice].lexema);
		res->tipo = TS[indice].tipoDato;
		res->tipo_lista = TS[indice].tipoDatoLista;
	}
	
}







/**
 * Comprobación semántica de los operadores unarios + y -
 */
void tsOpSumResUnario(atributos operador, atributos o, atributos* res) {
	if ( o.tipo != REAL && o.tipo != ENTERO ) {
		printf("(Linea %d) Error semantico: El operador + o - esperaba una expresion de tipo real o entera.\n", linea_actual);
		//hay_error=1;
	}
	res->tipo = o.tipo;
}

/**
 * Comprobación semántica de los operadores !, ? y #
 */
void tsOpUna(atributos operador, atributos o, atributos* res){
	if(strcmp(operador.lexema, "!") == 0){
		if( o.tipo!=BOOLEANO )
			printf("(Linea %d) Error semantico: El operador ! esperaba una expresion de tipo lógica.\n", linea_actual);

		res->tipo = BOOLEANO;
	}
	else{
		if( o.tipo!=LISTA )
			printf("(Linea %d) Error semantico: El operador ? o # esperaba una lista.\n", linea_actual);

		if(strcmp(operador.lexema, "?") == 0)
			res->tipo = o.tipo_lista;
		else
		    res->tipo = ENTERO;
	}
}

/**
 * Comprobación semántica del operador ||
 */

void tsOpOr(atributos o1, atributos o2, atributos* res){
    if(o1.tipo != BOOLEANO || o2.tipo!= BOOLEANO)
        printf("(Linea %d) Error semantico: El operador || esperaba dos booleanos.\n", linea_actual);

    res->tipo = BOOLEANO;
}


/**
 * Comprobación semántica del operador &&
 */

void tsOpAnd(atributos o1, atributos o2, atributos* res){
    if(o1.tipo != BOOLEANO || o2.tipo!= BOOLEANO)
        printf("(Linea %d) Error semantico: el operador && esperaba dos booleanos.\n", linea_actual);

    res->tipo = BOOLEANO;
}

/**
 * Comprobación semántica de los operadores ==, !=, <=, >=, < y >
 */

void tsOpRelacion(atributos o1, atributos o2, atributos* res){
 	if( (o1.tipo!=CARACTER || o2.tipo!=CARACTER) &&
 	    ( (o1.tipo!=ENTERO && o1.tipo!= REAL) || 
 	       (o2.tipo!=ENTERO && o2.tipo!= REAL) ) )
		printf("(Linea %d) Error semantico: el operador ==, !=, <=, >=, < o > esperaba una expresion de tipo real, entera o caracter.\n", linea_actual);

    res->tipo = BOOLEANO;
}


/**
 * Comprobación semántica de los operadores binario + y -
 */
 
void tsOpSumResBin(atributos o1, atributos o2, atributos* res){
    if( (( o1.tipo != o2.tipo ) || (o1.tipo != ENTERO && o1.tipo!= REAL))
        && (o1.tipo!=LISTA || (o2.tipo!=o1.tipo_lista))
        && (o2.tipo!=LISTA || (o1.tipo!=o2.tipo_lista)))

        printf("(Linea %d) Error semantico: El operador + o - esperaba una expresion de tipo real, entera o una lista y un valor del mismo tipo que la lista.\n", linea_actual);

    if(o1.tipo == LISTA){
        res->tipo = LISTA;
        res->tipo_lista = o2.tipo;
    }
    else if (o2.tipo == LISTA){
        res->tipo = LISTA;
        res->tipo_lista = o1.tipo;
    }
    else
        res->tipo = o1.tipo;
}


/**
 * Comprobación semántica del operador *
 */

void tsOpMult(atributos o1, atributos o2, atributos* res){
    if( (( o1.tipo != o2.tipo ) || (o1.tipo != ENTERO && o1.tipo!= REAL))
        && (o1.tipo!=LISTA || (o2.tipo!=o1.tipo_lista))
        && (o2.tipo!=LISTA || (o1.tipo!=o2.tipo_lista)))
        printf("(Linea %d) Error semantico: El operador * esperaba una expresion de tipo real, entera o una lista y un valor del mismo tipo que la lista.\n", linea_actual);

    if(o1.tipo == LISTA){
        res->tipo = LISTA;
        res->tipo_lista = o2.tipo;
    }
    else if (o2.tipo == LISTA){
        res->tipo = LISTA;
        res->tipo_lista = o1.tipo;
    }
    else
        res->tipo = o1.tipo;
}


/**
 * Comprobación semántica del operador /, % y **
 */

void tsOpBinario(atributos o1, atributos operador, atributos o2, atributos *res){
	/*
	if( o1.tipo!=LISTA || o2.tipo!=ENTERO )
		printf("Error en el operador SEURO");
	*/
	if( strcmp(operador.lexema, "**") != 0 ){
		if( ((o1.tipo!=ENTERO && o1.tipo!=REAL) || (o2.tipo!=ENTERO && o2.tipo!=REAL)) 
		    && (o1.tipo!=LISTA || o2.tipo!=ENTERO) )
			printf("(Linea %d) Error semantico: El operador / o %c esperaba una dos enteros/reales o una lista y una posicion.\n", linea_actual, '%');
	}
	else{
		if( o1.tipo!=LISTA || o2.tipo!=LISTA || o1.tipo_lista != o2.tipo_lista ){
			printf("(Linea %d) Error semantico: El operador ** esperaba una expresión de listas del mismo tipo.\n", linea_actual);
		}
	}

    if(o1.tipo == REAL || o2.tipo == REAL)
        res->tipo = REAL;
    else{    
        res->tipo = o1.tipo;
        res->tipo_lista = o1.tipo_lista;
    }
}


/**
 * Comprobación semántica del operador ternario ++ y @
 */

void tsOpSumArroba(atributos o1, atributos o2, atributos o3, atributos *res){
    if( o1.tipo!=LISTA || o2.tipo!=o1.tipo_lista || o3.tipo!=ENTERO)
        printf("(Linea %d) Error semantico: Se esperaba la expresión Lista++Elemento(del mismo tipo que la lista)@Posicion(entero).\n", linea_actual);

    res->tipo = LISTA;
    res->tipo_lista = o2.tipo;
}




/**
 * Comprobacion semantica de la llamada a subprograma
 */
void tsLlamadaFuncion(atributos identificador, atributos* res) {
	int indice = tsBuscarFuncion (identificador);
	if(indice==-1) {
		funcionActual = -1;
		//printf("(Linea %d) Error semantico: No se ha encontrado el identificador %s.\n", linea_actual, identificador.lexema);
	}
	else {
		funcionActual = indice;
		/*if (numParam != TS[indice].nParam) { 
			printf("(Linea %d) Error semantico: numero de parametros incorrecto, %d, %d\n", linea_actual, numParam, TS[indice].nParam); 
		} else {*/
			res->lexema = strdup(TS[indice].lexema);
			res->tipo = TS[indice].tipoDato;
			if(res->tipo==LISTA)
				res->tipo_lista = TS[indice].tipoDatoLista;
		//}
	}
}

/**
 * Comprobacion semantica de cada parametro en una llamada a una funcion
 */
void tsCompruebaParametro(atributos parametro, int parametroAComprobar) {
	//printf("%d\n", parametroAComprobar);
	int posicionParametro = (funcionActual + TS[funcionActual].nParam) - (parametroAComprobar - 1);
	//printf("%d\n", posicionParametro);
	int errorRealmente = TS[funcionActual].nParam - parametroAComprobar + 1;
	//printf("voy a comprobar el parametro %d\n", parametroAComprobar);
	//imprimeAtributo(parametro, "a Comprobar");
	//imprimeEntrada(posicionParametro);
	if (parametro.tipo != TS[posicionParametro].tipoDato) {
		//imprimeTS();
		printf("(Linea %d) Error semantico: El tipo del parametro esperado en el parametro %d no es el correcto,%d \n", linea_actual, errorRealmente, posicionParametro);
		return;
	}
	
}

void tsNParam(int numParam){
	if( TS[funcionActual].nParam != numParam){
		printf("(Linea %d) Error semantico: El numero de parametros no es correcto\n", linea_actual);
	}
}


//----------------------  Funciones de Impresion --------------------------------------

/**
 * Imprime como una cadena de caracteres una entrada de la tabla de simbolos dada
 */
void imprimeEntrada(int indice) {
	entradaTS e = TS[indice];
	printf("\n\nTipo Entrada: %d\nLexema: %s\nTipo Dato: %d\nNum Parametros: %d\ntam: %d\n", 
		e.entrada, e.lexema, e.tipoDato, e.nParam, e.tam);
}

/**
  * Imprime como cadena el tipo de entrada dado
**/
void imprimeTipoEntrada(tEntrada tipo);

/**
  * Imprime como cadena el tipo de dato dado
**/ 
void imprimeTipoDato(tDato tipo);

/**
 * Imprime por pantalla la tabla de simbolos a continuacion del mensaje dado
 */
void imprimeTS() { 
	int j = 0; 
	char *t, *e, *s;

	printf("--------------------------------\n"); 
	while(j <= TOPE-1) {
		if(TS[j].entrada == 0) { e = "MARCA"; }
		if(TS[j].entrada == 1) { e = "FUNCION"; }
		if(TS[j].entrada == 2) { e = "VARIABLE"; }
		if(TS[j].entrada == 3) { e = "PAR_FORMAL"; }

		if(TS[j].tipoDato == 0) { t = "NO_ASIG"; }
		if(TS[j].tipoDato == 1) { t = "ENTERO"; }
		if(TS[j].tipoDato == 2) { t = "REAL"; }
		if(TS[j].tipoDato == 3) { t = "CARACTER"; }
		if(TS[j].tipoDato == 4) { t = "BOOLEANO"; }
		if(TS[j].tipoDato == 5) { t = "STRING"; }
		s = "NO_ASIG";
		if(TS[j].tipoDato == 6) { 
		    t = "LISTA"; 
		    if(TS[j].tipoDatoLista == 1) { s = "ENTERO"; }
		    if(TS[j].tipoDatoLista == 2) { s = "REAL"; }
		    if(TS[j].tipoDatoLista == 3) { s = "CARACTER"; }
		    if(TS[j].tipoDatoLista == 4) { s = "BOOLEANO"; }
		    if(TS[j].tipoDatoLista == 5) { s = "STRING"; }   
		}
		if(TS[j].tipoDato == 7) { t = "DESC"; }
		printf("----ELEMENTO %d-----------------\n", j); 
		printf("-Entrada: %-12s", e); 
		printf("-Lexema: %-12s", TS[j].lexema); 
		printf("-tipoDato: %-10s", t); 
		printf("-tipoDatoLista: %-10s", s); 
		printf("-nParam: %-4d\n\n", TS[j].nParam); 
		//printf("-tam: %-4d", TS[j].tam); 
		j++; 
	} 
	printf("--------------------------------\n"); 
}

/**
 * Imprime por pantalla un atributo dado
 */
void imprimeAtributo(atributos elem, char *msj){
	char *t;

	if(elem.tipo == 0) { t = "NO_ASIG"; }
	if(elem.tipo == 1) { t = "ENTERO"; }
	if(elem.tipo == 2) { t = "REAL"; }
	if(elem.tipo == 3) { t = "CARACTER"; }
	if(elem.tipo == 4) { t = "BOOLEANO"; }
	if(elem.tipo == 5) { t = "STRING"; }
	if(elem.tipo == 6) { t = "MATRIZ"; }
	if(elem.tipo == 7) { t = "DESC"; }
	printf("------%s-------------------------\n", msj);
	printf("-Atributos: %-4d", elem.atrib); 
	printf("-Lexema: %-12s", elem.lexema); 
	printf("-tipoDato: %-10s", t); 
	printf("-tam: %-4d", elem.tam);
	printf("-------------------------------\n");
}

