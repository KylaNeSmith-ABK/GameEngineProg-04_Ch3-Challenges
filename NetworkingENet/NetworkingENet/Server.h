#pragma once

#include <enet/enet.h>
#include <iostream>
#include <string>
#include <thread>

#include "PacketTypes.h"
#include "BroadcastingPackets.h"

class Server
{
private:
	ENetHost* NetHost = nullptr;
	ENetPeer* Peer = nullptr;
	std::thread* PacketThread = nullptr;

	bool bRun = true;
	int NumToGuess = 0;
	int NumPlayers = 0;

	void HandleReceivedPackets(const ENetEvent& event);
	void ProcessPackets();

public:
	Server();
	~Server();

	void Run();

	bool HasStopped() { return bRun == false; }
};