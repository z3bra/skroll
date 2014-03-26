include config.mk

.SUFFIXES: .c .o .gz
.PHONY : all clean install uninstall

.c.o:
	@echo -e "CC $<"
	@${CC} -c ${CFLAGS} $< -o $@

skroll: skroll.o
	@echo -e "LD skroll"
	@${LD} $^ -o $@ ${LDFLAGS}

skroll.1.gz: skroll.1
	@echo "GZ $<"
	@${GZ} -c $< > $@

all : skroll skroll.1.gz

clean :
	${RM} -f skroll *.o *.gz *~

install: skroll skroll.1.gz
	install -D -m 0755 skroll ${DESTDIR}${PREFIX}/bin/skroll
	install -D -m 0644 skroll.1.gz ${DESTDIR}${MANPREFIX}/man1/skroll.1.gz

uninstall:
	${RM} ${DESTDIR}${PREFIX}/bin/skroll
	${RM} ${DESTDIR}${MANPREFIX}/man1/skroll.1.gz
