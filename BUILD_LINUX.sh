#! /bin/bash -x

# build the source directory
make clean
make release

# clean up the source directory
cd release/
rm -rf sen44-*
rm sps30-uart-*.zip
rm sps30-uart
mv sps30-uart-* sps30-uart

# build the executable
cd sps30-uart/
mv sample-implementations/linux/sensirion_uart_implementation.c .
rm -rf sample-implementations/ test_projects/
make

echo "The executable can be found at release/sps30-uart/sps30_example_usage"

# vim: set sw=4 ts=8 et ic ai:
