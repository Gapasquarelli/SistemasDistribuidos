#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 12; 
    int local_n = n / size;
    float *vector = NULL;
    float *local_vector = (float*)malloc(local_n * sizeof(float));
    float l_max = 0.0; 
    float g_max = 0.0;

    if (rank == 0) {
        vector = (float*)malloc(n * sizeof(float));
        for (int i = 0; i < n; i++) {
            vector[i] = (float)rand() / RAND_MAX;
        }
    }

    // Scatter
    MPI_Scatter(vector, local_n, MPI_FLOAT, local_vector, local_n, MPI_FLOAT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < local_n; i++) {
        if (local_vector[i] > l_max) {
            l_max = local_vector[i];
        }
    }

    // Gather
    float *gathered_maxima = NULL;
    if (rank == 0) {
        gathered_maxima = (float*)malloc(size * sizeof(float));
    }
    MPI_Gather(&l_max, 1, MPI_FLOAT, gathered_maxima, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < size; i++) {
            if (gathered_maxima[i] > g_max) {
                g_max = gathered_maxima[i];
            }
        }
        printf("O maior elemento: %f\n", g_max);
        free(gathered_maxima);
        free(vector);
    }

    free(local_vector);
    MPI_Finalize();

    return 0;
}
