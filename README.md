<div align="center">
<a href="https://www.glowbuzzer.com">
    <img src="images/tiny-logo.svg" alt="Logo" width="100" height="120">
  </a>
<h3 align="center">glowbuzzer - gbsm</h3>
  <h4 align="center">
    <b>Stepmaster</b>
    <br />
    <br />
    <br />
  </h4>
</div>


# What does it do?

GBSM is the part of the glowbuzzer toolkit used for communicating with motor driver ICs over SPI from Linux or microcontroller platforms.

On Linux, GBSM talks to GBC over shared memory and, at bus cycle speed (1ms, 2ms , 4ms etc.), it receives commands to control motors and IO from GBC.

On microcontroller platforms GBSM uses SPI to talk to GBC.

GBSM translates these high-level commands from GBC into those needed by the motor drivers and sends them out over SPI to the individual driver ICs.

It also receives position and status information over SPI from the motor drivers and passes this back over shared memory to GBC.

GBSM needs access to GPIO pins so running on desktop Linux platforms or WSL/ Virtual Machines is hard.

# Compiling

## pipgio

pigpio is a library for the Raspberry Pi which allows control of the General Purpose Input Outputs (GPIO) pins.

pigpio is available on GitHub at: <a href="https://github.com/joan2937/pigpio">https://github.com/joan2937/pigpio</a>

To install:

````shell
wget https://github.com/joan2937/pigpio/archive/master.zip
unzip master.zip
cd pigpio-master
make
sudo make install

````
## gbsm

GBSM needs to be downloaded from GitHub and compiled locally.

First, download from GitHub by cloning the repositiory. The `--recursive` option needs to be added to the `clone` command as the repository uses sub-modules and you need to also download these to be able to compile it.

```shell
git clone --recursive https://github.com/glowbuzzer/gbsm
```
This will clone the project and the sub-modules it depends on into `gbsm` directory.

Next we need to compile the code. This can either be done by importing the code into your IDE and doing the build there or from the command line.

First `cd` into the `gbsm` directory. Then

```shell
mkdir build
cd build
cmake .. -DFLAVOUR=PI
make
```
If you don't specify `-DFLAVOUR=PI` then a generic Linux build will be processed.

This will create the GBSM binary in the `builds` directory that can be run with `./GBSM` and the appropriate command line options..

You can also do a `sudo make install` if you want to copy the executable into the `bin` directory.

# Running

GBSM takes a single optional command line argument `-p` which denotes the program within GBSM to run.

Usually you just run GBSM without the `-pCYCLIC` option as this is the default.

`-pCYCLIC` makes GBSM run in the normal cyclic mode where it communicates with GBC.

The optional programs are used to test and configured the motor drivers.

Available programs:

* `-pREG_READ` to read a register
* `-pTEST_MOVE` to execute a test oscillating move on all connected motors
* `-pHYST_TUNE` to execute hysteresis tuning on the motors


When you run GBSM, the first thing you will see is the welcome banner like this:

```shell
[INFO  ] GBSM: **************************************************************************
[INFO  ] GBSM: ***                  Starting GB Stepper Controller                    ***
[INFO  ] GBSM: **************************************************************************
[INFO  ] GBSM: Software project name [GBSM]
[INFO  ] GBSM: Software project version [0.0.1]
```

Then, GBSM will need to connect to GBC and the motor driver ICs over SPI to continue its execution so make sure GBC is running and the SPI bus is connected.

