PROJDIR	=	$(realpath $(CURDIR))
BINDIR	=	$(PROJDIR)/bin
OBJDIR	=	$(PROJDIR)/obj
SRCDIR	=	$(PROJDIR)/src
DOCDIR	=	$(PROJDIR)/docs
DOCOUTDIR	= $(DOCDIR)/out
INCDIR	=	$(PROJDIR)/include
make_dirs := $(shell mkdir -p $(OBJDIR) $(BINDIR))

#TARGET	=	$(BINDIR)/vm
DOCTARG	=	$(DOCOUTDIR)/html/index.html

VM		=	$(BINDIR)/vm
WRI		=	$(BINDIR)/wri
DIS		=	$(BINDIR)/dis
VMLIB	=	$(BINDIR)/libvm_support.a

SLST	=	file_io.c \
			instr.c \
			value.c \
			opcodes.c \
			error.c \
			vm_support.c

HLST	=	common.h \
			file_io.h \
			instr.h \
			value.h \
			opcodes.h \
			error.h \
			vm_support.h

OBJS 	=	$(foreach item, $(SLST:.c=.o), $(addprefix $(OBJDIR)/, $(item)))
SRCS	=	$(foreach item, $(SLST), $(addprefix $(SRCDIR)/, $(item)))
HEADERS	=	$(foreach item, $(HLST), $(addprefix $(SRCDIR)/, $(item)))

VM_TRACE	=	-DDEBUG_VM_TRACE
EXE_TRACE	=	-DDEBUG_EXE_TRACE
#DEBUG	=	-g3 -Og
DEBUG	=	-g
#OPTO	=	-Ofast
#INCS	= 	-I$(INCDIR)
COPTS	=	-Wall -Wextra -std=c99 $(VM_TRACE) $(EXE_TRACE) $(OPTO) $(DEBUG)
LIBS	=	-L$(BINDIR) -lreadline -lvm_support

all: $(DIS) $(WRI) $(VM)

$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) $(COPTS) -c $< -o $@

$(VMLIB): $(OBJS) $(HEADERS)
	$(AR) crs $(VMLIB) $(OBJS)

$(DIS): $(SRCDIR)/disassemble.c $(VMLIB)
	gcc $(COPTS) -o $(DIS) $(SRCDIR)/disassemble.c $(LIBS)

$(WRI): $(SRCDIR)/write_file.c $(VMLIB)
	gcc $(COPTS) -o $(WRI) $(SRCDIR)/write_file.c $(LIBS)

$(VM): $(SRCDIR)/vm.c $(VMLIB)
	gcc $(COPTS) -o $(VM) $(SRCDIR)/vm.c $(LIBS)

clean:
	-$(RM) -r $(OBJDIR) $(BINDIR)

clean-docs:
	-$(RM) -r $(DOCOUTDIR)

distclean: clean clean-docs

$(DOCTARG): $(SRCS) $(HEADERS)
	cd docs; \
	doxygen doxygen.cfg

docs: $(DOCTARG)

read: docs
	firefox $(DOCTARG)

format: $(SRCDIR)/astyle.rc $(SRCS) $(HEADERS)
	cd src; \
	astyle -v --options=astyle.rc $(SRCS) $(HEADERS); \
	mv -vf *.bak ../obj || true