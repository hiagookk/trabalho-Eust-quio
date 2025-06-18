#include <stdio.h>
#include <stdlib.h>
#include "processo.h"

int main() {
    Processo* processos = NULL;
    int quantidade;

    if (!carregar_processos("processo_043_202409032338.csv", &processos, &quantidade)) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    ordenar_por_id(processos, quantidade);
    salvar_em_csv("processo_043_202409032338.csv", processos, quantidade);

    ordenar_por_data(processos, quantidade);
    salvar_em_csv("processo_043_202409032338.csv", processos, quantidade);

    int classe_escolhida = 12377;
    int total_por_classe = contar_por_classe(processos, quantidade, classe_escolhida);
    printf("Quantidade com id_classe %d: %d\n", classe_escolhida, total_por_classe);

    int total_assuntos_unicos = contar_assuntos_unicos(processos, quantidade);
    printf("Assuntos unicos na base: %d\n", total_assuntos_unicos);

    listar_processos_multiplos_assuntos(processos, quantidade);

    if (quantidade > 0) {
        int dias = calcular_dias_em_tramitacao(processos[0].data_ajuizamento);
        printf("Dias em tramitacao do primeiro processo: %d dias\n", dias);
    }

    printf("\nPressione ENTER para sair...");
    getchar();
    getchar();

    free(processos);
    return 0;
}
