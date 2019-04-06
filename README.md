# E2M-wear: Wearable electromyography and adaptive haptic feedback for physical therapy

Protoype code to run a wearable phyiscal therapy device bits of code to read muscular activation potentals via it of code that shows you how to make a spectrogram/sonogram in python using numpy, scipy, and a few functions written by Kyle Kastner. I also show you how to invert those spectrograms back into wavform, filter those spectrograms to be mel-scaled, and invert those spectrograms as well. This should prove to be a useful tool for those interested in generative modelling (as I am). For example, running spectrograms through an LSTM, VAE, GAN, VAE-GAN, or experimenting with your own audio/waveform models. Check out the LibriSpeech dataset. for a 1000 hr dataset of transcripted speech from open source audio books.

# Signal processing in Jupyter

Start on a Docker container:

```
./start-jupyter
```

Jupyter will be at http://localhost:8888. First use requires a token, which is shown in the startup logs. 
 `docker ps` 