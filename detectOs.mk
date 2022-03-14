
############################################################################################

# OS Detecting 
# 
# Supported os: OSX WIN32 LINUX UNKNOWN
# 
# Fill OSDETECT with the detected os.
# Fill COMP with the adequate compilator (Support gcc && clang)
# Append -D$(OSDETECT) to the CFLAGS var
#

ifeq ($(OS),Windows_NT)
    OSDETECT		=	WIN32
	CFLAGS			+=	-DWIN32
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        OSDETECT	=	LINUX
		COMP		=	gcc
		CFLAGS			+=	-D$(OSDETECT) -std=gnu11  
    else ifeq ($(UNAME_S),Darwin)
        OSDETECT 	=	OSX
		COMP		=	clang
		CFLAGS			+=	-D$(OSDETECT)
	else
		OSDETECT	=	UNKNOWN
		COMP		=	gcc
    endif
endif


display_os:
ifeq ($(OSDETECT),WIN32)
	@echo "$(PREFIX_PROJECT)$(PREFIX_ERROR) This project cannot compile yet on windows"
	@exit 1
else ifeq ($(OSDETECT), UNKNOWN)
	@echo "$(PREFIX_PROJECT)$(PREFIX_ERROR) Cannot detect the target OS. Aborting."
	@exit 1
else
	@echo "$(PREFIX_PROJECT)$(PREFIX_INFO) Compilation target: $(OSDETECT)"
endif


############################################################################################
