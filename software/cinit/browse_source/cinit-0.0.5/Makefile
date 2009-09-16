#
# cinit
# Nico Schottelius
#
# Don't edit Makefiles, use conf/* for configuration.
#

#
# compile/link options
#
# do not use DEBUG and OPTIMIZE at the same time!
#DEBUG=-g -DDEBUG
OPTIMIZE=-Os

# if cinit should display how long it needed to start
TIMEME=-DTIME_ME

# init should be static per default!
LDFLAGS=-static      

# programs
CC=gcc $(DEBUG) $(OPTIMIZE) $(TIMEME)
CFLAGS=-pipe -Wall -Werror -I.
LD=gcc
STRIP=strip

# monotone
MT=monotone-0.18

# directories and files
DIRS=client bin conf comm generic doc sbin serv
FILES=BUGS Changelog Makefile README TODO cinit.h

# objects
SERV=serv/sigio.o serv/cinit.o serv/list.o  \
     serv/run_init_svc.o serv/panic.o serv/sig_reboot.o

CLIENT=client/msg_start_svc.o client/msg_change_status.o client/run_svc.o \
       client/exec_svc.o client/respawn_svc.o client/run_run_svc.o \
       client/connect_sock.o client/begin_msg.o

COMMUNICATION=comm/do_change_status.o comm/do_result.o comm/do_start_svc.o

BOTH=generic/set_signals.o generic/mini_printf.o

OBJ=$(SERV) $(CLIENT) $(BOTH) $(COMMUNICATION)

# DO NOT CHANGE THIS.
SBIN=sbin
CINIT_BIN=$(SBIN)/cinit

# targets
warn:
	@cat doc/.buildwarn

all: $(CINIT_BIN) sizecheck

$(CINIT_BIN): $(SBIN) config.h $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) -o $@
	$(STRIP) $@
$(SBIN):
	mkdir $(SBIN)
sizecheck:
	FILE="size/`date +%Y-%m-%d-%T`"; ls -l sbin/cinit > $$FILE; cat $$FILE
clean:
	rm -f *.o */*.o sbin/cinit config.h
config.h: conf/*
	./bin/cinit.mkheader > config.h

mt-update:
	$(MT) add $(DIRS) $(FILES) 2>/dev/null
mt-commit:
	$(MT) commit
mt-sync:
	$(MT) sync linux.schottelius.org info.clinux.cinit

install: install-dir
	@echo '*** Installing cinit ***'
	./bin/cinit.install.binary

install-dir:
	./bin/cinit.install.dir

install-conf: install-dir
	@echo '*** Creating very basic configuration ***'
	./bin/cinit.install.conf

install-test: install-dir
	@echo '***> Copying testing configuration'
	./bin/cinit.install.test
