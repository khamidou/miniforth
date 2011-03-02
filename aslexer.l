/* a lexer for the assembler */
%{
	#include "tokens.h"
%}

%s in_comment
%%


;              BEGIN(in_comment);
<in_comment>\n   BEGIN(INITIAL);
<in_comment>[^\n]+    // eat comment



"push"		{ return T_PUSH;	}
"pop"		{ return T_POP;		}
"nop"		{ return T_NOP;		}
"dprint"	{ return T_DPRINT;	}
"inc"		{ return T_INC;		}
"dec"		{ return T_DEC;		}
"call"		{ return T_CALL;	}
"ret"		{ return T_RET;		}
"jmp"		{ return T_JMP;		}
"db"		{ return T_DB;		}
"times"		{ return T_TIMES;	}
"peek"		{ return T_PEEK;	}
"poke"		{ return T_POKE;	}
[ \t\n]+ 	/* ignore whitespace */

[0-9]+		{ return NUMBER; 	}
[a-zA-Z][a-zA-Z0-9]*:	{ return T_LABEL;	}
[a-zA-Z][a-zA-Z0-9]*	{ return WORD;	}
\".+\"			{ return STRING;	}

<<EOF>>		{ return -1; 		}
