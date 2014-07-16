
#include "l3test.h"

#include "channel.h"

static const struct luaL_Reg arraylib_f[] = {
    { "channel", l3test_channel_new },
    { NULL, NULL }
};

extern "C"
int
luaopen_l3test(lua_State* L)
{
    l3test_channel_open(L);

    luaL_newlib(L, arraylib_f);
    return 1;
}
