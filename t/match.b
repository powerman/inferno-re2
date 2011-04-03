implement T;

include "opt/powerman/tap/module/t.m";
include "re2.m";
	re2: Re2;
	re, RE, StrPos: import re2;

test()
{
	re2 = load Re2 Re2->PATH;
	if(re2 == nil)
		bail_out("fail to load Re2");
	plan(26);

	match: array of string;

	match = re2->match("abc", re("zxc"));
	ok(match == nil, "match == nil");

	match = re2->match("", re(""));
	ok(match != nil, "match != nil");
	eq_int(len match, 0, "len match == 0");

	match = re2->match("abc", re(""));
	ok(match != nil, "match != nil");
	eq_int(len match, 0, "len match == 0");

	match = re2->match("", re("abc"));
	ok(match == nil, "match == nil");

	match = re2->match("abcdef", re("a"));
	ok(match != nil, "match != nil");
	eq_int(len match, 0, "len match == 0");

	match = re2->match("abcdef", re("(a)"));
	ok(match != nil, "match != nil");
	eq_int(len match, 1, "len match == 1");
	eq(match[0], "a", "match[0] == a");

	match = re2->match("abcdef", re("()(a)()"));
	ok(match != nil, "match != nil");
	eq_int(len match, 3, "len match == 3");
	eq(match[0], nil, "match[0] == nil");
	eq(match[1], "a", "match[1] == a");
	eq(match[2], nil, "match[2] == nil");

	match = re2->match("abcdef", re("a(.*c)(.*z)?(Z?)(.*e)"));
	ok(match != nil, "match != nil");
	eq_int(len match, 4, "len match == 4");
	eq(match[0], "bc", "match[0] == bc");
	eq(match[1], nil, "match[1] == nil");
	eq(match[2], nil, "match[2] == nil");
	eq(match[3], "de", "match[3] == de");

	match = re2->match("abcdef", re("a(.*c)(.*z)?(Z?)(.*e)$"));
	ok(match == nil, "match == nil");

	re_bad := ref RE(1, 1);
	{
		match = re2->match("abcdf", re_bad);
	} exception e { "*" => eq(e, "invalid argument", "exInval (fake RE)"); }
	{
		re_bad = re("(");
	} exception e { "*" => eq(e, "invalid argument", "exInval (bad regex)"); }
	{
		match = re2->match("abcdf", re(")"));
	} exception e { "*" => eq(e, "invalid argument", "exInval (bad regex)"); }
}

