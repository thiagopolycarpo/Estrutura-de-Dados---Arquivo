/* ORGANIZAÇÃO: REGISTROS E CAMPOS VARIAVEIS*/

//acrescentar a biblioteca conio.h e os getch() pra ver o resultado

//bibliotecas
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>

//defines
#define TAM_STRUCT 5

//structs
struct livro 
{
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
int carrega_arquivo();
int pega_registro(FILE *p_arq, char *p_reg);
int inserir(int qtd_livros, FILE **out);
int dump_arquivo(FILE *arq);

int main()
{
  	int resp, sair = 0, qtd_livros;
	FILE *out;
	do
  	{ 
	  //system("clear");
	  system("cls");
	    
	  printf("\n        Menu");
	  printf("\n1 - Insercao");
	  printf("\n2 - Remocao");
	  printf("\n3 - Compactacao");
	  printf("\n4 - Carregar Arquivos");
	  printf("\n5 - Dump Arquivo ");
	  printf("\n6 - Sair");
	  printf("\nopcao:");
	  scanf("%d",&resp);
	
		switch(resp)
    	{
			case 1:
      		{
		    inserir(qtd_livros, &out);
				break;
			}
		  case 2: break;
		  case 3: break;
		  case 4: 
    	  	{
		    qtd_livros = carrega_arquivo(); //retorna a quantidade de livros carregados 
				break;
			}
		  case 5:
    	  	{
		    dump_arquivo(out);
				break;
			}
		    case 6:
    		{
		    sair = 1;
				break;
			}
		  default:
      		{
		    printf("\nOpcao invalida!");
				break;
			} 
		}
	}while(sair != 1);
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


//carrega arquivo biblioteca.bin ou remove.bin  -- falta gerar a marcação de registros usados ou não
int carrega_arquivo(){
  char arq_cadastro[]="biblioteca.bin", arq_remove[]="remove.bin";
  FILE *arq;
  int i, aberto,resp, qtd_livros = 0;

  do{
    //system("clear");
    system("cls");
    
    printf("\nDigite 1- biblioteca.bin e 2-remove.bin: ");
    scanf("%d", &resp);
  }while(resp < 0 || resp > 1);
  //se resp == 1 carrega biblioteca.bin, senão remove.bin
  if(resp){
    //abre arquivo biblioteca, carrega em vetor de struct 
    aberto = abrir_arquivo(&arq, arq_cadastro);
    
    if(aberto){
      i=0;
      while(fread(&lr[i], sizeof(livro), 1, arq)){
      	qtd_livros++;
        i++;
      }
    }
  }else{
    //abre arquivo remove, carrega em vetor de struct -- pode usar o mesmo vetor?! 
    aberto = abrir_arquivo(&arq, arq_remove);
    if(aberto){
      i=0;
      while(fread(&lr, sizeof(livro), 1, arq)){
      	qtd_livros++;
        i++;
      }
    }
  }
  fechar_arquivo(&arq);
  return qtd_livros;  
}
//devolve o tamanho dos registros
int pega_registro(FILE *p_arq, char *p_reg){
    int bytes;
     
    if (!fread(&bytes, sizeof(int), 1, p_arq))
    	return 0;
    fread(p_reg, bytes, 1, p_arq);
   	return bytes;
} 

int inserir(int qtd_livros, FILE **out)
{
	int i = 0, tam_registro;
	char registro[119];
	
	system("cls");
	//system("clear");
	
	if((*out = fopen("livros.bin", "a+b")) == NULL){
		printf("\nNao foi possivel abrir o arquivo\n");
		system("pause");
		return 0;	
	}

	while(i < qtd_livros)
  	{
	 	//formatando os registros para a estrategia de tamanho variavel
	    sprintf(registro,"%s|%s|%s|%s|", lr[i].isbn, lr[i].titulo, lr[i].autor, lr[i].ano);
	    tam_registro = strlen(registro); //pega o tamanho de cada registro
	    tam_registro++;
	    printf("\n%s", registro);
	    printf("\n%d", tam_registro);
	        
	    //gravando tamanho do registro e registro no arquivo livros.bin 
	    fwrite(&tam_registro, sizeof(int), 1, *out);
	    fwrite(registro, sizeof(char), tam_registro, *out);
	   
	    i++;
  	}

	fclose(*out);
    printf("\n\n");
    system("pause");
    return 1;
} 

//faz o dump do arquivo passado por parametro.
int dump_arquivo(FILE *arq)
{
	char *pch, registro[119], tam_reg;
	
	system("cls");
	//system("clear");
	
	if((arq = fopen("livros.bin","r+b")) == NULL)
  	{
		printf("\nimpossivel abrir o arquivo\n");
		system("pause");
		return 0;
	}

 	fseek(arq,0,0);
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
  } 
  printf("\n");
  system("pause");
  fclose(arq);
  return 1;

}
  
