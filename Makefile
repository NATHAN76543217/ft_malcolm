.PHONY: clean fclean re all os check_sources check_headers

#######################################################################
#
# Makefile for C projects
# (OS detect)
#
#######################################################################

# Name of target executable
NAME		= ft_malcolm

# Locations 
SRC_DIR		= srcs
INC_DIR		= includes
BIN_DIR		= bin
LIB_DIR		= lib

# Sources & Headers 
# - fill only with name of the file
# - make will check for the file in SRC_DIR
# - use "-" if empty
SRCS		=	ft_malcolm.c \
				bpf.c \
				utils.c
HEADERS		=	ft_malcolm.h

LIBRARIES	= libft argparse


# Openssl
# OPENSSL_VERSION=1.1
# OPENSSL_PATH= $(shell brew --prefix openssl@$(OPENSSL_VERSION))

COMP		=	clang
C_DFLAGS	=	#-g3 -fsanitize=address
C_FLAGS		=	-Wextra -Wall -Werror -std=c99 $(C_DFLAGS)

C_IFLAGS	=	-I $(INC_DIR) -I$(LIB_DIR)/argparse -I$(LIB_DIR)/libft/includes
C_LFLAGS	=	-L $(LIB_DIR)/libft -L$(LIB_DIR)/argparse -lft -largparse

