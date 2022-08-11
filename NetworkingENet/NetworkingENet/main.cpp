#include <enet/enet.h>
#include <string>
#include <iostream>
#include <thread>

#include "PacketTypes.h"
#include "BroadcastingPackets.h"
#include "Server.h"
#include "Client.h"

using namespace std;

int main(int argc, char** argv)
{
    if (enet_initialize() != 0)
    {
        fprintf(stderr, "An error occurred while initializing ENet.\n");
        cout << "An error occurred while initializing ENet." << endl;
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    cout << "1) Create Server " << endl;
    cout << "2) Create Client " << endl;
    int UserInput;
    cin >> UserInput;

    switch (UserInput)
    {
        case 1:
        {
            //How many players?

            Server* server = new Server();

            server->Run();
            
            while(!server->HasStopped()){ }
            break;
        }
        case 2:
        {
            Client* client = new Client();

            if (!client->AttemptConnectToServer())
            {
                fprintf(stderr,
                    "No available peers for initiating an ENet connection.\n");
                exit(EXIT_FAILURE);
            }

            std::cout << "Enter name: ";
            string name;
            while (name.empty())
            {
                getline(std::cin, name);
            }
            client->SetUserName(name);

            client->Run();

            while (!client->HasQuit()) {}
            break;
        }
        default:
        {
            cout << "Invalid Input" << endl;
        }
    }

    cout << "...exiting..." << endl;
    
    return EXIT_SUCCESS;
}