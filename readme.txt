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
git clone https://github.com/particle-iot/particle-cli.git
cd particle-cli
sudo npm install -g

Building:
mkdir build
cd build
cmake ..

You must build pre-reqs first:
make particle-firmware-deps

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

# serial/usb debug message output
# using a USB-RS232 converter connected to a RS232-TTL converter
sudo chmod 666 /dev/ttyUSB0
screen /dev/ttyUSB0 115000
# using build in USB port, virtual COM
screen /dev/ttyACM1 9600



I/O Ports
D0 - I2C SDA (BME280 Atmospheric Sensor)
D1 - I2C SCL (BME280 Atmospheric Sensor)
D2 - Rain Gauge
D3 - Anemometer
D6-D7 SWD Programmer
A0 - Wind Vane
A1 - Battery Voltage
A2 - Additional temp sensor
WAKE - Rain Gauge, or Windspeed over X KPH would be nice using a counter circuit






