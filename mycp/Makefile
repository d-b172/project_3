CC = gcc
INC = -Iinclude
OBJDIR = obj
SRCDIR = src
CC_FLAGS = -w -pedantic $(INC)

EXEC = ../bin/mycp
SOURCES = $(shell find $(SRCDIR) -name '*.c')
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | make-folder
	$(CC) $(CC_FLAGS) -c $< -o $@

clean:
	rm -rf $(EXEC) $(OBJDIR)

make-folder:
	mkdir -p $(OBJDIR)