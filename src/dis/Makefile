PROJDIR	=	../..
-include $(PROJDIR)/vmachine.mak

DESTDIR	=	$(PROJDIR)/bin
TARGET	=	$(DESTDIR)/dis

make_dirs := $(shell mkdir -p $(OBJDIR))

SLST	=	dis.c

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(COPTS) -o $(TARGET) $(OBJS) $(LIBS)

clean:
	-$(RM) -r $(OBJDIR) $(TARGET)

format: $(PROJDIR)/astyle.rc dis.c
	astyle -v --options=$(PROJDIR)/astyle.rc dis.c; \
	mv -vf *.bak $(OBJDIR) || true