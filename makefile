.PHONY: lib test check

all: lib test cleanup

lib:
	@mkdir lib
	@make -s -C uintx_t/Src/ lib
	
test:
	@make -s -C test/Src test
	
check:
	@make -s -C check/Src check
	
cleanup:
	@make -s -C uintx_t/Src cleanup
	@make -s -C test/Src cleanup
	@make -s -C check/Src cleanup

