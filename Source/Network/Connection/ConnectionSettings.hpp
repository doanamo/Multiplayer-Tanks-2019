#pragma once

#include "Precompiled.hpp"

namespace ConnectionSettings
{
    // Maximum packet data size, excluding the header, that we know will not cause datagram to become fragmented.
    // This is used purely for a warning, until SPMD and MPSD protocols are implemented.
    // SPMD - Single Packet Multiple Datagrams / MPSD - Multiple Packets Single Datagram
    constexpr uint32_t MaxPacketDataSize = 1400;

    // Maximum number of oldest unacknowledged packets that can be resent again.
    // This is in case there is a disruption for prolonged amount of time which would result
    // in congestion when attempting to resend all unacknowledged packets at the same time.
    constexpr uint32_t MaxReliableResendCount = 16;

    // Minimum and maximum packet resend wait time, depending on queue size.
    // Time we should give remote to acknowledge reliable packets before we resend them.
    // High reliable resend count is used to determine which minimum/maximum wait time will be used.
    constexpr float MinimumReliableResendDelayMs = 100.0f;
    constexpr float MaximumReliableResendDelayMs = 1000.0f;
    constexpr uint32_t HighReliableResendCount = 256;

    // Maximum number of received packets to process in single iteration loop.
    // This is done to prevent infinite loops when sender keeps spamming us with packets faster than we can process them.
    constexpr uint32_t WorkerMaxReceivePacketBatch = 128;

    // Maximum rate at which we can spin the worker thread to prevent consuming all CPU resources.
    constexpr uint32_t WorkerMaxIterationRate = 120;
}
