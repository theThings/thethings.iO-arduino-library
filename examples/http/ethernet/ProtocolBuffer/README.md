# Protocol Buffers Example

This example uses [nanopb](http://koti.kapsi.fi/jpa/nanopb/) to send data to the [thethings.iO](https://thethings.io) platform. The nanopb library allows limited resources devices such as the arduino to use the [Protocol Buffers](https://developers.google.com/protocol-buffers/) protocol.

## How to use it

1. Download and install [Protocol Buffers](https://developers.google.com/protocol-buffers/)
1. Download and install [nanopb](http://koti.kapsi.fi/jpa/nanopb/)
1. copy or link the files `pb.h`, `pb_encode.h`, `pb_encode.c`, `pb_common.h`, `pb_common.c` from the root of nanopb to the project folder
1. compile the `.proto` file with `protoc -oexample.pb example.proto`
1. generate the `example.pb.h` and `example.pb.c` files with `python path/to/nanopb/generator/nanopb_generator.py example.pb`
1. Change the `TOKEN` in `ProtocolBuffer.ino`
1. Upload to the arduino and test it!
