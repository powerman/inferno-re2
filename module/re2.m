Re2: module
{
	PATH: con "$Re2";

	# regex(2) compatible interface
	Re: type ref RE;
	Range: type (int, int);
	compile:	fn(e: string, flag: int): (Re, string);
	execute:	fn(x: Re, s: string): array of Range;
#	executese:	fn(x: Re, s: string, se: Range, bol: int, eol: int): array of Range;

	# user-friendly interface
	re:		fn(re: string): ref RE; # throw on bad regexp
	match:		fn(s: string, re: ref RE): array of string;
	matchnext:	fn(sp: ref StrPos, re: ref RE): array of string;
	replace:	fn(s: string, re: ref RE, rewrite: string): (string, int);
	replaceall:	fn(s: string, re: ref RE, rewrite: string): (string, int);
	quotemeta:	fn(s: string): string;

	RE: adt
	{
		parens:	int; # auto-detected amount of capturing parens
		capture:int; # flag: if false make all parens non-capturing
		# there are some hidden state lurking around
	};
	StrPos: adt
	{
		str: string;
		pos: int; # CAUTION: it's offset in UTF-8 string, not character number
	};
};
