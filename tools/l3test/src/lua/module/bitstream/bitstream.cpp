
#include "bitstream.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <limits.h>
#include <stdint.h>

static const unsigned int bitsPerWord = 8;

struct Bitstream
{
	int size;

	// used as a placeholder for the complete array
	uint8_t values[1];
};

static size_t
numberOfBytesForCompleteObject(size_t n)
{
	const int numberOfWords = (n - 1) / bitsPerWord + 1;	// round up

	// Bitstream already contains one entry, therefore needs one byte less in
	// the attached storage area
	return sizeof(Bitstream) + (numberOfWords - 1) * sizeof(uint8_t);
}

/**
 * Calculate the number of words needed to store the given count of bits.
 */
static int
numberOfWords(size_t n)
{
	if (n == 0) {
		return 0;
	}
	else {
		return (n - 1) / bitsPerWord + 1;
	}
}

// ----------------------------------------------------------------------------
/**
 * Create a new bitstream object.
 */
static int
l_bitstream_new(lua_State* L)
{
    int type = lua_type(L, 1);
    if (type == LUA_TNUMBER)
    {
        int n = luaL_checkint(L, 1);
        luaL_argcheck(L, n >= 0, 1, "invalid size");

        size_t nbytes = numberOfBytesForCompleteObject(n);

        Bitstream** b = (Bitstream **) lua_newuserdata(L, sizeof(Bitstream *));
        *b = (Bitstream *) malloc(nbytes);

        // Initialize content to zero
        (*b)->size = n;
        for (int i = 0; i < numberOfWords(n); ++i) {
            (*b)->values[i] = 0;
        }
    }
    else if (type == LUA_TSTRING)
    {
        const char* s = luaL_checkstring(L, 1);
        int string_length = luaL_len(L, 1);

        size_t nbytes = sizeof(Bitstream) - sizeof(uint8_t) + string_length;

        Bitstream** b = (Bitstream **) lua_newuserdata(L, sizeof(Bitstream *));
        *b = (Bitstream *) malloc(nbytes);

        // Initialize content to zero
        (*b)->size = string_length * 8;
        for (int i = 0; i < string_length; ++i) {
            (*b)->values[i] = s[i];
        }
    }
    else {
        luaL_argcheck(L, false, 2, "Invalid type");
    }


	luaL_getmetatable(L, "dlr.bitstream");
	lua_setmetatable(L, -2);

	return 1;
}

// ----------------------------------------------------------------------------
/**
 *
 * \param       L
 *     Lua interpreter state
 * \param[out]  mask
 *     Bitmask
 */
static uint8_t*
getindex(lua_State* L, uint8_t* mask)
{
	Bitstream** b = (Bitstream **) luaL_checkudata(L, 1, "dlr.bitstream");

	int index = luaL_checkint(L, 2) - 1;
	luaL_argcheck(L, (0 <= index) && (index < (*b)->size), 2, "index out of range");

	div_t d = div(index, 8);
	*mask = (1 << (7 - d.rem));

	return &(*b)->values[d.quot];
}

/**
 * Set a single bit.
 *
 *   bitstream:set(index, value)
 */
static int
l_bitstream_set(lua_State* L)
{
	uint8_t mask;
	uint8_t* entry = getindex(L, &mask);

	luaL_checkany(L, 3);
	bool value;
	if (lua_type(L, 3) == LUA_TNUMBER) {
		value = (lua_tointeger(L, 3) > 0);
	}
	else {
		value = lua_toboolean(L, 3);
	}

	if (value) {
		*entry |= mask;
	}
	else {
		*entry &= ~mask;
	}

	return 0;
}

/**
 * Read a single bit.
 *
 *   value = bitstream:get(index)
 */
static int
l_bitstream_get(lua_State* L)
{
	uint8_t mask;
	uint8_t* entry = getindex(L, &mask);

	lua_pushinteger(L, (*entry & mask) ? 1 : 0);

	return 1;
}

// ----------------------------------------------------------------------------
static Bitstream* 
check_and_get_field_arguments(lua_State* L, int& startPos, int& width)
{
	Bitstream** b = (Bitstream **) luaL_checkudata(L, 1, "dlr.bitstream");

	startPos = luaL_checkint(L, 2) - 1;
	luaL_argcheck(L, (startPos >= 0) && (startPos < (*b)->size), 2, "index out of range");

	width = luaL_checkint(L, 3);
	luaL_argcheck(L, (width >= 0) && (width < 32), 3, "invalid word width");
	luaL_argcheck(L, (startPos + width) <= (*b)->size, 3, "word access out of range");

	return (*b);
}

