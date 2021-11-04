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

# Directory variables.
INC_DIR = ./include

# Flags passed to the C++ compiler.
CFLAGS += -g -Wall -Wextra


LIBXLSXWRITER = ./src/libxlsxwriter.a

LIB_EXTENSIONS = *.o *.a *.so *.so.* *.to *.dylib *.dll *.gcno *.gcda
LIB_PATHS = $(subst *.,./lib/*.,$(LIB_EXTENSIONS))

LIBS = $(LIBXLSXWRITER) -lz
ifdef USE_SYSTEM_MINIZIP
LIBS += -lminizip
endif
ifdef USE_OPENSSL_MD5
LIBS += -lcrypto
endif

all : $(LIBXLSXWRITER)

$(LIBXLSXWRITER):
	$(Q)$(MAKE) -C ./third_party/minizip
ifndef USE_STANDARD_TMPFILE
	$(Q)$(MAKE) -C ./third_party/tmpfileplus
endif
ifndef USE_STANDARD_DOUBLE
	$(Q)$(MAKE) -C ./third_party/dtoa
endif
ifndef USE_NO_MD5
	$(Q)$(MAKE) -C ./third_party/md5
endif
	$(Q)$(MAKE) -C ./src



clean :
	$(Q)$(MAKE) -C ./third_party/minizip clean
ifndef USE_STANDARD_TMPFILE
	$(Q)$(MAKE) -C ./third_party/tmpfileplus clean
endif
ifndef USE_STANDARD_DOUBLE
	$(Q)$(MAKE) -C ./third_party/dtoa clean
endif
ifndef USE_NO_MD5
	$(Q)$(MAKE) -C ./third_party/md5 clean
endif
	$(Q)$(MAKE) -C ./src clean
	
	$(Q)rm -f $(LIB_PATHS)