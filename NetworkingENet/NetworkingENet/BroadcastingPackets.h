#pragma once

#include <enet/enet.h>
#include <iostream>

#include "PacketTypes.h"

void BroadcastTextPacket(ENetHost* netHost, std::string text);

void BroadcastCanGuessPacket(ENetHost* netHost, bool canGuess);

void BroadcastPlayerGuessPacket(ENetHost* netHost, std::string userName, int guessNum);

void BroadcastGameOverPacket(ENetHost* netHost);

void BroadcastIsDeadPacket(ENetHost* netHost);