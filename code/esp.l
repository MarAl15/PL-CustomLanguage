%{
#include <stdlib.h>
#include <string.h>

int linea_actual=1;
%}

%option yylineno
%option noyywrap


letra 		[a-zA-Z]
digito 		[0-9]
alphanum 	[a-zA-Z_0-9]
punto 		"."
coma		,
otros 		.

%%

"var" 							return INI_VAR;
"endvar" 						return FIN_VAR;
";" 							{yylval.lexema = ";"; return FIN;}
"{" 							return INI_LLAVE;
"}" 							return FIN_LLAVE;
"[" 							return INI_LIST;
"]" 							return FIN_LIST;
"(" 							{yylval.lexema = "("; return INI_PARENT;}
")" 							return FIN_PARENT;
"main" 							return PROGRAMA;
"float" 						{yylval.tipo = REAL; return TIPO_VARIABLE;}
"int" 							{yylval.tipo = ENTERO; return TIPO_VARIABLE;}
"double" 						{yylval.tipo = REAL; return TIPO_VARIABLE;}
"char" 							{yylval.tipo = CARACTER; return TIPO_VARIABLE;}
"bool" 							{yylval.tipo = BOOLEANO; return TIPO_VARIABLE;}
"if" 							return SI;
"else" 							return SINO;
"while" 						return MIENTRAS;
"repeat" 						return REPETIR;
"until" 						return HASTA;
"cin" 							return ENTRADA;
"cout" 							return SALIDA;
"return"						return DEVOLVER;

"$"								return DOLAR;
"="								{yylval.lexema = "="; return OPIGUALDAD;}

"<<"							return LL;
">>"							return MM;
"@" 							return ARROBA;
"++" 							return SUM;
"!" 							{yylval.atrib=0; yylval.lexema = "!"; return OPUNARIO;}
"--" 							{yylval.atrib=0; return SEURO;}
"€"								{yylval.atrib=1; return SEURO;}
"+" 							{yylval.atrib=0; return SUMRES;}
"-" 							{yylval.atrib=1; return SUMRES;}
"?" 							{yylval.atrib=1; yylval.lexema = "?"; return OPUNARIO;}
"#" 							{yylval.atrib=2; yylval.lexema = "#"; return OPUNARIO;}
"/" 							{yylval.atrib=0; yylval.lexema = "/"; return OPBINARIO;}
"%" 							{yylval.atrib=1; yylval.lexema = "%"; return OPBINARIO;}
"**" 							{yylval.atrib=2; yylval.lexema = "**"; return OPBINARIO;}
"*" 							return OPMULT;
"==" 							{yylval.atrib=0; return OPRELACION;}
"!=" 							{yylval.atrib=1; return OPRELACION;}
"<" 							{yylval.atrib=2; return OPRELACION;}
"<=" 							{yylval.atrib=3; return OPRELACION;}
">" 							{yylval.atrib=4; return OPRELACION;}
">=" 							{yylval.atrib=5; return OPRELACION;}
"&&" 							return OPAND;
"||" 							return OPOR;

","								return COMA;

"true" 							{yylval.lexema = strdup(yytext); return CONST_BOOL;}
"false" 						{yylval.lexema = strdup(yytext); return CONST_BOOL;}
{letra}+{alphanum}* 			{yylval.lexema = strdup(yytext); return ID;}
{digito}+ 						{yylval.lexema = strdup(yytext); return CONST_ENTERA;}
{digito}*\.({digito}+)?  		{yylval.lexema = strdup(yytext); return CONST_REAL;}
"'"[^"'"]"'"					{yylval.lexema = strdup(yytext); return CONST_CARACTER;}
\"[^\"]+\"						{yylval.lexema = strdup(yytext); return CADENA;}
"list of" 						return LIST;
" "								{}
"\t"							{}
"\n"							linea_actual++;
{otros} 						{printf("\n(Linea %d) Error léxico: token %s\n", linea_actual, yytext);};

%%
