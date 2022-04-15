#!/bin/sh

set -e

USER=`whoami`
if [ "x$USER" != "xroot" ]; then
  echo "You need to be root to run this."
  exit -1
fi

mkdir -p /usr/local/lib/sdbm_tree
cp libbtree.a /usr/local/lib/sdbm_tree/
ln -sf /usr/local/lib/sdbm_tree/libbtree.a /usr/local/lib/libbtree.a

mkdir -p /usr/local/include/sdbm_tree
cp -rf td.h td_types.h /usr/local/include/sdbm_tree/
