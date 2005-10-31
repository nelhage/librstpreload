#!/bin/sh

RUNRST=/usr/local/bin/rst-run

FILE=$0
while
    LINK=$(readlink "$FILE")
    [ -n "$LINK" -a "$LINK" != "$RUNRST" ]
do
    echo "$LINK" | grep -q "^/" || LINK=$(dirname "$FILE")/$LINK
    LINK=$(readlink -f "$(dirname "$LINK")")/$(basename "$LINK")
    FILE=$LINK
done

LD_PRELOAD=/usr/local/lib/librstpreload.so exec -a "$0" "$FILE".distrib "$@"
