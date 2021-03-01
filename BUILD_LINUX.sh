#! /bin/bash -x

make clean
make release

cd release/
rm -rf sen44-*
rm sps30-uart-*.zip
rm sps30-uart
mv sps30-uart-* sps30-uart

cd sps30-uart/
mv sample-implementations/linux/sensirion_uart_implementation.c .
rm -rf sample-implementations/ test_projects/
#make
