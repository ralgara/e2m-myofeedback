# E2M-wear: Wearable electromyography and adaptive haptic feedback for physical therapy

Protoype code to run a wearable assistive tech for physical therapy. Contains:

* Python code for off-board signal processing
* Arduino code to read analog input from a Myoelectric sensor and transmit to Python analyzer via USB/Serial interface
* Management code (bash) for e.g. starting Jupyter to run Python with scipy, TensorFlow and other libraries

# Run Python interface to Arduino

Currently the sampler just writes to stdout, intended for file redirection and subsequent signal analysis on the capture file.

`./arduino-sampler.py > /tmp/myo`

With, e.g.:

```$ tail -f /tmp/myo

334
327
320
...
```

Still working in a real-time data interface to Jupyter. Containers seem to require privileged mode to access host devices for USB Arduino interface.

  
# Signal processing in Jupyter

Start on a Docker container:

```
./start-jupyter
```

Jupyter will be at http://localhost:8888. First use requires a token, which is shown in the startup logs. 
 `docker ps`
 
 