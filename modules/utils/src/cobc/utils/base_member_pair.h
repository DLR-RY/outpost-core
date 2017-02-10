/*
 * Copyright (c) 2017, German Aerospace Center (DLR)
 *
 * This file is part of libCOBC 0.6.
 *
 * It is distributed under the terms of the GNU General Public License with a
 * linking exception. See the file "LICENSE" for the full license governing
 * this code.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */
// ----------------------------------------------------------------------------

#ifndef COBC_UTILS_BASE_MEMBER_PAIR_H
#define COBC_UTILS_BASE_MEMBER_PAIR_H

namespace cobc
{
namespace utils
{

/**
 * Used to implement the EBCO (Empty Base Class Optimization) pattern.
 *
 * Merge two classes in the same address space if the base class is an
 * empty class.
 *
 * See the following article for more information:
 * http://www.informit.com/articles/article.aspx?p=31473&seqNum=2
 *
 * \author  Fabian Greif
 */
template<typename Base, typename Member>
class BaseMemberPair : private Base
{
public:
    BaseMemberPair(Base const & b, Member const & m) :
        Base(b),
        mMember(m)
    {
    }

    const Base&
    getBase() const
    {
        return reinterpret_cast<const Base&>(*this);
    }

    Base&
    getBase()
    {
        return reinterpret_cast<Base&>(*this);
    }

    const Member&
    getMember() const
    {
        return mMember;
    }

    Member&
    getMember()
    {
        return mMember;
    }

private:
    Member mMember;
};

}
}

#endif
