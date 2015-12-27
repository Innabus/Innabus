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

// Rendering defines
#ifdef IB_WINDOWS
#define IB_RENDER_DX11
#endif // IB_WINDOWS

//#define IB_RENDER_PREVENT_THREADING
#define IB_RENDER_PASS_SET_DEFAULT_STATE
#define IB_RENDER_DEBUG
#ifdef IB_RENDER_DEBUG
#define IB_RENDER_DEBUG_CLEAR
#define IB_RENDER_PASS_SET_DEFAULT_STATE
#define IB_RENDER_PASS_VERIFY_STATE
#define IB_RENDER_PASS_VERIFY_STATE_PER_OBJECT
#define IB_RENDER_VALIDATE_BOUND_RENDER_TARGET_VEIWS
#endif // IB_RENDER_DEBUG
#define IB_MAX_RENDER_TARGET_DESCS 8
#define IB_RENDER_COMMAND_LIST_SIZE (1024 * 1024)

#ifndef NDEBUG
//#define IB_TRACK_COM_REFERENCES
#endif
#endif // WIN32

#endif // IB_ENGINE_DEFINES_H
