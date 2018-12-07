# -*- tab-width:  4 -*- ;; Emacs
# vi: set tabstop=4     :: Vi/ViM

MDUNIT = 5
SR     = sudo

FREEBSD_REPOS_DRUID_SRC  = ../../druid

all:
	@echo "	open:		Open mfsroot.gz"
	@echo "	copy:		Copy in src to opened image"
	@echo "	close:		Close mfsroot.gz"
	@echo "	remake:		Open, re-copy src, and close"
	@echo "	from_dep:	Re-make from stored dependencies"
	@echo "	update_dep:	Update stored dependency from release"
	@echo "	install:	Copy mfsroot.gz to mdroot"

stage1:
	cp -i dep/mfsroot.gz mfsroot.gz

stage2:
	gunzip mfsroot.gz
	mkdir -p mnt
	$(SR) mdconfig -f mfsroot -u $(MDUNIT)
	$(SR) mount /dev/md$(MDUNIT) mnt

stage3:
	( cd src && \
	  find . -type d -mindepth 1 ! -name CVS -exec \
	  	$(SR) mkdir -pv '../mnt/{}' \; \
	)
	( cd src && \
	  find . -type f ! -path '*/CVS/*' ! -name .keep -exec \
	  	$(SR) cp -fv '{}' '../mnt/{}' \; \
	)
	( cd src && \
	  find . -type l -exec $(SR) cp -RPfv '{}' '../mnt/{}' \; )
	( cd mnt && \
	  $(SR) rm -f etc/pwd.db etc/spwd.db && \
	  $(SR) pwd_mkdb -i -d etc etc/master.passwd )
	$(SR) cp -fv $(FREEBSD_REPOS_DRUID_SRC)/etc/sh.subr mnt/fis/

stage4:
	$(SR) umount mnt
	rmdir mnt
	$(SR) mdconfig -d -u $(MDUNIT)
	gzip mfsroot

from_dep: stage1 stage2 stage3 stage4

remake: stage2 stage3 stage4

open: stage2

copy: stage3

close: stage4

dep/mfsroot.gz: /usr/release/R/stage/mfsroot/mfsroot.gz

update_dep: dep/mfsroot.gz
	cp -fv /usr/release/R/stage/mfsroot/mfsroot.gz dep/mfsroot.gz

install: mfsroot.gz
	mv -v mfsroot.gz ../../../../mdroot/boot/mfsroot8.gz

################################################################################
#
# $Header: /cvsroot/druidbsd/druidbsd/druid83/dep/freebsd/mfsroot/standard/Makefile,v 1.1 2012/10/10 23:29:58 devinteske Exp $
#
# $Copyright: 2006-2012 Devin Teske. All rights reserved. $
#
# $Log: Makefile,v $
# Revision 1.1  2012/10/10 23:29:58  devinteske
# Commit initial public beta release of 8.3 series (beta 57; on-par with
# the 9.0 series released recently, also beta 57).
#
#
################################################################################
