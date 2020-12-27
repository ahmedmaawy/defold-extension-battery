// myextension.cpp
// Extension lib defines
#define EXTENSION_NAME Battery
#define LIB_NAME "Battery"
#define MODULE_NAME "battery"

// include the Defold SDK
#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_ANDROID)

// Android Access Functions

static JNIEnv* Attach()
{
    JNIEnv* env;
    JavaVM* vm = dmGraphics::GetNativeAndroidJavaVM();
    vm->AttachCurrentThread(&env, NULL);
    return env;
}

static bool Detach(JNIEnv* env)
{
    bool exception = (bool) env->ExceptionCheck();
    env->ExceptionClear();
    JavaVM* vm = dmGraphics::GetNativeAndroidJavaVM();
    vm->DetachCurrentThread();
    return !exception;
}

namespace {
struct AttachScope
{
    JNIEnv* m_Env;
    AttachScope() : m_Env(Attach())
    {
    }
    ~AttachScope()
    {
        Detach(m_Env);
    }
};
}

#endif

// C / C++ Native Functions

static int Percentage(lua_State* L)
{
#if defined(DM_PLATFORM_ANDROID)
    DM_LUA_STACK_CHECK(L, 1);
    AttachScope attachscope;
    JNIEnv* env = attachscope.m_Env;

    jclass cls = GetClass(env, "com.defold.Battery.Battery");
    jmethodID method = env->GetStaticMethodID(cls, "GetBatteryPct", "()Ljava/lang/Float;");
    
    jfloat return_value = (jfloat)env->CallStaticObjectMethod(cls, method);
    lua_pushnumber(L, return_value);
    env->DeleteLocalRef(return_value);
#else
    lua_pushnumber(L, 1.0);  
#endif
    return 1;
}

static int IsCharging(lua_State* L)
{
#if defined(DM_PLATFORM_ANDROID)
    DM_LUA_STACK_CHECK(L, 1);
    AttachScope attachscope;
    JNIEnv* env = attachscope.m_Env;

    jclass cls = GetClass(env, "com.defold.Battery.Battery");
    jmethodID method = env->GetStaticMethodID(cls, "IsBatteryCharging", "()Ljava/lang/Boolean;");
    
    jboolean return_value = (jboolean)env->CallStaticObjectMethod(cls, method);
    lua_pushboolean(L, return_value);
    env->DeleteLocalRef(return_value);
#else
    lua_pushboolean(L, false);   
#endif
    return 1;
}

static int SelfCheck(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    lua_pushboolean(L, true);
    return 1;
}

// Functions exposed to Lua
static const luaL_reg Module_methods[] =
{
    {"percetage", Percentage},
    {"is_charging", IsCharging},
    {"self_check", SelfCheck},
    {0, 0}
};

static void LuaInit(lua_State* L)
{
    int top = lua_gettop(L);

    // Register lua names
    luaL_register(L, MODULE_NAME, Module_methods);

    lua_pop(L, 1);
    assert(top == lua_gettop(L));
}

// Defold Extension Lifeline

dmExtension::Result AppInitializeMyExtension(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

dmExtension::Result InitializeMyExtension(dmExtension::Params* params)
{
    // Init Lua
    LuaInit(params->m_L);
    return dmExtension::RESULT_OK;
}

dmExtension::Result AppFinalizeMyExtension(dmExtension::AppParams* params)
{
    return dmExtension::RESULT_OK;
}

dmExtension::Result FinalizeMyExtension(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

dmExtension::Result OnUpdateMyExtension(dmExtension::Params* params)
{
    return dmExtension::RESULT_OK;
}

void OnEventMyExtension(dmExtension::Params* params, const dmExtension::Event* event)
{
    switch(event->m_Event)
    {
        case dmExtension::EVENT_ID_ACTIVATEAPP:
            break;
        case dmExtension::EVENT_ID_DEACTIVATEAPP:
            break;
        case dmExtension::EVENT_ID_ICONIFYAPP:
            break;
        case dmExtension::EVENT_ID_DEICONIFYAPP:
            break;
        default:
            break;
    }
}

DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, AppInitializeMyExtension, AppFinalizeMyExtension, InitializeMyExtension, OnUpdateMyExtension, OnEventMyExtension, FinalizeMyExtension)
