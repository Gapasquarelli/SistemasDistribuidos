#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 12; 
    float *vector = (float*)malloc(n * sizeof(float));
    float l_max = 0.0; 
    float g_max = 0.0;

    if (rank == 0) {
        for (int i = 0; i < n; i++) {
            vector[i] = (float)rand() / RAND_MAX;
        }

        for (int i = 1; i < size; i++) {
            MPI_Send(vector, n, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
        }

        for (int i = 0; i < n; i++) {
            if (vector[i] > l_max) {
                l_max = vector[i];
            }
        }
    } 
    else {
        MPI_Recv(vector, n, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int i = 0; i < n; i++) {
            if (vector[i] > l_max) {
                l_max = vector[i];
            }
        }
    }

    MPI_Reduce(&l_max, &g_max, 1, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("O maior elemento: %f\n", g_max);
    }

    MPI_Finalize();
    free(vector);

    return 0;
}