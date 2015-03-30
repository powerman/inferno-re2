**Re2** is Limbo module for OS Inferno, which provide access to [regular expression library re2](https://code.google.com/p/re2/). It's implemented as Inferno driver in C, so you'll need to rebuild Inferno to install it. Also this project contain C++ library **re2wrap**, required to call C++ library re2 from C.

Version 1.0.0 support most of re2 features (but API is different). Unsupported: custom regexp options, custom regexp/text encodings, auto-converting matched substrings into non-string types (like int), named capturing groups.

In version 1.1.0 regex(2)-compatible interface partially implemented (except executese()).

# Install #

Tested only on Linux. You must have re2 library installed.

## Gentoo Linux ##

Add overlay "powerman" (using layman tool) and install both _dev-libs/libre2_ and _dev-inferno/inferno_ (with USE flag "re2" enabled).

## Using `hg clone` ##

```
cd $INFERNO_ROOT
hg clone https://inferno-re2.googlecode.com/hg/ tmp/inferno-re2
cp -a tmp/inferno-re2/* ./
rm -rf tmp/inferno-re2
./patch.re2
```

# Documentation #

See original docs for re2 library for [regexp syntax](http://code.google.com/p/re2/wiki/Syntax), see [/module/re2.m](http://code.google.com/p/inferno-re2/source/browse/module/re2.m) for this module API and re2(2) man page for more details.

# Examples #

```
include "re2.m";
        re2: Re2;
        re, StrPos: import re2;
re2 = load Re2 Re2->PATH;

match := re2->match("Hello World", re("(?i)hello\\s+(\\w+)"));
if(match != nil)
    sys->print("captured in first parens: %s\n", match[0]);

compiled := re("\\b(\\w+)\\b");
sp := ref StrPos("some text here",0);
while((match = re2->matchnext(sp, compiled)) != nil){
    sys->print("found next word: %s\n", match[0]);
}
```