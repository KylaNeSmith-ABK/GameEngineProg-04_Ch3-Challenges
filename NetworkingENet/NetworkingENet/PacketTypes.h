#pragma once

#include <iostream>

enum PacketHeaderTypes
{
    PHT_Invalid = 0,
    PHT_IsDead,
    PHT_Position,
    PHT_Count,
    PHT_Text,
    PHT_CanGuess,
    PHT_PlayerGuess,
    PHT_GameOver
};

struct GamePacket
{
    GamePacket() {}
    PacketHeaderTypes Type = PHT_Invalid;
    std::string userName = "ERROR";
};

struct IsDeadPacket : public GamePacket
{
    IsDeadPacket()
    {
        Type = PHT_IsDead;
    }

    int playerId = 0;
    bool IsDead = false;
};

struct PositionPacket : public GamePacket
{
    PositionPacket()
    {
        Type = PHT_Position;
    }

    int playerId = 0;
    int x = 0;
    int y = 0;
};

struct TextPacket : public GamePacket
{
    TextPacket()
    {
        Type = PHT_Text;
    }

    int playerId = 0;
    std::string text = "";
};

struct CanGuessPacket : public GamePacket
{
    CanGuessPacket()
    {
        Type = PHT_CanGuess;
    }

    int playerId = 0;
    bool canGuess = false;
};

struct PlayerGuessPacket : public GamePacket
{
    PlayerGuessPacket()
    {
        Type = PHT_PlayerGuess;
    }

    int playerId = 0;
    int playerGuess = 0;
};

struct GameOverPacket : public GamePacket
{
    GameOverPacket()
    {
        Type = PHT_GameOver;
    }

    int playerId = 0;
};