#!/bin/bash

find . -maxdepth 1 -type f | \
  grep -v git | \
  grep -v Makefile.am | \
  grep -v README  | \
  grep -v TODO | \
  grep -v autogen | \
  grep -v configure.ac |\
  grep -v tags | xargs rm -f

find . -name "*.o" | xargs rm -f
find . -name "*.Po" | xargs rm -f
find . -name ".deps" | xargs rm -rf
find . -name "autom4te.cache" | xargs rm -rf
