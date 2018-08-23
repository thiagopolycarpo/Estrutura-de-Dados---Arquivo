/* ORGANIZAÇÃO: REGISTROS E CAMPOS VARIAVEIS*/

//acrescentar a biblioteca conio.h e os getch() pra ver o resultado

//bibliotecas
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>

//funcoes
int abrir_arquivo(FILE *p_arq, char *nome_arq);
void criar_arquivo(FILE *p_arq, char *nome_arq);
void fechar_arquivo(FILE *p_arq);
char pega_registro(FILE *p_arq, char *p_reg);


int main()
{
	FILE *arq;
	char *pch, registro[60], tam_reg, cadastro[] = "cadastro.cpp";
  int aberto;
  
	aberto = abrir_arquivo(arq, cadastro); 


  if(!aberto) criar_arquivo(arq, cadastro);
	

 	/*fseek(arq,0,0);
  	tam_reg = pega_registro(arq,registro);
  	while (tam_reg > 0)
   	{
    	pch = strtok(registro,"|");
    	while (pch != NULL)
      	{
        	printf("%s\n",pch);
	        pch = strtok(NULL,"|");
      	}
	    printf("\n");
	    tam_reg = pega_registro(arq,registro);  
   	} */
 	fechar_arquivo(arq);
  return 0;
}

//abre arquivos
int abrir_arquivo(FILE *p_arq, char *nome_arq)
{
  if((p_arq = fopen(nome_arq,"r+b")) == NULL)
  {
    printf("\nimpossivel abrir o arquivo");
    return 0;
  }
  return 1;
} 

//cria arquivos
void criar_arquivo(FILE *p_arq, char *nome_arq)
{
  if((p_arq = fopen(nome_arq,"a+b")) == NULL)
  {
    printf("impossivel criar o arquivo");
    exit(0);
  }
}

//fecha arquivos
void fechar_arquivo(FILE *p_arq)
{
  fclose(p_arq);
}

//devolve o tamanho dos registros
char pega_registro(FILE *p_arq, char *p_reg)
{
    char bytes;
     
    if (!fread(&bytes, sizeof(char), 1, p_arq))
    	return 0;
    fread(p_reg, bytes, 1, p_arq);
   	return bytes;
}  
  
