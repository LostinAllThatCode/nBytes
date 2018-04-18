#!/bin/bash

failed() {  
  echo build returned with exit code: $1
  popd > /dev/null
  exit 0
}

srcdir="/mnt/d/dev/projects/byte_district"
bdlib="-I/mnt/d/dev/projects/byte_district/src"
linkerflags=""
cflags="-O0 -fno-caret-diagnostics -fno-show-column -fdiagnostics-format=msvc $bdlib" 

mkdir ../build/linux 2> /dev/null
pushd ../build/linux > /dev/null

#clang-3.7 -DLIBACX_BUILD -DLIBACX_MAKEDLL -shared -fpic $bdlib "../test/main.c" -lpthread -o bdlib.so
#ldconfig -v -n .

#clang-3.7 $cflags /media/sf_libacx/test/bake_font.c -o bake_font $linkerflags
#echo bake_font.c

#clang-3.7 $cflags $srcdir/bd_thread_test.c -o bd_thread_test $linkerflags
#echo bd_thread_test

#clang-3.7 $cflags $srcdir/bd_generic_ht.c -o bd_generic_ht $linkerflags
#echo bd_generic_ht

clang-3.7 $cflags $srcdir/main.cpp -o main $linkerflags
echo main.cpp


#clang-3.7 $cflags /media/sf_libacx/test/unit_test.c -o unit_test $linkerflags
#echo unit_test.c

#clang-3.7 -DLIBACX_BUILD $cflags /media/sf_libacx/test/glrender.c -o glrender $linkerflags
#echo glrender.c

ret=$?
if [ $ret -ne 0 ]
then
	failed $ret
fi

echo build succeeded
popd > /dev/null
