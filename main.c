#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAM 100
typedef struct {
    int n;
    int chaves[TAM];
    int filhos[TAM];
} nodo;

int NumeroSplitado=0;
int FilhoSplitado1=0;
int RaizAtual;
int Escritos=0, Lidos=0;

nodo N;

FILE *Arquivo;

int VerificaID(int ID){
    char Nome[10];
    snprintf(Nome,4,"%d",ID);
    strcat(Nome,".dat");
    Arquivo=fopen(Nome,"r");
    if(Arquivo==NULL){
        return 1;
    }
    fclose(Arquivo);
    return 2;
}

void troca(int *a, int *b){
    int aux=*a;
    *a=*b;
    *b=aux;
}

void InsertionSort (int x, int filho1) {
    int i = N.n;
    int aux, PosicaoSplit=0;
    if (N.n == 0)
        N.chaves[0] = x;

    else {
        N.chaves[N.n] = x;
        while (i > 0 && N.chaves[i] < N.chaves[i - 1]) {
            aux = N.chaves[i - 1];
            N.chaves[i - 1] = N.chaves[i];
            N.chaves[i] = aux;
            i--;
        }
    }
    if(N.n==0)
        N.filhos[1]=-1;
    else
        N.filhos[N.n+1]=filho1;

    for(i=0;i<=N.n;i++){
        if(N.chaves[i]==x){
            PosicaoSplit=i;
            break;
        }
    }
    for(i=PosicaoSplit;i<N.n+1;i++){
        troca(&N.filhos[i+1],&N.filhos[N.n+1]);
    }
}

void le(int ID){
    int i=0;
    char Nome[10];
    snprintf(Nome,4,"%d",ID);
    strcat(Nome,".dat");
    Arquivo=fopen(Nome,  "r");
    fscanf(Arquivo,"%d",&N.n);

    if(N.n!=0) {
        for (i = 0; i < N.n; i++)  //Le as chaves do arquivo
            fscanf(Arquivo, "%d", &N.chaves[i]);

        for (i = 0; i <= N.n; i++) //le os ids dos filhos
            fscanf(Arquivo, "%d", &N.filhos[i]);
    }
    printf("Lido (");  //printa quais chaves foram lidas
    for(i=0;i<N.n-1;i++)
        printf("%d ",N.chaves[i]);
    printf("%d)\n",N.chaves[N.n-1]);
    Lidos++;
    fclose(Arquivo);
}

void escreve(int ID, int Limite){
    int i=0;
    char Nome[10];
    snprintf(Nome,4,"%d",ID);
    strcat(Nome,".dat");
    Arquivo=fopen(Nome, "w");

    if(N.n!=0) {
        fprintf(Arquivo, "%d\n",N.n);
        for (i = 0; i < Limite; i++) //escreve as chaves
            fprintf(Arquivo, "%d ", N.chaves[i]);
        fprintf(Arquivo, "\n");

        for (i = 0; i <= Limite; i++) //escreve os ids dos filhos
            fprintf(Arquivo, "%d ", N.filhos[i]);

        printf("Escrito (");
        for(i=0;i<N.n-1;i++) //printa as novas chaves escritas
            printf("%d ",N.chaves[i]);
        printf("%d)\n",N.chaves[N.n-1]);

    } else{
        fprintf(Arquivo, "0\n");
        fprintf(Arquivo, "0\n");
        fprintf(Arquivo, "-1");
        printf("Escrito ()\n");
    }
    Escritos++;
    fclose(Arquivo);
}

void Split(int IDantigo){
    int NomeDir, i, j=0;
    int aux=0, aux2=N.n;
    do {
        NomeDir = rand() % 999 + 1;
    }while (VerificaID(NomeDir)!=1);

    if(N.n%2==0){
        aux=((N.n)/2)-1;
        N.n=aux;
        escreve(IDantigo, aux);
        N.n=aux2;
        NumeroSplitado=N.chaves[(N.n/2)-1];
        for (i = (N.n/2); i < N.n; ++i) {
            troca(&N.chaves[j],&N.chaves[i]);
            j++;
        }
        j=0;
        for (i = (N.n/2); i <= N.n; ++i) {
            troca(&N.filhos[j],&N.filhos[i]);
            j++;
        }
        aux=N.n/2;
        N.n=aux;
        escreve(NomeDir, aux);
        N.n=aux2;
        FilhoSplitado1=NomeDir;

    } else{
        aux=((N.n)/2);
        N.n=aux;
        escreve(IDantigo, aux);
        NumeroSplitado=N.chaves[aux];
        N.n=aux2;
        for (i = ((N.n/2)+1); i < N.n; ++i) {
            troca(&N.chaves[j],&N.chaves[i]);
            j++;
        }
        j=0;
        for (i = (N.n/2)+1; i <= N.n; ++i) {
            troca(&N.filhos[j],&N.filhos[i]);
            j++;
        }
        aux=N.n/2;
        N.n=aux;
        escreve(NomeDir, aux);
        N.n=aux2;
        FilhoSplitado1=NomeDir;
    }
}

