## Process this file with automake to produce Makefile.in
## Created by Anjuta

SUBDIRS = src 

dist_doc_DATA = \
	README \
	COPYING \
	AUTHORS \
	ChangeLog \
	INSTALL \
	NEWS

EXTRA_DIST = \
	autogen.sh \
	$(top_srcdir)/rpmspecs/*


# Remove doc directory on uninstall
uninstall-local:
	-rm -r $(docdir)

if HAVE_GIT

dist-local:
	build-aux/gitlog-to-changelog > ChangeLog

ChangeLog:
	build-aux/gitlog-to-changelog > ChangeLog
.PHONY: ChangeLog

clean-local:
	rm -f ChangeLog

endif
