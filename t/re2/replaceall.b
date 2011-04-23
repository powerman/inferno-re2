implement T;

include "opt/powerman/tap/module/t.m";
include "re2.m";
	re2: Re2;
	re, RE: import re2;

test()
{
	re2 = load Re2 Re2->PATH;
	if(re2 == nil)
		bail_out("fail to load Re2");
	plan(8);

	s: string;
	n: int;

	s = "  some &nbsp;spaces\t\r\n  &nbsp   WAS here!\n";
	(s,n) = re2->replaceall(s, re("(?:\\s|&nbsp;?)+"), " ");
	eq_int(n, 5, nil);
	eq(s, " some spaces WAS here! ", nil);
	(s,n) = re2->replaceall(s, re("^\\s+|\\s+$"), "");
	eq_int(n, 2, nil);
	eq(s, "some spaces WAS here!", nil);
	(s,n) = re2->replaceall(s, re("^\\s+|\\s+$"), "");
	eq_int(n, 0, nil);
	eq(s, "some spaces WAS here!", nil);

	{
	(s,n) = re2->replaceall("abc", ref RE(1,1), "B");
	} exception e { "*" => eq(e, "invalid argument", "exInval (fake RE)"); }
	{
	(s,n) = re2->replaceall("abc", re("a*"), "\\1");
	} exception e { "*" => eq(e, "value out of range", "exRange (\\1)"); }
}

