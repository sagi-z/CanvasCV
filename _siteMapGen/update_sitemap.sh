#!/bin/bash
# ex: ts=4 sw=4 et filetype=sh nu

pushd `dirname $0` > /dev/null
abs_path=$PWD
popd > /dev/null

WD=$abs_path/..
cd $WD

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

google-sitemapgen --config=./_siteMapGen/google-sitemapgen.xml

