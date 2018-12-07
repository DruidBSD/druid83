# $Header: /cvsroot/druidbsd/druidbsd/druid83/dep/freebsd/rebuild_crunch/Makefile,v 1.1 2012/10/10 23:30:01 devinteske Exp $

all:
	@echo "Targets:"
	@echo "    rebuild_crunch4   FreeBSD-4.11 compatible static rebuild tool"
	@echo "    rebuild_crunch8   FreeBSD-8.1  compatible static rebuild tool"

.PHONY: rebuild_crunch4
rebuild_crunch4:
	@required="FreeBSD 4."; \
	 case "$$( uname -sr )" in \
	 $$required*-*) : fall through ;; \
	 *) echo "Must be compiled on $${required}x !!" ; false ;; \
	 esac
	crunchgen "$@.conf"
	make -f "$@.mk"

.PHONY: rebuild_crunch8
rebuild_crunch8:
	@required="FreeBSD 8."; \
	 case "$$( uname -sr )" in \
	 $$required*-*) : fall through ;; \
	 *) echo "Must be compiled on $${required}x !!" ; false ;; \
	 esac
	crunchgen "$@.conf"
	make -f "$@.mk"

clean:
	rm -f *.c *.o *.lo *.mk *.cache

distclean: clean
	rm -f rebuild_crunch4
	rm -f rebuild_crunch8
