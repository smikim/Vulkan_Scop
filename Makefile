include .env

CXX = c++
CXXFLAGS = -std=c++20 -Wall -Wextra -g
LDFLAGS =  -v -ldl  -fsanitize=address

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LDFLAGS += -lglfw -lpthread -lX11 -lXxf86vm -lXrandr -lXi -lvulkan
endif
ifeq ($(UNAME_S),Darwin)
	GLFW_PATH = $(shell brew --prefix glfw)
	CXXFLAGS += -I$(GLFW_PATH)/include
	LDFLAGS += -Wl, -rpath /Users/semikim/VulkanSDK/1.3.296.0/macOS/lib -L$(GLFW_PATH)/lib -lglfw -framework Cocoa -framework Metal -framework IOKit -framework CoreVideo -lvulkan
endif

EXECUTABLE		:=	scop

APP_DIR			:=	Scop_App/Application
GRAPH_DIR 		:=	Scop_App/Graphics
INC_DIR 		:=	Scop_App/Includes
LIB_DIR 		:=	Scop_App/Library
MATHLIB_DIR 	:=	$(LIB_DIR)/Math
MAIN_DIR 		:=  Scop_App/main
SCOP_APP_DIR 	:=	Scop_App/App
SHADERS_DIR 	:=	Scop_App/Shaders
SRC_DIR 		:=	Scop_App/Src

OBJ_DIR			:=	Scop_App/obj

# cpp files
INC_FILES	:=	$(APP_DIR)/GlfwWindow.hpp \
				$(GRAPH_DIR)/BasicPSO.hpp \
				$(INC_DIR)/helpers.hpp \
				$(INC_DIR)/KeyboardMovementController.hpp \
				$(INC_DIR)/Types.hpp \
				$(INC_DIR)/VulkanBuffer.hpp \
				$(INC_DIR)/VulkanCommandBuffer.hpp \
				$(INC_DIR)/VulkanDebug.hpp \
				$(INC_DIR)/VulkanDevice.hpp \
				$(INC_DIR)/VulkanInitializers.hpp \
				$(INC_DIR)/VulkanInstance.hpp \
				$(INC_DIR)/VulkanModel.hpp \
				$(INC_DIR)/VulkanPhysicalDevice.hpp \
				$(INC_DIR)/VulkanPipeline.hpp \
				$(INC_DIR)/VulkanPipelineState.hpp \
				$(INC_DIR)/VulkanQueue.hpp \
				$(INC_DIR)/VulkanRenderer.hpp \
				$(INC_DIR)/VulkanSwapChain.hpp \
				$(INC_DIR)/VulkanTexture.hpp \
				$(INC_DIR)/VulkanTools.hpp \
				$(INC_DIR)/VulkanUtils.hpp \
				$(MATHLIB_DIR)/math.hpp \
				$(MATHLIB_DIR)/Matrix.hpp \
				$(MATHLIB_DIR)/Vector.hpp \
				$(LIB_DIR)/BmpLoader.hpp \
				$(LIB_DIR)/ObjMeshLoader.hpp \
				$(SCOP_APP_DIR)/Scop.hpp \
				$(SCOP_APP_DIR)/ScopObject.hpp 

