#!/bin/sh
LD_PRELOAD=/usr/local/lib/librstpreload.so exec -a "$0" "$0".distrib "$@"
