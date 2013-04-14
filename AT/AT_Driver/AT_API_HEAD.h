#pragma once
#ifdef AT_LIB 
#define DLL_API  extern "C" __declspec(dllexport)
#else
#define DLL_API extern "C" __declspec(dllimport)
#endif