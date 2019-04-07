# E2M-wear: Wearable electromyography and adaptive haptic feedback for physical therapy

Protoype code to run a wearable assistive tech for physical therapy. Contains:

* Python code for off-board signal processing
* Arduino code to read analog input from a Myoelectric sensor and transmit to Python analyzer via USB/Serial interface
* Management code (bash) for e.g. starting Jupyter to run Python with scipy, TensorFlow and other libraries

# Run Python interface to Arduino

Currently the sampler just writes to stdout, intended for file redirection and subsequent signal analysis on the capture file.

```
E2MWEAR_ARDUINO_DEV=/dev/cu.usbmodem14101
E2MWEAR_ARDUINO_BAUD=57600
DATA_FILE=jupyter/data/myo.$(date +%Y%m%d%H%M%S)
./arduino-sampler.py | tee $DATA_FILE
```

Redirecting to `jupyter/data/*` ensures data file is available in the Jupyter container for analysis through Docker volume

# Signal processing in Jupyter

Start on a Docker container:

```
./start-jupyter
```

The Jupyter notebook for signal processing is at: 
http://localhost:8888/notebooks/work/jupyter/myo-sigproc.ipynb

NOTE: Jupyter will be at http://localhost:8888. First use requires a token, which is shown in the container logs: `docker logs jupyter`
 
 