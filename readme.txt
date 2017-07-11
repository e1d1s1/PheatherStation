Pre-reqs:

sudo apt-get install dfu-util gcc-arm-none-eabi
or toolchain from https://po-util.com/
$ curl -fsSLO https://raw.githubusercontent.com/nrobinson2000/po-util/master/po-util.sh
$ chmod +x po-util.sh ; ./po-util.sh install

stlink v2 USB stick programmer tools:
git clone https://github.com/texane/stlink.git
cd stlink
mkdir build
cd build
../configure
make
sudo make install

get openocd for platform or source if using JTAG/SWD programming
source: git clone https://git.code.sf.net/p/openocd/code openocd-code
./bootstrap (when building from the git repository)
./configure [options]
make
sudo make install

get particle-cli, requires node.js/npm for platform if using USB flahsing
git clone git@github.com:spark/particle-cli.git
cd particle-cli
npm install

Building:
mkdir build
cd build
cmake ..

Flashing via cmake and particle-cli over on-board USB port:
make pheatherstation-firmware-upload

Flashing via openocd via STLinkv2 and SWD headers + external 3.3V power and DFU mode enabled:
make
# in one session of cmd
cd /usr/local/share/openocd/scripts
openocd -f interface/stlink-v2.cfg -f target/stm32f2x.cfg -c "telnet_port 4444"

# in another session
telnet localhost 4444
> reset halt
> program /path/to/photon.bin verify 0x80A0000
> reset






