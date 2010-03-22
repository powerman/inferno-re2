#include <lib9.h>
#include <isa.h>
#include <interp.h>
#include "runt.h"
#include "raise.h";
#include "re2mod.h"

#include "re2wrap.h"

/*
 * Connecting adt RE to gc
 */
Type*	TRE;
uchar	REmap[] = Re2_RE_map;
void	freeRE(Heap*, int);

typedef struct RE RE;
struct RE
{
	Re2_RE	re;
	void*	compiled;
};


void
re2modinit(void)
{
	builtinmod("$Re2", Re2modtab, Re2modlen);

	TRE = dtype(freeRE, sizeof(RE), REmap, sizeof(REmap));
}

void
freeRE(Heap *h, int swept)
{
	RE* handle;

	USED(swept);

	handle = H2D(RE*, h);
	DeleteRE(handle->compiled);
}

Re2_RE*
mkRE(void* compiled, int parens)
{
	Heap *h;
	RE* handle;

	h = heap(TRE);
	handle = H2D(RE*, h);
	handle->re.parens= parens;
	handle->compiled = compiled;

	return (Re2_RE*)handle;
}

void
Re2_re(void *fp)
{
	F_Re2_re *f = fp;
	void* compiled;
	int parens;

	compiled = NewRE(string2c(f->re), &parens);
	if(compiled == NULL)
	    error(exInval);

	destroy(*f->ret);
	*f->ret = mkRE(compiled, parens);
}

void
Re2_match(void *fp)
{
	F_Re2_match *f = fp;

	char** args;
	int n = 0;
	int match;

	char* s;
	RE* re;
	String** aofs;

	int i;

	s   = string2c(f->s);
	re  = (RE*)f->re;
	if(re == H || D2H(re)->t != TRE){
	    *f->ret = 0;
	    return;
	}
	if(f->parens != H && f->parens->len > 0){
	    if(f->parens->len > re->re.parens)
		error(exInval);
	    n = f->parens->len;
	    args = smalloc(n*sizeof(args));
	}

	match = PartialMatchN(s, re->compiled, args, n);

	if(n > 0){
	    if(match){
		aofs = (String **)f->parens->data;
		for(i = 0; i < n; i++){
		    destroy(aofs[i]);
		    if(args[i] == NULL){
			free(args);
			error(exHeap);
		    }
		    aofs[i] = c2string(args[i], strlen(args[i]));
		    free(args[i]);
		}
	    }
	    free(args);
	}

	*f->ret = match;
	return;
}