void insere(int ID, int x, int D){
    int i=0;
    le(ID);
    if(N.n==0){
        N.filhos[0]=-1;
        InsertionSort(x,-1);
        N.n=1;
        escreve(ID,1);
    }
    else if(N.filhos[0]==-1){
        if(N.n<D-1){
            InsertionSort(x,-1);
            N.n++;
            escreve(ID,N.n);
        } else{
            InsertionSort(x,-1);
            N.n++;
            Split(ID);
        }
    } else {
        if (x < N.chaves[0]) {
            insere(N.filhos[0], x, D);
        } else if (x > N.chaves[(N.n) - 1]){
            insere(N.filhos[N.n], x, D);
        }
        else {
            for (i = 0; i < ((N.n) - 1); i++) {
                if (x > N.chaves[i] && x < N.chaves[i + 1]) {
                    insere(N.filhos[i + 1], x, D);
                    break;
                }
            }
        }
        if (NumeroSplitado!=0){
            le(ID);
            if(N.n<D-1){
                InsertionSort(NumeroSplitado,FilhoSplitado1);
                N.n++;
                escreve(ID,N.n);
                NumeroSplitado=0;
            } else{
                InsertionSort(NumeroSplitado,FilhoSplitado1);
                N.n++;
                Split(ID);
            }
        }
    }
}

int busca(int ID, int x){
    int i;
        le(ID);
        for(i=0;i<N.n;i++){
            if(N.chaves[i]==x){
                return 1;
            }
        }
    if(N.filhos[0]==-1)
        return 3;
    else if (x < N.chaves[0]) {
        return busca(N.filhos[0],x);
    } else if (x > N.chaves[(N.n) - 1]){
        return busca(N.filhos[N.n], x);
    }
    else {
        for (i = 0; i < ((N.n) - 1); i++) {
            if (x > N.chaves[i] && x < N.chaves[i + 1]) {
                return busca(N.filhos[i + 1], x);
            }
        }
    }
}

