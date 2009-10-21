DESTDIR=/home/users/nico/privat/computer/net/netzseiten/www.nico.schottelius.org/dst/
PUBDIR=tee.schottelius.org:/home/services/www/nico/www.nico.schottelius.org/www/
BROWSER=conkeror
IKIWIKI=ikiwikitest.sh --refresh

view: all
	$(BROWSER) ${DESTDIR}/index.html

all:
	$(IKIWIKI) --setup ikiwiki.setup

clean: all
	rm -rf ${DESTDIR}

public: all
	git push --mirror
	rsync -av --delete ${DESTDIR} ${PUBDIR}


