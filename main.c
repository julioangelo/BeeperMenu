#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITENS 7

struct ItemMenu {
    char *nome;
    float preco;
};

struct Pedido {
    int numero_pedido;
    struct ItemMenu item;
    int quantidade;
    float preco_total;
    struct Pedido *prox;
};

struct FilaPedido {
    struct Pedido *inicio;
    struct Pedido *fim;
};

struct Categoria {
    char *nome;
    struct ItemMenu itens[MAX_ITENS]; 
    int num_itens;
};

struct MenuRestaurante {
    struct Categoria categorias[3];
    struct FilaPedido fila_pedidos;
};

void exibir_menu_inicial() {
    printf("Escolha uma categoria:\n");
    printf("1. Populares\n");
    printf("2. Entradas\n");
    printf("3. Acompanhamentos\n");
}

void exibir_itens_categoria(struct Categoria *categoria) {
    printf("%s:\n", categoria->nome);
    for (int i = 0; i < categoria->num_itens; i++) {
        if (categoria->itens[i].nome != NULL) {
            printf("%d: %s - R$ %.2f\n", i + 1, categoria->itens[i].nome, categoria->itens[i].preco);
        }
    }
}

void fazer_pedido(struct MenuRestaurante *menu) {
    int opcao_continuar;
    do {
        exibir_menu_inicial();
        int opcao_categoria;
        scanf("%d", &opcao_categoria);
        getchar(); 

        if (opcao_categoria < 1 || opcao_categoria > 3) {
            printf("Opção inválida. Tente novamente.\n");
            continue;
        }

        struct Categoria *categoria_selecionada = &menu->categorias[opcao_categoria - 1];

        exibir_itens_categoria(categoria_selecionada);
        int opcao_item;
        printf("Escolha um item:\n");
        scanf("%d", &opcao_item);
        getchar(); 

        if (opcao_item < 1 || opcao_item > categoria_selecionada->num_itens) {
            printf("Opção inválida. Tente novamente.\n");
            continue;
        }

        struct ItemMenu *item_selecionado = &categoria_selecionada->itens[opcao_item - 1];

        int quantidade;
        printf("Quantos %s você deseja? ", item_selecionado->nome);
        scanf("%d", &quantidade);
        getchar(); 

        int numero_mesa;
        printf("Informe o número da mesa: ");
        scanf("%d", &numero_mesa);
        getchar(); 

        struct Pedido *novo_pedido = (struct Pedido *)malloc(sizeof(struct Pedido));
        if (novo_pedido == NULL) {
            printf("Erro ao alocar memória para o novo pedido.\n");
            return;
        }

        novo_pedido->numero_pedido = numero_mesa;
        novo_pedido->item = *item_selecionado;
        novo_pedido->quantidade = quantidade;
        novo_pedido->preco_total = quantidade * item_selecionado->preco;
        novo_pedido->prox = NULL;

        if (menu->fila_pedidos.inicio == NULL) {
            menu->fila_pedidos.inicio = novo_pedido;
            menu->fila_pedidos.fim = novo_pedido;
        } else {
            menu->fila_pedidos.fim->prox = novo_pedido;
            menu->fila_pedidos.fim = novo_pedido;
        }

        printf("Pedido realizado com sucesso!\n");

        printf("Deseja pedir mais alguma coisa? (1 - Sim, 0 - Não): ");
        scanf("%d", &opcao_continuar);
        getchar();

    } while (opcao_continuar);
}

void exibir_proximo_pedido(struct MenuRestaurante *menu) {
    printf("\nPróximo Pedido na fila:\n");
    if (menu->fila_pedidos.inicio == NULL) {
        printf("Nenhum pedido na fila.\n");
        return;
    }

    struct Pedido *proximo_pedido = menu->fila_pedidos.inicio;
    printf("Número do Pedido (Mesa): %d\n", proximo_pedido->numero_pedido);
    printf("Item: %s\n", proximo_pedido->item.nome);
    printf("Quantidade: %d\n", proximo_pedido->quantidade);
    printf("Preço Total: R$ %.2f\n", proximo_pedido->preco_total);
}

void deletar_e_exibir_proximo_pedido(struct MenuRestaurante *menu) {
    if (menu->fila_pedidos.inicio == NULL) {
        printf("Nenhum pedido para deletar.\n");
        return;
    }

    struct Pedido *primeiro_pedido = menu->fila_pedidos.inicio;
    menu->fila_pedidos.inicio = primeiro_pedido->prox;
    free(primeiro_pedido);

    printf("Pedido deletado com sucesso!\n");

    // Após deletar, exibe o próximo pedido na fila
    exibir_proximo_pedido(menu);
}

int main() {
    struct MenuRestaurante menu = {
        {{"Populares", {
            {"Rodízio p/ pessoa", 60},
            {"Combo Skin /30 pçs", 30},
            {"Hot Roll Salmão", 12},
            {"Yakissoba Clássico", 18},
            {"Combinado de Sushi", 40},
            {"Sashimi", 25},
            {"Temaki", 15}
        }, 7},
         {"Entradas", {
            {"Ceviche", 25},
            {"Salmão Flambado", 18},
            {"Sassake de Polvo", 20},
            {"Gyoza", 15},
            {"Sunomono", 23},
            {"Rolo Primaveraa", 19}

         }, 6},
         {"Acompanhamentos", {
            {"Tempurá", 15},
            {"Yakimeshi", 10},
            {"Arroz (Frito ou Gohan)", 5},
            {"Batata Frita", 15}

         }, 4}},
        {NULL, NULL}
    };

    int opcao;
    do {
        printf("\nOpções:\n");
        printf("1. Fazer Pedido\n");
        printf("2. Exibir Próximo Pedido\n");
        printf("3. Deletar Pedido\n");
        printf("0. Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);
        getchar(); 

        switch (opcao) {
            case 1:
                fazer_pedido(&menu);
                break;
            case 2:
                exibir_proximo_pedido(&menu);
                break;
            case 3:
                deletar_e_exibir_proximo_pedido(&menu);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);


    struct Pedido *temp = menu.fila_pedidos.inicio;
    while (temp != NULL) {
        struct Pedido *prox = temp->prox;
        free(temp);
        temp = prox;
    }

    return 0;
}
