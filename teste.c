#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define VLR_BIAS (1)
#define QTD_DADOS_TREINAMENTOS (500)
#define EPOCAS (1)

#define STR2

#define DESIRED_CLASS (2) // 0 indexed
#define NUMBER_OF_CLASSES (9)

#ifdef STR1
    #define WIDTH (48)
    #define HEIGHT (48)
#else
    #define WIDTH (48 * 48)
    #define HEIGHT (1)
#endif
#define ENTRADA (WIDTH)

unsigned char ***dadosTreinamento;

float soma(unsigned char *e, float* sinapses);
int degrau(float s);
void testarRede(float *sinapses);

void leDados() {
#ifdef STR1
        FILE *fp = fopen("str1/test.dat", "r");
    #else
        FILE *fp = fopen("str2/test.dat", "r");
    #endif

    if (!fp) {
        fprintf(stderr, "File not found\n");
        return;
    }
    int i;
    int x, y;
    int w;
    dadosTreinamento = (unsigned char ***)malloc(sizeof(unsigned char **) * QTD_DADOS_TREINAMENTOS);
    for (i = 0; i < QTD_DADOS_TREINAMENTOS; i++) {
        dadosTreinamento[i] = (unsigned char **)malloc(sizeof(unsigned char *) * HEIGHT);
        for (x = 0; x < HEIGHT; x++) {
            dadosTreinamento[i][x] = (unsigned char *)malloc(sizeof(unsigned char) * (WIDTH + 1));
            for (y = 0; y < WIDTH; y++) {
                fscanf(fp, "%d,", &w);
                dadosTreinamento[i][x][y] = (unsigned char)w;
            }
            for (y = 0; y < NUMBER_OF_CLASSES - 1; y++) {
                fscanf(fp, "%d,", &w);
                if (y == DESIRED_CLASS) {
                    dadosTreinamento[i][x][WIDTH] = (unsigned char)w;
                }
            }
            fscanf(fp, "%d", &w);
        }
    }

    fclose(fp);
}

void liberaDados() {
    int i, x;
    for (i = 0; i < QTD_DADOS_TREINAMENTOS; i++) {
        for (x = 0; x < HEIGHT; x++) {
            free(dadosTreinamento[i][x]);
        }
        free(dadosTreinamento[i]);
    }
    free(dadosTreinamento);
}

int main() {
    leDados();
    int i, x, y;
    // for (x = 0; x < HEIGHT; x++) {
    //     for (y = 0; y < WIDTH; y++) {
    //         int w = (int) dadosTreinamento[1][x][y];
    //         printf("%d\t", w);
    //     }
    //     printf("\n");
    // }
    float *sinapses = (float *)malloc((ENTRADA + 1) * sizeof(float));
    FILE *fp = fopen("pesos.dat", "rb");
    fread(sinapses, sizeof(float), ENTRADA + 1, fp);
    fclose(fp);

    testarRede(sinapses);
    free(sinapses);
    liberaDados();
    return 0;
}

float soma(unsigned char *e, float* sinapses) {
    float resultado = 0;
    resultado = resultado + sinapses[0] * VLR_BIAS;
    int i;
    for (i = 0; i < WIDTH; i++) {
        resultado = resultado + sinapses[i + 1] * e[i];
    }
    // printf("resultado: %f\n", resultado);
    return resultado;
}

int degrau(float s) {
    if(s > 127) return 1;
    return 0;
}

void testarRede(float * sinapses) {
    int i, j, k, resposta, erro;

    int nr_acertos = 0;
    int t_positivo = 0;
    int t_negativo = 0;
    int f_positivo = 0;
    int f_negativo = 0;
    int total = 0;

    int nr_carros = 0;
    for(i = 0; i < EPOCAS; i++){
        // CONJUNTO DE TREINAMENTOS
        for(j = 0; j < QTD_DADOS_TREINAMENTOS; j++){
            for (k = 0; k < HEIGHT; k++) {
                resposta = degrau(soma(dadosTreinamento[j][k], sinapses));
                // printf("resposta: %d\n", resposta);
                erro = abs(dadosTreinamento[j][k][WIDTH] - resposta);

                if (dadosTreinamento[j][k][WIDTH] && resposta) {
                    nr_carros += 1;
            t_positivo += 1;
                }

                if(!erro) {
                    nr_acertos += 1;
                }

        /*As partes utilizadas para a formação da matriz de confusão não são realmente necessários para a
        realização da classificação, apenas para entender como o treinamento da rede está ocorrendo. Essa parte
        pode ser agilizada retirando-se os if's desnecessários*/
        if(dadosTreinamento[j][k][WIDTH] && !resposta)
            f_negativo += 1;
        if(!dadosTreinamento[j][k][WIDTH] && !resposta)
            t_negativo += 1;
        if(!dadosTreinamento[j][k][WIDTH] && resposta)
            f_positivo += 1;

                total += 1;
            }
        }
    }
    /*Nem todos os prints são necessários*/
    puts("RESULTADO TESTE");
    printf("%d acertos, %d total\n", nr_acertos, total);
    printf("%f %%\n", (100.0 * nr_acertos)/total);
    printf("\n\nMATRIZ DE CONFUSÃO\nPositivo = %d   Falso positivo = %d\nNegativo = %d   Falso Negativo = %d\n\n", t_positivo, f_positivo, t_negativo, f_negativo);

    // printf("%d carros, %d total\n", nr_carros, total);
    printf("%f %%\n", (100.0 * nr_carros)/total);
    // printf("w[bias] = %f\n", sinapses[0]);
    // for (i = 0; i < ENTRADA; i++) {
    //     printf("w[e%d] = %f\n", i + 1, sinapses[i + 1]);
    // }
}
