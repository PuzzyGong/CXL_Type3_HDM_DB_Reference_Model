##### Configuration
CXX = g++
CXXFLAGS = -Wall -g -std=c++17 -Iinclude

INC_DIR = include
SRC_DIR = src
OUT_DIR = out

# Linux : TARGET  OR  Windows : TARGET.exe
TARGET = main


##### Compilation
# find all .cpp files, mapping to .o files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OUT_DIR)/%.o, $(SRCS))

# convert to .o files
$(OUT_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OUT_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)


##### Clean
.PHONY: clean

clean:
	rm -rf $(OUT_DIR) $(TARGET)