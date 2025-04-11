#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "processo.h"

int carregar_processos(const char* nome_arquivo, Processo** processos, int* qtd) {
    FILE* arq = fopen(nome_arquivo, "r");
    if (!arq) return 0;

    char linha[512];
    fgets(linha, sizeof(linha), arq);

    int capacidade = 100;
    int i = 0;
    *processos = malloc(capacidade * sizeof(Processo));
    if (*processos == NULL) return 0;

    while (fgets(linha, sizeof(linha), arq)) {
        if (i >= capacidade) {
            capacidade *= 2;
            *processos = realloc(*processos, capacidade * sizeof(Processo));
            if (*processos == NULL) return 0;
        }

        Processo p;
        char* campo;

        campo = strtok(linha, ",");
        p.id = atoi(campo);

        campo = strtok(NULL, ",");
        strcpy(p.numero, campo);

        campo = strtok(NULL, ",");
        strcpy(p.data_ajuizamento, campo);

        campo = strtok(NULL, ",{}\n");
        p.id_classe = atoi(campo);

        char* assuntos_brutos = strtok(NULL, "}");
        p.num_assuntos = 0;

        if (assuntos_brutos != NULL) {
            if (assuntos_brutos[0] == '{') assuntos_brutos++;

            char* assunto = strtok(assuntos_brutos, ",");
            while (assunto && p.num_assuntos < MAX_ASSUNTOS) {
                p.id_assunto[p.num_assuntos++] = atoi(assunto);
                assunto = strtok(NULL, ",");
            }
        }

        campo = strtok(NULL, ",\n");
        p.ano_eleicao = campo ? atoi(campo) : 0;

        (*processos)[i++] = p;
    }

    fclose(arq);
    *qtd = i;
    return 1;
}

void ordenar_por_id(Processo processos[], int qtd) {
    for (int i = 0; i < qtd - 1; i++) {
        for (int j = i + 1; j < qtd; j++) {
            if (processos[i].id > processos[j].id) {
                Processo aux = processos[i];
                processos[i] = processos[j];
                processos[j] = aux;
            }
        }
    }
}

void ordenar_por_data(Processo processos[], int qtd) {
    for (int i = 0; i < qtd - 1; i++) {
        for (int j = i + 1; j < qtd; j++) {
            if (strcmp(processos[i].data_ajuizamento, processos[j].data_ajuizamento) < 0) {
                Processo aux = processos[i];
                processos[i] = processos[j];
                processos[j] = aux;
            }
        }
    }
}

int contar_por_classe(Processo processos[], int qtd, int classe) {
    int total = 0;
    for (int i = 0; i < qtd; i++) {
        if (processos[i].id_classe == classe) total++;
    }
    return total;
}

int contar_assuntos_unicos(Processo processos[], int qtd) {
    int lista[10000] = {0};
    int total = 0;
    for (int i = 0; i < qtd; i++) {
        for (int j = 0; j < processos[i].num_assuntos; j++) {
            int atual = processos[i].id_assunto[j];
            int ja_tem = 0;
            for (int k = 0; k < total; k++) {
                if (lista[k] == atual) {
                    ja_tem = 1;
                    break;
                }
            }
            if (!ja_tem) lista[total++] = atual;
        }
    }
    return total;
}

void listar_processos_multiplos_assuntos(Processo processos[], int qtd) {
    for (int i = 0; i < qtd; i++) {
        if (processos[i].num_assuntos > 1) {
            printf("Processo com mais de um assunto - ID: %d, Numero: %s\n", processos[i].id, processos[i].numero);
        }
    }
}

int calcular_dias_em_tramitacao(const char* data) {
    struct tm dt = {0};
    sscanf(data, "%d-%d-%d %d:%d:%d",
           &dt.tm_year, &dt.tm_mon, &dt.tm_mday,
           &dt.tm_hour, &dt.tm_min, &dt.tm_sec);

    dt.tm_year -= 1900;
    dt.tm_mon -= 1;

    time_t t1 = mktime(&dt);
    time_t agora = time(NULL);
    return (int)(difftime(agora, t1) / (60 * 60 * 24));
}

void salvar_em_csv(const char* nome_arquivo, Processo processos[], int qtd) {
    FILE* f = fopen(nome_arquivo, "w");
    if (!f) return;
    fprintf(f, "id,numero,data_ajuizamento,id_classe,id_assunto,ano_eleicao\n");
    for (int i = 0; i < qtd; i++) {
        fprintf(f, "%d,%s,%s,%d,{", processos[i].id, processos[i].numero, processos[i].data_ajuizamento, processos[i].id_classe);
        for (int j = 0; j < processos[i].num_assuntos; j++) {
            fprintf(f, "%d", processos[i].id_assunto[j]);
            if (j < processos[i].num_assuntos - 1) fprintf(f, ",");
        }
        fprintf(f, "},%d\n", processos[i].ano_eleicao);
    }
    fclose(f);
}