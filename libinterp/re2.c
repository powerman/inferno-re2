#include <lib9.h>
#include <isa.h>
#include <interp.h>
#include "runt.h"
#include "raise.h"
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
mkRE(void* compiled, int parens, int capture)
{
	Heap *h;
	RE* handle;

	h = heap(TRE);
	handle = H2D(RE*, h);
	handle->re.parens  = parens;
	handle->re.capture = capture;
	handle->compiled   = compiled;

	return (Re2_RE*)handle;
}

void
fixoffend(char* s, int* off, int* end, int n)
{
    // TODO
}

// Public interface

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
	*f->ret = mkRE(compiled, parens, 1);
}

void
Re2_match(void *fp)
{
	F_Re2_match *f = fp;

	char *s;
	RE *re;
	int *off, *end, n;
	int is_match, i;
        Heap *h;
        Array *a;
	String** match;

	s   = string2c(f->s);
	re  = (RE*)f->re;
	if(re == H || D2H(re)->t != TRE)
	    error(exInval);
	n   = 1;
	if(re->re.parens > 0 && re->re.capture != 0)
	    n += re->re.parens;
	off = (int*)smalloc(n*sizeof(int));
	end = (int*)smalloc(n*sizeof(int));

	is_match = Match(s, re->compiled, off, end, n);

	if(!is_match){
	    free(off);
	    free(end);
	    destroy(*f->ret);
	    *f->ret = H;
	    return;
	}

	if(f->s->len < 0)
	    fixoffend(s, off, end, n);
	n--;
	h = heaparray(&Tptr, n);
	a = H2D(Array*, h);
	match = (String**)a->data;
	for(i = 0; i < n; i++)
	    if(off[i+1] < 0)
	        match[i] = H;
	    else
		match[i] = slicer(off[i+1], end[i+1], f->s);
	free(off);
	free(end);
	destroy(*f->ret);
	*f->ret = a;
}

