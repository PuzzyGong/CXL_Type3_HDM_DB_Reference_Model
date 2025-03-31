##### Configuration
CXX = g++
CXXFLAGS = -Wall -g -std=c++17 -Iinclude

INC_DIR = include
SRC_DIR = src
OUT_DIR = out
TARGET = main		# Linux : TARGET  OR  Windows : TARGET.exe

##### Compilation
# find all .cpp files, mapping to .o files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OUT_DIR)/%.o, $(SRCS))
INCS = $(wildcard $(INC_DIR)/*.h)

# convert to .o files
$(OUT_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCS) | $(OUT_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 
$(OUT_DIR)/$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)
