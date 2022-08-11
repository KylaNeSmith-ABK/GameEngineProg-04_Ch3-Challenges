#include "BroadcastingPackets.h"

void BroadcastTextPacket(ENetHost* netHost, std::string text)
{
    TextPacket* TPacket = new TextPacket();
    TPacket->text = text;
    ENetPacket* packet = enet_packet_create(TPacket,
        sizeof(TextPacket) + 1,
        ENET_PACKET_FLAG_RELIABLE);

    /* One could also broadcast the packet by         */
    enet_host_broadcast(netHost, 0, packet);
    //enet_peer_send(event.peer, 0, packet);

    /* One could just use enet_host_service() instead. */
    //enet_host_service();
    enet_host_flush(netHost);
    delete TPacket;
}

void BroadcastCanGuessPacket(ENetHost* netHost, bool canGuess)
{
    CanGuessPacket* CanPacket = new CanGuessPacket();
    CanPacket->canGuess = canGuess;
    ENetPacket* packet = enet_packet_create(CanPacket,
        sizeof(CanGuessPacket),
        ENET_PACKET_FLAG_RELIABLE);

    /* One could also broadcast the packet by         */
    enet_host_broadcast(netHost, 0, packet);
    //enet_peer_send(event.peer, 0, packet);

    /* One could just use enet_host_service() instead. */
    //enet_host_service();
    enet_host_flush(netHost);
    delete CanPacket;
}

void BroadcastPlayerGuessPacket(ENetHost* netHost, std::string userName, int guessNum)
{
    PlayerGuessPacket* GuessPacket = new PlayerGuessPacket();
    GuessPacket->userName = userName;
    GuessPacket->playerGuess = guessNum;
    ENetPacket* packet = enet_packet_create(GuessPacket,
        sizeof(PlayerGuessPacket),
        ENET_PACKET_FLAG_RELIABLE);

    /* One could also broadcast the packet by         */
    enet_host_broadcast(netHost, 0, packet);
    //enet_peer_send(event.peer, 0, packet);

    /* One could just use enet_host_service() instead. */
    //enet_host_service();
    enet_host_flush(netHost);
    delete GuessPacket;
}

void BroadcastGameOverPacket(ENetHost* netHost)
{
    GameOverPacket* GOPacket = new GameOverPacket();
    ENetPacket* packet = enet_packet_create(GOPacket,
        sizeof(GameOverPacket),
        ENET_PACKET_FLAG_RELIABLE);

    /* One could also broadcast the packet by         */
    enet_host_broadcast(netHost, 0, packet);
    //enet_peer_send(event.peer, 0, packet);

    /* One could just use enet_host_service() instead. */
    //enet_host_service();
    enet_host_flush(netHost);
    delete GOPacket;
}

void BroadcastIsDeadPacket(ENetHost* netHost)
{
    IsDeadPacket* DeadPacket = new IsDeadPacket();
    DeadPacket->IsDead = true;
    ENetPacket* packet = enet_packet_create(DeadPacket,
        sizeof(IsDeadPacket),
        ENET_PACKET_FLAG_RELIABLE);

    /* One could also broadcast the packet by         */
    enet_host_broadcast(netHost, 0, packet);
    //enet_peer_send(event.peer, 0, packet);

    /* One could just use enet_host_service() instead. */
    //enet_host_service();
    enet_host_flush(netHost);
    delete DeadPacket;
}