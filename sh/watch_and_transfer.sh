#!/bin/bash

filewatcher 'src/*.cpp src/*.h sh/*.sh sh/*.sh' \
  'rsync -avz --progress --exclude=tests/read.csfasta ../egrl fedora:.;
   rsync -avz --progress --exclude=tests/read.csfasta ../egrl ardmore:/stornext/snfs6/rogers/drio_scratch/playground \
  '
