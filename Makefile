DESTDIR=/home/users/nico/privat/computer/net/netzseiten/www.nico.schottelius.org/dst

view: all
	firefox ${DESTDIR}/index.html

all:
	ikiwiki --setup ikiwiki.setup

clean: all
	rm -rf ${DESTDIR}

