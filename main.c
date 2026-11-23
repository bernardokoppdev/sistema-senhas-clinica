#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TAM_MAX 100

struct PacienteSenha {
    char codigo[10];
    int prioridade; /* 3 = urgente, 2 = preferencial, 1 = normal */
    char genero;
    int idade;
    int gestante;
    int lactante;
    int deficiente;
    int obeso;
};

struct Fila {
    struct PacienteSenha senhas[TAM_MAX];
    int inicio;
    int fim;
    int qtd;
    int proxNumero;
};

/* Funcoes do sistema */
void limparEntrada(void);
int lerNumero(char *texto, int min, int max);
char lerSexo(void);
int definirPrioridade(char genero, int idade, int gestante, int lactante, int deficiente, int obeso);
void montarCodigo(int prioridade, int numero, char *saida);
void inserirNaFila(struct Fila *f, struct PacienteSenha p);
int retirarDaFila(struct Fila *f, struct PacienteSenha *p);
int verProximo(struct Fila *f, struct PacienteSenha *p);
void cadastrar(struct Fila *urg, struct Fila *pref, struct Fila *norm);
void consultarFila(struct Fila *urg, struct Fila *pref, struct Fila *norm);
void chamarProximo(struct Fila *urg, struct Fila *pref, struct Fila *norm, struct PacienteSenha *atual, int *emAtendimento);
void encerrarAtendimento(struct PacienteSenha *atual, int *emAtendimento);
char* textoPrioridade(int p);
void mostrarPainel(struct Fila *urg, struct Fila *pref, struct Fila *norm, struct PacienteSenha atual, int emAtendimento);
void mostrarMenu(void);

int main(void) {
    struct Fila filaUrgente = { .inicio = 0, .fim = 0, .qtd = 0, .proxNumero = 1 };
    struct Fila filaPref = { .inicio = 0, .fim = 0, .qtd = 0, .proxNumero = 1 };
    struct Fila filaNormal = { .inicio = 0, .fim = 0, .qtd = 0, .proxNumero = 1 };

    struct PacienteSenha senhaAtual;
    int emAtendimento = 0;
    int escolha = -1;

    strcpy(senhaAtual.codigo, "NENHUMA");

    while (escolha != 0) {
        mostrarPainel(&filaUrgente, &filaPref, &filaNormal, senhaAtual, emAtendimento);
        mostrarMenu();
        escolha = lerNumero("Opcao: ", 0, 5);

        if (escolha == 1) {
            cadastrar(&filaUrgente, &filaPref, &filaNormal);
        } else if (escolha == 2) {
            consultarFila(&filaUrgente, &filaPref, &filaNormal);
        } else if (escolha == 3) {
            chamarProximo(&filaUrgente, &filaPref, &filaNormal, &senhaAtual, &emAtendimento);
        } else if (escolha == 4) {
            encerrarAtendimento(&senhaAtual, &emAtendimento);
        } else if (escolha == 5) {
            printf("\nPainel atualizado.\n");
        } else if (escolha == 0) {
            printf("\nSaindo do sistema...\n");
        }
    }

    return 0;
}

