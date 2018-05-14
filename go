	
	INSTALL_DIR=~/Amiga/hdd/A600/V2

	rm -rf ahb
	vc +aos68k -c99 -I$NDK_INC -Iinclude -lamiga mouse.c interrupts.c screen.c ahb.c -o ahb
	cp ahb $INSTALL_DIR
