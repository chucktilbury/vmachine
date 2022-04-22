#ifndef COMP_EXPR_H
#define COMP_EXPR_H

StkVal notVal(StkVal val);
StkVal eqVal(StkVal left, StkVal right);
StkVal neqVal(StkVal left, StkVal right);
StkVal leqVal(StkVal left, StkVal right);
StkVal geqVal(StkVal left, StkVal right);
StkVal lessVal(StkVal left, StkVal right);
StkVal gtrVal(StkVal left, StkVal right);

#endif