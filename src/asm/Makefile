PROJDIR	=	../..
-include $(PROJDIR)/vmachine.mak

DESTDIR	=	$(PROJDIR)/bin
TARGET	=	$(DESTDIR)/asm

make_dirs := $(shell mkdir -p $(OBJDIR))

SLST	=	parser.c \
			scanner.c \
			arith_expr.c \
			emit.c \
			symbols.c \
			listing.c \
			error.c \
			asm.c

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(COPTS) -o $(TARGET) $(OBJS) $(LIBS)

parser.c parser.h: parser.y
	bison --report=all -tvdo parser.c parser.y

scanner.c: scanner.l
	flex -io scanner.c scanner.l

clean:
	-$(RM) -r $(OBJDIR) $(TARGET) parser.c parser.h scanner.c parser.output

format: $(PROJDIR)/astyle.rc $(SRCS) $(HEADERS)
	astyle -v --options=$(PROJDIR)/astyle.rc $(SRCS) $(HEADERS); \
	mv -vf *.bak $(OBJDIR) || true