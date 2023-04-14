#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    false, true
} bool;

typedef struct No_aux{
    struct No_aux **filhos;
    int *chaves;
    int qtdChaves;
    bool folha, raiz;
}No;

typedef struct {
    int grauMin;
    No *raiz;
} Arvore;

No *criaNo(int t) {
    No *novoNo =(No*) malloc(sizeof(No));
    novoNo->chaves = (int *)malloc(sizeof(int) * (t * 2));
    novoNo->filhos = (No **) malloc(sizeof(No*) * (t*2+1));
    novoNo->qtdChaves = 0;
    novoNo->folha = true;
    novoNo->raiz = true;
    return novoNo;
}

Arvore criaArvore(int t) {
    Arvore arvore;
    arvore.raiz = criaNo(t);
    arvore.grauMin = t;
    return arvore;
}

void imprimeNo(No *no) {
    printf("(");
    int i;
    for(i = 0; i < no->qtdChaves; i++) {
        printf("%i", no->chaves[i]);
        if(i != no->qtdChaves-1) 
            printf(" ");
    }
    printf(")");
}

void imprimeArvore(No *noAtual) {
    if(noAtual->folha == true) {
        imprimeNo(noAtual);
    } else {
        int i;
        printf("(");
        for(i=0; i <= noAtual->qtdChaves; i++) {
            imprimeArvore(noAtual->filhos[i]);
            if(i < noAtual->qtdChaves) {
                printf(" %i ", noAtual->chaves[i]);
            }
        }
        printf(")");
    }
}

void buscaChave (No* noAtual, int chave) {
    int i;

    if(noAtual->folha == true) {
        for(i = 0; i < noAtual->qtdChaves; i++) {
            if(noAtual->chaves[i] == chave) {
                printf("%i encontrado\n", chave);
                return;
            } 
        }
        printf("%i nao encontrado\n", chave);
    } else {
        for(i = 0; i < noAtual->qtdChaves; i++) {
            if(chave <= noAtual->chaves[i]) {
                if(chave == noAtual->chaves[i]) {
                    printf("%i encontrado\n", chave);
                    return;
                } else {
                    buscaChave(noAtual->filhos[i], chave);
                    break;
                }
            }
            if(i == noAtual->qtdChaves-1) {
                buscaChave(noAtual->filhos[noAtual->qtdChaves], chave);
            }
        }
    }
}

void split(No*noPai, No *noFilho, int indice, int grauMin) {
    No* novoNoIrmao = criaNo(grauMin);
    novoNoIrmao->raiz = false;
    novoNoIrmao->folha = noFilho->folha;

    int i;
    for(i=0; i < grauMin; i++) {
        novoNoIrmao->chaves[i] = noFilho->chaves[grauMin+i];
        novoNoIrmao->qtdChaves++;
        noFilho->qtdChaves--;
    }

    if(novoNoIrmao->folha == false) {
        int i;
        for(i=0; i <= grauMin; i++) {
            novoNoIrmao->filhos[i] = noFilho->filhos[i+grauMin];
        }
    }

    for(i=noPai->qtdChaves; i > indice; i--) {
        noPai->chaves[i] = noPai->chaves[i-1];
    }
    noPai->chaves[i] = noFilho->chaves[grauMin-1];
    noPai->qtdChaves++;
    noFilho->qtdChaves--;

    for(i=noPai->qtdChaves-1; i > indice; i--) {
        noPai->filhos[i+1] = noPai->filhos[i];
    }    
    noPai->filhos[indice] = noFilho;
    noPai->filhos[i+1] = novoNoIrmao;
}

void insereChaveRecursivo (No *noAtual, int chave, int grauMin) {
    if(noAtual->folha == true) {
        int i;
        for(i=0; i < noAtual->qtdChaves; i++) {
            if(noAtual->chaves[i] == chave) {
                return;
            }
        }
        for(i=noAtual->qtdChaves; i > 0; i--) {
            if(noAtual->chaves[i-1] > chave) {
                noAtual->chaves[i] = noAtual->chaves[i-1];
            } else {
                break;
            }
        }
        noAtual->chaves[i] = chave;
        noAtual->qtdChaves++;

    } else {
        int i=0;
        while(i < noAtual->qtdChaves) {
            if(chave == noAtual->chaves[i]){
                return;
            } else if(chave < noAtual->chaves[i]){
                break;
            } 
            i++;
        }

        insereChaveRecursivo(noAtual->filhos[i], chave, grauMin);
        if(noAtual->filhos[i]->qtdChaves == grauMin*2) {
            split(noAtual, noAtual->filhos[i], i, grauMin);
        }
    }
}

void insereChave(Arvore *arvore, int chave) {
    
    insereChaveRecursivo(arvore->raiz, chave, arvore->grauMin);
    
    if(arvore->raiz->qtdChaves == arvore->grauMin*2) {
         No *novoNoPai = criaNo(arvore->grauMin);
        novoNoPai->folha = false;
        arvore->raiz->raiz = false;

        split(novoNoPai, arvore->raiz, 0, arvore->grauMin);
        arvore->raiz = novoNoPai;
    }
}

void libera(No* noAtual) {

    if(noAtual->folha == true) {
        free(noAtual->chaves);
        free(noAtual->filhos);
        free(noAtual);
    } else {
        int i;
        for(i=0; i <= noAtual->qtdChaves; i++) {
            libera(noAtual->filhos[i]);
        }
        // noAtual->folha = true;
        // libera(noAtual);

        free(noAtual->filhos);
        free(noAtual->chaves);
        free(noAtual);
    }
}

int main (int argc, char *argv[]) {
    char s[100];
    int grauMin, valor;
    Arvore arvore;

    scanf("%i", &grauMin);

    arvore = criaArvore(grauMin);
    while(scanf("%s", s) == 1) {
        if(strcmp(s, "insere") == 0) {
            scanf("%i", &valor);
            if(valor != 0) {
                insereChave(&arvore, valor);
            } 
        }

        if(strcmp(s, "busca") == 0) {
            scanf("%i", &valor);
            buscaChave(arvore.raiz, valor);

        }
        if(strcmp(s, "imprime_arvore") == 0) {
            imprimeArvore(arvore.raiz);
            printf("\n");
        }
    }

    libera(arvore.raiz);
    return 0;
}