static inline void
set_bit(uint8_t* field, size_t index, bool bit)
{
	div_t d = div(index, 8);
	uint8_t mask = (1 << (7 - d.rem));

	if (bit) {
		field[d.quot] |= mask;
	}
	else {
		field[d.quot] &= ~mask;
	}
}

static inline bool
get_bit(uint8_t* field, size_t index)
{
	div_t d = div(index, 8);
	uint8_t mask = (1 << (7 - d.rem));

	return (field[d.quot] & mask);
}

static int
l_bitstream_insert(lua_State* L)
{
	int startPos;
	int width;

	Bitstream* a = check_and_get_field_arguments(L, startPos, width);

	uint32_t value = luaL_checkint(L, 4);
	uint32_t mask = 1 << (width - 1);

	for (uint_fast8_t i = 0; i < width; ++i) {
		set_bit(a->values, startPos + i, (mask & value));
		mask >>= 1;
	}

	return 0;
}

// ----------------------------------------------------------------------------
static Bitstream*
concat(lua_State* L, Bitstream** own)
{
    Bitstream* field = 0;

    int type = lua_type(L, 2);
    if (type == LUA_TUSERDATA)
    {
        Bitstream** other = (Bitstream **) luaL_checkudata(L, 2, "dlr.bitstream");

        int new_bitsize = (*own)->size + (*other)->size;
        int nbytes = numberOfBytesForCompleteObject(new_bitsize);

        field = (Bitstream*) malloc(nbytes);

        // copy first bitfield
        memcpy(field, *own, numberOfBytesForCompleteObject((*own)->size));

        // adjust size
        field->size = new_bitsize;

        // add second bitfield
        // FIXME provide a optimized version of this
        int offset = (*own)->size;
        for (int i = 0; i < (*other)->size; ++i) {
            set_bit(field->values, i + offset, get_bit((*other)->values, i));
        }
    }
    else if (type == LUA_TSTRING)
    {
        div_t d = div((*own)->size, 8);
        if (d.rem != 0) {
            luaL_error(L, "bitstream size (here: %d) must be divisible by 8", (*own)->size);
        }

        const char* s = luaL_checkstring(L, 2);
        int string_length = luaL_len(L, 2);

        int old_size = sizeof(Bitstream) - sizeof(uint8_t) + d.quot;
        int new_size = old_size + string_length;

        field = (Bitstream*) malloc(new_size);

        // copy first bitfield
        memcpy(field, *own, old_size);

        // adjust (bit) size
        field->size = (d.quot + string_length) * 8;

        // add string
        memcpy(&field->values[d.quot], s, string_length);
    }
    else {
        luaL_argcheck(L, false, 2, "Invalid type");
    }

    return field;
}

/**
 * Append one bitstream to another.
 *
 * \return  Returns the original bitstream
 */
static int
l_bitstream_concat(lua_State* L)
{
    Bitstream** own   = (Bitstream **) luaL_checkudata(L, 1, "dlr.bitstream");

    Bitstream* field = concat(L, own);

    // generate a new Lua bitstream object
    Bitstream** b = (Bitstream **) lua_newuserdata(L, sizeof(Bitstream *));
    *b = field;

    // Set metatable for the new bitstream
    luaL_getmetatable(L, "dlr.bitstream");
    lua_setmetatable(L, -2);

    return 1;
}

/**
 * Concat two bitstream.
 *
 * \return  Returns a new bitstream
 */
static int
l_bitstream_append(lua_State* L)
{
    Bitstream** own   = (Bitstream **) luaL_checkudata(L, 1, "dlr.bitstream");

    Bitstream* field = concat(L, own);

    // adjust pointer to point to the new array and free original memory
    free(*own);
    *own = field;

    lua_settop(L, 1);
    return 1;
}

// ----------------------------------------------------------------------------
static int
l_bitstream_extract(lua_State* L)
{
	int startPos;
	int width;

	Bitstream* a = check_and_get_field_arguments(L, startPos, width);

	int32_t value = 0;
	for (uint_fast8_t i = 0; i < width; ++i) {
		value <<= 1;
		if (get_bit(a->values, startPos + i)) {
			value |= 1;
		}
	}

	lua_pushinteger(L, value);
	return 1;
}

// ----------------------------------------------------------------------------
/**
 * Get the length in bits.
 */
static int
l_bitstream_bit_length(lua_State* L)
{
	Bitstream** b = (Bitstream **) luaL_checkudata(L, 1, "dlr.bitstream");

	lua_pushinteger(L, (*b)->size);
	return 1;
}

// ----------------------------------------------------------------------------
/**
 * Get the length in bytes.
 *
 * \warning May fail if the length is not divisible by eight.
 */