SRC_FILES	:=	$(APP_DIR)/GlfwWindow.cpp \
				$(GRAPH_DIR)/BasicPSO.cpp \
				$(SRC_DIR)/KeyboardMovementController.cpp \
				$(SRC_DIR)/VulkanBuffer.cpp \
				$(SRC_DIR)/VulkanCommandBuffer.cpp \
				$(SRC_DIR)/VulkanDebug.cpp \
				$(SRC_DIR)/VulkanDevice.cpp \
				$(SRC_DIR)/VulkanInstance.cpp \
				$(SRC_DIR)/VulkanModel.cpp \
				$(SRC_DIR)/VulkanPhysicalDevice.cpp \
				$(SRC_DIR)/VulkanPipeline.cpp \
				$(SRC_DIR)/VulkanPipelineState.cpp \
				$(SRC_DIR)/VulkanQueue.cpp \
				$(SRC_DIR)/VulkanRenderer.cpp \
				$(SRC_DIR)/VulkanSwapChain.cpp \
				$(SRC_DIR)/VulkanTexture.cpp \
				$(SRC_DIR)/VulkanTools.cpp \
				$(SRC_DIR)/VulkanUtils.cpp \
				$(MATHLIB_DIR)/math.cpp \
				$(MATHLIB_DIR)/Matrix.cpp \
				$(MATHLIB_DIR)/Vector.cpp \
				$(LIB_DIR)/BmpLoader.cpp \
				$(LIB_DIR)/ObjMeshLoader.cpp \
				$(SCOP_APP_DIR)/Scop.cpp \
				$(SCOP_APP_DIR)/ScopObject.cpp  \
				$(MAIN_DIR)/main.cpp

RED=\033[0;31m
GREEN=\033[0;32m
YELLOW=\033[0;33m
BLUE=\033[0;34m
PURPLE=\033[0;35m
NC=\033[0m
BOLD=\033[1m

EMOJI_DONE=✅
EMOJI_BUILD=🚀
EMOJI_LINK=🔗
EMOJI_CLEAN=🚽
EMOJI_ERROR=❌

LINE_SEPARATOR=$(BOLD)$(PURPLE)<====================================>$(NC)

# object files
OBJ_FILES := $(addprefix $(OBJ_DIR)/, $(notdir $(SRC_FILES:.cpp=.o)))
DEPS = $(OBJ_FILES:.o=.d)

all: $(OBJ_DIR) shaders $(EXECUTABLE) 

$(EXECUTABLE): $(OBJ_FILES)
	@$(CXX) $^ -o $@ $(LDFLAGS)

-include $(DEPS)

# 오브젝트 파일 디렉토리 생성 규칙
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# 개별 소스 파일 컴파일 규칙
$(OBJ_DIR)/%.o: $(APP_DIR)/%.cpp
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OBJ_DIR)/%.o: $(GRAPH_DIR)/%.cpp
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OBJ_DIR)/%.o: $(MATHLIB_DIR)/%.cpp
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OBJ_DIR)/%.o: $(LIB_DIR)/%.cpp
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OBJ_DIR)/%.o: $(SCOP_APP_DIR)/%.cpp
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OBJ_DIR)/%.o: $(MAIN_DIR)/%.cpp
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

shaders:
	@echo "$(LINE_SEPARATOR)"
	@echo "$(YELLOW)$(EMOJI_BUILD) Compiling shaders...$(NC)"
	@cd Scop_App/shaders && bash compile.sh && echo "$(GREEN)$(EMOJI_DONE) Shaders compiled successfully!$(NC)" || echo "$(RED)$(EMOJI_ERROR) Error compiling shaders.$(NC)"
	@echo "$(LINE_SEPARATOR)"

clean:
	@echo "$(LINE_SEPARATOR)"
	@echo "$(YELLOW)$(EMOJI_CLEAN) Cleaning...$(NC)"
	@rm -f $(EXECUTABLE) $(OBJ_FILES) $(DEPS) 
	@rm -rf $(OBJ_DIR) 
	@echo "$(GREEN)$(EMOJI_DONE) Cleaned !$(NC)"

fclean: clean
	@echo "$(LINE_SEPARATOR)"
	@echo "$(YELLOW)$(EMOJI_CLEAN) Cleaning...$(NC)"
	@rm -f $(EXECUTABLE)
	@rm -rf Scop_App/shaders/*.spv

re: fclean shaders all

debug: CXXFLAGS += -g -DDEBUG
debug: re
	@echo "$(YELLOW)$(EMOJI_DONE) Debug mode activated:\n"

.PHONY: all fclean clean shaders