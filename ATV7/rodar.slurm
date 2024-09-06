#!/bin/bash
#SBATCH --job-name=convolucao_sequencial         # Nome do job
#SBATCH --output=resultado_%j.txt                # Nome do arquivo de saída com o ID do job
#SBATCH --ntasks=1                               # Número de tarefas (1 processo)
#SBATCH --cpus-per-task=2                        # Número de threads por tarefa (2 threads)
#SBATCH --mem=4096                               # Memória total alocada por nó (4096 MB = 4 GB)
#SBATCH --time=03:00:00                          # Tempo máximo de execução (3 horas)
#SBATCH --partition=espec                        # Fila do cluster a ser utilizada

# Exporta a variável de ambiente para configurar o número de threads OpenMP
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

# Cria diretório para armazenar os resultados, nomeado com o ID do job
mkdir -p resultados_$SLURM_JOB_ID
cd resultados_$SLURM_JOB_ID

# Função para verificar o sucesso de cada execução
verifica_sucesso() {
    if [ $? -ne 0 ]; then
        echo "Erro na execução de $1 - Execução $2" >> erro_execucao.txt
    fi
}

# Executa cada método 10 vezes e salva os resultados em arquivos separados

# Loop para executar 10 vezes o método "mochila cheia"
for i in {1..10}; do
    echo "Execução $i - Mochila Cheia" >> resultados_mochila_cheia.txt
    ../mochila_cheia >> resultados_mochila_cheia.txt
    verifica_sucesso "mochila_cheia" $i
    echo "---------------------" >> resultados_mochila_cheia.txt
done

# Loop para executar 10 vezes o método "substituicao de objeto"
for i in {1..10}; do
    echo "Execução $i - Substituição de Objeto" >> resultados_mochila_sub.txt
    ../sub_obj >> resultados_mochila_sub.txt
    verifica_sucesso "mochila_sub" $i
    echo "---------------------" >> resultados_mochila_sub.txt
done

# Loop para executar 10 vezes o método "hill climbing"
for i in {1..10}; do
    echo "Execução $i - Hill Climbing" >> resultados_mochila_hill.txt
    ../HillClimbing >> resultados_mochila_hill.txt
    verifica_sucesso "mochila_hill" $i
    echo "---------------------" >> resultados_mochila_hill.txt
done


