# This runs in an Ubuntu environment.
FROM ubuntu:22.04

RUN export DEBIAN_FRONTEND=noninteractive \
    && apt update \
    && apt install --no-install-recommends --yes git wget ca-certificates build-essential cmake libevdev-dev zlib1g-dev zip \
    && apt-get clean \
    && apt-get autoremove \
    && rm -rf /var/lib/apt/lists/*

# TODO: I don't like installing tcl twice: once for build and once for package.
# Get build to use the prefixed installation?
# Or copy or link a subset of files?
WORKDIR /work
RUN git clone --depth=1 --branch core-9-0-1-rc https://github.com/tcltk/tcl.git \
  && cd tcl/unix/ \
  && ./configure --prefix=/work/package/usr/local/dserv/tcl/ \
  && make \
  && make install \
  && ./configure \
  && make \
  && make install

WORKDIR /work
RUN git clone https://github.com/akheron/jansson.git \
  && cd jansson/ \
  && git checkout v2.14 \
  && mkdir build \
  && cd build/ \
  && cmake .. \
  && make \
  && make install

# TODO: get the installed dserv to locate packaged tcl at /work/package/usr/local/dserv/tcl
# Possibly with LD_RUN_PATH='$ORIGIN/lib'
# Or CMAKE_INSTALL_RPATH
WORKDIR /work
RUN git clone https://github.com/benjamin-heasly/dserv.git \
  && cd dserv \
  && git checkout ci-releases \
  && mkdir build \
  && cd build/ \
  && cmake -DCMAKE_INSTALL_PREFIX=/work/package/usr/local .. \
  && cmake --build . \
  && cmake --install .

WORKDIR /work/package/usr/local/dlsh
RUN wget https://sheinberglab.org/dserv/dlsh.zip

WORKDIR /work
RUN git clone https://github.com/sqlite/sqlite.git \
  && cd sqlite \
  && git checkout version-3.47.2 \
  && ./configure --enable-all --with-tclsh=/work/package/usr/local/dserv/tcl/bin/tclsh9.0 \
  && make tclextension

WORKDIR /work/package/usr/local/dlsh
RUN mkdir -p ./lib/sqlite/Linux/x86_64/ \
  && cp /work/sqlite/libtcl9sqlite3.47.2.so ./lib/sqlite/Linux/x86_64/ \
  && zip dlsh.zip ./lib/sqlite/Linux/x86_64/libtcl9sqlite3.47.2.so

# TODO: I don't like installing dlsh twice: once for tests and once for package.
# Point the tests at the package location?
# Or will these end up being in separate jobs anyway?
WORKDIR /work/dserv/build
RUN mkdir /work/dserv/dlsh \
  && cp /work/package/usr/local/dlsh/dlsh.zip /work/dserv/dlsh \
  && ctest -VV --timeout 10 ..

WORKDIR /work/package/DEBIAN
RUN cp /work/dserv/dpkg/control . \
  && cp /work/dserv/dpkg/install-dserv-service.sh . \
  && chmod +x ./install-dserv-service.sh

WORKDIR /work
RUN echo "Version: 0.0.test" >> /work/package/DEBIAN/control \
  && echo "Architecture: $(dpkg --print-architecture)" >> /work/package/DEBIAN/control \
  && dpkg-deb --build /work/package dserv-test.deb

WORKDIR /work
RUN export DEBIAN_FRONTEND=noninteractive \
&& apt update \
&& apt install --no-install-recommends --yes ./dserv-test.deb \
&& apt-get clean \
&& apt-get autoremove \
&& rm -rf /var/lib/apt/lists/*

# docker run --rm -ti dserv:local essctrl
# docker run --rm -ti dserv:local /usr/local/dserv/dserv --help
# docker run --rm -ti dserv:local /usr/local/dserv/dserv -c /usr/local/dserv/config/dsconf.tcl -t /usr/local/dserv/config/triggers.tcl
