PROJDIR	=	../..
-include $(PROJDIR)/vmachine.mak

DESTDIR	=	$(PROJDIR)/bin
TARGET	=	$(DESTDIR)/vm

make_dirs := $(shell mkdir -p $(OBJDIR))

SLST	=	run_arith.c \
			run_comp.c \
			run_jmp.c \
			run_other.c \
			run_push.c \
			runner.c \
			error.c \
			arith_expr.c \
			comp_expr.c \
			vm.c

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(COPTS) -o $(TARGET) $(OBJS) $(LIBS)

clean:
	-$(RM) -r $(OBJDIR) $(TARGET)

format: $(PROJDIR)/astyle.rc vm.c
	astyle -v --options=$(PROJDIR)/astyle.rc vm.c; \
	mv -vf *.bak $(OBJDIR) || true