#!/bin/bash
#SBATCH -A TRN039
#SBATCH -J bcast
#SBATCH -o %x-%j.out
#SBATCH -t 0:10:00
#SBATCH -p batch
#SBATCH -N 1


srun -n4 -c1 ./run
