SRC_DIR = src
BUILD_DIR = build/debug
IMGUI_DIR = lib/imgui
CC = g++
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
IMGUI_FILES = $(wildcard $(IMGUI_DIR)/*.cpp)
IMGUI_OBJS = $(patsubst $(IMGUI_DIR)/%.cpp,$(BUILD_DIR)/imgui/%.o,$(IMGUI_FILES))
OBJ_NAME = play
SFML_PREFIX = /usr/local/opt/sfml
INCLUDE_PATHS = -Iinclude -I$(IMGUI_DIR)
LIBRARY_PATHS = -Llib -L$(SFML_PREFIX)/lib
COMPILER_FLAGS = -std=c++17 -Wall -O0 -g -DGL_SILENCE_DEPRECATION
LINKER_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system -framework OpenGL

.PHONY: all run clean distclean

all: $(IMGUI_OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(COMPILER_FLAGS) $(INCLUDE_PATHS) $(SRC_FILES) $(IMGUI_OBJS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o $(BUILD_DIR)/$(OBJ_NAME)

# Dear ImGui / imgui-SFML are third-party and rarely change, so they're compiled
# once to object files (with optimisation) instead of on every build.
$(BUILD_DIR)/imgui/%.o: $(IMGUI_DIR)/%.cpp $(wildcard $(IMGUI_DIR)/*.h)
	@mkdir -p $(BUILD_DIR)/imgui
	$(CC) -std=c++17 -O2 -DGL_SILENCE_DEPRECATION $(INCLUDE_PATHS) -c $< -o $@

run: all
	./$(BUILD_DIR)/$(OBJ_NAME)

clean:
	rm -rf $(BUILD_DIR)/$(OBJ_NAME) $(BUILD_DIR)/$(OBJ_NAME).dSYM

distclean: clean
	rm -rf $(BUILD_DIR)/imgui
