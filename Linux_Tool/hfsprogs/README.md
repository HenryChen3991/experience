# Hfsprogs

* Build code order
	* wget -c http://ftp.de.debian.org/debian/pool/main/h/hfsprogs/hfsprogs_332.25.orig.tar.gz
	* tar -xzf hfsprogs_332.25.orig.tar.gz
	* cd diskdev_cmds-332.25
	* wget -c http://ftp.de.debian.org/debian/pool/main/h/hfsprogs/hfsprogs_332.25-11.debian.tar.gz
	* tar -xzf hfsprogs_332.25-11.debian.tar.gz
	* cat debian/patches/*.patch | patch -p1
	* make -f Makefile.lnx # Doesn't mind -j#
	* install -o 0.0 fsck_hfs.tproj/fsck_hfs /usr/bin/fsck.hfs
	* install -o 0.0 newfs_hfs.tproj/newfs_hfs /usr/bin/mkfs.hfs
