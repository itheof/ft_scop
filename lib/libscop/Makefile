NAME         = libscop.a
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

SOURCES     = file.c shader.c program.c ppm.c texture.c matrix.c vector.c \
			  angle_conv.c display.c libscop.c objects.c model.c
# Generation
vpath %.c $(SRC_PATH)
OBJ_PATH    = .obj
OBJECTS     = $(SOURCES:%.c=$(OBJ_PATH)/%.o)
DEP_PATH    = .dep/
DEPS        = $(SOURCES:%.c=$(DEP_PATH)/%.d)
BUILD_DIR   = $(OBJ_PATH) $(DEP_PATH)

# Libft
LIBFT_PATH = libft
LIBFT      = $(LIBFT_PATH)/libft.a
CFLAGS    += -I$(LIBFT_PATH)/inc

# Glad
ifeq ($(UNAME_S),Darwin)
else
	GLAD_PATH = glad
	GLAD = $(GLAD_PATH)/src/glad.c
	GLAD_OBJ = $(GLAD:%.c=%.o)
	GLAD_CFLAGS	= -I$(GLAD_PATH)/include
	CFLAGS    += $(GLAD_CFLAGS)
endif

.SECONDARY: $(OBJECTS)

all: $(DEPS) $(NAME)

-include $(DEPS)

$(NAME): $(OBJECTS) $(GLAD_OBJ)
	$(AR) rcs $@ $^

$(LIBFT):
	@$(MAKE) -q -C $(LIBFT_PATH) || echo $(MAKE) -C $(LIBFT_PATH) && \
		$(MAKE) -j8 -C $(LIBFT_PATH)

$(OBJECTS): $(OBJ_PATH)/%.o: %.c | $(GLAD) $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ -c $<

$(GLAD_OBJ): $(GLAD)
	$(CC) $(GLAD_CFLAGS) -o $@ -c $<

$(DEP_PATH)/%.d: %.c | $(DEP_PATH)
	$(CC) $(CFLAGS) -MM $< -MT $(OBJ_PATH)/$*.o -MF $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	$(RM) -rf $(OBJ_PATH) $(DEP_PATH)
	$(MAKE) -C $(LIBFT_PATH) clean

fclean: clean
	$(RM) -f $(NAME) $(EXEC_C) $(EXEC_H)
	$(MAKE) -C $(LIBFT_PATH) fclean

$(GLAD):
	mkdir -p `readlink $(GLAD_PATH)`
	pip install --user glad
	python -m glad --generator=c --profile=core --out-path $(GLAD_PATH)

re: fclean all

.PHONY: all clean fclean re $(LIBFT)
