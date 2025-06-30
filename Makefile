.PHONY: debug release all clean

CC = gcc
DEBUG_CFLAGS = -Wall -Wextra -g
RELEASE_CFLAGS = -Wall -Wextra -O2
OBJDIR = build
BINDIR = bin
SRCDIR = site_generator

SOURCES := $(wildcard $(SRCDIR)/*.c)
BASENAMES := $(notdir $(SOURCES:.c=)) 
OBJECTS := $(addprefix $(OBJDIR)/, $(addsuffix .o, $(BASENAMES)))
TARGET = $(BINDIR)/site_generator

debug: CFLAGS = $(DEBUG_CFLAGS)
debug : all

release : CFLAGS = $(RELEASE_CFLAGS)
release: all

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR) site/

site: all
	mkdir -p site
	mkdir -p site/posts
	cp -r static site
	./$(TARGET)
