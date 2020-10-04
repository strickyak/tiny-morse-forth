#undef FIRST_BI
#define FIRST_BI t_wh

OPCODE(  'w', 'h', t_wh, "while" )
OPCODE(  'r', 'e', t_re, "repeat" )
OPCODE(  'i', 'f', t_if, "if" )
OPCODE(  'e', 'l', t_el, "else" )
OPCODE(  'b', 'e', t_be, "begin" )
OPCODE(  't', 'h', t_th, "then" )

#undef FIRST_BI_NO_OPERAND
#define FIRST_BI_NO_OPERAND t_pl

OPCODE(  'p', 'l', t_pl, "plus" )
OPCODE(  'p', 'r', t_pr, "print" )
OPCODE(  's', 't', t_st, "Stop" )
OPCODE(  'b', 'y', t_by, "bye" )
OPCODE(  'e', 'x', t_ex, "exit" )
