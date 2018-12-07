#!/bin/sh
# -*- tab-width:  4 -*- ;; Emacs
# vi: set tabstop=4     :: Vi/ViM

# If running as xarchInstall, only use binaries in `/stand'
[ "$xarchInstall" ] && export PATH=/stand
############################################################ CONFIGURATION

# Indicate that we want color output
ANSI_COLOR=YES

PERL_VERSION="5.12.4"
MAKE_CONF=/etc/make.conf
MANPATH_CONFIG=/etc/manpath.config
PKG_PREFIX=/usr/local
BANNER=`date +"%F %T"`
POST_INSTALL_SH=/usr/local/etc/rc.d/perl_post_install.sh

############################################################ FUNCTIONS

# Include standard functions
. /dist/druid/etc/sh.subr

############################################################ MAIN SOURCE

task_begin "Configuring perl..."
eval_spin << END_SPIN

# Introduce a delay if configured
[ "$HUMAN_DELAY" ] && sleep $HUMAN_DELAY

#
# Make symbolic links (if /usr/bin files don't already exist)
#
[ -e /usr/bin/perl ] ||
	ln -sf $PKG_PREFIX/bin/perl$PERL_VERSION /usr/bin/perl
[ -e /usr/bin/perl5 ] ||
	ln -sf $PKG_PREFIX/bin/perl$PERL_VERSION /usr/bin/perl5

#
# Make some standard directories
#
mkdir -p $PKG_PREFIX/lib/perl5/site_perl/$PERL_VERSION/mach/auto
mkdir -p $PKG_PREFIX/lib/perl5/site_perl/$PERL_VERSION/auto
mkdir -p $PKG_PREFIX/lib/perl5/$PERL_VERSION/man/man3

#
# Make a mini boot-script to call h2ph on first-boot
#
mkdir -p ${POST_INSTALL_SH%/*}
cat >> $POST_INSTALL_SH << EOF
#!/bin/sh
cd /usr/include && $PKG_PREFIX/bin/h2ph *.h machine/*.h sys/*.h >/dev/null
rm -f $POST_INSTALL_SH
EOF
chmod +x $POST_INSTALL_SH

#
# Configure persistent make(1) variables
#
cat >> $MAKE_CONF << EOF
# added by use.perl $BANNER
PERL_VERSION=$PERL_VERSION
EOF

#
# Configure additional search paths for man(1)
#
cat >> $MANPATH_CONFIG << EOF
# added by use.perl 
OPTIONAL_MANPATH	$PKG_PREFIX/lib/perl5/$PERL_VERSION/man
OPTIONAL_MANPATH	$PKG_PREFIX/lib/perl5/$PERL_VERSION/perl/man
EOF

# XXX keep the below work-around which accounts for a bug in sh(1) XXX
cat >> /dev/null << EOF
EOF

END_SPIN
task_end $?

################################################################################
# END
################################################################################
#
# $Header: /cvsroot/druidbsd/druidbsd/druid83/src/freebsd/repos/8.3-RELEASE-amd64/run_once/perl_setup.sh,v 1.1 2012/10/10 23:31:19 devinteske Exp $
#
# $Copyright: 2006-2012 Devin Teske. All rights reserved. $
#
# $Log: perl_setup.sh,v $
# Revision 1.1  2012/10/10 23:31:19  devinteske
# Commit initial public beta release of 8.3 series (beta 57; on-par with
# the 9.0 series released recently, also beta 57).
#
#
################################################################################
