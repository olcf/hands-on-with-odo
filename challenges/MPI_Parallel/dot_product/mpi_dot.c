#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size;                  // rank = ID of this process, size = total number of processes
    double *a = NULL, *b = NULL;     // Full vectors (only meaningful on rank 0)
    double local_dot = 0.0;          // Partial dot product for each process
    double global_dot = 0.0;         // Final dot product (meaningful only on rank 0)

    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the rank (ID) of this process and total number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Get N from command-line argument
    if (argc < 2) {
        if (rank == 0)
            printf("Usage: %s N\n", argv[0]);
        MPI_Finalize();
        return 1;
    }
    int N = atoi(argv[1]);          // Total number of elements
    int chunk = N / size;           // Number of elements per process

    // Allocate local arrays for each process to store its chunk
    double *a_local = malloc(chunk * sizeof(double));
    double *b_local = malloc(chunk * sizeof(double));

    // Only rank 0 allocates and initializes the full vectors
    if (rank == 0) {
        a = malloc(N * sizeof(double));   // Allocate full vector a
        b = malloc(N * sizeof(double));   // Allocate full vector b
        for (int i = 0; i < N; i++) {
            a[i] = i + 1;               // Initialize a[i]
            b[i] = N - 1 ;               // Initialize b[i]
        }
    }

    // Start timer before scattering and computation
    double start_time = MPI_Wtime();

    // Distribute parts of vector a to all processes
    MPI_Scatter(a, chunk, MPI_DOUBLE, a_local, chunk, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Distribute parts of vector b to all processes
    MPI_Scatter(b, chunk, MPI_DOUBLE, b_local, chunk, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Each process computes the dot product of its own chunk
    for (int i = 0; i < chunk; i++)
        local_dot += a_local[i] * b_local[i];

    // Reduce all local dot products into global_dot on rank 0 using sum
    MPI_Reduce(&local_dot, &global_dot, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Stop timer after computation
    double end_time = MPI_Wtime();

    // Rank 0 prints the result and elapsed time
    if (rank == 0) {
        printf("Global dot product: %f\n", global_dot);
        printf("Time elapsed: %f seconds\n", end_time - start_time);
        free(a);   // Free full vectors on rank 0
        free(b);
    }

    // All ranks free their local arrays
    free(a_local);
    free(b_local);

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}
