#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALBUMS 10
#define MAX_ARTISTS 100

typedef struct {
    char nome[100];
    char genero[50];
    char local[100];
    char albums[MAX_ALBUMS][100];
    int num_albums;
} Artista;

void salvar_artistas(char *nome_arquivo, Artista artistas[], int num_artistas) {
    FILE *arquivo = fopen("Dado_txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }
    for (int i = 0; i < num_artistas; i++) {
        fprintf(arquivo, "\n%s\n%s\n%s\n", artistas[i].nome, artistas[i].genero, artistas[i].local);
        for (int j = 0; j < artistas[i].num_albums; j++) {
            fprintf(arquivo, "%s\n", artistas[i].albums[j]);
        }
        fprintf(arquivo, "===========\n");
    }
    fclose(arquivo);
}

void ler_artistas(char *nome_arquivo, Artista artistas[], int *num_artistas) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }

    *num_artistas = 0;
    char linha[100];
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        if (strcmp(linha, "===========\n") == 0) {
            (*num_artistas)++;
        }
        linha[strcspn(linha, "\n")] = 0; // Remove o caractere da nova linha
        switch ((*num_artistas) % 4) {
            case 0:
                strcpy(artistas[*num_artistas / 4].nome, linha);
                break;
            case 1:
                strcpy(artistas[*num_artistas / 4].genero, linha);
                break;
            case 2:
                strcpy(artistas[*num_artistas / 4].local, linha);
                break;
            default:
                strcpy(artistas[*num_artistas / 4].albums[artistas[*num_artistas / 4].num_albums], linha);
                artistas[*num_artistas / 4].num_albums++;
                break;
        }
    }
    fclose(arquivo);
}

int comparar_artistas(const void *a, const void *b) {
    return strcmp(((Artista *)a)->nome, ((Artista *)b)->nome);
}

void inserir_artista(Artista artistas[], int *num_artistas) {
    if (*num_artistas >= MAX_ARTISTS) {
        printf("Limite de artistas atingidos.\n");
        return;
    }

    printf("Nome do artista: ");
    fgets(artistas[*num_artistas].nome, sizeof(artistas[*num_artistas].nome), stdin);
    artistas[*num_artistas].nome[strcspn(artistas[*num_artistas].nome, "\n")] = 0;

    printf("Gênero musical: ");
    fgets(artistas[*num_artistas].genero, sizeof(artistas[*num_artistas].genero), stdin);
    artistas[*num_artistas].genero[strcspn(artistas[*num_artistas].genero, "\n")] = 0;

    printf("Local de criação/nascimento: ");
    fgets(artistas[*num_artistas].local, sizeof(artistas[*num_artistas].local), stdin);
    artistas[*num_artistas].local[strcspn(artistas[*num_artistas].local, "\n")] = 0;

    printf("Álbuns (digite 'fim' para parar):\n");
    artistas[*num_artistas].num_albums = 0;
    char album[100];
    while (1) {
        fgets(album, sizeof(album), stdin);
        if (strcmp(album, "fim\n") == 0 || artistas[*num_artistas].num_albums >= MAX_ALBUMS)
            break;
        album[strcspn(album, "\n")] = 0;
        strcpy(artistas[*num_artistas].albums[artistas[*num_artistas].num_albums], album);
        artistas[*num_artistas].num_albums++;
    }
    (*num_artistas)++;
}

