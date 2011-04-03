#!/bin/bash -i
DIS=$(basename $0 .t).dis
emu-g sh -c "run /lib/sh/profile; '#U*'^\$emuwdir^'/t/$DIS'; shutdown -h" || :
