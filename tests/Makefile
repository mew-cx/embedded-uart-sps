include ../embedded-uart-common/test-config/base_config.inc
sps_driver_dir := ../
include ${sps_driver_dir}/sps30-uart/default_config.inc
include ${sps_driver_dir}/sen44-uart/default_config.inc

sps30_test_binaries := sps30-test-uart
sen44_test_binaries := sen44-test-uart

uart_sources = ${sensirion_common_dir}/sample-implementations/linux/sensirion_uart_implementation.c

.PHONY: all clean prepare test

all: clean prepare test

prepare:
	cd ${sps_driver_dir} && $(MAKE) prepare

sps30-test-uart: sps30-uart-test.cpp ${sps30_uart_sources} ${uart_sources} ${sensirion_test_sources}
	$(CXX) ${TTYDEV} $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

sen44-test-uart: sen44-uart-test.cpp ${sen44_uart_sources} ${uart_sources} ${sensirion_test_sources}
	$(CXX) ${TTYDEV} $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) ${sps30_test_binaries} ${sen44_test_binaries}

test: prepare ${sps30_test_binaries} ${sen44_test_binaries}
	# TODO: SEN44 tests currently don't get executed since there is no device available
	set -ex; for test in ${sps30_test_binaries}; do echo $${test}; ./$${test}; echo; done;
