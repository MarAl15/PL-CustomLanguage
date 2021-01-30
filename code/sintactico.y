%{
/*********************************************************
**
** 
** 
**
********************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include "semantico.h"
#include "generacion.h"
//YYSTYPE DUMMY;

/** La siguiente declaracion permite que ’yyerror’ se pueda invocar desde el
*** fuente de lex
**/

//void yyerror(char * msg);
void yyerror(const char * msg);



/** La siguiente variable se usará para conocer el numero de la línea
*** que se esta leyendo en cada momento. También es posible usar la variable
*** ’yylineno’ que también nos muestra la línea actual. Para ello es necesario
*** invocar a flex con la opción ’-l’ (compatibilidad con lex).
**/

//int linea_actual = 1 ;
%}

/** Para uso de mensajes de error sintáctico con BISON.
*** La siguiente declaración provoca que ’bison’, ante un error sintáctico,
*** visualice mensajes de error con indicación de los tokens que se esperaban
*** en el lugar en el que produjo el error (SÓLO FUNCIONA CON BISON>=2.1).
*** Para Bison<2.1 es mediante
*** 
*** #define YYERROR_VERBOSE
***
*** En caso de usar mensajes de error mediante ’mes’ y ’mes2’ (ver apéndice D)
*** nada de lo anterior debe tenerse en cuenta.
**/

%error-verbose

/** A continuación declaramos los nombres simbólicos de los tokens.
*** byacc se encarga de asociar a cada uno un código
**/


/* Declaración de los símbolos */
%left OTRA
%left OPOR
%left OPAND
%left OPIGUALDAD
%left OPRELACION
%left SUMRES
%left OPMULT SEURO
%left OPBINARIO
%left SUM ARROBA
 
%right OPUNARIO


%token INI_VAR 		
%token FIN_VAR 		
%token FIN 			
%token INI_LLAVE 		
%token FIN_LLAVE 		
%token PROGRAMA 		
%token TIPO_VARIABLE 	
%token SI 				
%token SINO 			
%token MIENTRAS 		
%token REPETIR 		
%token HASTA 			
%token ENTRADA 		
%token SALIDA 						
%token DOLAR
%token OPIGUALDAD			 		
%token ID 				
%token CONST_REAL 		
%token CONST_BOOL 		
%token CONST_ENTERA 	
%token CONST_CARACTER 	
%token CADENA			
%token LIST		
%token LL				
%token MM				
%token INI_PARENT 		
%token FIN_PARENT 		
%token COMA						
%token DEVOLVER					
%token INI_LIST		
%token FIN_LIST		


%%


/* Reglas gramaticales + Acciones */
programa : {
                codigoAbrirDescriptores();
                codigoCabeceraMain(SALIDA_ACTUAL);
                globales = 1;
            }
            PROGRAMA bloque
            {
                codigoCerrarDescriptores();
            };

bloque : INI_LLAVE 
		{
		    if(!globales)
		        abrirBloque(SALIDA_ACTUAL);
		        
		    //if(ibloque==1){
		        
			    tsInsertaMarca();
			//}
		}
		declar_de_variables_locales
		declar_de_subprogs 
		sentencias 
		FIN_LLAVE
		{
		    //if(ibloque==1){
		        //imprimeTS();
			    tsVaciarEntradas();
			//}
			
			cerrarBloque(SALIDA_ACTUAL);
		};

declar_de_subprogs : declar_de_subprogs declar_subprog
					| ;
					
declar_subprog : cabecera_subprog 
				{
					subProg=1;
				}
				bloque
				{
					subProg=0;
					codigoProcedimiento(2, NULL, 0);
				};

declar_de_variables_locales : INI_VAR 
								{
									declarVar=1;
								}
								variables_locales FIN_VAR
								{
									declarVar=0;
									codigoDeclaracionVariables(SALIDA_ACTUAL, globales);
									globales = 0;									
								}
								| ;
								
variables_locales : variables_locales cuerpo_declar_variables
					| cuerpo_declar_variables;
					
