# Relevant codes
# $^ -> pre-requisitos
# $@ -> alvo
# $< -> primeiro pre-requisito

# %.o : %.c -> qq coisa .o depende de qq coisa .c

# flag -M -> mostra os includes de um arquivo
# flag -MM -> mostra as bibliotecas inclusas num arquivo, apenas do projeto

# detectando headers automaticos

# makefiles = %(OBJECTS:%.o=%.d) troca os .o por .d
# include $(makefiles) # inclue os arqs main.d
# %.d : %.c
#	-MM : criar uma regra de makefile
#	gcc -I. -MM -MT '$(<:.c=.o) $@' $< >$@ # redireciona para o alvo
#	-MT -> o arg na frente diz oq fica na regra alvo
#	'$(<:.c=.o) $@' : ler os pre-requisitos trocando .c por .o 

#------------------------------------------------------------------------------------------------

# Project configuration

# 
# CUSTOMIZE FOR YOUR PROJECT
#

# Give your project a name, e.g.
# PROJECT = FooBar
PROJECT = runcmd

# Name your project's binaires, eg.
# bin = foo bar

# For each item in $(bin), name the objects needed to build it, e.g.
# foo_obj = main_foo.o 
# bar_obj = main_bar.o 

# For each item in $(bin), name the project's libraries to which it shall be linked, e.g.
# foo_lib = libfoobar
# bar_lib = libfoobar libm

# For each item in $(bin), name the other libraries to which it shall be linked, e.g.
# foo_lib_other = libfoobar
# bar_lib_other = libfoobar libm


# Name your project's libraries (to be installed), e.g.
# lib = libfoobar
lib = libruncmd

# For each item in $(lib), name the objects needed to build it, e.g.
# libfoobar_obj = foo.o bar.o
libruncmd_obj = runcmd.o

# For each item in $(lib), name the headers your project installs, e.g.
#libfoobar_h = foos.h
libruncmd_h = runcmd.h

# Name your project's auxuliary binaries (not to be installed), e.g.
# noinst_bin = example
noinst_bin = test-runcmd delay io segfault t1

# For each item in $(noinst_bin), name the objects needed to build it, e.g.
# example_obj = example.o 
delay_obj       = delay.o
io_obj          = io.o
segfault_obj    = segfault.o
test-runcmd_obj = test-runcmd.o
t1_obj          = t1.o

# For each item in $(noinst_bin), name the libraries to which it's to be linked, e.g.
# example_lib = libfoobar
test-runcmd_lib = libruncmd

# Extra files to go in the distribution (add as needed), e.g. 
#EXTRA_DIST = runcmd.txt - documentation
EXTRA_DIST = runcmd.txt

# List the automatically generated, distributed source files of any kind.
# These files are generated in the development environment but should
# be already available in the building enviroment. E.g. config scripts.
gendist_files = debug.h Makefile

# Installation prefix
PREFIX=./local

# Customize the building environemnt (modify as desired)
CC        = gcc
CPP_FLAGS = -Wall --ansi --pedantic-errors -D_POSIX_C_SOURCE=200809L 
C_FLAGS   = 
LD_FLAGS  = 
MAKE      = make
AR        = ar

# Housekeeping (name temporary files to clean)
# EXTRA_GARBAGE = *~
EXTRA_GARBAGE = *~ \#*

# Name your tests
tests-bin = test-runcmd

