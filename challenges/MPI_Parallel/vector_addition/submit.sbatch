#!/bin/bash
#SBATCH -A STF007
#SBATCH -J vec_add
#SBATCH -o %x-%j.out
#SBATCH -t 00:10:00
#SBATCH -p batch
#SBATCH -N 1

# Set the number of processes
NP=4   # Change this number to adjust how many processes to use

# Set number of elements for the program
Ne=10000000000000  # You can also change this directly here

# NOTE: The total number of elements Ne must be evenly divisible by the number of processes NP.

echo "Running with $NP processes and N=$Ne elements"

srun -n $NP ./a.out $Ne
