CC ?= gcc

export PKG_CONFIG_PATH=lib/raylib/lib/pkgconfig/

CFLAGS := -std=c99 -pedantic -Wall -Wextra -Wshadow -Werror \
		  $(shell pkg-config --cflags raylib)
LIBS := -lm -lglfw -lpthread -ldl \
		 $(shell pkg-config --libs raylib)

COBJFLAGS := $(CFLAGS) -c

SRCDIR := ./src
OBJDIR := ./obj
OUTDIR := ./bin

TARGET_NAME := physics
TARGET := $(OUTDIR)/$(TARGET_NAME)

SRC := $(foreach x, $(SRCDIR), $(wildcard $(addprefix $(x)/*,.c*)))
OBJ := $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

CLEAN_LIST := $(OBJ) $(TARGET)
PREFIX ?= /usr/local/bin


default: makedir all

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(COBJFLAGS) -o $@ $< $(LIBS)


.PHONY: makedir
makedir:
	@mkdir -p $(OUTDIR) $(SRCDIR) $(OBJDIR)

.PHONY: install
install:
	chmod 755 $(TARGET)
	mkdir -p $(PREFIX)/bin
	cp -f $(TARGET) $(PREFIX)/bin/

.PHONY: uninstall
uninstall:
	rm -f $(PREFIX)/bin/$(TARGET_NAME)

.PHONY: clean
clean:
	@rm -f $(CLEAN_LIST)