cuerpo_declar_variables : tipo 
						{
							asignaTipoGlobal($1);
							asignaTipoGlobalLista($1);
							//tipo = $1.tipo;
						}
						lista_de_identificadores FIN
						 | error
						 {
						    printf("(Linea %d) Error semantico: Tipo de variable no existente.\n", linea_actual);
						 };

tipo : TIPO_VARIABLE
		{
			$$.tipo = $1.tipo;
		}
		| lista_tipo
		{
			$$.tipo = $1.tipo;
            $$.tipo_lista = $1.tipo_lista;
            
		};

lista_tipo : LIST TIPO_VARIABLE 
                {
                	//declarVar=1
                    $$.tipo = LISTA;
                    $$.tipo_lista = $2.tipo;              
                };

lista_de_identificadores : ID 
							{
								if(declarVar==1){
									tsInsertaIdent($1);
									
									if(tipoGlobal == LISTA){
									    llamada_lista = escribirFuncionLista("lista_ini");
									    llamada_lista = escribirParamListaID(llamada_lista, $1.lexema, 0);
									    llamada_lista = escribirParamListaI(llamada_lista, $1.tipo, 1);
									}
								}
								else{
									tsGetIdent($1, &$$);
									$1.tipo = $$.tipo;
									if(ENTRADA){
									    $$.lexema = codigoSentenciaEntradaTipo($1, SALIDA_ACTUAL, ""); 
									}
								}								
							}
							| lista_de_identificadores COMA ID 
							{
								if(declarVar==1){
									tsInsertaIdent($3);
								}
								else{
									tsGetIdent($3, &$$);
									$3.tipo = $$.tipo;
									
									if(ENTRADA){
									    $$.lexema = codigoSentenciaEntradaTipo($3, SALIDA_ACTUAL, $1.lexema);
									}
								}
							}
							| lista_de_identificadores error ID 
							{
								if(declarVar==1){
									$1.tam=0;
									tsInsertaIdent($3);
								}
								else{
									tsGetIdent($1, &$$);
								}
							};
							
cabecera_subprog : tipo ID 
					{
						declarPar = 1;
						asignaTipoGlobal($1);
						asignaTipoGlobalLista($1);
						tsInsertaSubprog($2);
						
						codigoProcedimiento(0, $2.lexema, $1.tipo);
					}
					INI_PARENT lista_variables FIN_PARENT
					{
						tsActualizaNparam($1); 
						numParam = 0; 
						declarPar = 0;
						
						//codigoParametrosFormales(SALIDA_ACTUAL);
						codigoProcedimiento(1, NULL,0);
					};
					
lista_variables : parametro
                    {
                        codigoParametro($1, "");
                    }
					| lista_variables COMA parametro
					{
                        codigoParametro($3, ", ");
                    }
					| lista_variables error tipo ID;

parametro : tipo ID
			{
				numParam++; 
				asignaTipoGlobal($1); 
				asignaTipoGlobalLista($1);
				tsInsertaParamFormal($2);
				
				$$.lexema = $2.lexema;
				//codigoParametro($2);
			};
sentencias : sentencias 
                //{abrirBloque(SALIDA_ACTUAL);}
                sentencia
                //{cerrarBloque(SALIDA_ACTUAL);}
				| 
				{abrirBloque(SALIDA_ACTUAL);}
				sentencia
				{cerrarBloque(SALIDA_ACTUAL);};
				
