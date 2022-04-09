
CC	=	gcc

OBJDIR	=	obj
BINDIR	=	bin
SRCDIR	=	.

OBJS 	=	$(foreach item, $(SLST:.c=.o), $(addprefix $(OBJDIR)/, $(item)))
SRCS	=	$(foreach item, $(SLST), $(addprefix $(SRCDIR)/, $(item)))
HEADERS	=	$(foreach item, $(HLST), $(addprefix $(SRCDIR)/, $(item)))

VM_TRACE	=	-DDEBUG_VM_TRACE
EXE_TRACE	=	-DDEBUG_EXE_TRACE
#DEBUG	=	-g3 -Og
DEBUG	=	-g
#OPTO	=	-Ofast
INCS	= 	-I../include -I../lib -I../asm -DNEED_STRDUP
COPTS	=	-Wall -Wextra -std=c99 $(INCS) $(VM_TRACE) $(EXE_TRACE) $(OPTO) $(DEBUG)
LIBS	=	-L../../$(BINDIR) -lreadline -lvm -lm

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(COPTS) -c $< -o $@
