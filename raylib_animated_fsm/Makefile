# ----------------------------------------
# Silence command echoing
# ----------------------------------------
.SILENT:

# ----------------------------------------
# Default Compiler and Directories
# ----------------------------------------
CC						:= gcc

# ----------------------------------------
# General Configuration
# ----------------------------------------
# Directories
RAYLIB_STARTER_DIR		:= ./raylib_starter
CUTEHEADERS_DIR			:= ./cute_headers

DEBUG_DIR 				:= ./debug
RELEASE_DIR 			:= ./release
OBJECTS_DIR				:= ./objects

SRC_DIR					:= ./src

RESOURCE_DIR 			:= ./assets

WEB_DIR					:= ./web

RAYLIB_INCLUDE			:= $(RAYLIB_STARTER_DIR)/raylib/build/raylib/include
RAYLIB_LIBRARY			:= $(RAYLIB_STARTER_DIR)/raylib/build/raylib
RAYLIB_LIBRARY_WEB		:= $(RAYLIB_STARTER_DIR)/raylib/build_web/raylib

CUTEHEADERS_INCLUDE		:= $(CUTEHEADERS_DIR)

EMSDK_DIR				:= $(RAYLIB_STARTER_DIR)/emsdk

# Game HTML Template
HTML_TEMPLATE			:= ./template/template.html

# ----------------------------------------
# OS detection
# ----------------------------------------
UNAME_S					:= $(shell uname -s)
IS_WINDOWS				:= $(if $(findstring MINGW,$(UNAME_S)),TRUE,FALSE)
IS_LINUX				:= $(if $(findstring Linux,$(UNAME_S)),TRUE,FALSE)
IS_WSL					:= $(shell grep -qi Microsoft /proc/version && echo TRUE || echo FALSE)
IS_MACOS				:= $(if $(findstring Darwin,$(UNAME_S)),TRUE,FALSE)

# ----------------------------------------
# OS detection
# ----------------------------------------
INCLUDES				:= -I. -I$(RAYLIB_INCLUDE) -I$(CUTEHEADERS_INCLUDE)
LIBS					:= -L$(RAYLIB_LIBRARY)
LIBRARIES				:= -lraylib -lm -lpthread

# ----------------------------------------
# Configuration Compiler Flags
# ----------------------------------------
CONFIG					?= debug
OPTIMISATION_LEVEL		?= 2

ifeq ($(CONFIG), debug)
	BUILD_DIR := $(DEBUG_DIR)
	CFLAGS := -std=c11 -Wall -Wextra -Werror -g -DDEBUG $(INCLUDES)
else ifeq ($(CONFIG), release)
	BUILD_DIR := $(RELEASE_DIR)
	CFLAGS := -std=c11 -O$(OPTIMISATION_LEVEL) -DNDEBUG $(INCLUDES)
else
	$(error Invalid CONFIG value: $(CONFIG))
endif

# ----------------------------------------
# Platform-specific variables
# ----------------------------------------
ifeq ($(IS_WINDOWS),TRUE)
	# Windows-specific settings
	TOOLCHAIN			:= ./toolchain/toolchain_windows.sh
	LIBRARIES			+= -lglfw3 -lopengl32 -lgdi32 -lwinmm
	TARGET				:= $(BUILD_DIR)/game.exe
	WEB_APP				:= start http://localhost:8000/

else ifeq ($(IS_MACOS),TRUE)
	# macOS-specific settings
	TOOLCHAIN			:= ./toolchain/toolchain_macos.sh
	CC 					:= clang
	LIBRARIES			+= -framework IOKit -framework CoreFoundation -framework Cocoa -framework CoreGraphics
	TARGET				:= $(BUILD_DIR)/game.bin
	WEB_APP				:= open http://[::]:8000/

else ifeq ($(IS_WSL),TRUE)
	# WSL settings
	TOOLCHAIN			:= ./toolchain/toolchain_linux.sh
	LIBRARIES			+= -lGL -ldl
	TARGET				:= $(BUILD_DIR)/game.bin
	WEB_APP				:= wslview http://localhost:8000/

else
	# Linux settings
	TOOLCHAIN			:= ./toolchain/toolchain_linux.sh
	LIBRARIES			+= -lGL -ldl
	TARGET				:= $(BUILD_DIR)/game.bin
	WEB_APP				:= xdg-open http://localhost:8000/
endif