sentencia : {ibloque=0;} bloque {ibloque=1;}
			| sentencia_asignacion
			| sentencia_if
			| sentencia_while
			| sentencia_entrada
			| sentencia_salida
			| sentencia_return
			| sentencia_repeat_until
			| DOLAR expresion FIN
			{ //inicio
				if($2.tipo!=LISTA)
					printf("(Linea %d) Error semantico: La expresion no es una lista.\n", linea_actual);
					
			    llamada_lista = escribirFuncionLista("inicio");
				llamada_lista = escribirParamListaID(llamada_lista, $2.lexema, 1);
				tabular(SALIDA_ACTUAL);
				escribir(llamada_lista, SALIDA_ACTUAL);
  		
			}
			| expresion LL FIN
			{ // retroceder
				if($1.tipo!=LISTA)
					printf("(Linea %d) Error semantico: La expresion no es una lista.\n", linea_actual);
					
				llamada_lista = escribirFuncionLista("retroceder");
				llamada_lista = escribirParamListaID(llamada_lista, $1.lexema, 1);
				tabular(SALIDA_ACTUAL);
				escribir(llamada_lista, SALIDA_ACTUAL);			
			}
			| expresion MM FIN
			{ // avanzar
				if($1.tipo!=LISTA)
					printf("(Linea %d) Error semantico: La expresion no es una lista.\n", linea_actual);
					
			    llamada_lista = escribirFuncionLista("avanzar");
				llamada_lista = escribirParamListaID(llamada_lista, $1.lexema, 1);
				tabular(SALIDA_ACTUAL);
				escribir(llamada_lista, SALIDA_ACTUAL);	
			};
			
			
sentencia_asignacion : ID 
						{
							tsGetIdent($1, &$$);
						}
						OPIGUALDAD expresion FIN
						{
							if($1.tipo!=$3.tipo){
								printf("(Linea %d) Error semantico: Los tipos de la parte izquierda y derecha no coinciden, %d, %d.\n",linea_actual, $1.tipo, $3.tipo);
							}
							else if($1.tipo==LISTA){
							    if($1.tipo_lista!=$3.tipo_lista){
								    printf("(Linea %d) Error semantico: Los tipos de la parte izquierda y derecha no coinciden.\n",linea_actual);
							    }
							}
							codigoSentenciaAsignacion($1, $4, SALIDA_ACTUAL);
						};



sentencia_if : inicio
                {
					cerrarBloque(SALIDA_ACTUAL);
					codigoIF(1, NULL, SALIDA_ACTUAL); 
                }   
                | inicio SINO 
				{
				    codigoIF(2, NULL, SALIDA_ACTUAL);
				}
				sentencia
				{
					cerrarBloque(SALIDA_ACTUAL);
					codigoIF(3, NULL, SALIDA_ACTUAL);
                };
                
inicio: SI INI_PARENT expresion FIN_PARENT
				{
				    if($3.tipo != BOOLEANO)
						printf("(Linea %d) Error semantico: La expresion del IF no es de tipo logico.\n", linea_actual);
						
				    abrirBloque(SALIDA_ACTUAL);
				    codigoIF(0, $3.lexema, SALIDA_ACTUAL);
				}
				sentencia;

				
sentencia_while : MIENTRAS INI_PARENT 
		            {
                        abrirBloque(SALIDA_ACTUAL);
                        codigoWHILE(0, NULL, SALIDA_ACTUAL);
                        //imprimeTS();
                    }
                    expresion 
                    { codigoWHILE(1, $4.lexema, SALIDA_ACTUAL); }
                    FIN_PARENT sentencia
                    {
                        if( $4.tipo != BOOLEANO )
                            printf("(Linea %d) Error semantico: La expresion del WHILE no es de tipo logico.\n", linea_actual);

                        //imprimeTS();
                        codigoWHILE(2, NULL, SALIDA_ACTUAL);
                        cerrarBloque(SALIDA_ACTUAL);
                    };

sentencia_repeat_until : REPETIR 
                        {
                            codigoRepeatUntil(0, NULL, SALIDA_ACTUAL);
                        }
                        sentencia HASTA INI_PARENT
                        {
                            abrirBloque(SALIDA_ACTUAL);
                        }
                        expresion 
                        { 
                            if( $7.tipo != BOOLEANO )
								printf("(Linea %d) Error semantico: La expresion REPEAT-UNTIL no es de tipo logico.\n", linea_actual);
                            codigoRepeatUntil(1, $7.lexema, SALIDA_ACTUAL);
                            cerrarBloque(SALIDA_ACTUAL); 
                        }
                        FIN_PARENT FIN;
						

