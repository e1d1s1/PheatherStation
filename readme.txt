sudo apt-get install dfu-util gcc-arm-none-eabi
or toolchain from https://po-util.com/

$ curl -fsSLO https://raw.githubusercontent.com/nrobinson2000/po-util/master/po-util.sh
$ chmod +x po-util.sh ; ./po-util.sh install


git clone https://github.com/texane/stlink.git
cd stlink
mkdir build
cd build
../configure
make
sudo make install
