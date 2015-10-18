#ifndef IB_ENGINE_DEFINES_H
#define IB_ENGINE_DEFINES_H
#pragma once

#ifndef NDEBUG
// Enable unit tests during debugging
#define IB_UNIT_TESTS
#endif

// Control heap behavior
#ifndef NDEBUG
#define IB_HEAP_DEBUG
#endif // NDEBUG

#ifdef IB_HEAP_DEBUG
#define IB_HEAP_CHECK
#define IB_HEAP_ENABLE_DUMP
#define IB_ALLOC_GUARDS
#define IB_MEMORY_FILL
#endif // IB_HEAP_DEBUG

// Enable logging and asserts
#define IB_ENABLE_LOG
#define IB_ASSERT_ENABLE

// Enable networking
#define IB_NETWORK

// Enable external telemetry
// #define IB_ENABLE_TELEMETRY
#ifdef IB_ENABLE_TELEMETRY
#define IB_TELEMETRY_SPACE (40 * 1024 * 1024)
#define IB_TELEMETRY_PORT 7131
#endif

// Global epsilon values
#define IB_EPSILON_F 0.000001f
#define IB_EPSILON_D 0.0000000001

// File system and limits
#define IB_MAX_FILE_SIZE (40 * 1024 * 1024)

// Platform configuration
#ifdef WIN32
#define IB_WINDOWS
#define IB_PLATFORM_HAS_CRITICAL_SECTION
#endif // WIN32

#endif // IB_ENGINE_DEFINES_H
