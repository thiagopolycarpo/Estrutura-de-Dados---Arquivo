//Desenvolvedores: Bruno Domene e Thiago Polycarpo
//acrescentar a biblioteca conio.h e os getch() pra ver o resultado

//bibliotecas
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>

//defines
#define TAM_STRUCT 5

//structs
struct livro{
  char isbn[14];
  char titulo[50];
  char autor[50];
  char ano[5];
}lr[TAM_STRUCT];

struct remove{
  char isbn[14];
}rem[TAM_STRUCT];


//funcoes
int abrir_arquivo(FILE **p_arq, char *nome_arq);
void criar_arquivo(FILE **p_arq, char *nome_arq);
void fechar_arquivo(FILE **p_arq);
int carrega_arquivo();
int pega_registro(FILE **p_arq, char *p_reg);
int inserir(FILE **arq);
int dump_arquivo(FILE **arq);
int remover(FILE **arq);

int main(){
  int resp, sair = 0;
	FILE *arq;
	do{ 
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
		  case 2:{
		  	remover(&arq);
				break;
			}
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
  fwrite(&cont,sizeof(int),1,*p_arq);//cont do arquivo de inserção
  fwrite(&cont,sizeof(int),1,*p_arq);//cont do arquivo de remoção
  fwrite(&offset,sizeof(int),1,*p_arq);//offset
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
   	system("cls");
    
    printf("\nDigite 1- biblioteca.bin e 2-remove.bin: ");
    scanf("%d", &resp);
  }while(resp < 1 || resp > 2);
  //se resp == 1 carrega biblioteca.bin, senão remove.bin
  if(resp == 1){
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
	      printf("dados carregados:\n");
	      while(fread(&rem[i], sizeof(struct remove), 1, arq)){
	      	printf("%s\n", rem[i].isbn);
	      	i++;
	      }
	    }
	}
  system("pause");
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
	int i, cont_registro, tam_registro, offset, tam_disponivel, aberto,ignora;
	char registro[119], arq_livros[]="livros.bin";
	
	system("cls");
	
	
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
  fread(&ignora,1,sizeof(int),*arq);//ignora cont do arquivo de remocao
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
	int aberto, cont_insercao,cont_remocao, offset;
	system("cls");
	
	
  aberto = abrir_arquivo(arq, arq_livros);
	if(!aberto){
		printf("\nimpossivel abrir o arquivo\n");
		system("pause");
		return 0;
	}

 	fseek(*arq,0,0);
 	fread(&cont_insercao, sizeof(int), 1, *arq);
  fread(&cont_remocao, sizeof(int), 1, *arq);
 	fread(&offset, sizeof(int), 1, *arq);
 	
 	printf("contador insercao: %d\n", cont_insercao);
  printf("contador remocao: %d\n", cont_remocao);
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

//remove um elemento do arquivo 
int remover(FILE **arq){
	char registro[119], nome_arq[]="livros.bin", *pch, marcador = '*';
	int cont_insercao, cont_remocao, offset, tam_reg, i=0, posicao, posicao_corrente, achou = 0;
	
	system("cls");
	if(!abrir_arquivo(arq, nome_arq)) return 0;
	
	fseek(*arq,0,0);
 	fread(&cont_insercao, sizeof(int), 1, *arq);
  fread(&cont_remocao, sizeof(int), 1, *arq);
 	fread(&offset, sizeof(int), 1, *arq);
 	
 	printf("contador insercao: %d\n", cont_insercao);
  printf("contador remocao: %d\n", cont_remocao);
	printf("offset: %d\n\n", offset);
	
	posicao_corrente = ftell(*arq); 								//pega posicao do primeiro registro
	tam_reg = pega_registro(arq,registro); 					//pega tamanho do primeiro registro
	while (tam_reg > 0 && achou == 0){
		
    pch = strtok(registro,"|");							//pega o isbn
    
    if(!strcmp(pch, rem[cont_remocao].isbn)){			//compara o isbn do registro for igual com isbn passado
    	printf("posicao do registro a ser removido no arquivo: %d\n", posicao_corrente); //posicao do registro no arquivo
			printf("Tamanho registro: %d\n", tam_reg);
			printf("%s\n",pch);
    	posicao = posicao_corrente;									
    	achou = 1;																	//achou recebe 1 para sair do while	
		}
    
    posicao_corrente = ftell(*arq); 							//obtem a posicao do proximo registro
		printf("\n");
  	tam_reg = pega_registro(arq,registro);
  }
  
  if(achou){
  	fseek(*arq, posicao + 4, 0);								//aponta para o registro alvo pulando o tamanho dele
  	fwrite(&marcador, sizeof(char), 1, *arq);		//escreve no registro o marcador * 
  	fwrite(&offset, sizeof(int), 1, *arq);			//escreve no registro o valor do ultimo espa�o livre na lista 
  	fseek(*arq, 8, 0);													//aponta para o head da lista e faz ele apontar para esse registro 
  	fwrite(&posicao, sizeof(int), 1, *arq);
  	printf("registro removido\n");
	}else{
		printf("regstro n�o encontrado\n");
	}
	
	cont_remocao++;																//incrementa o contador de remo��o
	fseek(*arq, 4, 0);
	fwrite(&cont_remocao, sizeof(int), 1, *arq);	
	
	system("pause");
	fclose(*arq);
}
  
  
