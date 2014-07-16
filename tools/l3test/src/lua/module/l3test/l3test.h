
#ifndef L3TEST_H
#define L3TEST_H

extern "C"
{
#include <lauxlib.h>

/**
 *
 *
 * \param L
 *      Lua State.
 *
 * \return
 */
int
luaopen_l3test(lua_State* L);
}

#endif // L3TEST_H
