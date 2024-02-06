

.PHONY: apps
apps:
	@mkdir -p bin
	$(MAKE) --no-print-directory -j -C apps
	cp -up ./apps/bin/*.com ./bin/
	cp -up ./apps/bin/*.sys ./bin/

.PHONY: format
format:
	$(MAKE) -C apps format

apps/3rdparty/akid.com:
	@wget -nv https://msxhub.com/api/KIDS/1.0-1/get/KIDS/AKID.COM -O apps/3rdparty/akid.com

apps/3rdparty/vgmplay:
	@wget -nv https://www.grauw.nl/projects/vgmplay-msx/vgmplay-1.4.zip -O apps/3rdparty/vgmplay.zip
	unzip -oq apps/3rdparty/vgmplay.zip -d apps/3rdparty/vgmplay
	rm apps/3rdparty/vgmplay.zip
	mv apps/3rdparty/vgmplay/vgmplay.com apps/3rdparty/

apps/3rdparty/memtest:
	@wget -nv https://www.msx.org/download/download/2017/12/MEMTEST.ZIP -O apps/3rdparty/memtest.zip
	unzip -oq apps/3rdparty/memtest.zip -d apps/3rdparty/memtest
	rm apps/3rdparty/memtest.zip
	mv apps/3rdparty/memtest/MEMTEST.COM apps/3rdparty/memtest.com

apps/3rdparty/pt3play.com:
	@wget -nv https://www.msx.org/download/downloads/music/players/PT3PLAY.LZH -O apps/3rdparty/pt3play.lzh
	(cd apps/3rdparty && lha xq ./pt3play.lzh)
	(cd apps/3rdparty && rm msx.org)
	rm apps/3rdparty/pt3play.lzh

apps/3rdparty/sofarom:
	@wget -nv https://www.louthrax.net/mgr/downloads/download.php?file=sofarom.zip -O apps/3rdparty/sofarom.zip
	unzip -oq apps/3rdparty/sofarom.zip -d apps/3rdparty/sofarom
	mv apps/3rdparty/sofarom/SROM.COM apps/3rdparty/srom.com
	mv apps/3rdparty/sofarom/SROM.INI apps/3rdparty/srom.ini
	rm apps/3rdparty/sofarom.zip

apps/3rdparty/testram:
	@wget -nv https://msxhub.com/api/TESTRAM/1.0-1/get/TESTRAM.zip -O apps/3rdparty/testram.zip
	unzip -oq apps/3rdparty/testram.zip -d apps/3rdparty/testram
	rm apps/3rdparty/testram.zip
	mv apps/3rdparty/testram/TESTRAM.COM apps/3rdparty/testram.com

.PHONY: apps/3rdpartyapps
THIRDPARTYAPPS = apps/3rdparty/akid.com apps/3rdparty/vgmplay apps/3rdparty/memtest apps/3rdparty/pt3play.com apps/3rdparty/sofarom apps/3rdparty/testram
3rdpartyapps:
	$(MAKE) $(THIRDPARTYAPPS) -B -j


