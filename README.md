# Discovery Protocol
Prototype for Apparateq discovery protocol. The protocol uses multicast
for device discovery and configuration. The target platform is small
embedded systems with limited network protocol capabilities. In principle
the devices do not even need to have an assigned IP address, but they need
to be able to receive UDP over IP multicast.

The protocol data format is described in _src/Protocol.h_ and consists of
a single __Message__ struct which is identical whether communications goes
from server -> client or in the opposite direction.

Currently this concept is implemented as two unix processes, _client_ and
_server_ but could easily be adapted to a small embedded system.

## Prerequisites
The only requirements are __cmake__ and a __C++__ compiler.


## Building
The project builds on macOS and Linux and uses the _std::thread_ library.

    > git clone https://github.com/mortenjc/discovery.git
    > cd discovery
    > mkdir build
    > cd build
    > cmake ..
    > make


## Running the applications

The _client_ application instantiates a 'device' with an initial state of
'booting' and an 'ip address' of "0.0.0.0". It generates a random serial
numbe, product id and product name. As the serial number is based on
the unix process id there is a small risk of duplicate serial numbers if
the clients are run on different machines.

The _server_ application is a continuous loop asking for a command like
'discovery', 'address assignment' or 'configuration'. For the two latter
additional arguments must be specified.


### Clients
To start the clients, open a terminal window and type the following command

    build> ./bin/client

### Server

To start the server,

    build> ./bin/server


## Security considerations
This is __not__ designed to be a secure protocol. Anyone can currently configure
and reconfigure any device at any time. The target is a measurement laboratory
or similar with these devices performing non critical operations.

## Authors

* Morten Jagd Christensen

See also the [list of contributors](https://github.com/mortenjc/discovery/graphs/contributors) on Github.

## License

This project is licensed under the BSD-2 License see the [LICENSE](LICENSE) file for details.
