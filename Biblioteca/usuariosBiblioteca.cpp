#include<stdio.h>

int main() {
    FILE *fd;
    
    //////////////////////////////
    struct livro {
        char isbn[14];
        char titulo[50];
        char autor[50];
        char ano[5];
    } vet[5] = {{"1111111111111", "Dom Quixote", "Miguel de Cervantes", "1600"},
                {"2222222222222", "A Parte Que Falta", "Silverstein", "2016"},
                {"3333333333333", "O Poder do Habito", "C. Duhigg", "2015"},
                {"4444444444444", "A Culpa e das Estrelas", "G. John", "2015"},
                {"5555555555555", "O Teorema de Katherine", "G. John", "2016"},
                {"6666666666666", "Guerra dos Tronos", "G. R. Martin", "1996"},
                {"7777777777777", "Mistborn vol2", "Brandon Sanderson", "2010"},
               };
       
    fd = fopen("biblioteca.bin", "w+b");
    fwrite(vet, sizeof(vet), 1, fd);
    fclose(fd);
    
    //////////////////////////////
	struct remove {
        char isbn[14];
    } vet_r[5] = {{"1111111111111"},
                {"7777777777777"},
                {"3333333333333"},
                {"4444444444444"},
                {"8888888888888"},
                };
       
    fd = fopen("remove.bin", "w+b");
    fwrite(vet_r, sizeof(vet_r), 1, fd);
    fclose(fd);
}