static int
l_bitstream_length(lua_State* L)
{
    Bitstream** b = (Bitstream **) luaL_checkudata(L, 1, "dlr.bitstream");

    div_t d = div((*b)->size, 8);
    if (d.rem != 0) {
        luaL_error(L, "bitstream size (here: %d) must be divisible by 8", (*b)->size);
    }

    lua_pushinteger(L, d.quot);
    return 1;
}

// ----------------------------------------------------------------------------
/**
 * Create a copy of a bitstream.
 */
static int
l_bitstream_copy(lua_State* L)
{
	Bitstream** orig = (Bitstream **) luaL_checkudata(L, 1, "dlr.bitstream");

	size_t nbytes = numberOfBytesForCompleteObject((*orig)->size);

	// create a new user data object for the new bitstream
	Bitstream** other = (Bitstream **) lua_newuserdata(L, sizeof(Bitstream *));
	*other = (Bitstream *) malloc(nbytes);

	// Copy content
	(*other)->size = (*orig)->size;
	memcpy((*other)->values, (*orig)->values, numberOfWords((*orig)->size));

	luaL_getmetatable(L, "dlr.bitstream");
	lua_setmetatable(L, -2);

	return 1;
}

// ----------------------------------------------------------------------------
/**
 * Create a string of the binary representation.
 */
static int
l_bitstream_to_binary(lua_State* L)
{
	Bitstream** b = (Bitstream **) luaL_checkudata(L, 1, "dlr.bitstream");

	div_t d = div((*b)->size, 8);

	// 9 characters per byte
	int length = d.quot * 9 + d.rem + 1;
	char* s = new char[length];

	uint32_t index = 0;
	for (int i = 0; i < (*b)->size; ++i) {
		s[index++] = get_bit((*b)->values, i) ? '1' : '0';

		if (i % 8 == 7) {
			s[index++] = ' ';
		}
	}
	s[index++] = '\0';

	lua_pushstring(L, s);

	delete [] s;
	return 1;
}

// ----------------------------------------------------------------------------
/**
 * Create a table with an entry per byte of the bitstream.
 *
 * Works only for bitstreams whose size is divisible by eight.
 */
static int
l_bitstream_bytes(lua_State* L)
{
	Bitstream** b = (Bitstream **) luaL_checkudata(L, 1, "dlr.bitstream");

	div_t d = div((*b)->size, 8);
	if (d.rem != 0) {
		luaL_error(L, "bitstream size (here: %d) must be divisible by 8", (*b)->size);
	}

	// create a new table with the matching number of entries.
	lua_createtable(L, d.quot, 0);

	for (int i = 0; i < d.quot; ++i) {
		lua_pushinteger(L, (*b)->values[i]);
		lua_rawseti(L, -2, i+1);
	}

	return 1;
}

// ----------------------------------------------------------------------------
/**
 * Create a string representation.
 */
static int
l_bitstream_representation(lua_State* L)
{
	Bitstream** b = (Bitstream **) luaL_checkudata(L, 1, "dlr.bitstream");
	lua_pushfstring(L, "bitstream(%d)", (*b)->size);
	return 1;
}

// ----------------------------------------------------------------------------
static int
l_bitstream_gc(lua_State* L)
{
	Bitstream** b = (Bitstream **) luaL_checkudata(L, 1, "dlr.bitstream");

	free(*b);
	*b = 0;

	return 0;
}

// ----------------------------------------------------------------------------
static const struct luaL_Reg arraylib_f[] = {
	{ "new", l_bitstream_new },
	{ NULL, NULL }
};

static const struct luaL_Reg arraylib_m[] = {
	{ "set", l_bitstream_set },
	{ "get", l_bitstream_get },
	{ "insert", l_bitstream_insert },
	{ "append", l_bitstream_append },
	{ "extract", l_bitstream_extract },
	{ "bit_length", l_bitstream_bit_length },
	{ "length", l_bitstream_length },
	{ "copy", l_bitstream_copy },
	{ "to_binary", l_bitstream_to_binary },
	{ "bytes", l_bitstream_bytes },

	// TODO why doesn't this work?
	//{ "__newindex", l_bitstream_set },
	//{ "__index", l_bitstream_get },
	{ "__len", l_bitstream_length },
	{ "__tostring", l_bitstream_representation },
	{ "__concat", l_bitstream_concat },
	{ "__gc", l_bitstream_gc },

	{ NULL, NULL },
};

// ----------------------------------------------------------------------------
extern "C"
int
luaopen_bitstream(lua_State* L)
{
	luaL_newmetatable(L, "dlr.bitstream");

	// metatable.__index = metatable
	lua_pushvalue(L, -1);	// duplicate the metatable
	lua_setfield(L, -2, "__index");

	luaL_setfuncs(L, arraylib_m, 0);
	luaL_newlib(L, arraylib_f);
	return 1;
}

