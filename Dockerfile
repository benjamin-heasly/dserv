# This runs in an Ubuntu environment.
FROM ubuntu:22.04

# Install build tools, dserv build dependencies, and essgui build dependencies.
RUN export DEBIAN_FRONTEND=noninteractive \
    && apt update \
    && apt install --no-install-recommends --yes git wget ca-certificates build-essential cmake zip unzip \
    libevdev-dev zlib1g-dev \
    libpango1.0-dev libwayland-dev wayland-protocols libxkbcommon-dev libxinerama-dev libdbus-1-dev libglew-dev  libdecor-0-dev libgtk-3-dev liblz4-dev \
    && apt-get clean \
    && apt-get autoremove \
    && rm -rf /var/lib/apt/lists/*

# Install TLC 9 into the build environment and to a dserv packaging dir.
WORKDIR /work
RUN git clone --depth=1 --branch core-9-0-1-rc https://github.com/tcltk/tcl.git \
  && cd tcl/unix/ \
  && ./configure --prefix=/work/package/usr/local/dserv/tcl/ \
  && make \
  && make install \
  && ./configure \
  && make \
  && make install

# Install Jansson into the build environment.
WORKDIR /work
RUN git clone https://github.com/akheron/jansson.git \
  && cd jansson/ \
  && git checkout v2.14 \
  && cmake -B build \
  && cmake --build build \
  && cmake --install build

# Install FLTK into the build environment.
WORKDIR /work
RUN git clone https://github.com/fltk/fltk.git \
  && cd fltk \
  && git checkout release-1.4.2 \
  && cmake -B build \
  && cmake --build build \
  && cmake --install build  

# Install the libharu PDF dependency.
WORKDIR /work
RUN git clone https://github.com/libharu/libharu.git \
  && cd libharu \
  && git checkout v2.4.4 \
  && cmake -DBUILD_SHARED_LIBS=OFF -B build \
  && cmake --build build \
  && cmake --install build

# Install our dlsh libs and headers into the build environment.
ENV DLSH_VERSION=0.0.13
WORKDIR /work/dlsh
RUN wget https://github.com/benjamin-heasly/dlsh/releases/download/$DLSH_VERSION/dlsh-Linux-$(dpkg --print-architecture)-$DLSH_VERSION.zip \
  && unzip dlsh-Linux-$(dpkg --print-architecture)-$DLSH_VERSION.zip \
  && cp ./lib/*.a /usr/local/lib \
  && cp ./lib/*.so /usr/local/lib \
  && cp ./include/*.h /usr/local/include

# Install our dlsh utils and thier dependencies into the packaging dir.
WORKDIR /work/package/usr/local/dlsh
RUN wget https://github.com/benjamin-heasly/dlsh/releases/download/initial/dlsh.zip

# Build dserv itself and install to the packaging dir.
ADD . /work/dserv
WORKDIR /work/dserv/
RUN cmake -DCMAKE_INSTALL_PREFIX=/work/package/usr/local -B build \
  && cmake --build build \
  && cmake --install build

# Build essgui itself and install to the packaging dir.
WORKDIR /work/dserv/tools/essgui
RUN mkdir build \
  && cd build/ \
  && cmake -DCMAKE_INSTALL_PREFIX=/work/package/usr/local .. \
  && cmake --build . \
  && cmake --install .

# Add metadata and scripts to include in a .deb package.
WORKDIR /work/package/DEBIAN
RUN cp /work/dserv/dpkg/control . \
  && echo "Version: 0.0.test" >> ./control \
  && echo "Architecture: $(dpkg --print-architecture)" >> ./control \
  && cp /work/dserv/dpkg/postinst . \
  && chmod +x ./postinst \
  && cp /work/dserv/dpkg/install-dserv-service.sh . \
  && chmod +x ./install-dserv-service.sh

# Build the .deb package.
WORKDIR /work
RUN dpkg-deb --build /work/package dserv-test.deb

# As a sanity check, install the same .deb package.
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

# docker run -u $UID -v /home/ninjaben/.Xauthority:/.Xauthoruty -e XAUTHORITY=/.Xauthority -v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY --net=host --rm -ti dserv:local essgui
# essgui came up!
