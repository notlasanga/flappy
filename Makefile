# Usage:
# make        # compile all binary
# make run    # run executable
# make clean  # remove ALL binaries and objects

EXECUTABLE_NAME = flappy

SRC = src
BIN = bin
OBJ = $(BIN)/obj
RES = res

sdl        = -framework SDL2
sdl_image  = -framework SDL2_image

CC = gcc
CFLAGS = -c -I include/ -MMD -F/Library/Frameworks
LDFLAGS = ${sdl} ${sdl_image} -F/Library/Frameworks

EXECUTABLE_FILES = $(EXECUTABLE_NAME:%=$(BIN)/%)
SOURCE_FILES     = $(patsubst ${SRC}/%, %, $(wildcard ${SRC}/*.c))
OBJECT_FILES     = $(SOURCE_FILES:%.c=$(OBJ)/%.o)
DEPENDENCY_FILES = $(SOURCE_FILES:%.c=$(OBJ)/%.d)

GREEN = \033[0;32m
NC = \033[0m # No Color

build: $(EXECUTABLE_FILES)

-include ${DEPENDENCY_FILES}

run: $(EXECUTABLE_FILES)
	@echo "\n${GREEN}Running...${NC}"
	./bin/${EXECUTABLE_NAME}

clean:
	@echo "${GREEN}Cleaning up...${NC}"
	rm -rf $(BIN)

.PHONY: build run clean

$(EXECUTABLE_FILES): $(OBJECT_FILES)
	@echo "\n${GREEN}Linking ${@F}${NC}"
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "\n${GREEN}Build successful!${NC}"

$(OBJECT_FILES): $(OBJ)/%.o: $(SRC)/%.c
	@echo "${GREEN}Compiling $<${NC}"
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ $<