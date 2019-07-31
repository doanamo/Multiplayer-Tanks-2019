#pragma once

#include "Precompiled.hpp"

namespace ConnectionSettings
{
    // Maximum packet data size, excluding the header, that we know will not cause datagram to become fragmented.
    // This is used purely for a warning, until SPMD and MPSD protocols are implemented.
    // SPMD - Single Packet Multiple Datagrams / MPSD - Multiple Packets Single Datagram
    constexpr uint32_t MaxPacketDataSize = 1400;

    // Maximum number of oldest unacknowledged packets that can be resend again.
    // This is in case there is a disruption for prolonged amount of time which would result
    // in congestion when attempting to resend all unacknowledged packets at the same time.
    constexpr uint32_t MaxReliableResendCount = 16;

    // Maximum number of received packets to process in single iteration loop.
    // This is done to prevent infinite loops when sender keeps spamming us with packets faster than we can process them.
    constexpr uint32_t WorkerMaxReceivePacketBatch = 100;

    // Maximum rate at which we can spin the worker thread to prevent consuming all CPU resources.
    constexpr uint32_t WorkerMaxIterationRate = 120;
}

// Log trace macro wrappers.
// These affects only debug, as trace logging is not enabled in release.
#define ENABLE_CONNECTION_CONTEXT_LOG_TRACE true
#define ENABLE_CONNECTION_BACKEND_LOG_TRACE true

#if ENABLE_CONNECTION_CONTEXT_LOG_TRACE
    #define LOG_CONNECTION_CONTEXT_TRACE(format, ...) LOG_TRACE(format, ## __VA_ARGS__)
#else
    #define LOG_CONNECTION_CONTEXT_TRACE(format, ...)
#endif

#if ENABLE_CONNECTION_BACKEND_LOG_TRACE
    #define LOG_CONNECTION_BACKEND_TRACE(format, ...) LOG_TRACE(format, ## __VA_ARGS__)
#else
    #define LOG_CONNECTION_BACKEND_TRACE(format, ...)
#endif

