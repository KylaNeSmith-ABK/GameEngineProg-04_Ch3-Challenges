#include "Client.h"

Client::Client()
{
    NetHost = enet_host_create(NULL /* create a client host */,
        1 /* only allow 1 outgoing connection */,
        2 /* allow up 2 channels to be used, 0 and 1 */,
        0 /* assume any amount of incoming bandwidth */,
        0 /* assume any amount of outgoing bandwidth */);

    if (!NetHost)
    {
        fprintf(stderr,
            "An error occurred while trying to create an ENet client host.\n");
        exit(EXIT_FAILURE);
    }
}

Client::~Client()
{
    if (PacketThread)
    {
        PacketThread->join();
    }
    delete PacketThread;

    if (InputThread)
    {
        InputThread->join();
    }
    delete InputThread;

    if (NetHost != nullptr)
    {
        enet_host_destroy(NetHost);
    }
}

bool Client::AttemptConnectToServer()
{
    ENetAddress address;
    /* Connect to some.server.net:1234. */
    enet_address_set_host(&address, "127.0.0.1");
    address.port = 1234;
    /* Initiate the connection, allocating the two channels 0 and 1. */
    Peer = enet_host_connect(NetHost, &address, 2, 0);
    return Peer != nullptr;
}

void Client::SetUserName(std::string name)
{
    UserName = name;
}

void Client::Run()
{
    PacketThread = new std::thread([this] {ProcessPackets(); });
    InputThread = new std::thread([this] {ListenForInput(); });
}


void Client::HandleReceivedPackets(const ENetEvent& event)
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
                    std::cout << TPacket->text << std::endl;
                }

                break;
            }
            case PHT_CanGuess:
            {
                CanGuessPacket* CanPacket = (CanGuessPacket*)(event.packet->data);
                if (CanPacket)
                {
                    bCanGuess = CanPacket->canGuess;
                }
                
                break;
            }
            case PHT_GameOver:
            {
                std::cout << "Game Over" << std::endl;
                bRun = false;
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

void Client::ProcessPackets()
{
    while (bRun)
    {
        ENetEvent event;
        /* Wait up to 1000 milliseconds for an event. */
        while (enet_host_service(NetHost, &event, 1000) > 0)
        {
            switch (event.type)
            {
                case  ENET_EVENT_TYPE_CONNECT:
                {
                    std::cout << "Connection succeeded " << std::endl;
                    break;
                }
                case ENET_EVENT_TYPE_RECEIVE:
                {
                    HandleReceivedPackets(event);
                    break;
                }
            }
        }
    }
}

void Client::ListenForInput()
{
    while (bRun)
    {
        if (bCanGuess)
        {
            std::string Input;

            //std::cout << "Enter your guess: ";

            std::getline(std::cin, Input);
            Utils::ClearLine();
            if (!Input.empty())
            {
                bCanGuess = false;

                if ((Input == "q")
                    || (Input == "Q")
                    || (Input == "quit")
                    || (Input == "Quit"))
                {
                    bRun = false;
                    return;
                }

                BroadcastPlayerGuessPacket(NetHost, UserName, stoi(Input));
            }
        }
    }
}