void removedor_artista(Artista artistas[], int *num_artistas) {
    char nome[100];
    printf("Digite o nome do artista que deseja remover: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;

    int encontrado = 0;
    for (int i = 0; i < *num_artistas; i++) {
        if (strcmp(artistas[i].nome, nome) == 0) {
            encontrado = 1;
            for (int j = i; j < *num_artistas - 1; j++) {
                artistas[j] = artistas[j + 1];
            }
            (*num_artistas)--;
            break;
        }
    }
    if (!encontrado) {
        printf("Artista não encontrado.\n");
    }
}

void editar_artista(Artista artistas[], int num_artistas) {
    char nome[100];
    printf("Digite o nome do artista que deseja editar: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;

    int encontrado = 0;
    for (int i = 0; i < num_artistas; i++) {
        if (strcmp(artistas[i].nome, nome) == 0) {
            encontrado = 1;
            printf("Novo nome do artista: ");
            fgets(artistas[i].nome, sizeof(artistas[i].nome), stdin);
            artistas[i].nome[strcspn(artistas[i].nome, "\n")] = 0;

            printf("Novo gênero musical: ");
            fgets(artistas[i].genero, sizeof(artistas[i].genero), stdin);
            artistas[i].genero[strcspn(artistas[i].genero, "\n")] = 0;

            printf("Novo local de criação/nascimento: ");
            fgets(artistas[i].local, sizeof(artistas[i].local), stdin);
            artistas[i].local[strcspn(artistas[i].local, "\n")] = 0;

            printf("Novos álbuns (digite 'fim' para parar):\n");
            artistas[i].num_albums = 0;
            char album[100];
            while (1) {
                fgets(album, sizeof(album), stdin);
                if (strcmp(album, "fim\n") == 0 || artistas[i].num_albums >= MAX_ALBUMS)
                    break;
                album[strcspn(album, "\n")] = 0;
                strcpy(artistas[i].albums[artistas[i].num_albums], album);
                artistas[i].num_albums++;
            }
            break;
        }
    }
    if (!encontrado) {
        printf("Artista não encontrado.\n");
    }
}

void buscar_binaria_artista(Artista artistas[], int num_artistas) {
    char nome[100];
    printf("Digite o nome do artista que deseja buscar: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;

    qsort(artistas, num_artistas, sizeof(Artista), comparar_artistas);

    int inicio = 0, fim = num_artistas - 1, meio, encontrado = 0;
    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        if (strcmp(artistas[meio].nome, nome) == 0) {
            encontrado = 1;
            printf("Artista encontrado:\n");
            printf("Nome: %s\n", artistas[meio].nome);
            printf("Gênero: %s\n", artistas[meio].genero);
            printf("Local: %s\n", artistas[meio].local);
            printf("Álbuns:\n");
            for (int i = 0; i < artistas[meio].num_albums; i++) {
                printf("%s\n", artistas[meio].albums[i]);
            }
            break;
        } else if (strcmp(artistas[meio].nome, nome) < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    if (!encontrado) {
        printf("Artista não encontrado.\n");
    }
}

void buscar_sequencial_album(Artista artistas[], int num_artistas) {
    char album[100];
    printf("Digite o nome do álbum que deseja buscar: ");
    fgets(album, sizeof(album), stdin);
    album[strcspn(album, "\n")] = 0;

    int encontrado = 0;
    for (int i = 0; i < num_artistas; i++) {
        for (int j = 0; j < artistas[i].num_albums; j++) {
            if (strcmp(artistas[i].albums[j], album) == 0) {
                encontrado = 1;
                printf("Álbum encontrado:\n");
                printf("Nome do artista: %s\n", artistas[i].nome);
                printf("Gênero: %s\n", artistas[i].genero);
                printf("Local: %s\n", artistas[i].local);
                printf("Álbum: %s\n", artistas[i].albums[j]);
                break;
            }
        }
        if (encontrado)
            break;
    }
    if (!encontrado) {
        printf("Álbum não encontrado.\n");
    }
}

void mostrar_menu() {
    printf("\nMenu:\n");
    printf("1. Inserção ordenada de um novo artista\n");
    printf("2. Remoção de um artista\n");
    printf("3. Edição de um artista\n");
    printf("4. Busca binária por um artista\n");
    printf("5. Busca sequencial por um álbum\n");
    printf("6. Sair\n");
    printf("Escolha uma opção: ");
}

int main() {
    char nome_arquivo[] = "Dado_txt";
    Artista artistas[MAX_ARTISTS];
    int num_artistas = 0;
    ler_artistas(nome_arquivo, artistas, &num_artistas);
    int opcao;
    do {
        mostrar_menu();
        scanf("%d", &opcao);
        getchar(); // Limpar o buffer do teclado
        switch (opcao) {
            case 1:
                inserir_artista(artistas, &num_artistas);
                salvar_artistas(nome_arquivo, artistas, num_artistas);
                break;
            case 2:
                removedor_artista(artistas, &num_artistas);
                salvar_artistas(nome_arquivo, artistas, num_artistas);
                break;
            case 3:
                editar_artista(artistas, num_artistas);
                salvar_artistas(nome_arquivo, artistas, num_artistas);
                break;
            case 4:
                buscar_binaria_artista(artistas, num_artistas);
                break;
            case 5:
                buscar_sequencial_album(artistas, num_artistas);
                break;
            case 6:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 6);
    return 0;
}
