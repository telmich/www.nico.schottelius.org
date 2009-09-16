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
#DEBUG=-DDEBUG
#OPTIMIZE=-Werror 
#DEBUG=-g -DDEBUG
OPTIMIZE=-pipe -Os -Werror 

# init should be static per default!
LDFLAGS=-static      

# programs
CC=gcc $(DEBUG) $(OPTIMIZE) $(TIMEME)
CFLAGS=-Wall -I.
LD=gcc
STRIP=strip -R .comment -R .note.ABI-tag

# monotone
MT=monotone-0.18

# directories and files
SDIRS=bin client conf comm doc generic serv util
FILES=Changelog Makefile README TODO cinit.h

# objects
SERV=serv/sigio.o serv/cinit.o serv/list.o  \
     serv/run_init_svc.o serv/panic.o serv/sig_reboot.o \
     serv/sig_child.o

CLIENT=client/msg_svc_on_off.o client/msg_change_status.o client/run_svc.o \
       client/exec_svc.o client/respawn_svc.o client/run_run_svcs.o \
       client/connect_sock.o client/begin_msg.o client/sig_terminate.o

COMMUNICATION=comm/do_change_status.o comm/do_result.o comm/do_svc_name.o

BOTH=generic/set_signals.o generic/mini_printf.o

OBJ=$(SERV) $(CLIENT) $(BOTH) $(COMMUNICATION)

CSVC_OBJ=util/cservice.o generic/mini_printf.o util/msg_reboot.o \
         $(CLIENT) $(COMMUNICATION)

# DO NOT CHANGE THIS.
SBIN=sbin
CINIT_BIN=$(SBIN)/cinit

# targets
warn:
	@cat doc/.buildwarn

all: cinit cservice sizecheck

cinit: $(CINIT_BIN)

$(CSVC_OBJ) $(OBJ): config.h

$(CINIT_BIN): $(SBIN) $(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) -o $@
	$(STRIP) $@

$(SBIN):
	mkdir $(SBIN)

sizecheck: cinit cservice
	FILE="size/`date +%Y-%m-%d-%T`"; ls -l sbin/ > $$FILE; cat $$FILE
	@echo -n "Source size (in KiB): "
	@du -s $(SDIRS) | awk '{ sum+=$$1 } END { print sum }'
#	@du -s bin client comm conf doc generic serv | awk '{ sum+=$1 } END { print sum }'
clean:
	rm -f *.o */*.o sbin/cinit config.h

config.h: conf/*
	./bin/cinit.mkheader > config.h

cservice: $(SBIN)/cservice

$(SBIN)/cservice: $(SBIN) $(CSVC_OBJ)
	$(LD) $(LDFLAGS) $(CSVC_OBJ) -o $@
	$(STRIP) $@

# monotone
mt-update:
	$(MT) add $(SDIRS) $(FILES) 2>/dev/null
mt-commit:
	$(MT) commit
mt-sync:
	$(MT) sync linux.schottelius.org info.clinux.cinit

install: install-dir
	@echo '*** Installing cinit ***'
	./bin/cinit.install.binary

install-dir:
	./bin/cinit.install.dir

install-test:
	@echo "***> Please get a sample from http://linux.schottelius.org/cinit/"
	@echo "***\ This make target is no longer supported"
