# Compiler
CXX = g++

# Include directory for SFML
SFML_INCLUDE_DIR = C:/SFML-2.6.1/include  # Adjust path if necessary

# Library directory for SFML
SFML_LIB_DIR = C:/SFML-2.6.1/lib      # Adjust path if necessary

# Libraries to link with
SFML_LIBS = -lsfml-graphics -lsfml-window -lsfml-system

# Object files (automatically generated)
OBJ = main.o

# Executable name
BIN = app

# Target: Build the executable
all: $(BIN)

# Build the executable: link object files
$(BIN): $(OBJ)
    $(CXX) $(OBJ) -o $(BIN) -L$(SFML_LIB_DIR) $(SFML_LIBS) -lopengl32 -lwinmm -lgdi32

# Build object files: compile source files with SFML include directory
%.o: %.cpp
    $(CXX) -c $< -o $@ -I$(SFML_INCLUDE_DIR)

# Clean target: remove object files and executable
clean:
    rm -f $(OBJ) $(BIN)

# Phony target to open the executable (optional)
run:
    ./$(BIN)

