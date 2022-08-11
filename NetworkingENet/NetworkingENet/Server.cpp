#include "Server.h"

Server::Server()
{
    //can pass in a peer connection if wanting to limit
    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = 1234;
    //NetHost = enet_host_create(&address /* the address to bind the server host to */,
    //    32      /* allow up to 32 clients and/or outgoing connections */,
    //    2      /* allow up to 2 channels to be used, 0 and 1 */,
    //    0      /* assume any amount of incoming bandwidth */,
    //    0      /* assume any amount of outgoing bandwidth */);
    NetHost = enet_host_create(&address /* the address to bind the server host to */,
        2      /* allow up to 2 clients and/or outgoing connections */,
        2      /* allow up to 2 channels to be used, 0 and 1 */,
        0      /* assume any amount of incoming bandwidth */,
        0      /* assume any amount of outgoing bandwidth */);

    if (!NetHost)
    {
        fprintf(stderr,
            "An error occurred while trying to create an ENet server.\n");
        exit(EXIT_FAILURE);
    }

    NumToGuess = rand() % 10 + 1;

    std::cout << "waiting for players to join..." << std::endl;
    
}

Server::~Server()
{
    if (PacketThread)
    {
        PacketThread->join();
    }
    delete PacketThread;

    if (NetHost != nullptr)
    {
        enet_host_destroy(NetHost);
    }
}

void Server::Run()
{
    PacketThread = new std::thread([this] {ProcessPackets(); });
}

void Server::HandleReceivedPackets(const ENetEvent& event)
{
    GamePacket* RecGamePacket = (GamePacket*)(event.packet->data);
    if (RecGamePacket)
    {
        //cout << "Received Game Packet " << endl;

        switch (RecGamePacket->Type)
        {
            case PHT_Text:
            {
                TextPacket* TPacket = (TextPacket*)(event.packet->data);
                if (TPacket)
                {
                    BroadcastTextPacket(NetHost, TPacket->text);
                }

                break;
            }
            case PHT_PlayerGuess:
            {
                PlayerGuessPacket* GuessPacket = (PlayerGuessPacket*)(event.packet->data);
                if (GuessPacket)
                {
                    BroadcastCanGuessPacket(NetHost, false);

                    std::string message = GuessPacket->userName
                        + ":"
                        + std::to_string(GuessPacket->playerGuess);
                    BroadcastTextPacket(NetHost, message);

                    if (GuessPacket->playerGuess == NumToGuess)
                    {
                        BroadcastTextPacket(NetHost, "Correct!");
                        BroadcastGameOverPacket(NetHost);
                        bRun = false;
                        break;
                    }
                    else
                    {
                        BroadcastTextPacket(NetHost, "Incorrect!");
                    }

                    BroadcastCanGuessPacket(NetHost, true);
                }

                break;
            }
        }
    }
    else
    {
        std::cout << "Invalid Packet " << std::endl;
    }

    /* Clean up the packet now that we're done using it. */
    enet_packet_destroy(event.packet);
    {
        enet_host_flush(NetHost);
    }
}

void Server::ProcessPackets()
{
    while (bRun)
    {
        ENetEvent event;
        while (enet_host_service(NetHost, &event, 1000) > 0)
        {
            switch (event.type)
            {
                case ENET_EVENT_TYPE_CONNECT:
                {
                    std::cout << "A new client connected from "
                        << event.peer->address.host
                        << ":" << event.peer->address.port
                        << std::endl;
                    /* Store any relevant client information here. */
                    event.peer->data = (void*)("Client information");

                    NumPlayers++;

                    if (NumPlayers == 2)
                    {
                        BroadcastTextPacket(NetHost, "Hello Players!");
                        
                        BroadcastTextPacket(NetHost, "Let us play");
                        BroadcastTextPacket(NetHost, "a guessing game");

                        BroadcastTextPacket(NetHost, "I'm thinking");
                        BroadcastTextPacket(NetHost, "of a");
                        BroadcastTextPacket(NetHost, "number between");
                        BroadcastTextPacket(NetHost, "1 and 10");
                        BroadcastCanGuessPacket(NetHost, true);
                    }

                    break;
                }
                case ENET_EVENT_TYPE_RECEIVE:
                {
                    HandleReceivedPackets(event);
                    break;
                }
                case ENET_EVENT_TYPE_DISCONNECT:
                {
                    std::cout << (char*)event.peer->data << " disconnected." << std::endl;
                    /* Reset the peer's client information. */
                    event.peer->data = NULL;
                    //notify remaining player that the game is done due to player leaving
                    break;
                }
                default:
                {
                }
            }
        }
    }
}
