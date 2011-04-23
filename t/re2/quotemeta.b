implement T;

include "opt/powerman/tap/module/t.m";
include "re2.m";
	re2: Re2;
	quotemeta: import re2;

test()
{
	re2 = load Re2 Re2->PATH;
	if(re2 == nil)
		bail_out("fail to load Re2");
	plan(5);

	eq(quotemeta(""), "", nil);
	eq(quotemeta("a"), "a", nil);
	eq(quotemeta("абракадабра"), "абракадабра", nil);
	eq(quotemeta("1.5-2.0?"), "1\\.5\\-2\\.0\\?", nil);
	eq(quotemeta("1.5\n2.0?"), "1\\.5\\\n2\\.0\\?", nil);
}

