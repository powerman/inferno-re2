#include <lib9.h>
#include <isa.h>
#include <interp.h>
#include "runt.h"
#include "re2mod.h"

#include "re2wrap.h"


void
re2modinit(void)
{
	builtinmod("$Re2", Re2modtab, Re2modlen);
}

void
Re2_match(void *fp)
{
	F_Re2_match *f = fp;

	char** args;
	int n = 0;
	int match;

	char* s;
	char* re;
	String** aofs;

	int i;

	s   = string2c(f->s);
	re  = string2c(f->re);
	if(f->arg != H && f->arg->len > 0){
	    n = f->arg->len;
	    args = malloc(n*sizeof(args));
	}

	match = PartialMatchN(s, re, args, n);

	if(n > 0){
	    if(match){
		aofs = (String **)f->arg->data;
		for(i = 0; i < n; i++){
		    destroy(aofs[i]);
		    aofs[i] = c2string(args[i], strlen(args[i]));
		    free(args[i]);
		}
	    }
	    free(args);
	}

	*f->ret = match;
	return;
}

