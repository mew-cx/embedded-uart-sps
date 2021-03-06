# See user_config.inc for build customizations
-include user_config.inc
include default_config.inc

EXE = sps30_example_usage

.PHONY: all clean run

all : ${EXE}

run : ${EXE}
	./${EXE}

${EXE} : clean
	$(CC) $(CFLAGS) -o $@ ${sps30_uart_sources} ${uart_sources} ${sps30_uart_dir}/${EXE}.c

clean:
	$(RM) ${EXE}
