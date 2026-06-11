#!/bin/bash
#SBATCH -A TRN046
#SBATCH -J qml_basics
#SBATCH -o %x-%j.out
#SBATCH -N 1
#SBATCH -t 2:00:00
#SBATCH -p batch
#SBATCH -C nvme

cd $SLURM_SUBMIT_DIR
date

# Load modules
module load PrgEnv-amd/8.7.0
module load cpe/26.03
module load miniforge3/23.11.0-0
module load amd/7.1.1 rocm/7.1.1

# Because using a non-default CPE module
export LD_LIBRARY_PATH=$CRAY_LD_LIBRARY_PATH:$LD_LIBRARY_PATH

conda activate /gpfs/wolf2/olcf/stf007/world-shared/9b8/crashcourse_envs/qml-odo

# Needed to bypass MIOpen, Disk I/O Errors
export MIOPEN_USER_DB_PATH="/mnt/bb/${USER}/my-miopen-cache" 
export MIOPEN_CUSTOM_CACHE_DIR=${MIOPEN_USER_DB_PATH} 
rm -rf ${MIOPEN_USER_DB_PATH} 
mkdir -p ${MIOPEN_USER_DB_PATH}

# Get address of head node
export MASTER_ADDR=$(hostname -i)
export NCCL_SOCKET_IFNAME=hsn0 # necessary NCCL socket for Frontier

export OMP_NUM_THREADS=1

export all_proxy=socks://proxy.ccs.ornl.gov:3128/
export ftp_proxy=ftp://proxy.ccs.ornl.gov:3128/
export http_proxy=http://proxy.ccs.ornl.gov:3128/
export https_proxy=http://proxy.ccs.ornl.gov:3128/
export no_proxy='localhost,127.0.0.0/8,*.ccs.ornl.gov'

source keys.sh 

export USE_QUANTUM=1 # set value to 0 to use classical network instead

srun -n1 -c7 --gpus-per-task=1 --gpu-bind=closest python3 -W ignore -u qml.py

# Don't run the below on multiple ranks, just -n1
#srun -n1 -c7 --gpus-per-task=1 --gpu-bind=closest python3 -W ignore -u qml_eval.py
#srun -n1 -c7 --gpus-per-task=1 --gpu-bind=closest python3 -W ignore -u qml_eval_real.py
