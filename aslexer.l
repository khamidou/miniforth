/* a lexer for the assembler */
%{
	#include "tokens.h"
%}

%s in_comment
%%

<<EOF>>		{ return -1; 		}


;              BEGIN(in_comment);
<in_comment>\n   BEGIN(INITIAL);
<in_comment>[^\n]+    // eat comment
<in_comment>;       // eat the lone ';'


"push"		{ return T_PUSH;	}
"pop"		{ return T_POP;		}
"nop"		{ return T_NOP;		}
"dprint"	{ return T_DPRINT;	}
"inc"		{ return T_INC;		}
"dec"		{ return T_DEC;		}
"call"		{ return T_CALL;	}
"ret"		{ return T_RET;	}
"jmp"		{ return T_JMP;		}
[ \t\n]+ 	/* ignore whitespace */

[0-9]+		{ return NUMBER; 	}
[a-zA-Z][a-zA-Z0-9]*:	{ return T_LABEL;	}
[a-zA-Z][a-zA-Z0-9]*	{ return WORD;	}

