# This runs in an Ubuntu environment.
FROM ubuntu:22.04

RUN export DEBIAN_FRONTEND=noninteractive \
    && apt update \
    && apt install --no-install-recommends --yes git wget ca-certificates build-essential cmake libevdev-dev zlib1g-dev zip \
    && apt-get clean \
    && apt-get autoremove \
    && rm -rf /var/lib/apt/lists/*

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

WORKDIR /work
RUN git clone https://github.com/benjamin-heasly/dserv.git \
  && cd dserv \
  && git checkout ci-releases \
  && mkdir build \
  && cd build/ \
  && cmake -DCMAKE_INSTALL_PREFIX=/work/package/usr/local .. \
  && cmake --build . \
  && cmake --install .

RUN export DEBIAN_FRONTEND=noninteractive \
  && apt update \
  && apt install --no-install-recommends --yes libpango1.0-dev libwayland-dev wayland-protocols libxkbcommon-dev libxinerama-dev libdbus-1-dev libglew-dev  libdecor-0-dev libgtk-3-dev liblz4-dev libhpdf-dev \
  && apt-get clean \
  && apt-get autoremove \
  && rm -rf /var/lib/apt/lists/*

WORKDIR /work
RUN git clone https://github.com/fltk/fltk.git \
  && cd fltk \
  && git checkout release-1.4.2 \
  && cmake -B build \
  && cmake --build build \
  && cmake --install build

WORKDIR /work
RUN git clone https://github.com/SheinbergLab/dlsh \
  && cd dlsh \
  && cmake -B build \
  && cmake --build build \
  && cmake --install build

WORKDIR /work/dserv/tools/essgui
RUN mkdir build \
  && cd build/ \
  && cmake -DCMAKE_INSTALL_PREFIX=/work/package/usr/local .. \
  && cmake --build . \
  && cmake --install .

WORKDIR /work/package/usr/local/dlsh
RUN wget https://github.com/benjamin-heasly/dlsh/releases/download/initial/dlsh.zip

WORKDIR /work/package/DEBIAN
RUN cp /work/dserv/dpkg/control . \
  && cp /work/dserv/dpkg/postinst . \
  && chmod +x ./postinst \
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

# docker build . -t dserv:local

# docker run --rm -ti dserv:local /bin/sh -c 'echo "test" | essctrl && cat history.txt'
# docker run --rm -ti dserv:local /usr/local/dserv/dserv --help
# docker run --rm -ti dserv:local timeout --signal SIGINT 2 /usr/local/dserv/dserv -c /usr/local/dserv/config/dsconf.tcl -t /usr/local/dserv/config/triggers.tcl
# docker run --rm -ti dserv:local ls -alth /usr/local/bin
# docker run --rm -ti dserv:local ls -alth /usr/local/dserv/db