#include <stdio.h>
#include <stdlib.h>

struct Pessoa {
    char nome[50];
    int idade;
    float altura;
};

int compararAltura(const void *a, const void *b) {
    const struct Pessoa *p1 = (const struct Pessoa *)a;
    const struct Pessoa *p2 = (const struct Pessoa *)b;
    
    if (p1->altura < p2->altura) {
        return -1;
    } else if (p1->altura > p2->altura) {
        return 1;
    }
    return 0;
}

int main() {
    // Abrir o arquivo para leitura
    FILE *arquivo = fopen("data.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo original.\n");
        return 1;
    }

    int capacidade = 10; // Capacidade inicial da array
    int tamanho = 0;     // Número atual de elementos na array
    struct Pessoa *pessoas = (struct Pessoa *)malloc(capacidade * sizeof(struct Pessoa));

    if (pessoas == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    while (1) {
        // Verificar se o array está cheio
        if (tamanho >= capacidade) {
            capacidade *= 2;
            pessoas = (struct Pessoa *)realloc(pessoas, capacidade * sizeof(struct Pessoa));
            if (pessoas == NULL) {
                printf("Erro ao realocar memória.\n");
                return 1;
            }
        }

        // Ler os dados do arquivo e armazenar na struct
        if (fscanf(arquivo, "%s", pessoas[tamanho].nome) == EOF) {
            break;
        }
        fscanf(arquivo, "%d", &pessoas[tamanho].idade);
        fscanf(arquivo, "%f", &pessoas[tamanho].altura);

        tamanho++;
    }

    fclose(arquivo);

    // Ordenar as pessoas por altura
    qsort(pessoas, tamanho, sizeof(struct Pessoa), compararAltura);

    // Imprimir os dados lidos da struct
    for (int i = 0; i < tamanho; i++) {
        printf("Pessoa %d:\n", i + 1);
        printf("Nome: %s\n", pessoas[i].nome);
        printf("Idade: %d\n", pessoas[i].idade);
        printf("Altura: %.2f\n", pessoas[i].altura);
        printf("\n");
    }


    // Colocar os dados ordenados em outro arquivo
    FILE *arquivoOrdenado = fopen("dadosOrdenados.txt", "w");
    if (arquivoOrdenado == NULL) {
        printf("Erro ao abrir o arquivo novo.\n");
        return 1;
    }

    for(int i = 0; i < tamanho; i++) {
        fprintf(arquivoOrdenado, "%s\n", pessoas[i].nome);
        fprintf(arquivoOrdenado, "%d\n", pessoas[i].idade);
        fprintf(arquivoOrdenado, "%.2f\n", pessoas[i].altura);
    }

    return 0;
}
