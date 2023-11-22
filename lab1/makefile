# ---------------------------------------------------------------------------
# Makefile for Lab2 MPC - Multicore threads and processes 
# Module:     MPC
# Changes:  donn
# File:     makefile 
# Version:  v.fs21
# ---------------------------------------------------------------------------
SHELL := /bin/env bash

CC   := gcc -std=gnu99 -pg
LD   := $(CC)
LINT := flint

CFLAGS  := -g3 -O0 -Wall -Wextra -pedantic # -Werror, commentd out because the image library has a strange warning  which is then converted to an error
LDFLAGS := -static
SLFLAGS := -fPIC - shared

#
# Exemplary values for library names: pthread, m, rt, ...
# Can be a single token or a list of tokens, separated by blanks.
# #
LIBNAMES := m
LIBRARIES := $(addprefix -l, $(LIBNAMES))

#
# External library directories that the linker looks
# at, if one wants to link against a non standard library
# which is located in an arbitrary directory.
# Can be a single token or a list of tokens, separated by blanks.
# #
LIBDIRS :=
LINKER_LIBDIRS := $(addprefix -L, $(LIBDIRS))

#
# Include directory / directories. When invoked, the compiler
# looks into the directories defined here for header files.
# Can be a single token or a list of tokens, separated by blanks.
# #
INCDIRS := ./include
INCLUDES := $(addprefix -I, $(INCDIRS))

SRCDIR := ./src

OBJDIR := ./obj
BINDIR := ./bin

#
# Linting stuff (flint specific) ...
#
# Configuration directory for the compiler specific
# linter option files. Directory for a native
# GCC environment can be generated through the accompanying
# Makefile, i.e.: lnt-gcc-cfg.mk --> make -f lnt-gcc-cfg.mk
#
# TODO: Other compiler option configuration sets to be
#       elaborated for flexe-ling, e.g.:
#       Texas Instruments ARM Compiler, cross ARM gcc,
#       cross RISC-V gcc, et cetera.
# #
LNT_CFG_DIR := ./lint-options
#
# Compiler specific options main file for the linter.
# #
LNT_CO_MAIN_CFG := co-gcc.lnt
#
# Project specific options file for the linter.
# #
LNT_SPECIFIC_CFG := ./$(LNT_CFG_DIR)/lint-opts.lnt
LNT_CFG_FILES    := $(LNT_CO_MAIN_CFG) $(LNT_SPECIFIC_CFG)
LNT_INCDIRS      := ./ $(LNT_CFG_DIR) $(INCDIRS)
LNT_INCLUDES     := $(addprefix -i , $(LNT_INCDIRS))
#
# The linter shall always exit with zero. If the
# -zero flag is not passed to the linter, the
# return code of flint is the number of reported
# messages with upper bound 255.
# #
LINTFLAGS   := $(LNT_INCLUDES) $(LNT_CFG_FILES) -zero
LNT_OUT_DIR := ./lnt-out

#
# Source files of the current project which are
# located in the source directory, $(SRCDIR).
# But only the module file names, without path
# specification. See $(VPATH) below.
# #
SRC := sobel_main.c sobel_rgb2g.c write_csv.c sobel_algo_answer.c

OBJ := $(addprefix $(OBJDIR)/, $(SRC:.c=.o))
BINARY := $(BINDIR)/fancy-calc

#
# Generated reports per module by the linter.
# #
LOBS := $(addprefix $(LNT_OUT_DIR)/, $(SRC:.c=.lob))

#
# Directories to be created where the
# target files are stored into.
# #
MKDIRS := $(OBJDIR) $(BINDIR)

#
# Makes the compilation pattern rule (see further below) work. It
# is used for compiling the sources. If one has a target dependency
# of %.c, all the directories assigned to VPATH are searched for
# %.c.
# #
VPATH := $(SRCDIR)


#
# Phony targets declaration. These targets
# do not represent a file in the file system.
# Therefore declared as phony.
# #
.PHONY: all lint shared clean


all: $(MKDIRS) $(BINARY)


lint: $(LNT_CFG_DIR) $(LNT_OUT_DIR) $(LOBS)


$(MKDIRS):
	mkdir $@


$(LNT_OUT_DIR):
	mkdir $@


$(BINARY): $(OBJ)
	$(LD) $(LDFLAGS) $(LINKER_LIBDIRS) -o $@ $^ $(LIBRARIES)


$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@


$(LNT_OUT_DIR)/%.lob: %.c $(LNT_SPECIFIC_CFG)
	$(LINT) $(LINTFLAGS) $< > $@ 2>&1


clean:
	-rm -rf $(MKDIRS) $(LNT_OUT_DIR)


#-----------------------------------------------------------------------------