# ----------------------------------------
# Source files
# ----------------------------------------
SRC						:= $(wildcard $(SRC_DIR)/*.c)
OBJ						:= $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/$(OBJECTS_DIR)/%.o)

# ----------------------------------------
# Targets
# ----------------------------------------
# Default target
.PHONY: all
all: check_submodules build run

# Compile object files
$(BUILD_DIR)/$(OBJECTS_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)/$(OBJECTS_DIR)
    # Copy icon files and spritesheet(s) png
    # Just PNGs copied at the moment
	mkdir -p $(BUILD_DIR)/$(RESOURCE_DIR)
	cp $(RESOURCE_DIR)/*.png $(BUILD_DIR)/$(RESOURCE_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Conditionally include messages.mk and resources.mk if messages.mk exists
ifneq ("$(wildcard $(RAYLIB_STARTER_DIR)/toolchain/messages.mk)","")
    $(info Including messages.mk from $(RAYLIB_STARTER_DIR)/toolchain/)
    include $(RAYLIB_STARTER_DIR)/toolchain/messages.mk
    include $(RAYLIB_STARTER_DIR)/toolchain/resources.mk
else
    $(info messages.mk not found at $(RAYLIB_STARTER_DIR)/toolchain/)
endif

# Check if submodules exist and are properly initialized
check_submodules:
	if [ ! -d "$(RAYLIB_STARTER_DIR)" ] || [ ! -d "$(CUTEHEADERS_DIR)" ] || [ ! -d "$(RAYLIB_STARTER_DIR)/raylib" ] || [ ! -f "$(CUTEHEADERS_DIR)/cute_c2.h" ]; then \
		echo "Missing submodule directories. Running full reset..."; \
		$(MAKE) reset_submodules; \
	else \
		echo "Submodules present. To update Submodules run : "; \
		echo "make update_submodules"; \
	fi

# Update existing submodules to latest version
update_submodules:
	echo "Updating submodules to latest versions..."
	cd $(RAYLIB_STARTER_DIR) && \
		git checkout main && \
		git pull origin main && \
		cd ..
	cd $(CUTEHEADERS_DIR) && \
		git checkout master && \
		git pull origin master && \
		cd ..

# Manually reinitialize submodules using direct git commands
reset_submodules:
	echo "Cleaning up old submodule state..."
	rm -rf $(RAYLIB_STARTER_DIR) $(CUTEHEADERS_DIR)
	rm -rf .git/modules/$(RAYLIB_STARTER_DIR) .git/modules/$(CUTEHEADERS_DIR)
	git config --local --remove-section submodule.$(RAYLIB_STARTER_DIR) 2>/dev/null || true
	git config --local --remove-section submodule.$(CUTEHEADERS_DIR) 2>/dev/null || true
	
	echo "Manually cloning $(RAYLIB_STARTER_DIR)..."
	git clone https://bitbucket.org/MuddyGames/raylib_starter.git $(RAYLIB_STARTER_DIR) || \
		(echo "Failed to clone $(RAYLIB_STARTER_DIR) repository" && exit 1)
	
	echo "Manually cloning $(CUTEHEADERS_DIR)..."
	git clone https://github.com/RandyGaul/cute_headers.git $(CUTEHEADERS_DIR) || \
		(echo "Failed to clone $(CUTEHEADERS_DIR) repository" && exit 1)
	
	echo "Registering submodules in git config..."
	git config -f .gitmodules submodule.$(RAYLIB_STARTER_DIR).path $(RAYLIB_STARTER_DIR)
	git config -f .gitmodules submodule.$(RAYLIB_STARTER_DIR).url https://bitbucket.org/MuddyGames/raylib_starter.git
	git config -f .gitmodules submodule.$(CUTEHEADERS_DIR).path $(CUTEHEADERS_DIR)
	git config -f .gitmodules submodule.$(CUTEHEADERS_DIR).url https://github.com/RandyGaul/cute_headers.git
	
	echo "Initializing submodule configurations..."
	git submodule init
	
	echo "Checking out default branches for submodules..."
	cd $(RAYLIB_STARTER_DIR) && \
		git checkout $$(git symbolic-ref --short HEAD) && \
		cd ..
	cd $(CUTEHEADERS_DIR) && \
		git checkout $$(git symbolic-ref --short HEAD) && \
		cd ..
	
	echo "Verifying directories..."
	if [ ! -d "$(RAYLIB_STARTER_DIR)" ]; then \
		echo "Error: $(RAYLIB_STARTER_DIR) directory is missing"; \
		exit 1; \
	fi
	if [ ! -d "$(CUTEHEADERS_DIR)" ]; then \
		echo "Error: $(CUTEHEADERS_DIR) directory is missing"; \
		exit 1; \
	fi
	
	$(call SUCCESS_MSG,$(MSG_SUBMODULES_INITIALIZED))

# Install toolchain for build
install_toolchain: check_submodules
	if [ -d "$(RAYLIB_STARTER_DIR)" ]; then \
		$(MAKE) -C $(RAYLIB_STARTER_DIR) install_toolchain BUILD_TYPE=build; \
	fi

# Install toolchain for web build
install_toolchain_web: check_submodules
	if [ -d "$(RAYLIB_STARTER_DIR)" ]; then \
		$(MAKE) -C $(RAYLIB_STARTER_DIR) install_toolchain BUILD_TYPE=build_web; \
	fi

# Build target
.PHONY: build
build: BUILD_TYPE := build
build: check_submodules install_toolchain
	$(call INFO_MSG,$(MSG_BUILD_START))
	$(MAKE) $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LIBS) $(LIBRARIES)
	$(call SUCCESS_MSG,$(MSG_BUILD_END))

# Run target
.PHONY: run
run:
	$(call INFO_MSG,$(MSG_RUN_BINARY))
	./$(TARGET)

# Build target for web
.PHONY: build_web
build: BUILD_TYPE := build_web
build_web: check_submodules install_toolchain_web

	$(call INFO_MSG,$(MSG_BUILD_WEB_START))

	rm -rf $(WEB_DIR)
	mkdir -p $(WEB_DIR)

    # Copy icon file to web directory
	cp $(RESOURCE_DIR)/icon/favicon.ico $(WEB_DIR)/favicon.ico

    # Check if the Emscripten environment script exists, then:
    # 1. Print a message indicating that the web build is starting with Emscripten.
    # 2. Run a bash shell with interactive mode (-i), so environment variables are loaded.
    # 3. Source the Emscripten environment (emsdk_env.sh) to set up Emscripten-specific paths and tools.
    # 4. Run the `emcc` command to compile C source files from the source directory into WebAssembly, outputting an HTML file in the web directory.
    #    - `-I.`: Adds the current directory to the include path for headers.
    #    - `-I$(RAYLIB_INCLUDE) -I$(CUTEHEADERS_INCLUDE)`: Adds Raylib and Cuteheaders include paths.
    #    - `-L$(RAYLIB_LIBRARY_WEB)`: Specifies the Raylib library path for web builds.
    #    - `-lraylib`: Links the Raylib library.
    #    - `-s USE_GLFW=3 -s FULL_ES2=1 -s ASYNCIFY`: Enables GLFW, OpenGL ES2, and async support for compatibility.
    #    - `-s INITIAL_MEMORY=33554432`: Sets initial memory allocation to 32MB.
    #    - `-s ALLOW_MEMORY_GROWTH`: Allow memory allocation to grow, watch for memory leaks.
    #    - `--shell-file $(HTML_TEMPLATE)`: Uses a custom HTML shell template for the output file.
    #    - `-D WEB_BUILD`: Defines a preprocessor macro to indicate a web build.
    #    - `--preload-file ./assets`: Packages the assets folder to make it accessible in the web build, filename index.data
    #    - `-v`: Enables verbose output for detailed build information.

	if [ -f "$(EMSDK_DIR)/emsdk_env.sh" ]; then \
		$(call INFO_MSG, $(MSG_BUILD_WEB_EMSDK)); \
		bash -i -c "source $(EMSDK_DIR)/emsdk_env.sh && \
		emcc $(SRC_DIR)/*.c -o ./$(WEB_DIR)/index.html \
			-I. -I$(RAYLIB_INCLUDE) -I$(CUTEHEADERS_INCLUDE) \
			-L$(RAYLIB_LIBRARY_WEB) \
			-lraylib \
			-s USE_GLFW=3 -s FULL_ES2=1 -s ASYNCIFY \
			-s INITIAL_MEMORY=33554432 \
			-s ALLOW_MEMORY_GROWTH \
			--shell-file $(HTML_TEMPLATE) \
			-D WEB_BUILD \
			--preload-file ./assets \
			-v"; \
		if [ $$? -eq 0 ]; then \
			$(call SUCCESS_MSG, $(MSG_BUILD_WEB_END)); \
		else \
			$(call ERROR_MSG, $(MSG_BUILD_WEB_FAIL)); \
		fi \
	else \
		$(call ERROR_MSG, $(MSG_BUILD_WEB_EMSDK_FAIL)); \
	fi


# Stop Web Server target
stop_web_server:
	$(MAKE) -C $(RAYLIB_STARTER_DIR) stop_web_server

# Start Web Server target
start_web_server:
	$(call INFO_MSG,$(WEB_DIR))
	$(info WEB_DIR: $(WEB_DIR))

	python3 -m http.server --directory $(WEB_DIR) &
	sleep 3

# Run Web target
.PHONY: run_web
run_web: build_web stop_web_server start_web_server
	$(WEB_APP)

# Clean target
.PHONY: clean
clean:
	$(call INFO_MSG,"Cleaning...")
	$(MAKE) -C $(RAYLIB_STARTER_DIR) clean
	rm -f $(TARGET_DEBUG) $(TARGET_RELEASE)
	rm -rf $(DEBUG_DIR) $(RELEASE_DIR) ${WEB_DIR}
	$(call SUCCESS_MSG,"Clean complete")

# Clean target
.PHONY: clean_toolchain
clean_toolchain: clean
	$(call INFO_MSG,"Cleaning toolchain...")
	$(MAKE) -C $(RAYLIB_STARTER_DIR) clean_toolchain
	$(call SUCCESS_MSG,"Clean toolchain complete")