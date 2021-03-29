# get pwd to avoid a deletion of /bin if server of client is empty string
ROOT_DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

CC = gcc
CFLAGS =
LDFLAGS =

SRCDIR = src/
INCDIR = include/
BUILDDIR = build/
BINDIR = bin/

EXENAME = ftp_server

EXE = $(BINDIR)$(EXENAME)

OBJS = $(patsubst $(SRCDIR)%.c,$(BUILDDIR)%.o,$(wildcard $(SRCDIR)*.c))

LIBS = -lpthread


all: $(EXE)

$(EXE): $(OBJS)
	@mkdir -p $(BINDIR)
	$(CC) $(LDFLAGS) $(OBJS) -o $(EXE) $(LIBS)

$(BUILDDIR)%.o: $(SRCDIR)%.c
	@mkdir -p $(BUILDDIR)
	$(CC) -I$(INCDIR) -c -o $@ $<

clean:
	rm -f $(ROOT_DIR)/$(BINDIR)* $(ROOT_DIR)/$(BUILDDIR)*

.PHONY: clean, all