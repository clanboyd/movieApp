SUBDIRS = movieDaemon reader

subdirs:
	for dir in $(SUBDIRS); do \
	    cd $$dir; \
	    $(MAKE) -f $$dir.mak; \
	    cd ..; \
	done

clean:
	for dir in $(SUBDIRS); do \
	    cd $$dir; \
	    $(MAKE) -f $$dir.mak clean; \
	    cd ..; \
	done

install:
	for dir in $(SUBDIRS); do \
	    cd $$dir; \
	    $(MAKE) -f $$dir.mak install; \
	    cd ..; \
	done
	scp movieApp.mak yoda@raspberrypi:/home/yoda/movieApp/movieApp.mak
	scp common/include/* yoda@raspberrypi:/home/yoda/movieApp/common/include