int main() {
    int D,Numero;
    int Nome;
    char Operacao[10];

    printf("Digite Grau Maximo: ");
    scanf("%d",&D);
    printf("Digite ID raiz: ");
    scanf("%d",&Nome);
    if(VerificaID(Nome)==1){
        N.n=0;
        escreve(Nome,0);
        printf("Criada raiz vazia em %d.dat\n",  Nome);
        RaizAtual=Nome;
        Escritos=0;
        Lidos=0;
    } else{
        le(Nome);
        printf("Raiz em %d.dat encontrada\n", Nome);
        RaizAtual=Nome;
        Escritos=0;
        Lidos=0;
    }

    printf("> ");
    scanf("%s", &Operacao);

    while (strcmp(Operacao,"fim")!=0){
        if(strcmp(Operacao,"insere")==0){
            scanf("%d",&Numero);
            insere(RaizAtual,Numero,D);
            if(NumeroSplitado!=0){
                N.n=1;
                N.chaves[0]=NumeroSplitado;
                N.filhos[0]=RaizAtual;
                N.filhos[1]=FilhoSplitado1;
                do {
                    Nome = rand() % 999 + 1;
                }while (VerificaID(Nome)!=1);
                escreve(Nome,1);
                NumeroSplitado=0;
                RaizAtual=Nome;
                printf("-- Nova raiz com ID %d --\n", Nome);
            }
            printf("%d leitura(s), %d escrita(s)\n",Lidos,Escritos);
            Escritos=0;
            Lidos=0;
        }else if(strcmp(Operacao,"busca")==0){
            scanf("%d",&Numero);
            int Busca=busca(RaizAtual,Numero);
            if (Busca==1){
                printf("%d Encontrado.\n", Numero);
                printf("%d leitura(s), %d escrita(s)\n",Lidos,Escritos);
                Escritos=0;
                Lidos=0;
            } else{
                printf("%d Nao Encontrado\n", Numero);
                printf("%d leitura(s), %d escrita(s)\n",Lidos,Escritos);
                Escritos=0;
                Lidos=0;
            }
        } else
            printf("Entrada Invalida\n");
        printf("> ");
        scanf("%s", &Operacao);
    }

    printf("\n\n VALEU PROF, BOM FINAL DE ANO PRA VOCE, ATÉ O ANO QUE VEM :)\n\n");
    printf("       _____________,--,\n"
           "      | | | | | | |/ .-.\\   HANG IN THERE\n"
           "      |_|_|_|_|_|_/ /   `.      SANTA\n"
           "       |_|__|__|_; |      \\\n"
           "       |___|__|_/| |     .'`}\n"
           "       |_|__|__/ | |   .'.'`\\\n"
           "       |__|__|/  ; ;  / /    \\.-\"-.\n"
           "       ||__|_;   \\ \\  ||    /`___. \\\n"
           "       |_|___/\\  /;.`,\\\\   {_'___.;{}\n"
           "       |__|_/ `;`__|`-.;|  |C` e e`\\\n"
           "       |___`L  \\__|__|__|  | `'-o-' }\n"
           "       ||___|\\__)___|__||__|\\   ^  /`\\\n"
           "       |__|__|__|__|__|_{___}'.__.`\\_.'}\n"
           "       ||___|__|__|__|__;\\_)-'`\\   {_.-;\n"
           "       |__|__|__|__|__|/` (`\\__/     '-'\n"
           "       |_|___|__|__/`      |\n"
           "-jgs---|__|___|__/`         \\-------------------\n"
           "-.__.-.|___|___;`            |.__.-.__.-.__.-.__\n"
           "  |     |     ||             |  |     |     |\n"
           "-' '---' '---' \\             /-' '---' '---' '--\n"
           "     |     |    '.        .' |     |     |     |\n"
           "'---' '---' '---' `-===-'`--' '---' '---' '---'\n"
           "  |     |     |     |     |     |     |     |\n"
           "-' '---' '---' '---' '---' '---' '---' '---' '--\n"
           "     |     |     |     |     |     |     |     |\n"
           "'---' '---' '---' '---' '---' '---' '---' '---'");

    printf("\n .;;,     .;;, \n"
           "`  ;;   ;;  ' \n"
           "   ;;   ;; ,  .;;;.   .;;,;;;,  .;;,;;;,  .;;.  .;;. \n"
           " ,;;;;;;;;;'  `   ;;  ` ;;   ;; ` ;;   ;; ` ;;  ;; ' \n"
           " ` ;;   ;;    .;;.;;    ;;   ;;   ;;   ;;   ;;  ;; \n"
           "   ;;   ;;    ;;  ;; ,  ;;   ;;   ;;   ;;   ;;  ;; \n"
           ".  ;;    ';;' `;;;';;'  ;;';;'    ;;';;'     `;;'; \n"
           "';;'                    ;;        ;;            ;; \n"
           "                     .  ;;     .  ;;         .  ;; \n"
           "                     ';;'      ';;'          ';;' \n"
           "\n"
           "               .;;, ,;;;, \n"
           "               `  ;;    ;; \n"
           "                  ;;    ;;     ,;;,  .;;.      .;;, \n"
           "                  ;;    ;;    ;;  ;; ` ;;      ;; ' \n"
           "                  ;;    ;;    ;;;;;'   ;;  ;;  ;; \n"
           "                  ;;    ;;    ;;   .   ;;  ;;  ;; \n"
           "               .  ;;     ';;'  `;;;'    `;;'`;;' \n"
           "               ';;' \n"
           "\n"
           "                              .;;.     .;;. \n"
           "                              `  ;;   ;;  ' \n"
           "                                 ;;   ;;   .;;,  .;;;.   .;;.;;;, \n"
           "                                 ;;   ;;  ;;  ;; '   ;;  ` ;;   ' \n"
           "                                 ;;   ;;  ;;;;;' .;;,;;    ;; \n"
           "                                  `;;;';  ;;   . ;;  ;; ,  ;; \n"
           "                                      ;;   `;;;'  `;;';;'  ;' \n"
           "                                      ;; \n"
           "                                  .'  ;; \n"
           "                                  ';;;' ");
    return 0;
}
