/* ORGANIZAÇÃO: REGISTROS E CAMPOS VARIAVEIS*/

//acrescentar a biblioteca conio.h e os getch() pra ver o resultado

//bibliotecas
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>

struct livro {
    char isbn[13];
    char titulo[50];
    char autor[50];
    char ano[5];
}lr;

//funcoes
int abrir_arquivo(FILE **p_arq, char *nome_arq);
void criar_arquivo(FILE **p_arq, char *nome_arq);
void fechar_arquivo(FILE **p_arq);
void menu();
char pega_registro(FILE *p_arq, char *p_reg);

int main()
{
	FILE *arq;
	char *pch, registro[119], tam_reg, cadastro[]="biblioteca.bin";
 	int aberto;
  
  menu();
  //abrir arquivo, criar, exibir e fechar
	/*aberto = abrir_arquivo(&arq, cadastro);
	
	while(fread(&lr, sizeof(livro), 1, arq)){
		printf("\n%s", lr.isbn);
		printf("\n%s", lr.titulo);
		printf("\n%s", lr.autor);
		printf("\n%s\n", lr.ano);
	}
	
	//if(!aberto) criar_arquivo(arq, "Biblioteca.");
	
 	fechar_arquivo(&arq);*/
  return 0;
}

//abre arquivos
int abrir_arquivo(FILE **p_arq, char nome_arq[])
{
  if((*p_arq = fopen(nome_arq,"r+b")) == NULL)
  {
    printf("\nimpossivel abrir o arquivo");
    return 0;
  }
  return 1;
} 

//cria arquivos
void criar_arquivo(FILE **p_arq, char nome_arq[])
{
  if((*p_arq = fopen(nome_arq,"a+b")) == NULL)
  {
    printf("impossivel criar o arquivo");
    exit(0);
  }
}

//fecha arquivos
void fechar_arquivo(FILE **p_arq)
{
  fclose(*p_arq);
}

void menu()
{
  int resp;
  do
  {
    system("clear");
    printf("\n        Menu");
    printf("\n1 - Insercao");
    printf("\n2 - Remocao");
    printf("\n3 - Compactacao");
    printf("\n4 - Carregar Arquivos");
    printf("\n5 - Dump Arquivo: ");
    scanf("%d",&resp);

    if(resp < 1 || resp > 5)
    {
      printf("\n Opção inválida, digite novamente! (Aperte enter)");
      getchar();
    }
  }while(resp < 1 || resp > 5);

  switch(resp)
  {
    case 1: break;
    case 2: break;
    case 3: break;
    case 4: break;
    case 5: break;
    default: printf("\nOpção inválida!");
  }
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
  
