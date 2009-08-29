#!/bin/sh
set -u
set -e
set -x

libtoolize --automake --copy
aclocal -I m4

automake --add-missing --copy
autoconf
./configure --enable-maintainer-mode $*
