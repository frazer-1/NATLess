#pragma once
#include <string>
#include <vector>
#include <memory>

enum class CommandType : unsigned
{
    KeepAlive,
    ClientRegistryRequest,
    ClientRegistryResponse,
    ClientRoleDelegationRequest,
    ClientRoleDelegationResponse,
};

struct ClientRegistryRequestData
{
    enum ClientRegistryRequestDataImpl : unsigned
    {
        ClientName,
        Size
    };
};


class CommandFactory;

class Command
{
    friend CommandFactory;

public:
    using RawData_t = std::vector<char>;
    using RawDataSize_t = unsigned int;

public:
    Command(CommandType _commandType, RawDataSize_t _dataSize)
        : commandType(_commandType)
        , data(_dataSize, "")
    {
    }

    Command(const Command&) = default;

    ~Command() = default;

    std::string GetCommandName() const
    {
        switch (commandType)
        {
        case CommandType::KeepAlive:
            return "KeepAlive";
        case CommandType::ClientRegistryRequest:
            return "ClientRegistryRequest";
        case CommandType::ClientRegistryResponse:
            return "ClientRegistryResponse";
        case CommandType::ClientRoleDelegationRequest:
            return "ClientRoleDelegationRequest";
        case CommandType::ClientRoleDelegationResponse:
            return "ClientRoleDelegationResponse";
        default:
            return "Undefined";
        };
    }

    RawDataSize_t GetDataSize() const
    {
        return static_cast<RawDataSize_t>(data.size());
    }

    std::string GetDataByIndex(unsigned _index)
    {
        if (data.size() > _index)
            return data[_index];
        else
            throw std::logic_error("Invalid data index");
    }


    RawData_t ToBytes()
    {
        size_t size = sizeof(commandType) + sizeof(RawDataSize_t);
        for (size_t i = 0, sz = data.size(); i < sz; ++i)
        {
            size += sizeof(RawDataSize_t);
            size += data[i].size();
        }

        RawData_t rawData(size, 0);

        char* writeDataPosition = &rawData[0];
        memcpy(writeDataPosition, &commandType, sizeof(commandType));
        writeDataPosition += sizeof(commandType);

        RawDataSize_t dataSize = static_cast<RawDataSize_t>(data.size());
        memcpy(writeDataPosition, &dataSize, sizeof(dataSize));
        writeDataPosition += sizeof(dataSize);

        for (size_t i = 0, sz = data.size(); i < sz; ++i)
        {
            RawDataSize_t argSize = static_cast<RawDataSize_t>(data[i].size());
            memcpy(writeDataPosition, &argSize, sizeof(argSize));
            writeDataPosition += sizeof(argSize);
            if (argSize > 0)
            {
                memcpy(writeDataPosition, data[i].data(), data[i].size());
                writeDataPosition += data[i].size();
            }
        }

        return rawData;
    }
private:
    CommandType commandType;
    std::vector<std::string> data;
};

class CommandFactory
{
public:
    static Command CreateFromBytes(const Command::RawData_t& _rawData)
    {
        const char* readDataPosition = &_rawData[0];

        CommandType commandType;
        memcpy(&commandType,  + readDataPosition, sizeof(commandType));
        readDataPosition += sizeof(commandType);

        Command::RawDataSize_t dataSize = 0;
        memcpy(&dataSize, readDataPosition, sizeof(dataSize));
        readDataPosition += sizeof(dataSize);

        std::unique_ptr<Command> command(nullptr);
        switch (commandType)
        {
        case CommandType::KeepAlive:
            {
                if (dataSize == 0)
                    command = std::make_unique<Command>(CommandType::KeepAlive, 0);
                else
                    throw std::logic_error("KeepAlive command is invalid");
            }
            break;
        case CommandType::ClientRegistryRequest:
            {
                if (dataSize == ClientRegistryRequestData::Size)
                    command = std::make_unique<Command>(CommandType::ClientRegistryRequest, dataSize);
                else
                    throw std::logic_error("ClientRegistryRequest command is invalid");
            }
            break;
        case CommandType::ClientRegistryResponse:
        case CommandType::ClientRoleDelegationRequest:
        case CommandType::ClientRoleDelegationResponse:
        default:
            throw std::logic_error("Undefined command type");
            break;
        }

        for (size_t i = 0, sz = dataSize; i < sz; ++i)
        {
            Command::RawDataSize_t argSize = 0;
            memcpy(&argSize, readDataPosition, sizeof(argSize));
            readDataPosition += sizeof(argSize);
            if (argSize > 0)
            {
                command->data[i].resize(argSize);

                memcpy(&command->data[i][0], readDataPosition, argSize);
                readDataPosition += argSize;
            }
        }

        return *command;
    }

    static Command CreateRegistryCommand(std::string _clientName)
    {
        Command command(CommandType::ClientRegistryRequest, ClientRegistryRequestData::Size);
        command.data[ClientRegistryRequestData::ClientName] = _clientName;
        return command;
    }

};


