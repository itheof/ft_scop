NAME         = ft_scop
CFLAGS      += -g -Wall -Wextra -Werror
CFLAGS      += -std=c11

ifeq ($(DEBUG),yes)
	CFLAGS  += -g3 -O0 -fno-inline -DNOT_CORRECTION
else
	CFLAGS  += -O2
endif
ifeq ($(SAN),yes)
	CFLAGS	+= -fsanitize=address
	LDFLAGS += -fsanitize=address
endif
CFLAGS  += $(TMP_CFLAGS)

# Headers
CFLAGS    += -I./include
# Sources
SRC_PATH    = src

SOURCES     = $(NAME:%=%.c) init.c callbacks.c cube.c object_mgr.c

# Generation
vpath %.c $(SRC_PATH) $(addprefix $(SRC_PATH)/,$(SRC_SUBDIR))
OBJ_PATH    = .obj
OBJECTS     = $(SOURCES:%.c=$(OBJ_PATH)/%.o)
DEP_PATH    = .dep/
DEPS        = $(SOURCES:%.c=$(DEP_PATH)/%.d)
BUILD_DIR   = $(OBJ_PATH) $(OBJ_PATH)/common \
			  $(DEP_PATH)/common $(DEP_PATH)

# Libscop
LIBSCOP_PATH = lib/libscop
LIBSCOP      = $(LIBSCOP_PATH)/libscop.a
CFLAGS       += -I$(LIBSCOP_PATH)/include
LDFLAGS      += -L$(LIBSCOP_PATH) -lscop

# Libft
LIBFT_PATH = lib/libft
LIBFT      = $(LIBFT_PATH)/libft.a
CFLAGS    += -I $(LIBFT_PATH)/inc
LDFLAGS   += -L$(LIBFT_PATH) -lft

# GLFW
GLFW_PATH  = lib/glfw
GLFW       = $(GLFW_PATH)/build/src/libglfw3.a
LDFLAGS   += -L$(GLFW_PATH)/build/src -lglfw3
ifeq ($(UNAME_S),Darwin)
	LDFLAGS += -framework Cocoa -framework OpenGL -framework IOKit -framework \
		  CoreVideo
else
	LDFLAGS += `PKG_CONFIG_PATH=$(GLFW_PATH)/build/src/ pkg-config --libs --static glfw3`
endif
CFLAGS    += -I$(GLFW_PATH)/include

.SECONDARY: $(OBJECTS)

all: $(DEPS) $(NAME)

-include $(DEPS)

$(NAME): %:$(OBJ_PATH)/%.o $(OBJECTS) | $(LIBFT) $(LIBSCOP) $(GLFW)
	$(CC) -o $@ $^ $(LDFLAGS)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_PATH)

$(LIBSCOP):
	@$(MAKE) -C $(LIBSCOP_PATH)

$(OBJECTS): $(OBJ_PATH)/%.o: %.c | $(OBJ_PATH) $(GLFW_PATH)
	$(CC) $(CFLAGS) -o $@ -c $<

$(DEP_PATH)/%.d: %.c | $(DEP_PATH)/common
	$(CC) $(CFLAGS) -MM $< -MT $(OBJ_PATH)/$*.o -MF $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	$(RM) -rf $(OBJ_PATH) $(DEP_PATH)
	$(MAKE) -C $(LIBFT_PATH) clean
	$(MAKE) -C $(LIBSCOP_PATH) clean

fclean: clean
	$(RM) -f $(NAME) $(EXEC_C) $(EXEC_H)
	$(MAKE) -C $(LIBFT_PATH) fclean
	$(MAKE) -C $(LIBSCOP_PATH) fclean

$(GLFW_PATH):
	git submodule update --init --recursive

$(GLFW): $(GLFW_PATH)
	cd $(GLFW_PATH) && mkdir -p build && cd build && cmake ../ && make

re: fclean all

.PHONY: all clean fclean re $(LIBFT) $(LIBSCOP) $(NAME)
