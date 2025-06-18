#ifndef PROCESSO_H
#define PROCESSO_H

#define MAX_STRING 100
#define MAX_ASSUNTOS 10

#include <time.h>

typedef struct {
    int id;
    char numero[MAX_STRING];
    char data_ajuizamento[MAX_STRING];
    int id_classe;
    int id_assunto[MAX_ASSUNTOS];
    int num_assuntos;
    int ano_eleicao;
} Processo;

int carregar_processos(const char* nome_arquivo, Processo** processos, int* qtd);
void ordenar_por_id(Processo processos[], int qtd);
void ordenar_por_data(Processo processos[], int qtd);
int contar_por_classe(Processo processos[], int qtd, int classe);
int contar_assuntos_unicos(Processo processos[], int qtd);
void listar_processos_multiplos_assuntos(Processo processos[], int qtd);
int calcular_dias_em_tramitacao(const char* data);
void salvar_em_csv(const char* nome_arquivo, Processo processos[], int qtd);

#endif