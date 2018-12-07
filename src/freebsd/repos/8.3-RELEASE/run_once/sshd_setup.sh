#!/bin/sh
# -*- tab-width:  4 -*- ;; Emacs
# vi: set tabstop=4     :: Vi/ViM

# If running as xarchInstall, only use binaries in `/stand'
[ "$xarchInstall" ] && export PATH=/stand
############################################################ CONFIGURATION

# Indicate that we want color output
ANSI_COLOR=YES

# Pathnames
SSHD_CONFIG=/etc/ssh/sshd_config
RC_CONF=/etc/rc.conf

############################################################ FUNCTIONS

# Include standard functions
. /dist/druid/etc/sh.subr

############################################################ MAIN SOURCE

task_begin "Configuring sshd..."
eval_spin << END_SPIN

# Introduce a delay if configured
[ "$HUMAN_DELAY" ] && sleep $HUMAN_DELAY

# Enable sshd(8) at boot-time
cat >> $RC_CONF << EOF
sshd_enable="YES"
EOF

# XXX keep the below work-around which accounts for a bug in sh(1) XXX
cat >> /dev/null << EOF
EOF

# Enable root login
sed -i.bak -e 's/^#\(PermitRootLogin\) no$/\1 yes/' $SSHD_CONFIG

END_SPIN
task_end $?

################################################################################
# END
################################################################################
#
# $Header: /cvsroot/druidbsd/druidbsd/druid83/src/freebsd/repos/8.3-RELEASE/run_once/sshd_setup.sh,v 1.2 2012/10/30 22:32:58 devinteske Exp $
#
# $Copyright: 2006-2012 Devin Teske. All rights reserved. $
#
# $Log: sshd_setup.sh,v $
# Revision 1.2  2012/10/30 22:32:58  devinteske
# Bump FreeBSD version to 1.0b58 for:
# + host_rebuild: don't overwrite /etc/hosts
# + host_rebuild: backup /usr/local/etc
# + druid post-install: don't remove sshd_config.bak
#
# Revision 1.1  2012/10/10 23:30:51  devinteske
# Commit initial public beta release of 8.3 series (beta 57; on-par with
# the 9.0 series released recently, also beta 57).
#
#
################################################################################
