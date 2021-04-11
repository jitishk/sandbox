#!/bin/bash -x

usage() {
    echo "$0 <excutable> <core-file>"
}

if [[ ! -d .ACMEROOT ]]; then
    echo "Please run from root of Workspace"
    usage
    exit -1
fi

if [[ ! -f $1 ]];then
    echo "file $1 does not exist"
    usage
    exit -1
fi

if [[ ! -f $2 ]];then
    echo "core $2 does not exist"
    usage
    exit -1
fi

ROOT=`pwd`
WS='/nobackup/svenki/spitfire-dev-rebase-latest'
SYMDIR=/ws/jkolanje-sjc/symdir

SYMFILES=`find $WS -path link-thin-64 -prune -o \
    -path tgt-linux -prune -o \
    -path make-thinxr-ljam -prune -o \
    -name "*.sym" | grep -v "\/link\/"`
cd $SYMDIR
for FILE in $SYMFILES; do
    SYMNAME="$(basename $FILE)"
    SYMNAME=${SYMNAME/\.sym/}
    ln -s $FILE $SYMNAME
done
cd $WS

cat >> $SYMDIR/.gdb.script << EOF
set auto-solib-add on
set sysroot $WS/xr-deps/sysroots/WR9/x86_64
set solib-search-path $SYMDIR:$WS/xr-deps/sysroots/CPA/x86_64/usr/lib64/leaba/lib:$WS/xr-deps/sysroots/CPA/x86_64/usr/lib64
set auto-load safe-path /
dir $WS
file $1
core-file $2
EOF

echo $SYMDIR
/router/bin/gdb-7.8.2 --command=$SYMDIR/.gdb.script

