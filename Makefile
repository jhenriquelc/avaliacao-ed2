#
# 'make'        build executable file 'main'
# 'make clean'  removes all .o and executable files
#

# define the C compiler to use
CC = gcc

EXECNAME := avl

# define any compile-time flags
CFLAGS	:= -Wall -Wextra -g -O2

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS =

# define output directory
OUTDIR = out

# define source directory
SRCDIR		:= src

# define include directory
INCLUDEDIR	:= include

# define lib directory
LIBDIRS		:= lib


ifeq ($(OS),Windows_NT)
MAIN	:= $(EXECNAME).exe
FIXPATH = $(subst /,\,$1)
RM			:= del /q /r /f
MD	:= mkdir
else
MAIN	:= $(EXECNAME)
FIXPATH = $1
RM = rm -rf
MD	:= mkdir -p
endif

# define any directories containing header files other than /usr/include
INCLUDES	:= $(patsubst %,-I%, $(INCLUDEDIR:%/=%))

# define the C libs
LIBS		:= $(patsubst %,-L%, $(LIBDIRS:%/=%))

# define the C source files
SRCS = $(wildcard $(SRCDIR)/*.c)

# define the C object files 
OBJS = $(SRCS:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)


# Directories
SRCDIR = src
BUILDDIR = build


# Target: the final executable
all: $(OUTDIR)/$(MAIN)

# Build the object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(MD) $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link the object files into the executable
$(OUTDIR)/$(MAIN): $(OBJS)
	mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LFLAGS) $(LIBS)

# Clean up
clean:
	$(RM) $(BUILDDIR) $(OUTDIR)