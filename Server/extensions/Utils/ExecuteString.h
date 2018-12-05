#ifndef __EXECUTE_STRING__
#define __EXECUTE_STRING__

#include <angelscript.h>

extern int ExecuteString( asIScriptEngine* engine, const char* code );
extern int ExecuteString( asIScriptEngine* engine, const char* code, asIScriptModule* mod, asIScriptContext* ctx );
extern int ExecuteString( asIScriptEngine* engine, const char* code, void* ref, int refTypeId, asIScriptModule* mod, asIScriptContext* ctx );

#endif // __EXECUTE_STRING__ //
