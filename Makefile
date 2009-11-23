DESTDIR=/home/users/nico/privat/rechner/netz/seiten/www.nico.schottelius.org/dst/
PUBDIR=tee.schottelius.org:/home/services/www/nico/www.nico.schottelius.org/www/
BROWSER=conkeror
IKIWIKI=ikiwikitest.sh

all:
	$(IKIWIKI) --refresh --setup ikiwiki.setup

all-unfresh:
	$(IKIWIKI) --setup ikiwiki.setup

view: all
	$(BROWSER) ${DESTDIR}/index.html

clean: all
	rm -rf ${DESTDIR}

public: all
	git push --mirror
	find ${DESTDIR} -type f -exec chmod 0644 {} \;
	find ${DESTDIR} -type d -exec chmod 0755 {} \;
	rsync -av --delete ${DESTDIR} ${PUBDIR}


