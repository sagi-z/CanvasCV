#!/bin/bash
# ex: ts=4 sw=4 et filetype=sh nu

GIT_REPO_URL=$1

pushd `dirname $0` > /dev/null
abs_path=$PWD
popd > /dev/null

TARGET_DIR=$abs_path/../doc

CLR_RED="\033[31m"
CLR_RST="\033[m"
notice ()
{
    echo -e "${CLR_RED}${1}${CLR_RST}"
}

fatal ()
{
    notice "FATAL: $1"
    exit 1
}

warn ()
{
    notice "WARNING: $1"
}

RC_GOOD=0
RC_BAD=1

if [ "$GIT_REPO_URL" == "" ]; then
    fatal "Must get a git repository URL (local or remote)"
fi

TMP_MASTER=$HOME/tmp/CanvasCV-doxygen
rm -rf $TMP_MASTER
mkdir -p $TMP_MASTER
cd $HOME/tmp
if ! `git clone $GIT_REPO_URL CanvasCV-doxygen`; then
    fatal "Failed to clone '$GIT_REPO_URL'"
fi

# Copy all the needed files
cd $abs_path
cp doxygen_style.css doxygen_main.md doxygen_header.html.in $TMP_MASTER
#cp doxygen_main.md doxygen_header.html.in $TMP_MASTER

# Generate doxygen
cd $TMP_MASTER
mkdir build
cd build
cmake -DBUILD_DOCUMENTATION=ON ..
make doc
cd ..
rm -rf $TARGET_DIR

# HACK: modify images with maps to be in a scrollable div
find doc/html -type f -name "*html" |xargs grep -l 'img.*usemap'| xargs perl -i -pe 's/<img (.* usemap.*>)/<div style="overflow: auto;"><img style="max-width: 4096px;" $1<\/div>/'

# Take doxygen output
mv doc $TARGET_DIR

# Cleanup temp dir
rm -rf $TMP_MASTER