sentencia_entrada : ENTRADA MM 
                    {
                        tabular(SALIDA_ACTUAL);
                        fprintf(SALIDA_ACTUAL, "scanf(\"");
                        DENTRADA = 1;
                    }
                    lista_de_identificadores FIN
                    {
                        codigoSentenciaEntrada($4.lexema ,SALIDA_ACTUAL);
                    };

sentencia_salida : SALIDA LL 
                    {
                        //tabular(SALIDA_ACTUAL);
                        //fprintf(SALIDA_ACTUAL, "printf(\"");
                        salida = NULL; 
                    }
                    lista_expresiones_o_cadena FIN
                    {
                        if($4.tipo != LISTA)
                            codigoSentenciaSalida($4.lexema ,SALIDA_ACTUAL);
                        else{
                            tabular(SALIDA_ACTUAL);
                            llamada_lista = escribirFuncionLista("imprimir");
                            llamada_lista = escribirParamListaID(llamada_lista, $4.lexema, 1);
                            escribir(llamada_lista, SALIDA_ACTUAL);
                        }                            
                    };

lista_expresiones_o_cadena : lista_expresiones_o_cadena LL expresion_cadena
                                {
                                    $$.lexema = codigoSentenciaSalidaTipo($3, SALIDA_ACTUAL, $1.lexema);
                                }
								| expresion_cadena
								{
								    if($1.tipo != LISTA)
								        $$.lexema = codigoSentenciaSalidaTipo($1, SALIDA_ACTUAL, "");
								    else
								        $$.lexema = $1.lexema;
								};
								
expresion_cadena : expresion
                    {
                        $$.tipo = $1.tipo;
                        $$.lexema = $1.lexema;
                    }
					| CADENA
					{
					    $$.tipo = STRING;
					    $$.lexema = $1.lexema;
					};
					
sentencia_return : DEVOLVER expresion FIN
                    { 
                        tsCompruebaRetorno($2,&$$);
                        $$.tipo = $2.tipo;
                        $$.lexema = $2.lexema;
                        
                        tabular(SALIDA_ACTUAL);
                        fprintf(SALIDA_ACTUAL, "return %s;\n", $2.lexema);
                    };

expresion : INI_PARENT expresion FIN_PARENT
            {
                $$.tipo = $2.tipo;
                $$.lexema = $2.lexema;
            }   
            | OPUNARIO expresion
            {
                tsOpUna($1, $2, &$$);
                $$.lexema = codigoOpUna($1, $2, SALIDA_ACTUAL);
            }
			| SUMRES expresion %prec OPUNARIO 
			{ 
			     tsOpSumResUnario($1, $2, &$$);
			     $$.lexema = codigoSumResUnario($1, $2, SALIDA_ACTUAL); 
			}
			| expresion OPOR expresion
			{
			    tsOpOr($1, $3, &$$);
			    $$.lexema = codigoOpOr($1, $3, SALIDA_ACTUAL);
			}
			| expresion OPAND expresion
			{   tsOpAnd($1, $3, &$$);
			    $$.lexema = codigoOpAnd($1, $3, SALIDA_ACTUAL);
			}
			| expresion OPRELACION expresion
			{
			    tsOpRelacion($1,$3,&$$);
			    $$.lexema = codigoOpRelacion($1, $2, $3, SALIDA_ACTUAL);
			}
			| expresion SUMRES expresion
			{
			    tsOpSumResBin($1, $3, &$$);
			    $$.lexema = codigoOpSumResBinario($1, $2, $3, SALIDA_ACTUAL);
			}
			| expresion OPMULT expresion
			{ 
			    tsOpMult($1, $3, &$$);
			    $$.lexema = codigoOpMult($1, $3, SALIDA_ACTUAL);
			}
			| expresion OPBINARIO expresion
			{
			    tsOpBinario($1, $2, $3, &$$);
			    $$.lexema = codigoOpBinario($1, $2, $3, SALIDA_ACTUAL);
			}
			| expresion SEURO expresion
			{ 
			    tsOpBinario($1, $2, $3, &$$);
			    $$.tipo = LISTA;
			    $$.lexema = codigoSeuro($1, $3, SALIDA_ACTUAL);
			}
			| expresion ARROBA expresion %prec OTRA
			{ 
			    tsOpBinario($1, $2, $3, &$$);
			    $$.tipo = $1.tipo_lista;
			    $$.lexema = codigoArroba($1, $3, SALIDA_ACTUAL);
			}
			| expresion SUM expresion ARROBA expresion
			{
			    tsOpSumArroba($1, $3, $5, &$$);
			    $$.lexema = codigoOpSumArroba($1, $3, $5, SALIDA_ACTUAL);
			}
			| ID
			{
			    tsGetIdent($1, &$$);
			    $$.lexema = $1.lexema;
			}
			| constante
			{
			    $$.tipo = $1.tipo;
			    //$$.lexema = codigoCte($1);
			    $$.lexema = $1.lexema;
			}
			| lista
			{
			    $$.tipo = $1.tipo;
			    $$.tipo_lista = $1.tipo_lista;
			}
			| funcion
			{
			    $$.tipo = $1.tipo;
			    $$.lexema = $1.lexema;
			}
			| error;
 
			
