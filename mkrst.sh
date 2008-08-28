#!/bin/bash

RUNRST=/usr/local/bin/rst-run

is_rst()
{
	[ -L "$1" ] && [ `readlink "$1"` = "$RUNRST" ]
}

is_debian()
{
	if [ "$DEBIAN" -eq 0 ]; then
		return 1
	fi
	dpkg -S "$1"
}

is_diverted()
{
	if [ "$DEBIAN" -eq 0 ]; then
		return 1
	fi
	
	[ -n "`dpkg-divert --list "$1"`" ]
}

divert()
{
	if [ ! -w "$1" ]; then
		echo "ERROR: $1 is not writable." >&2
		exit 1
	fi

	if [ "$DEBIAN" -eq 0 ]; then
		mv "$1" "$1.distrib"
	else
		if dpkg -S "$1" &>/dev/null; then
			if ! dpkg-divert --add --rename "$1" &>/dev/null; then
				echo "Error running dpkg-divert -- try again as root?" >&2
				exit 1
			fi
		else
			mv "$1" "$1.distrib"
		fi
	fi
}

make_rst ()
{
	EXE="$1"
	if is_rst "$EXE"; then
		echo "ERROR: $EXE is already RST-ized." >&2
		exit 1
	fi
	if is_diverted "$EXE"; then
		echo "ERROR: $EXE is already dpkg-divert'd, bailing." >&2
		exit 1
	fi
	
	divert "$EXE"
	ln -s "$RUNRST" "$EXE"
	echo "$EXE successfully RST-ified." >&2
}

remove_rst ()
{
	EXE=$1
	if ! is_rst "$EXE"; then
		echo "$EXE is not RST-ified." >&2
		exit 1
	fi
	rm "$EXE"
	if is_diverted "$EXE"; then
		if dpkg-divert --remove --rename "$EXE" &>/dev/null; then
			echo "Error running dpkg-divert -- try again as root?" >&2
			exit 1
		fi
	else
		mv "$EXE.distrib" "$EXE"
	fi
	echo "$EXE de-RST-ified." >&2
}

usage()
{
	echo "Usage: mkrst [action] <file>" >&2
	echo "ACTION is one of:" >&2
	echo "		--make-rst (default)" >&2
	echo "		--remove" >&2
	echo "		--run" >&2
	exit 1
}

ACTION=$1
case "$ACTION" in
	--*)
		shift
		;;
	*)
		ACTION="--make-rst"
		;;
esac

PROG=$1
if [ -z "$PROG" ]; then
	usage
fi

if [ ! -x "$PROG" ]; then
	if which "$PROG" &>/dev/null; then
		PROG=`which "$PROG"`
	else
		echo "Unable to find $PROG!" >&2
		usage
		exit 1
	fi
fi

PROG=$(readlink -f "$(dirname "$PROG")")/$(basename "$PROG")

DEBIAN=0
if type "dpkg" &>/dev/null; then
	DEBIAN=1
fi

case "$ACTION" in
	--make-rst)
		make_rst "$PROG"
		;;
	--remove)
		remove_rst "$PROG"
		;;
	--run)
		LD_PRELOAD=/usr/local/lib/librstpreload.so exec "$@"
esac
