DESTDIR=/home/users/nico/privat/computer/net/netzseiten/www.nico.schottelius.org/dst/
PUBDIR=tee.schottelius.org:/home/services/www/nico/www.nico.schottelius.org/www/

view: all
	firefox ${DESTDIR}/index.html

all:
	ikiwiki --setup ikiwiki.setup --refresh

clean: all
	rm -rf ${DESTDIR}

public: all
	git push --mirror
	rsync -av --delete ${DESTDIR} ${PUBDIR}


