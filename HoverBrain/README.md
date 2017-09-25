## Running this code

```
make

LD_LIBRARY_PATH="libwebsock/src/.libs;pigpio" sudo bin/brain
```

## Configuration
there are some configuration options in [config.h](config.h):
- The path to the log file
- The ports for the motors/servos
- The bounding address:port for the websocket server
