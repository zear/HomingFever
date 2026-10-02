FROM ubuntu:18.04
RUN dpkg --add-architecture arm64 \
 && sed -i 's/^deb /deb [arch=amd64] /' /etc/apt/sources.list \
 && printf '%s\n' 'deb [arch=arm64] http://ports.ubuntu.com/ubuntu-ports bionic main universe' 'deb [arch=arm64] http://ports.ubuntu.com/ubuntu-ports bionic-updates main universe' 'deb [arch=arm64] http://ports.ubuntu.com/ubuntu-ports bionic-security main universe' > /etc/apt/sources.list.d/arm64.list \
 && apt-get update \
 && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends gcc-aarch64-linux-gnu libc6-dev-arm64-cross libsdl1.2-dev:arm64 make zip file cmake curl ca-certificates libsdl2-dev:arm64 \
 && rm -rf /var/lib/apt/lists/*
# Use newer SDL2 headers for building compat; SDL2 itself stays device-provided.
RUN curl -fL --connect-timeout 20 --max-time 120 --retry 3 https://github.com/libsdl-org/SDL/archive/refs/tags/release-2.26.2.tar.gz -o /tmp/SDL2.tar.gz \
 && mkdir -p /opt/SDL2 \
 && tar -xzf /tmp/SDL2.tar.gz --strip-components=1 -C /opt/SDL2 \
 && rm /tmp/SDL2.tar.gz
# Pin the compatibility layer release; only its shared library is shipped.
RUN curl -fL --connect-timeout 20 --max-time 120 --retry 3 https://github.com/libsdl-org/sdl12-compat/archive/refs/tags/release-1.2.68.tar.gz -o /tmp/sdl12-compat.tar.gz \
 && mkdir -p /opt/sdl12-compat \
 && tar -xzf /tmp/sdl12-compat.tar.gz --strip-components=1 -C /opt/sdl12-compat \
 && mkdir /opt/sdl12-compat/build \
 && cd /opt/sdl12-compat/build \
 && cmake .. -DCMAKE_SYSTEM_NAME=Linux -DCMAKE_SYSTEM_PROCESSOR=aarch64 -DCMAKE_C_COMPILER=aarch64-linux-gnu-gcc -DCMAKE_BUILD_TYPE=Release -DSDL2_INCLUDE_DIR=/opt/SDL2/include -DSDL12TESTS=OFF -DSDL12DEVEL=OFF \
 && make -j2 \
 && aarch64-linux-gnu-strip libSDL-1.2.so.0 \
 && rm /tmp/sdl12-compat.tar.gz
WORKDIR /workspace
