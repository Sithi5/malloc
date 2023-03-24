################################################################################
#                                BINARIES NAME                                 #
################################################################################

NAME =  libft_malloc.so

################################################################################
#                                   DEFINE                                     #
################################################################################

ifeq ($(HOSTTYPE),)
    HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

################################################################################
#                                COMPILATION MODE                              #
################################################################################

WALL				:=	yes
WEXTRA				:=	yes
WSHADOW				:=	yes
WERROR				:=	no
FSANITIZE			:=	no
DEBUG				:=	no
O2					:=	no

CC					:= gcc
GEN					:= Generation in mode

ifeq ($(WALL), yes)
	CC				+=	-Wall
	GEN				+=	 all
endif

ifeq ($(WEXTRA), yes)
	CC				+=	-Wextra
	GEN				+=	extra
endif

ifeq ($(WSHADOW), yes)
	CC				+=	-Wshadow
	GEN				+=	shadow
endif

ifeq ($(WERROR), yes)
	CC				+=	-Werror
	GEN				+=	error
endif

ifeq ($(FSANITIZE), yes)
	CC				+=	-fsanitize=address
	GEN				+=	sanitize
endif

ifeq ($(DEBUG), yes)
	CC				+=	-g
	CC				+= -D DEBUG
	GEN				+=	debug
endif

ifeq ($(O2),yes)
	CC				+=	-O2
	GEN				+=	O2
endif

ifeq ($(GEN), Generation in mode)
	GEN				+=	no flags
endif




################################################################################
#                                     NAME                                     #
################################################################################

SRC_NAME			:=	malloc.c							\
						free.c 								\
						show_alloc_mem.c 					\
						realloc.c


INCLUDE_NAME		:=	malloc.h

TESTS_SRC_NAME		:= 	./tests/test*.sh					\

LIBRARY_FILE = $(LIBRARY_NAME)_$(HOSTTYPE).so

LDFLAGS = -shared


################################################################################
#                                     PATH                                     #
################################################################################

SRC_PATH			:=	./src/

OBJ_PATH 			:=	./obj/

INCLUDE_PATH		:=	./include/



################################################################################
#                                 NAME + PATH                                  #
################################################################################

SRC					:=	$(addprefix $(SRC_PATH), $(SRC_NAME))

OBJ					:=	$(patsubst $(SRC_PATH)%.c, $(OBJ_PATH)%.o,	$(SRC))

INCLUDE				:=	$(addprefix $(INCLUDE_PATH), $(INCLUDE_NAME))

DYNAMIC_LIB =	$(basename $(NAME))_$(HOSTTYPE).so

all: $(NAME)

$(NAME): $(DYNAMIC_LIB) $(OBJ)
	@echo "\n$(_WHITE)====================================================$(_END)"
	@echo "$(_YELLOW)	COMPILING AND LINKING $(NAME)$(_END)"
	@echo "$(_WHITE)====================================================$(_END)"
	@ln -sf $(DYNAMIC_LIB) $(NAME)
	@echo "\n$(_WHITE)$(_BOLD)$@\t$(_END)$(_GREEN)[OK]\n$(_END)"

$(DYNAMIC_LIB) : $(OBJ)
	@$(CC) $(OBJ) -shared -o $(DYNAMIC_LIB)

$(OBJ_PATH)%.o: $(SRC_PATH)%.c $(INCLUDE)
	@mkdir -p $(OBJ_PATH)
	@$(CC) -I $(INCLUDE_PATH) -c $< -o $@
	@echo "$(_END)$(_GREEN)[OK]\t$(_UNDER)$(_YELLOW)\t"	\
		"COMPILE :$(_END)$(_BOLD)$(_WHITE)\t$<"

tests: all
	@echo "\n$(_CYAN)====================================================$(_END)"
	@echo "$(_YELLOW)		LAUNCHING TESTS$(_END)"
	@echo "$(_CYAN)====================================================$(_END)"
	@for f in $(TESTS_SRC_NAME); do \
        (export HOSTTYPE="$(HOSTTYPE)"; sh $$f); \
    done

clean:
	@rm -rf $(OBJ_PATH) 2> /dev/null || true
	@echo "$(_YELLOW)Remove :\t$(_RED)" $(OBJ_PATH)"$(_END)"

fclean: clean
	@rm -f $(NAME) 2> /dev/null
	@echo "$(_YELLOW)Remove :\t$(_RED)" $(NAME)
	@rm -rf $(DYNAMIC_LIB) 2> /dev/null || true
	@echo "$(_YELLOW)Remove :\t$(_RED)" $(DYNAMIC_LIB)"$(_END)"
	@echo "$(_END)"

re: fclean all

help:
	@echo "$(_YELLOW)Makefile for generating binary infectors."
	@echo "$(_YELLOW)Usage:                                                                    "
	@echo "$(_YELLOW)   make                                runs all                           "
	@echo "$(_YELLOW)   make all                            generates all binaries             "
	@echo "$(_YELLOW)   make clean                          remove the generated files         "
	@echo "$(_YELLOW)   make fclean                  		clean and remove binaries files    "
	@echo "$(_YELLOW)   make tests                          launch tests scripts               "
	@echo "$(_YELLOW)   make help                           prints this message                $(_END)"

.PHONY: all clean fclean re check help tests


################################################################################
#                                    FORMAT                                    #
################################################################################

# Text format
_END		=	\033[0m
_GRAS		=	\033[1m
_SOUL		=	\033[4m
_CLIG		=	\033[5m
_SURL		=	\033[7m

# Colors format
_BLACK		=	\033[30m
_RED		=	\033[31m
_GREEN		=	\033[32m
_YELLOW		=	\033[33m
_BLUE		=	\033[34m
_PURPLE		=	\033[35m
_CYAN		=	\033[36m
_GREY		=	\033[37m

# Background
_IBLACK		=	\033[40m
_IRED		=	\033[41m
_IGREEN		=	\033[42m
_IYELLOW	=	\033[43m
_IBLUE		=	\033[44m
_IPURPLE	=	\033[45m
_ICYAN		=	\033[46m
_IGREY		=	\033[47m