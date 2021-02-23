#!/bin/sh
set -e

test -n "$srcdir" || srcdir=$(dirname "$0")
test -n "$srcdir" || srcdir=.

AUTORECONF=$(which autoreconf || true)
AUTOMAKE=$(which automake || true)

if [ -z "$AUTORECONF" ] || [ -z "$AUTOMAKE" ]; then
        echo "*** No autoreconf or automake found, please intall it ***"
        exit 1
fi

olddir=$(pwd)
cd "$srcdir"

autoreconf --force --install --verbose

# Install 'install-sh' and do not bother with missing 'Makefile.am'
automake --add-missing --copy 2>/dev/null || true

cd "$olddir"
