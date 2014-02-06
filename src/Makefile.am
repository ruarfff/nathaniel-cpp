bin_PROGRAMS = nathaniel
dist_nathaniel_SOURCES = Nathaniel.cc \
			 ResourceManager.cc \
			 input/Button.cc \
			 game/GameCharacter.cc

AM_CPPFLAGS = -DDATADIR=\"$(DESTDIR)$(pkgdatadir)\" \
	      -DLOCALEDIR=\"$(localedir)\" \
	      -I./input -I./game -I./libs
