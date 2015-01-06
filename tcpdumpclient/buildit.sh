#!/bin/bash
# use this as the "make" command under Emacs...
here=`pwd`
cd ../..
source build/envsetup.sh
lunch aosp_x86_64-eng
cd -
mm
