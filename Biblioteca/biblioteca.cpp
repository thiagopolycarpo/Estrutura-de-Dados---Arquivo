//Desenvolvedores: Bruno Domene e Thiago Polycarpo

//bibliotecas
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>

//defines
#define TAM_STRUCT 1000

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
int abrir_arquivo(FILE **p_arq, char nome_arq[]);
void criar_arquivo(FILE **p_arq, char nome_arq[]);
void fechar_arquivo(FILE **p_arq);
int carregar_arquivo();
int pegar_registro(FILE **p_arq, char *p_reg);
int inserir(FILE **arq, char nome_arq[], int tam_vet_inserir);
int dump_arquivo(FILE **arq);
int remover(FILE **arq);
int compactar(FILE **arq);
int deletar_arquivo(char arq[]);

int main(){
  int resp, sair = 0;
	FILE *arq;
	char arq_livros[]="livros.bin";
	int tam_vet_inserir;
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
		    inserir(&arq, arq_livros, tam_vet_inserir);
				break;
			}
		  	case 2:{
		  	remover(&arq);
				break;
			}
		  	case 3: {
		  	compactar(&arq);
		  		break;
		  	}
		  	case 4:{
		    tam_vet_inserir = carregar_arquivo(); 
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

//fecha arquivos
void fechar_arquivo(FILE **p_arq){
  fclose(*p_arq);
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
  fwrite(&cont,sizeof(int),1,*p_arq);//cont do arquivo de insercao
  fwrite(&cont,sizeof(int),1,*p_arq);//cont do arquivo de remocao
  fwrite(&offset,sizeof(int),1,*p_arq);//offset
  fclose(*p_arq);
}

//carrega arquivo biblioteca.bin e remove.bin
int carregar_arquivo(){
  char arq_cadastro[]="biblioteca.bin", arq_remove[]="remove.bin";
  FILE *arq;
  int i, aberto,resp,tam_vet_inserir;
  //abre arquivo biblioteca, carrega em vetor de struct 
  aberto = abrir_arquivo(&arq, arq_cadastro);
  if(aberto){
    i=0;
    while(fread(&lr[i], sizeof(livro), 1, arq)){
      printf("\n%s | %s | %s | %s", lr[i].isbn, lr[i].titulo, lr[i].autor, lr[i].ano);	
      i++;
    }
    printf("\nDados de insercao carregados!!\n");
    fechar_arquivo(&arq);
  }
  tam_vet_inserir = i;  
  //abre arquivo remove, carrega em vetor de struct 
  aberto = abrir_arquivo(&arq, arq_remove);
  if(aberto){
    i=0;
	while(fread(&rem[i], sizeof(struct remove), 1, arq)){
	  printf("\n%s ", rem[i].isbn);
	  i++;
	}
	printf("\nDados de remocao carregados!!\n ");
  }
  system("pause");
  fechar_arquivo(&arq);
  return tam_vet_inserir;
}
//devolve o tamanho dos registros e obtem o registro
int pegar_registro(FILE **p_arq, char *p_reg){
  int bytes;
     
  if (!(fread(&bytes, sizeof(int), 1, *p_arq)))
    return 0;
  fread(p_reg, bytes, 1, *p_arq);
  return bytes;
}

int inserir(FILE **arq, char nome_arq[], int tam_vet_inserir){
	int i, cont_registro, tam_registro, offset, tam_disponivel, aberto;
	char registro[119],  teste;
	
	system("cls");
	
   //abre arquivo e se nao conseguir abrir o arquivo porque Ã© a primeira vez, cria o arquivo
	if((fopen(nome_arq, "r+b")) == NULL){
		criar_arquivo(arq,nome_arq);
    printf("\narquivo criado\n");
	}
		
	abrir_arquivo(arq, nome_arq);
	
  //volta arquivo no inicio le um byte que Ã© o valor da posicao do registo no vetor
  //e le mais um byte que Ã© o offset do arquivo indicando arquivos deletados ou nao
  fseek(*arq,0,0);
  fread(&cont_registro,1,sizeof(int),*arq);
  if(cont_registro < tam_vet_inserir){
	  printf("contador: %d\n", cont_registro);
	  fseek(*arq,4,1); //pula o cont do arquivo de remoção 
	  fread(&offset,1,sizeof(int),*arq);
	  printf("offset: %d\n\n", offset);
	  i=cont_registro;
	  
	  //formatando os registros para a estrategia de tamanho variavel a partir do i que voltou do arquivo
	  sprintf(registro,"%s|%s|%s|%s|", lr[i].isbn, lr[i].titulo, lr[i].autor, lr[i].ano);
	  tam_registro = strlen(registro); //pega o tamanho de cada registro
		tam_registro++;
		printf("registro na funcao inserir:  %s\n", registro);
		printf("tamanho registro na funcao inserir: %d\n\n", tam_registro);
	  
	  //Inserindo no meio do arquivo se tiver espaço disponivel com tamanho que caiba o registro
		if(offset != -1){
			int proximo, tam_excluido, aux, achou = 0, anterior;
			aux = offset;
			anterior = 3;				//posição do head da lista no arquivo, obs.: o valor é 3 pq 3 + 5 = 8, que é a posição correta
		
			//acha se possivel um espaço valido para o registro 
			while(achou != 1 && proximo != -1){
				fseek(*arq, aux, 0);												//vai para o espaço disponivel
				fread(&tam_excluido, sizeof(int), 1, *arq);	//lê o tamanho do espaço valido naquele espaço disponivel
				printf("tamanho do resgistro excluido: %d\n", tam_excluido);
				fseek(*arq, 1, 1);													//pula o marcador *
				fread(&proximo, sizeof(int), 1, *arq);			//pega a posição do proximo espaço livre na lista
				printf("proximo elemento: %d\n", proximo);
				
				if(tam_excluido >= tam_registro){
					achou = 1;
					fseek(*arq, anterior + 5, 0); 			//aponta para o "apontador" do registro anterior na lista, se o removido for o primeiro... 
					fwrite(&proximo, sizeof(int), 1, *arq);																				//...aponta para o head da lista
				}else{
					anterior = aux;
					aux = proximo;
				}
				system("pause");
			}
			
			if(achou){			
				fseek(*arq, aux + 4, 0);   //aponta para o espaço disponivel
				
				fwrite(registro,sizeof(char),tam_registro, *arq); //insere o registro
				
				printf("\nRegistro inserido\n");
				i++;
			  fseek(*arq,0,0);							//aponta para o contador
			  fwrite(&i,sizeof(int),1,*arq);//grava no inicio no primeiro byte
			  
			  system("pause");
			  fclose(*arq);
			  return 1;
			}else{
				printf("\nNao foi possivel achar um tamanho disponivel. Inserindo registro no final do arquivo\n");
			}
		}
		
		fseek(*arq,0,2); 
		fwrite(&tam_registro, sizeof(int), 1, *arq);
		fwrite(registro,sizeof(char),tam_registro, *arq);
		
		i++;
	  fseek(*arq,0,0);//aponta para o contador
	  fwrite(&i,sizeof(int),1,*arq);//grava no inicio no primeiro byte
  }else{
  	printf("\nSem mais registros a serem inseridos!!");
  }
  printf("\n\n");
  system("pause");
  fechar_arquivo(arq);
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
 	
 	tam_reg = pegar_registro(arq,registro);
	while (tam_reg > 0){
		pch = strtok(registro,"|");
			while (pch != NULL){
				//exibindo o hexadecimal de cada caractere
				for(int i = 0; i < strlen(pch); i++)
					printf("%X ", pch[i]);
				printf("- %s\n",pch);
				pch = strtok(NULL,"|");
			}
		printf("\n");
	  tam_reg = pegar_registro(arq,registro);
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
	tam_reg = pegar_registro(arq,registro); 					//pega tamanho do primeiro registro
	while (tam_reg > 0 && achou == 0){
		
    pch = strtok(registro,"|");										//pega o isbn
    
    if(!strcmp(pch, rem[cont_remocao].isbn)){			//compara o isbn do registro for igual com isbn passado
    	printf("posicao do registro a ser removido no arquivo: %d\n", posicao_corrente); //posicao do registro no arquivo
			printf("Tamanho registro: %d\n", tam_reg);
			printf("%s\n",pch);
    	posicao = posicao_corrente;									
    	achou = 1;																	//achou recebe 1 para sair do while	
		}
    
    posicao_corrente = ftell(*arq); 							//obtem a posicao do proximo registro
		printf("\n");
  	tam_reg = pegar_registro(arq,registro);
  }
  
  if(achou){
  	fseek(*arq, posicao + 4, 0);								//aponta para o registro alvo pulando o tamanho dele
  	fwrite(&marcador, sizeof(char), 1, *arq);		//escreve no registro o marcador * 
  	fwrite(&offset, sizeof(int), 1, *arq);			//escreve no registro o valor do ultimo espaço livre na lista 
  	fseek(*arq, 8, 0);													//aponta para o head da lista e faz ele apontar para esse registro 
  	fwrite(&posicao, sizeof(int), 1, *arq);
  	printf("registro removido\n");
	}else{
		printf("registro %s nao encontrado\n", rem[cont_remocao].isbn);
	}
	cont_remocao++;																//incrementa o contador de remoção
	fseek(*arq, 4, 0);
	fwrite(&cont_remocao, sizeof(int), 1, *arq);	
	system("pause");
	fechar_arquivo(arq);
	return 1;
}

//deleta arquivo
int deletar_arquivo(char arq[]){
	int ret;
    
    ret = remove(arq);
	
   	if(ret == 0) {
     	printf("\nRemovido!!! ");
   	}else{
    	printf("\n Error: impossivel remover!!\n");
   	}
 	return 1;
}

//compacta o arquivo
int compactar(FILE **arq){
	FILE *arq_temporario;
	char *pch, registro[119],arq_livros[]="livros.bin", arq_temp[]="temp.bin";
	int  tam_reg, novo_tam_reg, cont_insercao, cont_remocao, offset;
	
	system("cls");
	
	//abre biblioteca
	if(!abrir_arquivo(arq, arq_livros)){
		return 0;
	}
	
	//cria arquivo temporario
	criar_arquivo(&arq_temporario, arq_temp);
	if(!abrir_arquivo(&arq_temporario, arq_temp)){
		return 0;
	}
	
	fseek(*arq,0,0);
	fseek(arq_temporario,0,0);
	fread(&cont_insercao, sizeof(int), 1, *arq);
  fread(&cont_remocao, sizeof(int), 1, *arq);
 	fread(&offset, sizeof(int), 1, *arq);
 	offset=-1;
 	fwrite(&cont_insercao,sizeof(int),1, arq_temporario);
 	fwrite(&cont_remocao,sizeof(int),1, arq_temporario);
 	fwrite(&offset,sizeof(int),1, arq_temporario);

	//copiando registros para o arquivo temporario
	tam_reg = pegar_registro(arq,registro);
	while (tam_reg > 0){
		if(registro[0] != '*'){
			novo_tam_reg = strlen(registro);
			novo_tam_reg++;
			fwrite(&novo_tam_reg, sizeof(int), 1, arq_temporario);
			fwrite(registro,sizeof(char),novo_tam_reg,arq_temporario);
		}
	  tam_reg = pegar_registro(arq,registro);
	}
	
	fclose(*arq);
	
	//Copiando os registros de volta para o arquivo
	//abrindo o arquivo livros.bin no modo escrita
	if((*arq = fopen(arq_livros, "wb")) == NULL){
		printf("\nimpossivel abrir o arquivo\n");
		system("pause");
		return 0;
	}
	//escrevendo o cabeçalho com offeset = -1
	fwrite(&cont_insercao,sizeof(int),1, *arq);
 	fwrite(&cont_remocao,sizeof(int),1, *arq);
 	fwrite(&offset,sizeof(int),1, *arq);
 	
	fseek(arq_temporario, 12, 0);			//apontando para o primeiro registro do arquivo temporario
	tam_reg = pegar_registro(&arq_temporario, registro);
	while(tam_reg > 0){
		fwrite(&tam_reg, sizeof(int), 1, *arq);
		fwrite(registro,sizeof(char),tam_reg, *arq);	
		tam_reg = pegar_registro(&arq_temporario, registro);
	}
	
	fclose(arq_temporario);
	fclose(*arq);
	
	printf("Arquivo compactado com sucesso\n");
	deletar_arquivo(arq_temp); 
		
	system("pause");
	return 1;
}
