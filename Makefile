MIX=mix
CC=clang
LIBEJDB_PATH=deps/libejdb
LIBEJDB_INSTALL=$(LIBEJDB_PATH)/install_prefix
CFLAGS=-O3 -std=c99 -pedantic -Wall

ERLANG_PATH = $(shell erl -eval 'io:format("~s", [lists:concat([code:root_dir(), "/erts-", erlang:system_info(version), "/include"])])' -s init stop -noshell)
CFLAGS += -I$(ERLANG_PATH) -I$(LIBEJDB_INSTALL)/include

OBJDIR = priv
SRCDIR = src
C_SRCS = nif.c ejdb.c
SRCS = $(addprefix $(SRCDIR)/,$(C_SRCS))
OBJS = $(addprefix $(OBJDIR)/,$(C_SRCS:.c=.o))

ifneq ($(OS),Windows_NT)
	CFLAGS += -fPIC

	ifeq ($(shell uname),Darwin)
		LDFLAGS += -dynamiclib -undefined dynamic_lookup
	endif
endif


.PHONY: libejdb clean

libejdb:
	cd $(LIBEJDB_PATH) && \
	mkdir -p build && \
	cd build && \
	cmake -DBUILD_SAMPLES=OFF -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=$(CURDIR)/$(LIBEJDB_INSTALL) -DCMAKE_BUILD_TYPE=Release ../ && \
	make && \
	make install

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

priv/ejdb.so: $(OBJS)
	$(CC) $(CFLAGS) -shared $(LDFLAGS) -o $@ $^ $(LIBEJDB_INSTALL)/lib/libejdb-1.a

clean:
	$(MIX) clean
	make -C $(LIBEJDB_PATH)/build clean
	rm -rf priv/ejdb.so priv/*.o
