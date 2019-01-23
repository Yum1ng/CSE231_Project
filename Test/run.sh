#!/bin/bash

# path to clang++, llvm-dis, and opt
LLVM_BIN=/LLVM_ROOT/build/bin
# path to CSE231.so
LLVM_SO=/LLVM_ROOT/build/lib
# path to lib231.c
LIB_DIR=/lib231
# path to the test directory
TEST_DIR=/tests

FLAGS=`llvm-config --system-libs --cppflags --ldflags --libs core`
FLAGS="$FLAGS -Wno-unused-command-line-argument"

$LLVM_BIN/clang -c -O0 $TEST_DIR/test1.c -emit-llvm -S -o /tmp/test1-c.ll
$LLVM_BIN/clang++ -c -O0 $TEST_DIR/test1.cpp -emit-llvm -S -o /tmp/test1.ll
$LLVM_BIN/clang++ -c $LIB_DIR/lib231.cpp -emit-llvm -S $FLAGS -o /tmp/lib231.ll
$LLVM_BIN/clang++ -c $TEST_DIR/test1-main.cpp -emit-llvm -S -o /tmp/test1-main.ll
$LLVM_BIN/clang++ -c $TEST_DIR/MainTest.cpp -emit-llvm -S -o /tmp/MainTest.ll

# # Part1
# $LLVM_BIN/opt -load $LLVM_SO/231_solution.so -cse231-csi < /tmp/test1-c.ll > /dev/null 2> /tmp/test1_csi_so.result
# $LLVM_BIN/opt -load $LLVM_SO/submission_pt1.so -cse231-csi < /tmp/test1-c.ll > /dev/null 2> /tmp/test1_csi_my.result
#
#
# # Part2
# $LLVM_BIN/opt -load $LLVM_SO/231_solution.so -cse231-cdi < /tmp/MainTest.ll -o /tmp/MainTest-cdi_so.bc
# $LLVM_BIN/opt -load $LLVM_SO/submission_pt1.so -cse231-cdi < /tmp/MainTest.ll -o /tmp/MainTest-cdi_my.bc
# $LLVM_BIN/clang++ /lib231/lib231.cpp $FLAGS /tmp/MainTest-cdi_so.bc -o /tmp/MainTest_cdi_so
# $LLVM_BIN/clang++ /lib231/lib231.cpp $FLAGS /tmp/MainTest-cdi_my.bc -o /tmp/MainTest_cdi_my
# /tmp/MainTest_cdi_so 2> /tmp/MainTest_cdi_so.result
# /tmp/MainTest_cdi_my 2> /tmp/MainTest_cdi_my.result

# Part3
$LLVM_BIN/opt -load $LLVM_SO/231_solution.so -cse231-bb < /tmp/MainTest.ll -o /tmp/MainTest-bb_so.bc
$LLVM_BIN/opt -load $LLVM_SO/submission_pt1.so -cse231-bb < /tmp/MainTest.ll -o /tmp/MainTest-bb_my.bc
$LLVM_BIN/clang++ /lib231/lib231.cpp $FLAGS /tmp/MainTest-bb_so.bc -o /tmp/MainTest_bb_so
$LLVM_BIN/clang++ /lib231/lib231.cpp $FLAGS /tmp/MainTest-bb_my.bc -o /tmp/MainTest_bb_my
/tmp/MainTest_bb_so 2> /tmp/MainTest_bb_so.result
/tmp/MainTest_bb_my 2> /tmp/MainTest_bb_my.result
#diff /tmp/MainTest_cdi_so.result /tmp/MainTest_cdi_my.result








#$LLVM_BIN/llvm-dis /tmp/test1-cdi.bc
#$LLVM_BIN/llvm-dis /tmp/test1-bb.bc


#$LLVM_BIN/clang++ /lib231/lib231.cpp $FLAGS test1-bb_so.bc -o /tmp/test1_bb_so


#$LLVM_BIN/clang++ /lib231/lib231.cpp $FLAGS test1-bb_my.bc -o /tmp/test1_bb_my


#/tmp/test1_bb_so 2> /tmp/test1_bb_so.result


#/tmp/test1_bb_my 2> /tmp/test1_bb_my.result


#diff /tmp/test1_bb_so.result /tmp/test1_bb_my.result