# Fancy prefixes 
PREFIX_PROJECT=[\033[1;32m$(NAME)\033[0m]
PREFIX_COMP=\033[1;30m-\033[0m-\033[1;37m>\033[0m[\033[1;32m✔\033[0m]
PREFIX_LINK=[\033[1;32mLINK\033[0m]
PREFIX_INFO=[\033[1;32mINFO\033[0m]
PREFIX_WARN=[\033[0;33mWARN\033[0m]
PREFIX_ERROR=[\033[0;91mERROR\033[0m]
PREFIX_DUPL=[\033[1;33mDUPLICATES\033[0m]
PREFIX_CLEAN=[\033[1;31mCLEAN\033[0m]


############################################################################################

# OS Detecting 
# Compiler and flags (include and library include are autogenerated if they are local)
ifeq ($(OS),Windows_NT)
    OSDETECT = WIN32
	C_FLAGS += -DWIN32
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        OSDETECT = LINUX
		C_FLAGS += -DLINUX
    else ifeq ($(UNAME_S),Darwin)
        OSDETECT = OSX
		C_FLAGS += -DOSX
	else
		OSDETECT = UNKNOW
    endif
endif

############################################################################################


SRC_FILES	=	$(shell find $(SRC_DIR) | grep -E '$(shell echo $(SRCS) | tr ' ' '|')')
HEADER_FILES=	$(shell find $(INC_DIR) | grep -E '$(shell echo $(HEADERS) | tr ' ' '|')')
OBJS		=	$(addprefix $(BIN_DIR)/, $(SRC_FILES:.c=.o))
C_IFLAGS	+=	$(addprefix -I,$(shell echo $(HEADER_FILES) | tr ' ' '\n' | rev | cut -d'/' -f2- | rev | sort | uniq))
C_LFLAG		+=	$(addprefix -L,$(addprefix $(LIB_DIR), $(LIBRARIES)))

#   Main rule
all: os comp_lib check_sources check_headers $(NAME)
	@echo "$(PREFIX_PROJECT)$(PREFIX_INFO) done"

os:
ifeq ($(OSDETECT),WIN32)
	@echo "$(PREFIX_PROJECT)$(PREFIX_ERROR) This project cannot compile yet on windows"
	@exit 0
else ifeq ($(OSDETECT), OSX)
	@echo "$(PREFIX_PROJECT)$(PREFIX_INFO) Compilation target: OSX"
else ifeq ($(OSDETECT), LINUX)
	@echo "$(PREFIX_PROJECT)$(PREFIX_INFO) Compilation target: Linux"
endif


#	check_sources :
#	simple bash script to check duplicates sources files 
check_sources:
	@duplicates=$$( echo $(SRC_FILES) | tr ' ' '\n' | rev | cut -d'/' -f1 | rev | sort | uniq -c | sed 's/ //g' ) ; \
	error=0 ; \
	for source in $$duplicates ; do \
			if [ $$(echo $$source | sed 's/[^0-9]//g') -gt 1 ] ; then \
				echo "$(PREFIX_PROJECT)$(PREFIX_DUPL)" Duplicates source files found for \"$$(echo $$source | sed 's/[0-9]//g')\" in: ; \
				find $(SRC_DIR) | grep $$(echo $$source | sed 's/[0-9]//g') ; \
				error=1 ; \
			fi \
	done ; \
	if [ $$error -eq 1 ] ; then \
		echo "$(PREFIX_PROJECT)$(PREFIX_ERROR) Cannot manage duplicates files, aborting..." ; \
		exit 1 ; \
	fi

#	check_headers :
#	simple bash script to check duplicates header files 
check_headers:
	@duplicates=$$( echo $(HEADER_FILES) | tr ' ' '\n' | rev | cut -d'/' -f1 | rev | sort | uniq -c | sed 's/ //g' ) ; \
	error=0 ; \
	for source in $$duplicates ; do \
			if [ $$(echo $$source | sed 's/[^0-9]//g') -gt 1 ] ; then \
				echo "$(PREFIX_PROJECT)$(PREFIX_DUPL)" Duplicates header files found for \"$$(echo $$source | sed 's/[0-9]//g')\" in: ; \
				find $(INC_DIR) | grep $$(echo $$source | sed 's/[0-9]//g'); \
				error=1 ; \
			fi \
	done ; \
	if [ $$error -eq 1 ] ; then \
		echo "$(PREFIX_PROJECT)$(PREFIX_ERROR) Cannot manage duplicates files, aborting..." ; \
		exit 1 ; \
	fi

#	Compilation of libraries
comp_lib:
	@echo "$(PREFIX_PROJECT)$(PREFIX_INFO) Compiling libraries."
	@for lib in $(LIBRARIES) ; do \
		echo "$(PREFIX_PROJECT)$(PREFIX_INFO) Compiling library $$lib"; \
		make -C $(LIB_DIR)/$$lib ; \
	done

#	Clean of libraries
fclean_lib:
	@echo "$(PREFIX_PROJECT)$(PREFIX_CLEAN) Cleaning libraries."
	@for lib in $(LIBRARIES) ; do \
		echo "$(PREFIX_PROJECT)$(PREFIX_CLEAN) Cleaning library $$lib"; \
		make -C $(LIB_DIR)/$$lib fclean; \
	done

#	Bin directory
$(BIN_DIR):
	@echo "$(PREFIX_PROJECT)$(PREFIX_WARN) No bin dir found. Creating one."
	@mkdir -p $(BIN_DIR)

#	Linking rule
$(NAME): $(BIN_DIR) $(OBJS)
	@$(COMP) $(OBJS) -o $(NAME) $(C_FLAGS) $(C_LFLAGS)
	@echo "$(PREFIX_PROJECT)$(PREFIX_LINK) Linking done for: $(NAME)"


# Compilation rule 
$(BIN_DIR)/$(SRC_DIR)/%.o : $(SRC_DIR)/%.c $(HEADER_FILES)
	@mkdir -p $(BIN_DIR)/$(shell dirname $<)
	@$(COMP) -c $< -o $@ $(C_FLAGS) $(C_IFLAGS)
	@echo "$(PREFIX_PROJECT)$(PREFIX_COMP) Compiled: $(shell basename $<)"

# clean rule
clean:
	@echo "$(PREFIX_PROJECT)$(PREFIX_CLEAN) Cleaning $(BIN_DIR)/"
	@rm -rf $(BIN_DIR)

# final clean rule
fclean: fclean_lib clean
	@echo "$(PREFIX_PROJECT)$(PREFIX_CLEAN) Cleaning $(NAME)"
	@rm -f $(NAME)

# remake rule
re: fclean all