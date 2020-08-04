# OBJS specifies which files to compile as part of the project
OBJS = StarField.cpp StarField_OnSDL.cpp

# CC specifies which compiler we're using
CC = g++

# COMPILER_FLAGS specifies the additional compilation options we're using
# -w supresses all warnings
COMPILER_FLAGS = -w

# LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 #-lSDL2_image

# OBJ_NAME specifies the name of our executable
OBJ_NAME = ../binary/star

# This is the target that compiles our executables
all : $(OBJS)
	$(CC) $(OBJS) $(LINKER_FLAGS) -o $(OBJ_NAME)
