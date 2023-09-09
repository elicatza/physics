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

TARGETS := slope_force
TARGET := $(addsuffix, $(OUTDIR)/, $(TARGETS))

SRC := $(foreach x, $(SRCDIR), $(wildcard $(addprefix $(x)/*,.c*)))
OBJ := $(addprefix $(OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))


SLOPE_FORCE_SRCDIR := ./src/slope_force
SLOPE_FORCE_OBJDIR := ./obj/slope_force

SLOPE_FORCE_SRC := $(foreach x, $(SLOPE_FORCE_SRCDIR), $(wildcard $(addprefix $(x)/*,.c*)))
SLOPE_FORCE_OBJ := $(addprefix $(SLOPE_FORCE_OBJDIR)/, $(addsuffix .o, $(notdir $(basename $(SLOPE_FORCE_SRC)))))

SLOPE_FORCE_SRC += $(SRC)
SLOPE_FORCE_OBJ += $(OBJ)

CLEAN_LIST := $(OBJ) $(TARGET)
PREFIX ?= /usr/local/bin


default: makedir all

all: $(TARGET)

# $(TARGET): $(OBJ)
	# $(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(COBJFLAGS) -o $@ $< $(LIBS)

$(TARGET): $(SLOPE_FORCE_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(SLOPE_FORCE_OBJDIR)/%.o: $(SLOPE_FORCE_SRCDIR)/%.c
	$(CC) $(COBJFLAGS) -o $@ $< $(LIBS)


.PHONY: makedir
makedir:
	@mkdir -p $(OUTDIR) $(SRCDIR) $(OBJDIR)
	@mkdir -p $(SLOPE_FORCE_SRCDIR) $(SLOPE_FORCE_OBJDIR)

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