constante : CONST_ENTERA
            {
                $$.tipo = ENTERO;
            }
			| CONST_REAL
			{
                $$.tipo = REAL;
            }
			| CONST_CARACTER
			{
                $$.tipo = CARACTER;
            }
			| CONST_BOOL
			{
                $$.tipo = BOOLEANO;
            };
			
funcion : ID 
            {
                tsLlamadaFuncion($1, &$$);
                
                //PARAMETROS_ACTUALES[0] = '\0';
            }
            INI_PARENT lista_expresiones FIN_PARENT
            {
            	tsNParam(numParam);
            	
                strcpy ($$.lexema,$1.lexema);
                strcat ($$.lexema,"(");
                strcat ($$.lexema,$4.lexema);
                strcat ($$.lexema,")");
            };
			
lista_expresiones : expresion COMA lista_expresiones
					{
						numParam++;
						tsCompruebaParametro($1, numParam);
						strcpy ($$.lexema,$1.lexema);
						strcat ($$.lexema,", ");
						strcat ($$.lexema, $3.lexema);
					}
					| expresion
					{
						numParam = 1;
						tsCompruebaParametro($1, numParam);
						$$.lexema = $1.lexema;
					};

		
lista : const_lista_enteros
		{
			$$.tipo = LISTA;
			$$.tipo_lista = ENTERO;
		}
		| const_lista_caracteres
		{
			$$.tipo = LISTA;
			$$.tipo_lista = CARACTER;	
		}
		| const_lista_booleanos
		{
			$$.tipo = LISTA;
			$$.tipo_lista = BOOLEANO;	
		}
		| const_lista_reales
		{
			$$.tipo = LISTA;
			$$.tipo_lista = REAL;	
		};
		
const_lista_enteros : INI_LIST lista_enteros FIN_LIST;

lista_enteros : lista_enteros COMA CONST_ENTERA
				| CONST_ENTERA;

const_lista_reales : INI_LIST lista_reales FIN_LIST;

lista_reales : lista_reales COMA CONST_REAL
				| CONST_REAL;
								
const_lista_caracteres : INI_LIST lista_caracteres FIN_LIST;

lista_caracteres : lista_caracteres COMA CONST_CARACTER
				| CONST_CARACTER;
				
const_lista_booleanos : INI_LIST lista_booleanos FIN_LIST;

lista_booleanos : lista_booleanos COMA CONST_BOOL
				| CONST_BOOL;
				
						

%%

/* Procedimientos del usuario */


/** aqui incluimos el fichero generado por el ’lex’
*** que implementa la función ’yylex’
**/


#ifdef DOSWINDOWS 	/* Variable de entorno que indica la plataforma */
#include "lexyy.c"
#else
#include "lex.yy.c"
#endif


/** se debe implementar la función yyerror. En este caso
*** simplemente escribimos el mensaje de error en pantalla
**/

void yyerror(const char * msg)
{
	fprintf(stderr,"(Linea %d): %s\n", linea_actual, msg) ;
}






