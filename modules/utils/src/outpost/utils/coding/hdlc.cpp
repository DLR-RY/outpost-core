/*
 * Copyright (c) 2021, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2021, Felix Passenberg (DLR RY-AVS)
 */

#include "hdlc.h"

namespace outpost
{
namespace utils
{
// ----------------------------------------------------------------------------
size_t
HdlcStuffing::encode(outpost::Slice<const uint8_t> const& input, outpost::Slice<uint8_t>& output)
{
    if (output.getNumberOfElements() < (input.getNumberOfElements() + boundary_overhead))
    {
        // At least two additional boundary bytes are required.
        // size is checked during encoding, allowing more compact memory, but might fail later.
        return 0;
    }
    if (input.getDataPointer() == output.getDataPointer())
    {
        // encoding enlarges the data, thus it cannot be done in place.
        return 0;
    }

    auto input_it = input.begin();
    auto output_it = output.begin();

    *output_it = boundary_byte;
    output_it++;

    while ((input_it != input.end()) && (output_it != output.end()))
    {
        uint8_t data = *input_it;
        if ((data == boundary_byte) || (data == escape_byte))
        {
            // all special characters are escaped
            *output_it = escape_byte;
            output_it++;
            if (output_it == output.end())
            {
                // happens if output is too small and input has lots of special bytes
                return 0;
            }
            data ^= mask;
        }
        *output_it = data;

        input_it++;
        output_it++;
    }
    if (output_it == output.end())
    {
        // happens if output is too small and input has lots of special bytes
        return 0;
    }
    *output_it = boundary_byte;
    output_it++;

    size_t out_length = std::distance(output.begin(), output_it);
    output = output.first(out_length);
    return out_length;
}

size_t
HdlcStuffing::decode(outpost::Slice<const uint8_t> const& input, outpost::Slice<uint8_t>& output)
{
    auto input_it = input.begin();
    auto input_frame_start = input_it;  // returned if no end marker is found
    auto output_it = output.begin();

    bool escaped = false;
    bool inframe = false;
    while (input_it != input.end())
    {
        uint8_t data = *input_it;
        if (data == boundary_byte)
        {
            if (inframe)
            {
                if (escaped)
                {
                    // escaped boundary bytes are aborts
                    output = output.first(0);
                }
                else
                {
                    output = output.first(std::distance(output.begin(), output_it));
                }
                // frame marker of abort might still be the start of the next frame, so the
                // aborted frame can be skipped
                return std::distance(input.begin(), input_it);
            }
            else
            {
                // start of a frame
                input_frame_start = input_it;
                output_it = output.begin();
                inframe = true;
            }
        }
        else if (data == escape_byte)
        {
            if (escaped)
            {
                // two escape bytes following each other are invalid
                output = output.first(0);
                return std::distance(input.begin(), input_it);
            }
            if (inframe)
            {
                // next byte is escaped, only useful if in frame
                escaped = true;
            }
        }
        else
        {
            if (inframe)
            {
                if (output_it != output.end())
                {
                    *output_it = (!escaped) ? data : (data ^ mask);
                    output_it++;
                }
                else
                {
                    // end of output stream reached without an end frame boundary
                    output = output.first(0);
                    // as no end frame marker was found: return distance to frame start marker to
                    // allow cutting
                    return std::distance(input.begin(), input_frame_start);
                }
            }
            escaped = false;  // the next byte is not escaped
        }
        input_it++;
    }

    // end of the input reached without an end frame boundary
    output = output.first(0);
    // as no end frame marker was found: return distance to frame start marker to allow cutting
    return std::distance(input.begin(), input_frame_start);
}

}  // namespace utils
}  // namespace outpost
