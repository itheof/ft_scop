NAME         = ft_scop
CFLAGS      += -g -Wall -Wextra
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
CFLAGS    += -I./ -I./inc

# Sources
SRC_PATH    = src

SOURCES     = $(NAME:%=%.c) file.c shader.c program.c ppm.c texture.c matrix.c vector.c misc.c
# Generation
vpath %.c $(SRC_PATH) $(addprefix $(SRC_PATH)/,$(SRC_SUBDIR))
OBJ_PATH    = .obj
OBJECTS     = $(SOURCES:%.c=$(OBJ_PATH)/%.o)
DEP_PATH    = .dep/
DEPS        = $(SOURCES:%.c=$(DEP_PATH)/%.d)
BUILD_DIR   = $(OBJ_PATH) $(OBJ_PATH)/common \
			  $(DEP_PATH)/common $(DEP_PATH)

# Libft
LIBFT_PATH = libft
LIBFT      = $(LIBFT_PATH)/libft.a
CFLAGS    += -I $(LIBFT_PATH)/inc
LDFLAGS   += -L$(LIBFT_PATH) -lft

# GLFW
GLFW_PATH  = glfw
GLFW       = glfw/build/src/libglfw3.a
CFLAGS    += -I./glfw/include
LDFLAGS   += -L glfw/build/src -lglfw3

ifeq ($(UNAME_S),Darwin)
	LDFLAGS += -framework Cocoa -framework OpenGL -framework IOKit -framework \
		  CoreVideo
else
	GLAD_PATH = glad
	GLAD = $(GLAD_PATH)/src/glad.c
	CPATH = CPATH=$(GLAD_PATH)/include
	CFLAGS	+= -I $(GLAD_PATH)/include
	LDFLAGS += `PKG_CONFIG_PATH=glfw/build/src/ pkg-config --libs --static glfw3`
endif

.SECONDARY: $(OBJECTS)

all: $(DEPS) $(NAME)

-include $(DEPS)

$(NAME): %:$(OBJ_PATH)/%.o $(OBJECTS) $(GLAD) | $(LIBFT) $(GLFW)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(LIBFT):
	@$(MAKE) -q -C $(LIBFT_PATH) || echo $(MAKE) -C $(LIBFT_PATH) && \
		$(MAKE) -j8 -C $(LIBFT_PATH)

$(OBJECTS): $(OBJ_PATH)/%.o: %.c | $(OBJ_PATH)/common $(GLFW_PATH)
	$(CC) $(CFLAGS) -o $@ -c $<

$(DEP_PATH)/%.d: %.c | $(DEP_PATH)/common
	$(CC) $(CFLAGS) -MM $< -MT $(OBJ_PATH)/$*.o -MF $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	$(RM) -rf $(OBJ_PATH) $(DEP_PATH)
	$(MAKE) -C $(LIBFT_PATH) clean

fclean: clean
	$(RM) -f $(NAME) $(EXEC_C) $(EXEC_H)
	$(MAKE) -C $(LIBFT_PATH) fclean

$(GLFW_PATH):
	git submodule update --init --recursive

$(GLFW): $(GLFW_PATH)
	cd glfw && mkdir -p build && cd build && cmake ../ && make

$(GLAD_PATH):
	pip install --user glad
	python -m glad --generator=c --profile=core --out-path glad

$(GLAD): $(GLAD_PATH)

re: fclean all

.PHONY: all clean fclean re $(LIBFT)
