INCLUDE_PATH = ./headers
C_FILES_PATH = ./c_files
BIN 		 = ./bin

all:
	gcc -Wall -I$(INCLUDE_PATH) -c $(C_FILES_PATH)/parser.c -o $(BIN)/parser.o
	gcc -Wall -I$(INCLUDE_PATH) -c $(C_FILES_PATH)/commands.c -o $(BIN)/commands.o
	gcc -Wall -I$(INCLUDE_PATH) -c $(C_FILES_PATH)/pager.c -o $(BIN)/pager.o
	gcc -Wall -I$(INCLUDE_PATH) -c $(C_FILES_PATH)/util.c -o $(BIN)/util.o	
	ar rcs $(BIN)/libUtil.a $(BIN)/parser.o $(BIN)/commands.o $(BIN)/pager.o $(BIN)/util.o

	gcc -Wall -I$(INCLUDE_PATH) -L$(BIN) minishell.c -o minishell -lUtil