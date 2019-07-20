#include "Precompiled.hpp"
#include "ReplicationCommand.hpp"

bool serialize(MemoryStream& stream, const ReplicationCommand& command)
{
    if(!serialize(stream, command.type))
        return false;

    if(!serialize(stream, command.handle))
        return false;

    if(!serialize(stream, command.data))
        return false;

    return true;
}

bool deserialize(MemoryStream& stream, ReplicationCommand* command)
{
    using enumType = std::underlying_type<ReplicationCommand::ReplicationType>::type;

    if(!deserialize(stream, (enumType*)&command->type))
        return false;

    if(!deserialize(stream, &command->handle))
        return false;

    if(!deserialize(stream, &command->data))
        return false;

    return true;
}
