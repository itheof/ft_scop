NAME         = ft_scop
CFLAGS      += -g -Wall -Wextra
CFLAGS      += -std=c11 -pedantic -pedantic-errors

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
CFLAGS    += -I./ -I./inc -I./glfw/include

# Sources
SRC_PATH    = src

SOURCES     = $(NAME:%=%.c) file.c shader.c program.c
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

# FW
FW		= -framework Cocoa -framework OpenGL -framework IOKit -framework \
		  CoreVideo

LDFLAGS   += -L $(LIBFT_PATH) -lft -L glfw/build/src -lglfw3 $(FW)

.SECONDARY: $(OBJECTS)

all: $(DEPS) $(NAME)

-include $(DEPS)

$(NAME): %:$(OBJ_PATH)/%.o $(OBJECTS) | $(LIBFT) glfw/build/src/libglfw3.a
	$(CC) $(LDFLAGS) -o $@ $^

$(LIBFT):
	@$(MAKE) -q -C $(LIBFT_PATH) || echo $(MAKE) -C $(LIBFT_PATH) && \
		$(MAKE) -j8 -C $(LIBFT_PATH)

$(OBJECTS): $(OBJ_PATH)/%.o: %.c | $(OBJ_PATH)/common glfw
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

glfw:
	git submodule update --init --recursive

glfw/build/src/libglfw3.a: glfw
	cd glfw && mkdir build && cd build && cmake ../ && make

re: fclean all

.PHONY: all clean fclean re $(LIBFT)
