# This runs in an Ubuntu environment.
FROM ubuntu:22.04

# Install git, wget, and https certificate info, which we'll use to install things, below.
# Clean up cache and temp files along the way.
RUN export DEBIAN_FRONTEND=noninteractive \
    && apt update \
    && apt install --no-install-recommends --yes git wget ca-certificates \
    && apt-get clean \
    && apt-get autoremove \
    && rm -rf /var/lib/apt/lists/*

# Try installing dserv from a release artifact!
ENV DSERV_VERSION=0.0.11
WORKDIR /opt
RUN wget https://github.com/benjamin-heasly/dserv/releases/download/$DSERV_VERSION/dserv-Linux-amd64-$DSERV_VERSION.deb
RUN export DEBIAN_FRONTEND=noninteractive \
    && apt update \
    && apt install --no-install-recommends --yes /opt/dserv-Linux-amd64-$DSERV_VERSION.deb \
    && apt-get clean \
    && apt-get autoremove \
    && rm -rf /var/lib/apt/lists/*

# docker run -ti --rm dserv:local
