#!/bin/bash
#SBATCH -A TRN046
#SBATCH -J pytorch_cnn
#SBATCH -o %x-%j.out
#SBATCH -t 1:00:00
#SBATCH -p batch
#SBATCH -N 1

date

module load PrgEnv-gnu/8.7.0
module load cpe/26.03
module load miniforge3/23.11.0-0
module load rocm/7.1.1
module load craype-accel-amd-gfx90a

# Because using a non-default CPE
export LD_LIBRARY_PATH=$CRAY_LD_LIBRARY_PATH:$LD_LIBRARY_PATH

export MIOPEN_USER_DB_PATH="/tmp/my-miopen-cache"
export MIOPEN_CUSTOM_CACHE_DIR=${MIOPEN_USER_DB_PATH}
rm -rf ${MIOPEN_USER_DB_PATH}
mkdir -p ${MIOPEN_USER_DB_PATH}

conda activate /gpfs/wolf2/olcf/stf007/world-shared/9b8/crashcourse_envs/torch-odo

python3 -u cnn.py
