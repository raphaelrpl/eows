#!/bin/bash

VERSION="$1"

if [ "$VERSION" = "" ]; then
  echo "Please, inform EOWS version!"
  exit
fi

cd ..

#https://git-scm.com/docs/git-archive
git archive master --format tar.gz --prefix=eows-$VERSION/ -o eows-$VERSION.tar.gz
