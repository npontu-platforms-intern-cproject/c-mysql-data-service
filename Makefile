###############################################################################
#
# Makefile for libxlsxwriter examples.
#
# Copyright 2014-2021, John McNamara, jmcnamara@cpan.org
#

# Keep the output quiet by default.
Q=@
ifdef V
Q=
endif

MYSQL_INC_DIR := $(shell mysql_config --cflags)
LIBMYSQLCLIENT := $(shell mysql_config --libs)

# Directory variables.
INC_DIR = ./include/libxlsxwriter/include

# Flags passed to the C++ compiler.
CFLAGS += -g -Wall -Wextra
CFLAGS += $(MYSQL_INC_DIR)

# Source files to compile.
SRCS = $(wildcard ./src/*.c)
EXES = $(patsubst ./src/%.c,./bin/%,$(SRCS))

BIN_PATH = ./bin
LIBXLSXWRITER = ./include/libxlsxwriter/src/libxlsxwriter.a

# Libraries to include
LIBS = $(LIBXLSXWRITER) -lz
ifdef USE_SYSTEM_MINIZIP
LIBS += -lminizip
endif
ifdef USE_OPENSSL_MD5
LIBS += -lcrypto
endif

LIBS += $(LIBMYSQLCLIENT)

all : $(LIBXLSXWRITER) $(EXES)

$(LIBXLSXWRITER):
	$(Q)$(MAKE) -C ./include/libxlsxwriter

clean :
	$(Q)rm -f $(EXES)

# Executable targets.
$(EXES): $(SRCS) $(LIBXLSXWRITER) | $(BIN_PATH)
	$(Q)$(CC) -I$(INC_DIR) $(CFLAGS) $< -o $@ $(LIBS)

$(BIN_PATH):
	mkdir $(BIN_PATH)
