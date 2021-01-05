
../bin/lines.com: lines.c v9958.c msx.asm v9958.asm
	zcc +msx -create-app -subtype=msxdos2 lines.c v9958.c v9958.asm msx.asm -compiler=sdcc -o lines.com
	mv lines.com ../bin
	rm lines.img
