CC = gcc
CFLAGS = -Wall -Wextra -O3 -std=c99 -Iinclude
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

# Arquivos fonte e objetos
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Nome do executável
TARGET = main

all: $(TARGET) output_dir

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Regra genérica para criar arquivos objeto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Criar os diretórios se não existirem
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

output_dir:
	mkdir -p output

clean:
	rm -rf $(OBJ_DIR) $(TARGET) output/ saida_bfs.txt saida_dfs.txt informacoes_grafo.txt

.PHONY: all clean output_dir
