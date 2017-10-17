.PHONY: libejdb

libejdb:
	cd deps/libejdb && \
	mkdir -p build && \
	cd build && \
	cmake -DCMAKE_INSTALL_PREFIX=${CURDIR}/libejdb -DCMAKE_BUILD_TYPE=Release ../ && \
	make && \
	make install
