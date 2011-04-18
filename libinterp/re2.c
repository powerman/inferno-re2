#include <lib9.h>
#include <isa.h>
#include <interp.h>
#include "runt.h"
#include "raise.h"
#include "re2mod.h"

#include "re2wrap.h"

extern void*   smalloc(size_t);

/*
 * Connecting adt RE to gc
 */
Type*	TRE;
uchar	REmap[] = Re2_RE_map;
void	freeRE(Heap*, int);
Type*	TRange;
uchar	Rangemap[] = Re2_Range_map;

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
	TRange = dtype(freeheap, Re2_Range_size, Rangemap, sizeof(Rangemap));
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

	is_match = Match(s, 0, strlen(s), re->compiled, r, n);

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
Re2_matchnext(void *fp)
{
	F_Re2_matchnext *f = fp;

	char *s;
	RE *re;
	Range *r;
	int n, is_match, i;
	Heap *h;
	Array *a;
	String** match;

	s = string2c(f->sp->str);
	re = (RE*)f->re;
	if(re == H || D2H(re)->t != TRE)
		error(exInval);
	n = 1;
	if(re->re.parens > 0)
		n += re->re.parens;
	if(f->sp->pos < 0 || f->sp->pos > strlen(s))
		error(exRange);
	r = (Range*)smalloc(n*sizeof(Range));

	is_match = Match(s, f->sp->pos, strlen(s), re->compiled, r, n);

	if(!is_match){
		free(r);
		destroy(*f->ret);
		*f->ret = H;
		return;
	}

	f->sp->pos = r[0].t1;
	if(f->sp->str != H && f->sp->str->len < 0)
		fixoffsets(s, r, n);
	n--;
	h = heaparray(&Tptr, n);
	a = H2D(Array*, h);
	match = (String**)a->data;
	for(i = 0; i < n; i++){
		if(r[i+1].t0 < 0)
			match[i] = H;
		else
			match[i] = slicer(r[i+1].t0, r[i+1].t1, f->sp->str);
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

void
Re2_quotemeta(void *fp)
{
	F_Re2_quotemeta *f = fp;

	char *unquoted, *quoted;

	unquoted = string2c(f->s);
	quoted = QuoteMeta(unquoted);

	if(quoted == NULL)
		error(exHeap);
	destroy(*f->ret);
	*f->ret = c2string(quoted, strlen(quoted));
	free(quoted);
}

void
Re2_compile(void *fp)
{
	F_Re2_compile *f = fp;

	char *pattern, *errc;
	void* compiled;
	int parens;
	void* tmp;

	tmp = f->ret->t0;
	f->ret->t0 = H;
	destroy(tmp);
	tmp = f->ret->t1;
	f->ret->t1 = H;
	destroy(tmp);

	pattern = string2c(f->e);
	compiled = NewRE(pattern, &parens);
	if(compiled == NULL){
		errc = GetPatternError(pattern);
		if(errc == NULL)
			error(exHeap);
		f->ret->t1 = c2string(errc, strlen(errc));
		free(errc);
		return;
	}
	f->ret->t0 = mkRE(compiled, parens, f->flag);
}

void
Re2_execute(void *fp)
{
	F_Re2_execute *f = fp;

	char *s;
	RE *re;
	Range *r, *rdata;
	int n, is_match, i;
	Heap *h;
	Array *a;

	s = string2c(f->s);
	re = (RE*)f->x;
	if(re == H || D2H(re)->t != TRE)
		error(exInval);
	n = 1;
	if(re->re.parens > 0 && re->re.capture == 1)
		n += re->re.parens;
	r = (Range*)smalloc(n*sizeof(Range));

	is_match = Match(s, 0, strlen(s), re->compiled, r, n);

	if(!is_match){
		free(r);
		destroy(*f->ret);
		*f->ret = H;
		return;
	}

	if(f->s != H && f->s->len < 0)
		fixoffsets(s, r, n);
	h = heaparray(TRange, n);
	a = H2D(Array*, h);
	rdata = (Range*)a->data;
	for(i = 0; i < n; i++)
		rdata[i] = r[i];
	free(r);
	destroy(*f->ret);
	*f->ret = a;
}

