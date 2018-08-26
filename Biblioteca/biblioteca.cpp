//Desenvolvedores: Bruno Domene e Thiago Polycarpo
//acrescentar a biblioteca conio.h e os getch() pra ver o resultado

//bibliotecas
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>

//defines
#define TAM_STRUCT 5

//structs
struct livro {
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
int pega_registro(FILE **p_arq, char *p_reg);
int inserir(FILE **arq);
int dump_arquivo(FILE **arq);

int main(){
  int resp, sair = 0;
	FILE *arq;
	do{ 
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
	
		switch(resp){
			case 1:{
		    inserir(&arq);
				break;
			}
		  case 2: break;
		  case 3: break;
		  case 4:{
		    carrega_arquivo(); 
				break;
			}
		  case 5:{
		    dump_arquivo(&arq);
				break;
			}
		  case 6:{
		    sair = 1;
				break;
			}
		  default:{
		    printf("\nOpcao invalida!");
				break;
			} 
		}
	}while(sair != 1);
}

//abre arquivos
int abrir_arquivo(FILE **p_arq, char nome_arq[]){
  if((*p_arq = fopen(nome_arq,"r+b")) == NULL){
    printf("\nimpossivel abrir o arquivo");
    return 0;
  }
  return 1;
} 

//cria arquivo livros.bin e adiciona o byte int do contador e o byte offset
void criar_arquivo(FILE **p_arq, char nome_arq[]){
  int offset=-1,cont=0;
  if((*p_arq = fopen(nome_arq,"w+b")) == NULL){
    printf("impossivel criar o arquivo");
    //system("pause");
    exit(0);
  }
  //reserva os 4 primeiros bytes para o contador e os outros 4 para o offset
  fwrite(&cont,sizeof(int),1,*p_arq);
  fwrite(&offset,sizeof(int),1,*p_arq);
  fclose(*p_arq);
}

//fecha arquivos
void fechar_arquivo(FILE **p_arq){
  fclose(*p_arq);
}


//carrega arquivo biblioteca.bin ou remove.bin  -- falta gerar a marcação de registros usados ou não
int carrega_arquivo(){
  char arq_cadastro[]="biblioteca.bin", arq_remove[]="remove.bin";
  FILE *arq;
  int i, aberto,resp;

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
      	i++;
      }
      printf("\ncarreguei ");//teste
      fechar_arquivo(&arq);
      return 1;
    }
  }else{
    //abre arquivo remove, carrega em vetor de struct -- pode usar o mesmo vetor?! 
    aberto = abrir_arquivo(&arq, arq_remove);
    if(aberto){
      i=0;
      while(fread(&lr, sizeof(livro), 1, arq)){
      	i++;
      }
    }
  }
  fechar_arquivo(&arq);
  return 1;
}
//devolve o tamanho dos registros
int pega_registro(FILE **p_arq, char *p_reg){
  int bytes;
     
  if (!(fread(&bytes, sizeof(int), 1, *p_arq)))
    return 0;
  fread(p_reg, bytes, 1, *p_arq);
  return bytes;
}

int inserir(FILE **arq){
	int i, cont_registro, tam_registro, offset, tam_disponivel, aberto;
	char registro[119], arq_livros[]="livros.bin";
	
	system("cls");
	//system("clear");
	
  //abre arquivo e se nao conseguir abrir o arquivo porque é a primeira vez, cria o arquivo
	if((fopen(arq_livros, "r+b")) == NULL){
		criar_arquivo(arq,arq_livros);
    printf("\narquivo criado\n");
	}
		
	abrir_arquivo(arq, arq_livros);
	
  //volta arquivo no inicio le um byte que é o valor da posicao do registo no vetor
  //e le mais um byte que é o offset do arquivo indicando arquivos deletados ou nao
  fseek(*arq,0,0);
  fread(&cont_registro,1,sizeof(int),*arq);
  printf("\ncontador: %d", cont_registro);
  fread(&offset,1,sizeof(int),*arq);
  printf("\noffset: %d", offset);
  i=cont_registro;

  //formatando os registros para a estrategia de tamanho variavel a partir do i que voltou do arquivo
	sprintf(registro,"%s|%s|%s|%s|", lr[i].isbn, lr[i].titulo, lr[i].autor, lr[i].ano);
  tam_registro = strlen(registro); //pega o tamanho de cada registro
	tam_registro++;
	printf("\nregistro na funcao inserir:  %s", registro);
	printf("\ntamanho registro na funcao inserir: %d", tam_registro);
  
	//gravando tamanho do registro e registro no arquivo livros.bin
  fseek(*arq,0,2); 
	fwrite(&tam_registro, sizeof(int), 1, *arq);
	fwrite(registro,sizeof(char),tam_registro, *arq);
  i++;
  fseek(*arq,0,0);							//aponta para o contador
  fwrite(&i,sizeof(int),1,*arq);//grava no inicio no primeiro byte
  fclose(*arq);
  printf("\n\n");
  system("pause");
  return 1;
} 

//faz o dump do arquivo passado por parametro.
int dump_arquivo(FILE **arq){
	char *pch, registro[119], tam_reg, arq_livros[]="livros.bin";
	int aberto, cont, offset;
	system("cls");
	//system("clear");
	
  aberto = abrir_arquivo(arq, arq_livros);
	if(!aberto){
		printf("\nimpossivel abrir o arquivo\n");
		///system("pause");
		return 0;
	}

 	fseek(*arq,0,0);
 	fread(&cont, sizeof(int), 1, *arq);
 	fread(&offset, sizeof(int), 1, *arq);
 	
 	printf("contador: %d\n", cont);
	printf("offset: %d\n\n", offset); 	
 	
 	
	tam_reg = pega_registro(arq,registro);
	while (tam_reg > 0){
		printf("Tamanho registro: %d\n", tam_reg);
    pch = strtok(registro,"|");
    while (pch != NULL){
    	printf("%s\n",pch);
	    pch = strtok(NULL,"|");
    }
		printf("\n");
  	tam_reg = pega_registro(arq,registro);
	} 
	printf("\n");
	system("pause");
	fclose(*arq);
	return 1;
}
  
