
./bin/print.c.asm: c/print.c c/print.h
./bin/spike.c.asm: c/spike.c c/print.h
./bin/probing.o: ./probing.asm
./bin/msx.o: ./c/msx.asm
./bin/print.o: ./c/print.asm ./c/msx.asm
./bin/crt.o: ./c/crt.asm
./bin/fossil.o: ./fossil.asm
./bin/utils.o: ./utils.asm
./bin/segment1.o: ./segment1.asm
./bin/rc2014-extended.o: ./rc2014-extended.asm ./msx.inc ./segment1.inc ./probing.asm ./extended-bios.asm ./segment1.asm
./bin/alloc.o: ./alloc.asm
./bin/sio.o: ./sio.asm
./bin/extended-bios.o: ./extended-bios.asm
./bin/rs232.o: ./rs232.asm
./bin/rominit.o: ./rominit.asm
