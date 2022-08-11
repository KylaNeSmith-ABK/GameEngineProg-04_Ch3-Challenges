#pragma once

#include <enet/enet.h>
#include <iostream>
#include <string>
#include <thread>

#include "PacketTypes.h"
#include "BroadcastingPackets.h"
#include "Utils.h"

class Client
{
private:
	ENetHost* NetHost = nullptr;
	ENetPeer* Peer = nullptr;
	std::thread* PacketThread = nullptr;
	std::thread* InputThread = nullptr;

	std::string UserName;
	bool bRun = true;
	bool bCanGuess = false;

	void HandleReceivedPackets(const ENetEvent& event);
	void ProcessPackets();
	void ListenForInput();

public:
	Client();
	~Client();

	bool AttemptConnectToServer();

	void SetUserName(std::string name);
	void Run();

	bool HasQuit() { return bRun == false; }
};