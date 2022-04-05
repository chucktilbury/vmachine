PROJDIR	=	../..
-include $(PROJDIR)/vmachine.mak

DESTDIR	=	$(PROJDIR)/bin
TARGET	=	$(DESTDIR)/asm

make_dirs := $(shell mkdir -p $(OBJDIR))

SLST	=	parser.c \
			scanner.c \
			main.c

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(COPTS) -o $(TARGET) $(OBJS) $(LIBS)

parser.c parser.h: parser.y
	bison --report=lookahead -tvdo parser.c parser.y

scanner.c: scanner.l
	flex -io scanner.c scanner.l

clean:
	-$(RM) -r $(OBJDIR) $(TARGET) parser.c parser.h scanner.c

format: $(PROJDIR)/astyle.rc main.c
	astyle -v --options=$(PROJDIR)/astyle.rc main.c; \
	mv -vf *.bak $(OBJDIR) || true