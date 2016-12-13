/**
	\file network_interface.h
	\brief Interface to read from and write to the network
	\author Natesh Narain <nnaraindev@gmail.com>
	\date Dec 12 2016
*/

#ifndef DOTRIX_NET_NETWORK_INTERFACE_H
#define DOTRIX_NET_NETWORK_INTERFACE_H

#include <cstdint>

class NetworkInterface
{
public:

	virtual void write(uint8_t* buffer, size_t len) = 0;
	virtual void read(uint8_t* buffer, size_t len) = 0;
	virtual size_t available() = 0;

private:
};

#endif