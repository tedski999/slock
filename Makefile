# slock - simple screen locker
# Modified by and for Ted Johnson <ski@h8c.de>
# See LICENSE file for copyright and license details.

PREFIX = /usr/local
CFLAGS = -std=c99 -pedantic -Wall -Os -s -lc -lcrypt -lX11 -lXext -lXrandr -lImlib2

.PHONY: clean install uninstall

slock: slock.c
	${CC} -o $@ ${CFLAGS} $^

clean:
	rm -f slock

install: slock
	mkdir -p ${PREFIX}/bin
	cp -f slock ${PREFIX}/bin
	chmod 755 ${PREFIX}/bin/slock
	chmod u+s ${PREFIX}/bin/slock

uninstall:
	rm -f ${PREFIX}/bin/slock
