## Makefile.am -- Process this file with automake to process Makefile.in

angband_files = \
  delete.me

EXTRA_DIST = \
  $(angband_files)

if SET_GID
angbanddir = @DEFAULT_PATH@data

angband_DATA = \
  $(angband_files)
endif


install-data-hook:
if SET_GID
	chown -R root.@GAMEGROUP@ "$(DESTDIR)$(angbanddir)"
	chmod g+w "$(DESTDIR)$(angbanddir)"
endif
