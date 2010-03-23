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
fixoffsets(char *s, Range *r, int n)
{
	int **qdata, **q;
	int ch;
	int *fix;
	Range *er;
	Rune junk;
	Range *origr = r;
	char *t;

	if(n == 0 || r[0].t0 == -1){
		return;
	}
	qdata = malloc((n+1) * sizeof(int*));
	q = qdata;
	er = &r[n];
	ch = 0;
	t = s;
	while(q > qdata || r < er){
		if(q == qdata || r < er && r->t0 < *q[-1]){
			*q++ = &r->t1;
			*q++ = &r->t0;
			// skip unmatched ranges
			for(r++; r < er && r->t0 == -1; r++)
				;
		}
		fix = *--q;
		ch += utfnlen(t, *fix - (t - s));
		t = s + *fix;
		*fix = ch;
	}
	free(qdata);
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
	Range *r;
	int n, is_match, i;
	Heap *h;
	Array *a;
	String** match;

	s = string2c(f->s);
	re = (RE*)f->re;
	if(re == H || D2H(re)->t != TRE)
		error(exInval);
	n = 0;
	if(re->re.parens > 0)
		n = 1 + re->re.parens;
	r = (Range*)smalloc(n*sizeof(Range));

	is_match = Match(s, re->compiled, r, n);

	if(!is_match){
		free(r);
		destroy(*f->ret);
		*f->ret = H;
		return;
	}

	if(n > 0 && f->s != H && f->s->len < 0)
		fixoffsets(s, r, n);
	if(n > 0)
		n--;
	h = heaparray(&Tptr, n);
	a = H2D(Array*, h);
	match = (String**)a->data;
	for(i = 0; i < n; i++){
		if(r[i+1].t0 < 0)
			match[i] = H;
		else
			match[i] = slicer(r[i+1].t0, r[i+1].t1, f->s);
	}
	free(r);
	destroy(*f->ret);
	*f->ret = a;
}

void
Re2_replace(void *fp)
{
	F_Re2_replace *f = fp;

	char *s, *rewrite, *new;
	RE *re;
	int res;
	void *tmp;

	s = string2c(f->s);
	rewrite = string2c(f->rewrite);
	re = (RE*)f->re;
	if(re == H || D2H(re)->t != TRE)
		error(exInval);
	tmp = f->ret->t0;
	f->ret->t0 = H;
	destroy(tmp);

	res = CheckRewriteString(re->compiled, rewrite);
	if(res == 0)
		error(exRange);
	res = Replace(s, re->compiled, rewrite, &new);

	if(res < 0)
		error(exHeap);
	f->ret->t1 = res;
	if(res > 0){
		f->ret->t0 = c2string(new, strlen(new));
		free(new);
	}else
		f->ret->t0 = stringdup(f->s);
}

void
Re2_replaceall(void *fp)
{
	F_Re2_replaceall *f = fp;

	char *s, *rewrite, *new;
	RE *re;
	int res;
	void *tmp;

	s = string2c(f->s);
	rewrite = string2c(f->rewrite);
	re = (RE*)f->re;
	if(re == H || D2H(re)->t != TRE)
		error(exInval);
	tmp = f->ret->t0;
	f->ret->t0 = H;
	destroy(tmp);

	res = CheckRewriteString(re->compiled, rewrite);
	if(res == 0)
		error(exRange);
	res = GlobalReplace(s, re->compiled, rewrite, &new);

	if(res < 0)
		error(exHeap);
	f->ret->t1 = res;
	if(res > 0){
		f->ret->t0 = c2string(new, strlen(new));
		free(new);
	}else
		f->ret->t0 = stringdup(f->s);
}

