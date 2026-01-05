// In each of the TODO sections below replace the <TODO> 
// with the needed line of code

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size;                  // rank = ID of this process, size = total number of processes
    double *a = NULL, *b = NULL;     // Full vectors (only meaningful on rank 0)
    double *c = NULL;                // Full result vector (only meaningful on rank 0)


    //TODO-TODO-TODO-TODO-TODO-TODO-TODO   
    //Initialize the MPI environment
    <TODO Initialize>



    //TODO-TODO-TODO-TODO-TODO-TODO-TODO   
    //Get the rank (ID) of this process and total number of processes
    <TODO rank>
    <TODO size>


    // Get N from command-line argument
    if (argc < 2) {
        if (rank == 0)
            printf("Usage: %s N\n", argv[0]);
        MPI_Finalize();
        return 1;
    }
    int N = atoi(argv[1]);          // Total number of elements

    //TODO-TODO-TODO-TODO-TODO-TODO-TODO   
    // Calculate the chuck of the vectors for each process to work on
    // for this your will need "N", the nubmer of element in the vectors
    // and "size" the number of processes
    // Don't forget to put the ";" at the end of the line
    // to let the c compiler know your are done. 

    int chunk = <TODO chunk size>         // Number of elements per process

    // Allocate local arrays for each process to store its chunk
    double *a_local = malloc(chunk * sizeof(double));
    double *b_local = malloc(chunk * sizeof(double));
    double *c_local = malloc(chunk * sizeof(double));

    // Only rank 0 allocates and initializes the full vectors
    if (rank == 0) {
        a = malloc(N * sizeof(double));   // Allocate full vector a
        b = malloc(N * sizeof(double));   // Allocate full vector b
        c = malloc(N * sizeof(double));   // Allocate full result vector c
        for (int i = 0; i < N; i++) {
            a[i] = i + 1;               // Initialize a[i]
            b[i] = N - i;               // Initialize b[i]
        }
    }

    // Start timer before scattering and computation
    double start_time = MPI_Wtime();

    //TODO-TODO-TODO-TODO-TODO-TODO-TODO  
    // Use MPI_Scatter to distribute parts of vector a to all processes
    // Look at the MPI_Scatter here https://www.mpich.org/static/docs/latest/www3/MPI_Scatter.html 
    // and the examples in the mpi_dot.c code to help you. 
    <TODO a and a_local MPI_Scatter>

    //TODO-TODO-TODO-TODO-TODO-TODO-TODO  
    // Use MPI_Scatter to distribute parts of vector b to all processes
    <TODO b and b_local MPI_Scatter>

    //TODO-TODO-TODO-TODO-TODO-TODO-TODO   
    // Have each process compute each element's c_local 
    // by adding each of the a_local and b_local elemnts 
    // for its chunk of the vector addition
    // Don't forget to put the ";" at the end of the line
    for (int i = 0; i < chunk; i++) {
        <TODO and a_local[i] to b_local[i] and store in c_local[i]>
    }

    // Gather all local results into full vector c on rank 0
    MPI_Gather(c_local, chunk, MPI_DOUBLE, c, chunk, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Stop timer after computation
    double end_time = MPI_Wtime();

    // Rank 0 prints the result and elapsed time
    if (rank == 0) {
        printf("Vector addition result (first 10 elements):\n");
        for (int i = 0; i < (N < 10 ? N : 10); i++)
            printf("c[%d] = %f\n", i, c[i]);

        printf("Time elapsed: %f seconds\n", end_time - start_time);

        free(a);   // Free full vectors on rank 0
        free(b);
        free(c);
    }

    // All ranks free their local arrays
    free(a_local);
    free(b_local);
    free(c_local);

    // Finalize the MPI environment
    MPI_Finalize();

    return 0;
}
