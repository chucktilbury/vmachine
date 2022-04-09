
PROJDIR	=	../..
include $(PROJDIR)/vmachine.mak

DESTDIR	=	$(PROJDIR)/bin
TARGET	=	$(DESTDIR)/libvm.a

make_dirs := $(shell mkdir -p $(OBJDIR) )

SLST	=	error.c \
			memory.c \
			file_io.c \
			instr.c \
			opcodes.c \
			strings.c \
			symbols.c \
			value.c \
			cmd_line.c \
			generic_tree.c \
			support.c

all: $(TARGET)

$(TARGET): $(OBJS)
	ar cr $(TARGET) $(OBJS)

clean:
	-$(RM) -r $(OBJDIR) $(TARGET)

format: $(PROJDIR)/astyle.rc $(SRCS) $(HEADERS)
	astyle -v --options=$(PROJDIR)/astyle.rc $(SRCS) $(HEADERS); \
	mv -vf *.bak $(OBJDIR) || true