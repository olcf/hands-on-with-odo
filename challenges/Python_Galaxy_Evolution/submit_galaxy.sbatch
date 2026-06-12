#!/bin/bash
#SBATCH -A TRN046
#SBATCH -J galaxy
#SBATCH -o %x-%j.out
#SBATCH -t 0:10:00
#SBATCH -p batch
#SBATCH -N 1

date

module load PrgEnv-gnu/8.6.0
module load cray-hdf5-parallel/1.12.2.11
module load miniforge3

conda activate /gpfs/wolf2/olcf/stf007/world-shared/9b8/crashcourse_envs/h5pympi-odo

srun -n4 -c14 python3 -u galaxy.py
