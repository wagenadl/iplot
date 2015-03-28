ALL: iplot octave/ip_internal.oct

debug: iplot_debug

iplot:; +make -C src

iplot_debug:; +make -C src debug

octave/ip_internal.oct: src/ip_internal.cpp src/LRBase.cpp src/LRBase.h \
	src/Launcher.cpp src/Launcher.h src/Command.h
	mkoctfile -W -Wall -o $@ \
		src/ip_internal.cpp src/LRBase.cpp src/Launcher.cpp

.PHONY: ALL debug iplot iplot_debug
