CXX = g++
MODE=debug
CMODE = -O3
ifeq ($(MODE), debug)
CMODE = -g
endif
CFLAGS = -Werror -Wextra -Wall $(CMODE)
TARGET = aeon_graphDB
SRC_DIR = ./src
INCLUDE_DIR =	./include	\
		-I ./include/obsolet
BUILD_DIR = ./builds

SRC =				main.cpp			\
					$(SRC_AEON_GDB)		\
					$(SRC_STRING)		\
					$(SRC_ERROR)

SRC_AEON_GDB =		wdb_mutex.cpp		\
			node.cpp		\
			graphDB.cpp

SRC_ERROR =		error_handling.cpp

SRC_STRING =		str_isolate.cpp

ifeq ($(MODE), debug)
COLOR_ID = \033[0;32m
else
COLOR_ID = \033[0;34m
endif
COLOR_NO = \033[0m
OBJS = $(addprefix $(BUILD_DIR)/, $(SRC:.cpp=.o))
SRCS = $(addprefix $(SRC_DIR)/, $(SRC)) 
INCLUDES = -I$(INCLUDE_DIR) 
LIB_LINKS =  -lwgdb -lsfml-graphics -lsfml-window -lsfml-system


all: $(BUILD_DIR) $(TARGET)

$(TARGET): $(OBJS)
	@echo " - Building $(COLOR_ID)$(TARGET)$(COLOR_NO)..."
	@$(CXX) $(CFLAGS) $(INCLUDES) $(OBJS) -o $(TARGET) $(LIB_LINKS)


$(addprefix $(BUILD_DIR)/, %.o): $(addprefix $(SRC_DIR)/, %.cpp) 
	@echo " - Building $(COLOR_ID)$(@:.cpp=.o) (c++) $(COLOR_NO)..."
	@$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@

re: fclean $(BUILD_DIR)  $(TARGET)

clean:
	@echo " - Removing objects files:"
	@rm -fr $(OBJS)

fclean: clean
	@echo " - Removing $(COLOR_ID)$(TARGET)$(COLOR_NO)..."
	@rm -rf $(TARGET)

$(BUILD_DIR):
	@echo " - Creating build directory '$(COLOR_ID)$(BUILD_DIR)$(COLOR_NO)'..."
	@mkdir $(BUILD_DIR)

