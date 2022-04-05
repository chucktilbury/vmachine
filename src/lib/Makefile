
PROJDIR	=	../..
include $(PROJDIR)/vmachine.mak

DESTDIR	=	$(PROJDIR)/bin
TARGET	=	$(DESTDIR)/libvm.a

make_dirs := $(shell mkdir -p $(OBJDIR) )

SLST	=	error.c \
			expressions.c \
			file_io.c \
			instr.c \
			opcodes.c \
			strings.c \
			symbols.c \
			utils.c \
			value.c \
			support.c

all: $(TARGET)

$(TARGET): $(OBJS)
	ar cr $(TARGET) $(OBJS)

clean:
	-$(RM) -r $(OBJDIR) $(TARGET)

format: $(PROJDIR)/astyle.rc $(SRCS) $(HEADERS)
	astyle -v --options=$(PROJDIR)/astyle.rc $(SRCS) $(HEADERS); \
	mv -vf *.bak $(OBJDIR) || true