void limparEntrada(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

int lerNumero(char *texto, int min, int max) {
    int num;
    while (1) {
        printf("%s", texto);
        if (scanf("%d", &num) == 1 && num >= min && num <= max) {
            limparEntrada();
            return num;
        }
        printf("Valor invalido. Digite entre %d e %d.\n", min, max);
        limparEntrada();
    }
}

char lerSexo(void) {
    char sexo;
    while (1) {
        printf("Genero (M/F): ");
        if (scanf(" %c", &sexo) == 1) {
            limparEntrada();
            sexo = (char)toupper((unsigned char)sexo);
            if (sexo == 'M' || sexo == 'F') {
                return sexo;
            }
        }
        printf("Digite apenas M ou F.\n");
    }
}

int definirPrioridade(char genero, int idade, int gestante, int lactante, int deficiente, int obeso) {
    if (idade > 80) {
        return 3;
    }
    if (gestante == 1 || lactante == 1 || deficiente == 1 || obeso == 1) {
        return 2;
    }
    if ((genero == 'F' && idade >= 60) || (genero == 'M' && idade >= 65)) {
        return 2;
    }
    return 1;
}

void montarCodigo(int prioridade, int numero, char *saida) {
    if (prioridade == 3) {
        sprintf(saida, "U%03d", numero);
    } else if (prioridade == 2) {
        sprintf(saida, "P%03d", numero);
    } else {
        sprintf(saida, "N%03d", numero);
    }
}

void inserirNaFila(struct Fila *f, struct PacienteSenha p) {
    if (f->qtd == TAM_MAX) {
        return;
    }
    f->senhas[f->fim] = p;
    f->fim = (f->fim + 1) % TAM_MAX;
    f->qtd++;
}

int retirarDaFila(struct Fila *f, struct PacienteSenha *p) {
    if (f->qtd == 0) {
        return 0;
    }
    *p = f->senhas[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_MAX;
    f->qtd--;
    return 1;
}

int verProximo(struct Fila *f, struct PacienteSenha *p) {
    if (f->qtd == 0) {
        return 0;
    }
    *p = f->senhas[f->inicio];
    return 1;
}

void cadastrar(struct Fila *urg, struct Fila *pref, struct Fila *norm) {
    struct PacienteSenha novo;

    printf("\n--- Novo Cadastro ---\n");
    novo.genero = lerSexo();
    novo.idade = lerNumero("Idade: ", 0, 130);

    if (novo.genero == 'F' && novo.idade >= 10 && novo.idade <= 60) {
        novo.gestante = lerNumero("Gestante (1-Sim / 0-Nao): ", 0, 1);
        novo.lactante = lerNumero("Lactante (1-Sim / 0-Nao): ", 0, 1);
    } else {
        novo.gestante = 0;
        novo.lactante = 0;
    }

    novo.deficiente = lerNumero("Necessidade especial (1-Sim / 0-Nao): ", 0, 1);
    novo.obeso = lerNumero("Obesidade (1-Sim / 0-Nao): ", 0, 1);

    novo.prioridade = definirPrioridade(
        novo.genero, novo.idade, novo.gestante,
        novo.lactante, novo.deficiente, novo.obeso
    );

    if (novo.prioridade == 3) {
        if (urg->qtd >= TAM_MAX) {
            printf("Fila de urgentes cheia.\n");
            return;
        }
        montarCodigo(3, urg->proxNumero++, novo.codigo);
        inserirNaFila(urg, novo);
    } else if (novo.prioridade == 2) {
        if (pref->qtd >= TAM_MAX) {
            printf("Fila preferencial cheia.\n");
            return;
        }
        montarCodigo(2, pref->proxNumero++, novo.codigo);
        inserirNaFila(pref, novo);
    } else {
        if (norm->qtd >= TAM_MAX) {
            printf("Fila normal cheia.\n");
            return;
        }
        montarCodigo(1, norm->proxNumero++, novo.codigo);
        inserirNaFila(norm, novo);
    }

    printf("\nSenha gerada: %s (%s)\n", novo.codigo, textoPrioridade(novo.prioridade));
}

void consultarFila(struct Fila *urg, struct Fila *pref, struct Fila *norm) {
    struct PacienteSenha prox;

    if (verProximo(urg, &prox) || verProximo(pref, &prox) || verProximo(norm, &prox)) {
        printf("\nProxima senha na fila: %s - %s\n", prox.codigo, textoPrioridade(prox.prioridade));
    } else {
        printf("\nNao ha senhas na fila no momento.\n");
    }
}

void chamarProximo(struct Fila *urg, struct Fila *pref, struct Fila *norm, struct PacienteSenha *atual, int *emAtendimento) {
    if (*emAtendimento) {
        printf("\nJa existe uma senha em atendimento (%s). Finalize primeiro.\n", atual->codigo);
        return;
    }

    int chamou = 0;
    if (urg->qtd > 0) {
        chamou = retirarDaFila(urg, atual);
    } else if (pref->qtd > 0) {
        chamou = retirarDaFila(pref, atual);
    } else if (norm->qtd > 0) {
        chamou = retirarDaFila(norm, atual);
    }

    if (chamou) {
        *emAtendimento = 1;
        printf("\nChamando senha: %s (%s)\n", atual->codigo, textoPrioridade(atual->prioridade));
    } else {
        printf("\nFila vazia, ninguem para chamar.\n");
    }
}

void encerrarAtendimento(struct PacienteSenha *atual, int *emAtendimento) {
    if (!(*emAtendimento)) {
        printf("\nNenhum paciente esta em atendimento agora.\n");
        return;
    }

    printf("\nAtendimento da senha %s finalizado.\n", atual->codigo);
    *emAtendimento = 0;
    strcpy(atual->codigo, "NENHUMA");
}

char* textoPrioridade(int p) {
    if (p == 3) return "Urgente";
    if (p == 2) return "Preferencial";
    return "Normal";
}

void mostrarPainel(struct Fila *urg, struct Fila *pref, struct Fila *norm, struct PacienteSenha atual, int emAtendimento) {
    printf("\n==========================================\n");
    printf("          PAINEL DE ATENDIMENTO\n");
    printf("==========================================\n");

    if (emAtendimento) {
        printf("Em atendimento: %s (%s)\n", atual.codigo, textoPrioridade(atual.prioridade));
    } else {
        printf("Em atendimento: NENHUMA\n");
    }

    printf("Aguardando -> Urgente: %d | Preferencial: %d | Normal: %d\n",
           urg->qtd, pref->qtd, norm->qtd);

    printf("Proximas senhas: ");
    int temAlguem = 0;

    for (int i = 0; i < urg->qtd; i++) {
        int pos = (urg->inicio + i) % TAM_MAX;
        printf("%s ", urg->senhas[pos].codigo);
        temAlguem = 1;
    }
    for (int i = 0; i < pref->qtd; i++) {
        int pos = (pref->inicio + i) % TAM_MAX;
        printf("%s ", pref->senhas[pos].codigo);
        temAlguem = 1;
    }
    for (int i = 0; i < norm->qtd; i++) {
        int pos = (norm->inicio + i) % TAM_MAX;
        printf("%s ", norm->senhas[pos].codigo);
        temAlguem = 1;
    }

    if (!temAlguem) {
        printf("nenhuma");
    }
    printf("\n==========================================\n");
}

void mostrarMenu(void) {
    printf("\n1 - Cadastrar paciente\n");
    printf("2 - Consultar proxima senha\n");
    printf("3 - Chamar proximo\n");
    printf("4 - Finalizar atendimento\n");
    printf("5 - Atualizar painel\n");
    printf("0 - Sair\n");
}
