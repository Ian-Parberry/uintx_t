.PHONY: lib test

all: lib test cleanup

lib:
	@make -s -C uintx_t/Src/ lib
	
test:
	@make -s -C test/Src test
	
cleanup:
	@make -s -C uintx_t/Src cleanup
	@make -s -C test/Src cleanup

