#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define VLR_BIAS (1)
#define TX_APR (0.03)
#define QTD_DADOS_TREINAMENTOS (500)
#define EPOCAS (10000)

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
float regraDelta(int erro, unsigned char entrada, float sinapse);
void treinarRede(float *sinapses);

void leDados() {
    #ifdef STR1
        FILE *fp = fopen("str1/train.dat", "r");
    #else
        FILE *fp = fopen("str2/train.dat", "r");
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
    srand(time(NULL));
    // srand(1123);
    float *sinapses = (float *)malloc((ENTRADA + 1) * sizeof(float));
    for (i = 0; i < ENTRADA + 1; i++) {
        sinapses[i] = -0.5 + ((1.0 * rand())/RAND_MAX);
    }

    treinarRede(sinapses);
    FILE *fp = fopen("pesos.dat", "wb");
    fwrite(sinapses, sizeof(float), ENTRADA + 1, fp);
    fclose(fp);
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
    return resultado;
}

int degrau(float s) {
    if(s > 127) return 1;
    return 0;
}

float regraDelta(int erro, unsigned char entrada, float sinapse) {
    // printf("erro: %d - Entrada: %d - SAIDA: %f\n", erro, entrada, (sinapse + (TX_APR * erro * entrada)));
    return (sinapse + (TX_APR * erro * entrada));
}

void treinarRede(float * sinapses) {
    int i, j, k, l, resposta, erro;
    // NUMERO DE EPOCAS

    int nr_acertos = 0;
    int total = 0;

    int nr_carros = 0;
    for(i = 0; i < EPOCAS; i++){
        // CONJUNTO DE TREINAMENTOS
        for(j = 0; j < QTD_DADOS_TREINAMENTOS; j++){
            for (k = 0; k < HEIGHT; k++) {
                resposta = degrau(soma(dadosTreinamento[j][k], sinapses));
                erro = dadosTreinamento[j][k][WIDTH] - resposta;
                if (dadosTreinamento[j][k][WIDTH]) {
                    nr_carros += 1;
                }
                if(erro){
                    sinapses[0] = regraDelta(erro, VLR_BIAS, sinapses[0]);
                    for (l = 0; l < ENTRADA; l++) {
                        sinapses[l + 1] = regraDelta(erro, dadosTreinamento[j][k][l], sinapses[l + 1]);
                    }
                } else {
                    nr_acertos += 1;
                }
                total += 1;
            }
        }
    }
    puts("RESULTADO TREINAMENTO");
    printf("%d acertos, %d total\n", nr_acertos, total);
    printf("%f %%\n", (100.0 * nr_acertos)/total);

    // printf("%d carros, %d total\n", nr_carros, total);
    // printf("%f %%\n", (100.0 * nr_carros)/total);
    // printf("w[bias] = %f\n", sinapses[0]);
    // for (i = 0; i < ENTRADA; i++) {
    //     printf("w[e%d] = %f\n", i + 1, sinapses[i + 1]);
    // }
}
