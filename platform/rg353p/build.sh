#!/bin/bash
set -euo pipefail
cd "$(dirname "$0")/../.."
image=homingfever-rg353p-builder
docker build -t "$image" platform/rg353p
docker run --rm --user "$(id -u):$(id -g)" -v "$PWD:/workspace" "$image" bash -euc '
make -B PLATFORM=rg353p DEBUG=1
make -B PLATFORM=rg353p
mkdir -p release/rg353p/homingfever/licenses release/rg353p/homingfever/libs.aarch64
cp -L /opt/sdl12-compat/build/libSDL-1.2.so.0 release/rg353p/homingfever/libs.aarch64/
cp /opt/sdl12-compat/LICENSE.txt release/rg353p/homingfever/licenses/SDL12-COMPAT-LICENSE.txt
printf "%s\n" "sdl12-compat release-1.2.68 (unmodified), https://github.com/libsdl-org/sdl12-compat" > release/rg353p/homingfever/licenses/SDL12-COMPAT-SOURCE.txt
file release/rg353p/homingfever/libs.aarch64/libSDL-1.2.so.0
readelf --version-info release/rg353p/homingfever/libs.aarch64/libSDL-1.2.so.0
cp fever.aarch64 release/rg353p/homingfever/
cp -R data release/rg353p/homingfever/
cp LICENSE.txt release/rg353p/homingfever/licenses/
cp README.md release/rg353p/homingfever/licenses/UPSTREAM-README.md
cp platform/rg353p/README.md release/rg353p/homingfever/README.md
cp platform/rg353p/homingfever.gptk release/rg353p/homingfever/
cp "platform/rg353p/Homing Fever.sh" release/rg353p/
file fever.aarch64
readelf -d fever.aarch64 | grep NEEDED
readelf --version-info fever.aarch64
cd release/rg353p
rm -f ../homingfever-rg353p.zip
zip -qr ../homingfever-rg353p.zip "Homing Fever.sh" homingfever/fever.aarch64 homingfever/data homingfever/licenses homingfever/README.md homingfever/homingfever.gptk homingfever/libs.aarch64
'
