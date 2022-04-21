
PROJDIR	=	../..
include $(PROJDIR)/vmachine.mak

DESTDIR	=	$(PROJDIR)/bin
TARGET	=	$(DESTDIR)/libvm.a

make_dirs := $(shell mkdir -p $(OBJDIR) )

#			value.c \
#			strings.c \
#			ptr_list.c \

SLST	=	logging.c \
			file_io.c \
			memory.c \
			instr.c \
			opcodes.c \
			cmd_line.c \
			call_stack.c \
			var_store.c \
			val_stack.c \
			misc.c

all: $(TARGET)

$(TARGET): $(OBJS)
	ar cr $(TARGET) $(OBJS)

clean:
	-$(RM) -r $(OBJDIR) $(TARGET)

format: $(PROJDIR)/astyle.rc $(SRCS) $(HEADERS)
	astyle -v --options=$(PROJDIR)/astyle.rc $(SRCS) $(HEADERS); \
	mv -vf *.bak $(OBJDIR) || true