//Bibliotecas

#include <stdio.h>
#include <stdlib.h>

extern FILE *yyin, *yyout;
FILE *errores, *secuencia_funciones;

extern int yylex( );
extern void yyerror( char *evaluacion );
int token;
int ID=0,NUM=1,PR1=2,PR2=3,PR3=4,PR4 =5,PR5=6, PR6=7, PR7=8;
int PR8=9, SEP=10, OP1=11 OP2=12, OP3 =13, TERMINADOR=14, PAR1=15, PAR2=16;
int conta_er = 0;

void P();
void D();
void DP();
void T();
void L();
void LP();
void S();
void SP();
void E();
void EP();
void EPP();
void F();
void A();
void F();

int main(int argc,char **argv)    //recibira archivo
{
  if ( argc > 1 )
  {
    yyin = fopen( argv[1], "r" );
  }
  else
  {
    yyin = stdin;
  }
  yyout = fopen( "Tokens.txt", "w" ); // crea un fichero de texto para escritura para ls tokens
  if( !yyout )
  {
    printf( "<Error al abrir archivo>" );
    return 1;
  }
  else
  {
    errores = fopen( "Errores.txt", "w" ); //se escribiran los errores en dicho archivo d salida
   	if( !archivo_errores )
    {
    	printf( "<Error al abrir archivo>" );   
    	return 1;
    }
    else
    {
      secuencia_funciones = fopen( "secuencia_funciones.txt", "w" );    //Guardara un historico de la utilizacion de las funciones
      if( !secuencia_funciones )
      {
        printf( "<Error al abrir archivo>" );
        return 1;
      }
      else
      {
        fprintf( secuencia_funciones, "HISTORICO USO DE FUNCIONES:\n" );
        fprintf( errores, "ERRORES OBTENIDOS:\n" );
        fprintf( yyout, "Tokens:\n" );
      	token = yylex( );
        P( );             //Llamada a la primer funcion de la gramática
        if ( token != 0 )
          printf( "\n %i Errores encontrados...", conta_er);
      }
    }
  }
	fclose( errores );
  fclose( yyout );
  fclose( yyin );
}

void P( )
{
  fprintf( secuencia_funciones, "P->\n");
  D( );
  S( );
}

void D( ) 
{
  fprintf( secuencia_funciones,"D->\n" );
  T( );
  L( );
  if ( token == 14 ) //Token debe ser terminador
  {
    token = yylex( );
    DP( ); //Func D' prima
  }
  else
  {
    conta_er++; //No se ha ingresado el simbolo correcto
    yyerror( "Error de Sintaxis" );
  }
}

void DP()  
{
  fprintf( secuencia_funciones, "DP->\n" );
  if ( token == 14 || token ==  ) 
  {
    D( );
  }
}

void T()
{
  fprintf( secuencia_funciones, "T->\n" );
  if ( token == 2 || token == 3 )
  {
    token = yylex( ); //Guarda tipo valido
  }
  else
  {
    conta_er++;
    yyerror( "Error de Sintaxis" );
  }
}

void L()
{
  fprintf( secuencia_funciones, "L ->\n" );
  if ( token == 0 )
  {
    token = yylex( );
    LP( );
  }
  else
  {
    conta_er++;
    yyerror( "Error de Sintaxis");
  }
}

void LP( )
{
  fprintf( secuencia_funciones, "LP->\n" );
  if ( token == 10 )  //Separador
  {
    token = yylex( );
    L( );
  }
}

void S( )
{
  fprintf( secuencia_funciones, "S->\n" );
  E( );
  SP( );
}

void SP( )
{
  fprintf( secuencia_funciones, "SP->\n" );
  E( );
  SP( );
}
  
void E( )
{
  fprintf( secuencia_funciones, "E\n" );
  if ( token == 4 )//¬WHILE
      {  
        token = yylex( );
        E( );
        if ( token == 5 )  //DO
        { 
             token = yylex( );
             E( );
             if ( token == 6 ) //END
             { 
              token = yylex( );
              E( );
             }
             else
             {      conta_er++;
                    yyerror( "Error de Sintaxis" );
             }
          }
          else
          {
                    conta_er++;
                    yyerror( "Error de Sintaxis" );
          }
        }
       if ( token == 0 ) //id
            { 
                token = yylex( );
                if( token == 11) //#           
                  {
                    token = yylex( );
                    A( );
                    if( token == 14) //;           
                      {
                        token = yylex( );
                      }
                      else 
                       {
                          conta_er++;
                          yyerror( "Error de Sintaxis" );
                       }
                  }
                  else
                   {
                      conta_er++;
                      yyerror( "Error de Sintaxis" );
                   } 
              }
           if( token ==7 ) //IF           
              {
                    token = yylex( );
                    E( );
                    if( token ==8 ) //then          
                        {
                        token = yylex( );
                         E( );
                         EPP( );               
                        }
                     else 
                         {
                          conta_er++;
                          yyerror( "Error de Sintaxis" );
                         }
                }
                else 
                {
                       A( );
                       EP( );
                }
 }
//-----
void EPP ()
{
  fprintf( secuencia_funciones, "EPP\n" );
  switch( token )
  {
    case 6: //END
      token = yylex( );
      break;

    case 9: //ELS
      token = yylex( );
      E( );
      if ( token == 6 ) //END
      {
        token = yylex( );
      }else
        yyerror( "Error de Sintaxis" );
      break;
    default:
      conta_er++;
      yyerror( "Error de Sintaxis" );
      break;
  }
}

void EP( )
{
  fprintf( secuencia_funciones, "EP\n" );
     if ( token == 13 ) //~
      {
        token = yylex( );
        A( );
        EP( );
       }
       
  }
  
 void A( )
 {  fprintf( secuencia_funciones, "A\n" )
   AP( );
   F();
 }
void AP( )
  {fprintf( secuencia_funciones, "AP\n" )
   E( );
   if ( token == 12 ) //&
      {
        token = yylex( );
       }else
        yyerror( "Error de Sintaxis" );
   }
  void F
  {fprintf( secuencia_funciones, "F\n" );
  if ( token == 15)
  {
    token = yylex( );
    E( );
    if ( token == 16)
    {
    token = yylex( );
    }
    else 
    yyerror( "Error de Sintaxis" );
  }
 if ( token == 1 )
  {
    token = yylex( );
  }
}

