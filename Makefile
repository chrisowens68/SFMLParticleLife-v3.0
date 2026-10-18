SRC_DIR = src
BUILD_DIR = build/debug
OBJ_DIR = $(BUILD_DIR)/obj
IMGUI_DIR = lib/imgui
CC = g++
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
IMGUI_FILES = $(wildcard $(IMGUI_DIR)/*.cpp)
IMGUI_OBJS = $(patsubst $(IMGUI_DIR)/%.cpp,$(BUILD_DIR)/imgui/%.o,$(IMGUI_FILES))
OBJ_NAME = play
TARGET = $(BUILD_DIR)/$(OBJ_NAME)
SFML_PREFIX = /usr/local/opt/sfml
INCLUDE_PATHS = -Iinclude -I$(IMGUI_DIR)
LIBRARY_PATHS = -Llib -L$(SFML_PREFIX)/lib
COMPILER_FLAGS = -std=c++17 -Wall -O0 -g -DGL_SILENCE_DEPRECATION
# -MMD writes a .d file next to each object listing the headers it includes;
# -MP adds dummy targets so deleting a header doesn't break the build.
DEP_FLAGS = -MMD -MP
LINKER_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system -framework OpenGL

# Show the full compiler commands with `make V=1`.
ifeq ($(V),1)
Q =
else
Q = @
endif

.PHONY: all run clean distclean

all: $(TARGET)

$(TARGET): $(OBJ_FILES) $(IMGUI_OBJS)
	@echo "Linking $@"
	$(Q)$(CC) $^ $(LIBRARY_PATHS) $(LINKER_FLAGS) -o $@

# Only sources that changed (or whose headers changed) are recompiled.
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	@echo "Compiling $<"
	$(Q)$(CC) $(COMPILER_FLAGS) $(DEP_FLAGS) $(INCLUDE_PATHS) -c $< -o $@

# Dear ImGui / imgui-SFML are third-party and rarely change, so they're compiled
# with optimisation and kept across `make clean`.
$(BUILD_DIR)/imgui/%.o: $(IMGUI_DIR)/%.cpp $(wildcard $(IMGUI_DIR)/*.h)
	@mkdir -p $(BUILD_DIR)/imgui
	@echo "Compiling $< (ImGui)"
	$(Q)$(CC) -std=c++17 -O2 -DGL_SILENCE_DEPRECATION $(INCLUDE_PATHS) -c $< -o $@

-include $(OBJ_FILES:.o=.d)

run: all
	./$(TARGET)

clean:
	rm -rf $(TARGET) $(TARGET).dSYM $(OBJ_DIR)

distclean: clean
	rm -rf $(BUILD_DIR)/imgui
