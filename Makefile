DESTDIR=/home/users/nico/privat/rechner/netz/seiten/www.nico.schottelius.org/dst/
PUBDIR=tee.schottelius.org:/home/services/www/nico/www.nico.schottelius.org/www/
BROWSER=conkeror
#IKIWIKI=ikiwikitest.sh
IKIWIKI=ikiwiki

all:
	$(IKIWIKI) --refresh --setup ikiwiki.setup

lall:
	$(IKIWIKI) --refresh --setup ikiwiki.setup --set destdir=../dst --set srcdir=. --set git_wrapper= --set git_wrappermode= --set gitorigin_branch= --set gitmaster_branch=

all-unfresh:
	$(IKIWIKI) --setup ikiwiki.setup

lall-unfresh:
	$(IKIWIKI) --setup ikiwiki.setup --set destdir=../dst --set srcdir=. --set git_wrapper= --set git_wrappermode= --set gitorigin_branch= --set gitmaster_branch=

view:
	$(BROWSER) ${DESTDIR}/index.html

clean: all
	rm -rf ${DESTDIR}

public:
	git push --mirror

#public: ask public-f

#public-f: all
#	find ${DESTDIR} -type f -exec chmod 0644 {} \;
#	find ${DESTDIR} -type d -exec chmod 0755 {} \;
#	rsync -av --delete ${DESTDIR} ${PUBDIR}

#ask:
#	@echo "Press return to publish..."
#	@read avariable
#	git push --mirror

