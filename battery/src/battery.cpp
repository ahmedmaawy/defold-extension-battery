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

// C / C++ Native Functions

static int Percentage(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    AttachScope attachscope;
    JNIEnv* env = attachscope.m_Env;

    jclass cls = GetClass(env, "com.defold.Battery.Battery");
    jmethodID method = env->GetStaticMethodID(cls, "GetBatteryPct", "()Ljava/lang/Float;");
    
    jfloat return_value = (jfloat)env->CallStaticObjectMethod(cls, method);
    lua_pushnumber(L, return_value);
    env->DeleteLocalRef(return_value);
    return 1;
}

static int IsCharging(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    AttachScope attachscope;
    JNIEnv* env = attachscope.m_Env;

    jclass cls = GetClass(env, "com.defold.Battery.Battery");
    jmethodID method = env->GetStaticMethodID(cls, "IsBatteryCharging", "()Ljava/lang/Boolean;");
    
    jboolean return_value = (jboolean)env->CallStaticObjectMethod(cls, method);
    lua_pushboolean(L, return_value);
    env->DeleteLocalRef(return_value);
    return 1;
}

#endif

static int SelfCheck(lua_State* L)
{
    DM_LUA_STACK_CHECK(L, 1);
    lua_pushboolean(L, true);
    return 1;
}

// Functions exposed to Lua
static const luaL_reg Module_methods[] =
{
#if defined(DM_PLATFORM_ANDROID)
    {"percetage", Percentage},
    {"is_charging", IsCharging},
#endif
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
    dmLogInfo("AppInitializeMyExtension\n");
    return dmExtension::RESULT_OK;
}

dmExtension::Result InitializeMyExtension(dmExtension::Params* params)
{
    // Init Lua
    LuaInit(params->m_L);
    dmLogInfo("Registered %s Extension\n", MODULE_NAME);
    return dmExtension::RESULT_OK;
}

dmExtension::Result AppFinalizeMyExtension(dmExtension::AppParams* params)
{
    dmLogInfo("AppFinalizeMyExtension\n");
    return dmExtension::RESULT_OK;
}

dmExtension::Result FinalizeMyExtension(dmExtension::Params* params)
{
    dmLogInfo("FinalizeMyExtension\n");
    return dmExtension::RESULT_OK;
}

dmExtension::Result OnUpdateMyExtension(dmExtension::Params* params)
{
    dmLogInfo("OnUpdateMyExtension\n");
    return dmExtension::RESULT_OK;
}

void OnEventMyExtension(dmExtension::Params* params, const dmExtension::Event* event)
{
    switch(event->m_Event)
    {
        case dmExtension::EVENT_ID_ACTIVATEAPP:
            dmLogInfo("OnEventMyExtension - EVENT_ID_ACTIVATEAPP\n");
            break;
        case dmExtension::EVENT_ID_DEACTIVATEAPP:
            dmLogInfo("OnEventMyExtension - EVENT_ID_DEACTIVATEAPP\n");
            break;
        case dmExtension::EVENT_ID_ICONIFYAPP:
            dmLogInfo("OnEventMyExtension - EVENT_ID_ICONIFYAPP\n");
            break;
        case dmExtension::EVENT_ID_DEICONIFYAPP:
            dmLogInfo("OnEventMyExtension - EVENT_ID_DEICONIFYAPP\n");
            break;
        default:
            dmLogWarning("OnEventMyExtension - Unknown event id\n");
            break;
    }
}

DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, AppInitializeMyExtension, AppFinalizeMyExtension, InitializeMyExtension, OnUpdateMyExtension, OnEventMyExtension, FinalizeMyExtension)
