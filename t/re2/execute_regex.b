implement T;

include "opt/powerman/tap/module/t.m";
include "regex.m";
	re2: Regex;
	Re: import re2;

test()
{
	re2 = load Regex Regex->PATH;
	if(re2 == nil)
		bail_out("fail to load Regex");
	plan(33);

	x: Re;
	err: string;
	offset: array of (int, int);

	(x, err) = re2->compile("(zxc", 0);
	ok(x == nil && err != nil, "not compiled");
	eq(err, "invalid regular expression", "got error message");

	(x, err) = re2->compile("zxc", 0);
	ok(x != nil && err == nil, "compiled");
	offset = re2->execute(x, "abc");
	ok(offset == nil, "not match");

	(x, err) = re2->compile("a(.*c)(Z)?(Z?)(.*e)", 0);
	ok(x != nil && err == nil, "compiled");
	offset = re2->execute(x, "abcdef");
	ok(offset != nil, "match");
	eq_int(len offset, 1, "capture flag off");
	eq_int(offset[0].t0, 0, "offset[0].t0 == 0");
	eq_int(offset[0].t1, 5, "offset[0].t1 == 5");

	(x, err) = re2->compile("a(.*c)(Z)?(Z?)(.*e)", 1);
	ok(x != nil && err == nil, "compiled");
	offset = re2->execute(x, "abcdef");
	ok(offset != nil, "match");
	eq_int(len offset, 5, "capture flag on");
	eq_int(offset[0].t0, 0, "offset[0].t0 == 0");
	eq_int(offset[0].t1, 5, "offset[0].t1 == 5");
	eq_int(offset[1].t0, 1, "offset[1].t0 == 1");
	eq_int(offset[1].t1, 3, "offset[1].t1 == 3");
	eq_int(offset[2].t0, -1,"offset[2].t0 == -1");
	eq_int(offset[2].t1, -1,"offset[2].t1 == -1");
	eq_int(offset[3].t0, 3, "offset[3].t0 == 3");
	eq_int(offset[3].t1, 3, "offset[3].t1 == 3");
	eq_int(offset[4].t0, 3, "offset[4].t0 == 0");
	eq_int(offset[4].t1, 5, "offset[4].t1 == 5");

	(x, err) = re2->compile("a((.*)c(Z)?.*e)", 1);
	ok(x != nil && err == nil, "compiled");
	offset = re2->execute(x, "abcdef");
	ok(offset != nil, "match");
	eq_int(len offset, 4, "capture flag on");
	eq_int(offset[0].t0, 0, "offset[0].t0 == 0");
	eq_int(offset[0].t1, 5, "offset[0].t1 == 5");
	eq_int(offset[1].t0, 1, "offset[1].t0 == 1");
	eq_int(offset[1].t1, 5, "offset[1].t1 == 5");
	eq_int(offset[2].t0, 1, "offset[2].t0 == 1");
	eq_int(offset[2].t1, 2, "offset[2].t1 == 2");
	eq_int(offset[3].t0, -1,"offset[3].t0 == -1");
	eq_int(offset[3].t1, -1,"offset[3].t1 == -1");
}

