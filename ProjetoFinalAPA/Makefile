# Compilador
CC = gcc
# Opções de compilação
CFLAGS =
# Bibliotecas
LIBS =

# Diretórios
SRC_DIR = src
BUILD_DIR = build

# Arquivos fonte
SOURCES = $(wildcard $(SRC_DIR)/*.c)
# Nomes dos objetos correspondentes
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

# Nome do executável
EXECUTABLE = executavel_Projeto_APA

# Regra padrão - compilação de todos os arquivos
all: $(EXECUTABLE)

# Regra para construir o executável
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

# Regra para compilar cada arquivo objeto, incluindo main.c
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@
