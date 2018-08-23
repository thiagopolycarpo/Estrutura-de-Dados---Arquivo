/* ORGANIZAÇÃO: REGISTROS E CAMPOS VARIAVEIS*/

//acrescentar a biblioteca conio.h e os getch() pra ver o resultado

//bibliotecas
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>

//defines
#define TAM_STRUCT 5

//structs
struct livro {
    //int  usado=0; // 0 para não usado, 1 para usado
    char isbn[14];
    char titulo[50];
    char autor[50];
    char ano[5];
}lr[TAM_STRUCT];


//funcoes
int abrir_arquivo(FILE **p_arq, char *nome_arq);
void criar_arquivo(FILE **p_arq, char *nome_arq);
void fechar_arquivo(FILE **p_arq);
void menu();
void carrega_arquivo();
char pega_registro(FILE *p_arq, char *p_reg);

int main()
{
	//FILE *arq;
	char *pch, registro[119], tam_reg;
 	int aberto;
  
  menu();
  //abrir arquivo, criar, exibir e fechar -- teste pra ver
	/*aberto = abrir_arquivo(&arq, cadastro);
	
	while(fread(&lr, sizeof(livro), 1, arq)){
		printf("\n%s", lr.isbn);
		printf("\n%s", lr.titulo);
		printf("\n%s", lr.autor);
		printf("\n%s\n", lr.ano);
	}
	
	//if(!aberto) criar_arquivo(arq, "Biblioteca.");*/
	
 	//fechar_arquivo(&arq);
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
    case 4: carrega_arquivo(); break;
    case 5: break;
    default: printf("\nOpção inválida!");
  }
}

//carrega arquivo biblioteca.bin ou remove.bin  -- falta gerar a marcação de registros usados ou não
void carrega_arquivo()
{
  char arq_cadastro[]="biblioteca.bin", arq_remove[]="remove.bin";
  FILE *arq;
  int i, aberto,resp;

  do
  {
    system("clear");
    printf("Digite 1- biblioteca.bin e 2-remove.bin\n: ");
    scanf("%d", &resp);
  }while(resp < 0 || resp > 1);
  //se resp == 1 carrega biblioteca.bin, senão remove.bin
  if(resp)
  {
    //abre arquivo biblioteca, carrega em vetor de struct 
    aberto = abrir_arquivo(&arq, arq_cadastro);
    
    if(aberto)
    {
      i=0;
      while(fread(&lr[i], sizeof(livro), 1, arq))
      {
        i++;
      }
     /*i=0; // exibe o vetor de struct do arquivo biblioteca.bin
      while(i<TAM_STRUCT)
      {
        printf("\n%s", lr[i].isbn);
        printf("\n%s", lr[i].titulo);
        printf("\n%s", lr[i].autor);
        printf("\n%s\n", lr[i].ano);
        i++;
      }*/
    }
    fechar_arquivo(&arq);
  }
  else
  {
    //abre arquivo remove, carrega em vetor de struct -- pode usar o mesmo vetor?! 
    aberto = abrir_arquivo(&arq, arq_remove);
      
    if(aberto)
    {
      i=0;
      while(fread(&lr, sizeof(livro), 1, arq))
      {
        i++;
      }/*
      i=0;
      while(fread(&lr, sizeof(livro), 1, arq)) //exibe o vetor de struct do arquivo remove.bin
      {
        printf("\n%s", lr.isbn);
        i++;
      }*/
    }
    fechar_arquivo(&arq);
  }
  menu();   
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
  
