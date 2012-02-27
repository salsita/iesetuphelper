#pragma once

#include <Windows.h>
#include <Shlwapi.h>

// Macro for defining all exported functions - can be used to autogenerate setup script external definitions
#define EXPORTED(retVal, name, paramBody) extern "C" retVal __stdcall name paramBody