/* a lexer for the assembler */
%{
	#include "tokens.h"
%}

%%

<<EOF>>		{ return -1; 		}
"push"		{ return T_PUSH;	}
"pop"		{ return T_POP;		}
"nop"		{ return T_NOP;		}
"dprint"	{ return T_DPRINT;	}
"inc"		{ return T_INC;		}
"dec"		{ return T_DEC;		}
[ \t\n]+ 	/* ignore whitespace */

[0-9]+		{ return NUMBER; 	}
;		{ return COMMA;		}
[a-zA-Z][a-zA-Z0-9]*:	{ print("ho"); return LABEL;	}
[a-zA-Z][a-zA-Z0-9]*	{ return WORD;	}

