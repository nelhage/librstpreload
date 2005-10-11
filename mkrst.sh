#!/bin/sh

RUNRST=/usr/local/bin/rst-run.sh

is_rst()
{
	grep -Fq 'librstpreload.so' $1
}

is_debian()
{
	if [ ! $DEBIAN ]; then
		return 1;
	fi
	dpkg -S "$1"
}

is_diverted()
{
	if [ ! $DEBIAN ]; then
		return 1;
	fi
	
	[ -n "`dpkg-divert --list "$1"`" ];
}

divert()
{
	if [ ! -w "$1" ]; then
		echo "ERROR: $1 is not writable.";
		exit 1;
	fi

	if [ ! "$DEBIAN" ]; then
		mv "$1" "$1.distrib";
	else
		if [ dpkg -S "$1" > /dev/null 2>&1 ]; then
			if ! dpkg-divert --add --rename $1 > /dev/null 2>&1; then
				echo "Error running dpkg-divert -- try again as root?";
				exit 1;
			fi;
		else
			mv "$1" "$1.distrib";
		fi;
	fi
}

make_rst ()
{
	EXE=$1
	if is_rst "$EXE"; then
		echo "ERROR: $EXE is already RST-ized."
		exit 1;
	fi;
	if is_diverted "$EXE"; then
		echo "ERROR: $EXE is already dpkg-divert'd, bailing."
		exit 1;
	fi
	
	divert "$EXE";
	cp "$RUNRST" "$EXE";
	echo "$EXE successfully RST-ified."
}

remove_rst ()
{
	EXE=$1
	if ! is_rst "$EXE"; then
		echo "$EXE is not RST-ified.";
		exit 1
	fi
	rm "$EXE";
	if is_diverted "$EXE"; then
		if dpkg-divert --remove --rename "$EXE" > /dev/null 2>&1; then
			echo "Error running dpkg-divert -- try again as root?";
			exit 1;
		fi
	else
		mv "$EXE.distrib" "$EXE";
	fi;
	echo "$EXE de-RST-ified."
}

usage()
{
	echo "Usage: mkrst [action] <file>"
	echo "ACTION is one of:"
	echo "		--make-rst (default)"
	echo "		--remove"
	echo "		--run"
	exit 1;
}

ACTION=$1;
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
	usage;
fi

if [ ! -x "$PROG" ]; then
	if which "$PROG" > /dev/null 2>&1; then
		PROG=`which "$PROG"`;
	else
		echo "Unable to find $PROG!";
		usage;
		exit 1;
	fi
fi;

DEBIAN=0
if type "dpkg" > /dev/null 2>&1; then
	DEBIAN=1
fi;

case "$ACTION" in
	--make-rst)
		make_rst "$PROG";
		;;
	--remove)
		remove_rst "$PROG";
		;;
	--run)
		LD_PRELOAD=/usr/local/lib/librstpreload.so exec "$@"
esac