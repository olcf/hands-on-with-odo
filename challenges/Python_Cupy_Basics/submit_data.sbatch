#!/bin/bash
#SBATCH -A TRN046
#SBATCH -J cupy_xfer
#SBATCH -o %x-%j.out
#SBATCH -t 0:10:00
#SBATCH -p batch
#SBATCH -N 1

date

module load PrgEnv-gnu/8.7.0
module load cpe/26.03
module load miniforge3/23.11.0-0
module load rocm/7.0.2
module load craype-accel-amd-gfx90a

# Because using a non-default CPE module (set this when building and running)
export LD_LIBRARY_PATH=$CRAY_LD_LIBRARY_PATH:$LD_LIBRARY_PATH

# Necessary for this cupy installation specifically
export ROCM_HOME=${ROCM_PATH}
export LD_PRELOAD=/opt/rocm-7.0.2/lib/librocsparse.so.1:${LD_PRELOAD} # due to bug on Odo pointing to /opt/rocm

conda activate /gpfs/wolf2/olcf/stf007/world-shared/9b8/crashcourse_envs/cupy-odo

srun -n1 --gpus-per-task=3 python3 data_transfer.py
