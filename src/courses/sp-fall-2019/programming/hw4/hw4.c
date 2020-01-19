#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <assert.h>

void err_exit(char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(128);
}

int min(int a, int b) { return a < b ? a : b; }

const double INIT_LR = 0.1;
const int UPD_L = 45;
int UPD = 100;

#define TRAIN_N 60000
#define TEST_N 10000
#define PICSZ 784
#define K 10

unsigned char X_train[TRAIN_N][PICSZ], X_test[TEST_N][PICSZ];
unsigned char Y_train[TRAIN_N];
int Y_test[TEST_N];

double X[TRAIN_N][PICSZ], XT[PICSZ][TRAIN_N], Xtest[TEST_N][PICSZ];
double y[TRAIN_N][K], yhat[TRAIN_N][K];
double W[PICSZ][K], Wg[PICSZ][K], Wf[PICSZ][K];

int thread_n;
int R;

void softmax(int row_n) {
    for ( int i = 0; i < row_n; ++i) {
        double tmp[K];
        for ( int k = 0; k < K; ++k)
            tmp[k] = yhat[i][k];
        for ( int j = 0; j < K; ++j) {
            double sum = 0.0;
            for ( int k = 0; k < K; ++k)
                sum += exp(tmp[k] - tmp[j]);
            yhat[i][j] = 1.0 / sum;
        }
    }
}

double calc_accuracy() {
    int correct = 0;
    for ( int i = 0; i < TRAIN_N; ++i) {
        int p = 0; double v = yhat[i][0];
        for ( int k = 1; k < K; ++k)
            if ( yhat[i][k] > v )
                p = k, v = yhat[i][k];
        if ( p == Y_train[i] )
            ++correct;
    }
    return (double)correct / TRAIN_N;
}

void* XT_mul_err_r(void *arg) {
    int id = (int)arg;
    int start_row = id * R, end_row = min(id * R + R, PICSZ);
    for ( int i = start_row; i < end_row; ++i)
        for ( int k = 0; k < TRAIN_N; ++k)
            for ( int j = 0; j < K; ++j)
                Wg[i][j] += XT[i][k] * yhat[k][j];
}
void XT_mul_err() {
    memset(Wg, 0, sizeof(Wg));
    R = PICSZ / thread_n + !!(PICSZ % thread_n);
    pthread_t tid[thread_n];
    for ( int i = 0; i < thread_n; ++i)
        pthread_create(&tid[i], NULL, XT_mul_err_r, (void*)i);
    void *ret;
    for ( int i = 0; i < thread_n; ++i)
        pthread_join(tid[i], &ret);
}

void* X_mul_W_r(void *arg) {
    int id = (int)arg;
    int start_row = id * R, end_row = min(id * R + R, TRAIN_N);
    for ( int i = start_row; i < end_row; ++i)
        for ( int k = 0; k < PICSZ; ++k)
            for ( int j = 0; j < K; ++j)
                yhat[i][j] += X[i][k] * W[k][j];
}
void X_mul_W() {
    memset(yhat, 0, sizeof(yhat));
    R = TRAIN_N / thread_n + !!(TRAIN_N % thread_n);
    pthread_t tid[thread_n];
    for ( int i = 0; i < thread_n; ++i)
        pthread_create(&tid[i], NULL, X_mul_W_r, (void*)i);
    void *ret;
    for ( int i = 0; i < thread_n; ++i)
        pthread_join(tid[i], &ret);
}

void train() {
    double drop = 0.8, epochs_drop = 20.0;
    double lr = INIT_LR;
    for ( int epoch = 0; epoch < UPD; ++epoch) {
        X_mul_W();
        softmax(TRAIN_N);

        //double acc = calc_accuracy();
        //fprintf(stderr, "%d: %lf\n", epoch, acc);

        for ( int i = 0; i < TRAIN_N; ++i)
            for ( int j = 0; j < K; ++j)
                yhat[i][j] -= y[i][j];   

        XT_mul_err();

        for ( int i = 0; i < PICSZ; ++i)
            for ( int j = 0; j < K; ++j)
                W[i][j] -= lr * Wg[i][j];
        
        lr = INIT_LR * pow(drop, floor((1 + epoch) / epochs_drop));
    }
    memcpy(Wf, W, sizeof(W));
}

void* Xtest_mul_W_r(void *arg) {
    int id = (int)arg;
    int start_row = id * R, end_row = min(id * R + R, TEST_N);
    for ( int i = start_row; i < end_row; ++i)
        for ( int j = 0; j < K; ++j)
            for ( int k = 0; k < PICSZ; ++k)
                yhat[i][j] += Xtest[i][k] * Wf[k][j];
}
void Xtest_mul_W() {
    memset(yhat, 0, sizeof(yhat));
    R = TEST_N / thread_n + !!(TEST_N % thread_n);
    pthread_t tid[thread_n];
    for ( int i = 0; i < thread_n; ++i)
        pthread_create(&tid[i], NULL, Xtest_mul_W_r, (void*)i);
    void *ret;
    for ( int i = 0; i < thread_n; ++i)
        pthread_join(tid[i], &ret);
}
void predict() {
    Xtest_mul_W();
    softmax(TEST_N);
    for ( int i = 0; i < TEST_N; i++) {
        int p = 0; double v = yhat[i][0];
        for ( int k = 1; k < K; k++)
            if ( yhat[i][k] > v )
                p = k, v = yhat[i][k];
        Y_test[i] = p;
    }
}

void write_result() {
    FILE *fp_output = fopen("result.csv", "w");
    fprintf(fp_output, "id,label\n");
    for ( int i = 0; i < TEST_N; i++)
        fprintf(fp_output, "%d,%d\n", i, Y_test[i]);
    fclose(fp_output);
}

int main(int argc, char *argv[])
{
    if ( argc != 5 ) err_exit("wrong number of argument");

    FILE *fp_x_train = fopen(argv[1], "rb");
    FILE *fp_y_train = fopen(argv[2], "rb");
    FILE *fp_x_test = fopen(argv[3], "rb");
    thread_n = atoi(argv[4]);

    if ( thread_n <= 5 ) UPD = UPD_L;

    for ( int i = 0; i < TRAIN_N; i++)
        if ( fread(X_train[i], 1, PICSZ, fp_x_train) != PICSZ )
            err_exit("data reading error");

    for ( int i = 0; i < TRAIN_N; i++)
        for ( int j = 0; j < PICSZ; j++)
            X[i][j] = XT[j][i] = (double)X_train[i][j];
    
    if ( fread(Y_train, 1, TRAIN_N, fp_y_train) != TRAIN_N )
        err_exit("data reading error");

    for ( int i = 0; i < TRAIN_N; i++)
       y[i][ Y_train[i] ] = 1.0;

    for ( int i = 0; i < TEST_N; i++)
        if ( fread(X_test[i], 1, PICSZ, fp_x_test) != PICSZ )
            err_exit("data reading error");

    for ( int i = 0; i < TEST_N; i++)
        for ( int j = 0; j < PICSZ; j++)
            Xtest[i][j] = (double)X_test[i][j];

    train();
    predict();
    write_result();

    return EXIT_SUCCESS;
}
