/*
 * Copyright (c) 2013-2018, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2020, Jan Malburg (DLR RY-AVS)
 */

#include "file_system_stub.h"

#include <outpost/time/time_epoch.h>
#include <outpost/utils/minmax.h>

#include <string.h>

using namespace outpost::hal;
using namespace std;
using outpost::hal::FileSystem;

namespace unittest
{
namespace hal
{
FileSystem::Result
FileSystemStub::mount(bool readOnly)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (mMounted)
    {
        return FileSystem::Result::invalidState;
    }
    else
    {
        mMounted = true;
        mReadOnly = readOnly;
        return FileSystem::Result::success;
    }
}

FileSystem::Result
FileSystemStub::unmount()
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (!mMounted)
    {
        return FileSystem::Result::invalidState;
    }
    else
    {
        mMounted = false;
        return FileSystem::Result::success;
    }
}

FileSystem::Result
FileSystemStub::mkDir(const char* fullPath, Permission mask)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (nullptr == fullPath)
    {
        return FileSystem::Result::invalidInput;
    }
    std::string path(fullPath);
    if (!mMounted)
    {
        return FileSystem::Result::invalidState;
    }

    if (mReadOnly)
    {
        return FileSystem::Result::readOnly;
    }

    Entry* parent = getParent(fullPath);
    if (nullptr == parent)
    {
        return FileSystem::Result::notFound;
    }

    if (parent->permission & W)
    {
        size_t pos = path.find_last_of('/');
        std::string filename = path.substr(pos + 1);
        if (getEntryInFolder(*parent, filename) != nullptr)
        {
            return FileSystem::Result::alreadyExists;
        }
        Entry toAdd;
        toAdd.isFile = false;
        toAdd.name = filename;
        toAdd.permission = mask;
        toAdd.directoryEntries.clear();
        toAdd.fileData.clear();
        toAdd.creation =
                outpost::time::TimeEpochConverter<outpost::time::SpacecraftElapsedTimeEpoch,
                                                  outpost::time::GpsEpoch>::convert(mClock.now());
        toAdd.modify =
                outpost::time::TimeEpochConverter<outpost::time::SpacecraftElapsedTimeEpoch,
                                                  outpost::time::GpsEpoch>::convert(mClock.now());

        parent->directoryEntries.push_back(toAdd);
        parent->modify =
                outpost::time::TimeEpochConverter<outpost::time::SpacecraftElapsedTimeEpoch,
                                                  outpost::time::GpsEpoch>::convert(mClock.now());
        return FileSystem::Result::success;
    }
    else
    {
        return FileSystem::Result::accessDenied;
    }
}

FileSystem::Result
FileSystemStub::createFile(const char* fullPath, Permission permission)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (nullptr == fullPath)
    {
        return FileSystem::Result::invalidInput;
    }
    std::string path(fullPath);
    if (!mMounted)
    {
        return FileSystem::Result::invalidState;
    }

    if (mReadOnly)
    {
        return FileSystem::Result::readOnly;
    }

    Entry* parent = getParent(fullPath);
    if (nullptr == parent)
    {
        return FileSystem::Result::notFound;
    }

    if (parent->permission & W)
    {
        size_t pos = path.find_last_of('/');
        std::string filename = path.substr(pos + 1);
        if (getEntryInFolder(*parent, filename) != nullptr)
        {
            return FileSystem::Result::alreadyExists;
        }
        Entry toAdd;
        toAdd.isFile = true;
        toAdd.name = filename;
        toAdd.permission = permission;
        toAdd.directoryEntries.clear();
        toAdd.fileData.clear();
        toAdd.creation =
                outpost::time::TimeEpochConverter<outpost::time::SpacecraftElapsedTimeEpoch,
                                                  outpost::time::GpsEpoch>::convert(mClock.now());
        toAdd.modify =
                outpost::time::TimeEpochConverter<outpost::time::SpacecraftElapsedTimeEpoch,
                                                  outpost::time::GpsEpoch>::convert(mClock.now());

        parent->directoryEntries.push_back(toAdd);
        parent->modify =
                outpost::time::TimeEpochConverter<outpost::time::SpacecraftElapsedTimeEpoch,
                                                  outpost::time::GpsEpoch>::convert(mClock.now());
        return FileSystem::Result::success;
    }
    else
    {
        return FileSystem::Result::accessDenied;
    }
}

FileSystem::Result
FileSystemStub::openDir(const char* path, Directory& folder)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (nullptr == path)
    {
        return FileSystem::Result::invalidInput;
    }
    if (!mMounted)
    {
        return FileSystem::Result::invalidState;
    }

    Entry* ele = getEntry(path);

    if (nullptr == ele)
    {
        return FileSystem::Result::notFound;
    }

    // fix counting
    mCountToForce++;
    return openDir(ele, folder);
}

FileSystem::Result
FileSystemStub::openDir(DirectoryEntry& entry, Directory& folder)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (!mMounted)
    {
        return FileSystem::Result::invalidState;
    }

    if (entry.data == 0)
    {
        return FileSystem::Result::invalidInput;
    }

    Entry* ele = reinterpret_cast<Entry*>(entry.data);

    // fix counting
    mCountToForce++;
    return openDir(ele, folder);
}

FileSystem::Result
FileSystemStub::openDir(Entry* entry, Directory& folder)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (nullptr == entry)
    {
        return FileSystem::Result::invalidInput;
    }
    if (folder.data != 0)
    {
        return FileSystem::Result::invalidInput;
    }

    if (entry->isFile)
    {
        return FileSystem::Result::notADirectory;
    }

    DirectoryStub* value = new DirectoryStub;
    value->directory = entry;
    value->iterator = value->directory->directoryEntries.begin();

    folder.data = reinterpret_cast<uintptr_t>(value);
    return FileSystem::Result::success;
}

FileSystem::Result
FileSystemStub::readDir(Directory dir, DirectoryEntry& entry)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (dir.data == 0)
    {
        return FileSystem::Result::invalidInput;
    }
    else
    {
        DirectoryStub* stub = reinterpret_cast<DirectoryStub*>(dir.data);
        if (stub->iterator == stub->directory->directoryEntries.end())
        {
            entry.data = 0;
            return FileSystem::Result::endOfData;
        }
        else
        {
            entry.data = reinterpret_cast<uintptr_t>(&(*stub->iterator));
            ++stub->iterator;
            return FileSystem::Result::success;
        }
    }
}

FileSystem::Result
FileSystemStub::getName(DirectoryEntry& entry, outpost::Slice<const char>& name)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (entry.data == 0)
    {
        return Result::invalidInput;
    }

    Entry* obj = reinterpret_cast<Entry*>(entry.data);
    name = outpost::Slice<const char>::unsafe(obj->name.c_str(), obj->name.size());
    return FileSystem::Result::success;
}

FileSystem::Result
FileSystemStub::closeDir(Directory& dir)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (dir.data == 0)
    {
        return FileSystem::Result::invalidInput;
    }
    else
    {
        DirectoryStub* stub = reinterpret_cast<DirectoryStub*>(dir.data);
        dir.data = 0;
        delete stub;
        return FileSystem::Result::success;
    }
}

FileSystem::Result
FileSystemStub::rewindDir(Directory& dir)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (dir.data == 0)
    {
        return FileSystem::Result::invalidInput;
    }
    else
    {
        DirectoryStub* stub = reinterpret_cast<DirectoryStub*>(dir.data);
        stub->iterator = stub->directory->directoryEntries.begin();
        return FileSystem::Result::success;
    }
}

FileSystem::Result
FileSystemStub::open(const char* path, OpenMask mask, File& file)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (nullptr == path)
    {
        return FileSystem::Result::invalidInput;
    }
    if (!mMounted)
    {
        return FileSystem::Result::invalidState;
    }

    if (mReadOnly && (mask & FileSystem::WRITE))
    {
        return FileSystem::Result::accessDenied;
    }

    if (file.data != 0)
    {
        return FileSystem::Result::invalidInput;
    }

    Entry* entry = getEntry(path);

    if (nullptr == entry)
    {
        if (mask & FileSystem::CREATE)
        {
            FileSystem::Result ret = createFile(path, R | W | X);
            if (ret != FileSystem::Result::success)
            {
                return ret;
            }

            entry = getEntry(path);

            FileStub* stub = new FileStub;
            stub->file = entry;
            stub->pos = 0;

            if (mask & FileSystem::READ)
            {
                stub->perm = stub->perm | FileSystem::R;
            }

            if (mask & FileSystem::WRITE)
            {
                stub->perm = stub->perm | FileSystem::W;
            }

            if (mask & FileSystem::EXECUTE)
            {
                stub->perm = stub->perm | FileSystem::X;
            }

            entry->isOpen = true;

            file.data = reinterpret_cast<uintptr_t>(stub);
            return FileSystem::Result::success;
        }
        else
        {
            return FileSystem::Result::notFound;
        }
    }
    else
    {
        // fix counting
        mCountToForce++;
        return open(entry, mask, file);
    }
}

FileSystem::Result
FileSystemStub::open(DirectoryEntry& dir, OpenMask mask, File& file)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (!mMounted)
    {
        return FileSystem::Result::invalidState;
    }

    if (mReadOnly && (mask & FileSystem::WRITE))
    {
        return FileSystem::Result::accessDenied;
    }

    if (dir.data == 0)
    {
        return FileSystem::Result::invalidInput;
    }

    if (file.data != 0)
    {
        return FileSystem::Result::invalidInput;
    }

    Entry* entry = reinterpret_cast<Entry*>(dir.data);

    // fix counting
    mCountToForce++;
    return open(entry, mask, file);
}

FileSystem::Result
FileSystemStub::open(Entry* entry, OpenMask mask, File& file)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (nullptr == entry)
    {
        return FileSystem::Result::invalidInput;
    }
    if (!entry->isFile)
    {
        return FileSystem::Result::notAFile;
    }

    if ((mask & FileSystem::READ) && !(entry->permission & FileSystem::R))
    {
        return FileSystem::Result::accessDenied;
    }

    if ((mask & FileSystem::WRITE) && !(entry->permission & FileSystem::W))
    {
        return FileSystem::Result::accessDenied;
    }

    if ((mask & FileSystem::WRITE) && (entry->isOpen))
    {
        return FileSystem::Result::fileInUse;
    }

    if ((mask & FileSystem::EXECUTE) && !(entry->permission & FileSystem::X))
    {
        return FileSystem::Result::accessDenied;
    }

    FileStub* stub = new FileStub;
    stub->file = entry;
    if (mask & FileSystem::APPEND)
    {
        stub->pos = entry->fileData.size();
    }
    else
    {
        stub->pos = 0;
    }

    if (mask & FileSystem::READ)
    {
        stub->perm = stub->perm | FileSystem::R;
    }

    if (mask & FileSystem::WRITE)
    {
        stub->perm = stub->perm | FileSystem::W;
    }

    if (mask & FileSystem::EXECUTE)
    {
        stub->perm = stub->perm | FileSystem::X;
    }

    entry->isOpen = true;

    file.data = reinterpret_cast<uintptr_t>(stub);
    return FileSystem::Result::success;
}

FileSystem::Result
FileSystemStub::truncate(const char* path, uint64_t newLength)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (nullptr == path)
    {
        return FileSystem::Result::invalidInput;
    }
    if (!mMounted)
    {
        return FileSystem::Result::invalidState;
    }

    if (mReadOnly)
    {
        return FileSystem::Result::accessDenied;
    }

    Entry* entry = getEntry(path);
    if (nullptr == entry)
    {
        return FileSystem::Result::notFound;
    }

    // fix counting
    mCountToForce++;
    return truncate(entry, newLength);
}

FileSystem::Result
FileSystemStub::truncate(DirectoryEntry& file, uint64_t newLength)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (!mMounted)
    {
        return FileSystem::Result::invalidState;
    }

    if (mReadOnly)
    {
        return FileSystem::Result::accessDenied;
    }

    if (file.data == 0)
    {
        return FileSystem::Result::invalidInput;
    }

    Entry* entry = reinterpret_cast<Entry*>(file.data);

    // fix counting
    mCountToForce++;
    return truncate(entry, newLength);
}

FileSystem::Result
FileSystemStub::truncate(Entry* entry, uint64_t newLength)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (nullptr == entry)
    {
        return FileSystem::Result::invalidInput;
    }
    if (!entry->isFile)
    {
        return FileSystem::Result::notAFile;
    }

    if (entry->isOpen)
    {
        return FileSystem::Result::fileInUse;
    }

    if (!(entry->permission & W))
    {
        return FileSystem::Result::accessDenied;
    }

    if (entry->fileData.size() < newLength)
    {
        return FileSystem::Result::invalidInput;
    }

    entry->fileData.resize(newLength);
    entry->modify =
            outpost::time::TimeEpochConverter<outpost::time::SpacecraftElapsedTimeEpoch,
                                              outpost::time::GpsEpoch>::convert(mClock.now());
    return FileSystem::Result::success;
}

FileSystem::Result
FileSystemStub::close(File& file)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (file.data == 0)
    {
        return FileSystem::Result::invalidInput;
    }
    else
    {
        FileStub* stub = reinterpret_cast<FileStub*>(file.data);
        stub->file->isOpen = false;
        file.data = 0;
        delete stub;
        return FileSystem::Result::success;
    }
}

FileSystem::Result
FileSystemStub::read(File& file, outpost::Slice<uint8_t>& data)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (file.data == 0)
    {
        return FileSystem::Result::invalidInput;
    }

    FileStub* stub = reinterpret_cast<FileStub*>(file.data);
    if (!(stub->perm & FileSystem::R))
    {
        return FileSystem::Result::accessDenied;
    }

    if (stub->file->fileData.size() <= stub->pos)
    {
        return FileSystem::Result::endOfData;
    }

    uint64_t readSize = outpost::utils::min<uint64_t>(data.getNumberOfElements(),
                                                      stub->file->fileData.size() - stub->pos);

    data.copyFrom(&stub->file->fileData[stub->pos], readSize);
    data = data.first(readSize);
    stub->pos += readSize;
    return FileSystem::Result::success;
}

FileSystem::Result
FileSystemStub::write(File& file, outpost::Slice<const uint8_t>& data)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (file.data == 0)
    {
        return FileSystem::Result::invalidInput;
    }

    FileStub* stub = reinterpret_cast<FileStub*>(file.data);

    if (!(stub->perm & FileSystem::W))
    {
        return FileSystem::Result::accessDenied;
    }

    if (stub->file->fileData.size() < data.getNumberOfElements() + stub->pos)
    {
        stub->file->fileData.resize(data.getNumberOfElements() + stub->pos);
    }

    memcpy(&stub->file->fileData[stub->pos], data.getDataPointer(), data.getNumberOfElements());
    stub->pos += data.getNumberOfElements();

    stub->file->modify =
            outpost::time::TimeEpochConverter<outpost::time::SpacecraftElapsedTimeEpoch,
                                              outpost::time::GpsEpoch>::convert(mClock.now());

    return FileSystem::Result::success;
}

FileSystem::Result
FileSystemStub::seek(File& file, int64_t diff, SeekMode mode)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (file.data == 0)
    {
        return FileSystem::Result::invalidInput;
    }

    FileStub* stub = reinterpret_cast<FileStub*>(file.data);

    int64_t newPos = 0;

    switch (mode)
    {
        case SeekMode::set: newPos = diff; break;
        case SeekMode::current: newPos = diff + stub->pos; break;
        case SeekMode::end: newPos = diff + stub->file->fileData.size(); break;
    }

    // cast is safe as if negative the first part already hits
    if (newPos < 0 || static_cast<uint64_t>(newPos) > stub->file->fileData.size())
    {
        return FileSystem::Result::invalidInput;
    }
    else
    {
        stub->pos = newPos;
        return FileSystem::Result::success;
    }
}

FileSystem::Result
FileSystemStub::getInfo(const char* fullPath, Info& info)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (nullptr == fullPath)
    {
        return FileSystem::Result::invalidInput;
    }
    if (!mMounted)
    {
        return FileSystem::Result::invalidState;
    }

    if (info.data != 0)
    {
        return FileSystem::Result::invalidInput;
    }

    Entry* entry = getEntry(fullPath);
    if (nullptr == entry)
    {
        return FileSystem::Result::notFound;
    }
    info.data = reinterpret_cast<uintptr_t>(entry);
    return FileSystem::Result::success;
}

FileSystem::Result
FileSystemStub::getInfo(DirectoryEntry& entry, Info& info)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (!mMounted)
    {
        return FileSystem::Result::invalidState;
    }

    if (entry.data == 0 || info.data != 0)
    {
        return FileSystem::Result::invalidInput;
    }
    info.data = entry.data;
    return FileSystem::Result::success;
}

FileSystem::Result
FileSystemStub::releaseInfo(Info& info)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    info.data = 0;
    return FileSystem::Result::success;
}

FileSystem::Result
FileSystemStub::isFile(Info& info, bool& answer)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (info.data == 0)
    {
        return FileSystem::Result::invalidInput;
    }
    Entry* entry = reinterpret_cast<Entry*>(info.data);
    answer = entry->isFile;
    return FileSystem::Result::success;
}

FileSystem::Result
FileSystemStub::isDirectory(Info& info, bool& answer)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (info.data == 0)
    {
        return FileSystem::Result::invalidInput;
    }
    Entry* entry = reinterpret_cast<Entry*>(info.data);
    answer = !entry->isFile;
    return FileSystem::Result::success;
}

FileSystem::Result
FileSystemStub::getSize(Info& info, uint64_t& size)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (info.data == 0)
    {
        return FileSystem::Result::invalidInput;
    }
    Entry* entry = reinterpret_cast<Entry*>(info.data);
    if (!entry->isFile)
    {
        return FileSystem::Result::notAFile;
    }
    size = entry->fileData.size();
    return FileSystem::Result::success;
}

FileSystem::Result
FileSystemStub::getPermissions(Info& info, Permission& permission)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (info.data == 0)
    {
        return FileSystem::Result::invalidInput;
    }
    Entry* entry = reinterpret_cast<Entry*>(info.data);
    permission = entry->permission;
    return FileSystem::Result::success;
}

FileSystem::Result
FileSystemStub::getCreationTime(Info& info, outpost::time::GpsTime& time)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (info.data == 0)
    {
        return FileSystem::Result::invalidInput;
    }
    Entry* entry = reinterpret_cast<Entry*>(info.data);
    time = entry->creation;
    return FileSystem::Result::success;
}

FileSystem::Result
FileSystemStub::getModifyTime(Info& info, outpost::time::GpsTime& time)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (info.data == 0)
    {
        return FileSystem::Result::invalidInput;
    }
    Entry* entry = reinterpret_cast<Entry*>(info.data);
    time = entry->modify;
    return FileSystem::Result::success;
}

FileSystem::Result
FileSystemStub::chmod(const char* path, Permission perm)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (nullptr == path)
    {
        return FileSystem::Result::invalidInput;
    }
    if (!mMounted)
    {
        return FileSystem::Result::invalidState;
    }

    if (mReadOnly)
    {
        return FileSystem::Result::accessDenied;
    }

    Entry* entry = getEntry(path);
    if (nullptr == entry)
    {
        return FileSystem::Result::notFound;
    }

    entry->permission = perm;

    return FileSystem::Result::success;
}

FileSystem::Result
FileSystemStub::chmod(DirectoryEntry& obj, Permission perm)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (!mMounted)
    {
        return FileSystem::Result::invalidState;
    }

    if (mReadOnly)
    {
        return FileSystem::Result::accessDenied;
    }

    Entry* entry = reinterpret_cast<Entry*>(obj.data);
    if (nullptr == entry)
    {
        return FileSystem::Result::invalidInput;
    }

    entry->permission = perm;

    return FileSystem::Result::success;
}

FileSystem::Result
FileSystemStub::remove(const char* path)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (nullptr == path)
    {
        return FileSystem::Result::invalidInput;
    }
    if (!mMounted)
    {
        return FileSystem::Result::invalidState;
    }

    if (mReadOnly)
    {
        return FileSystem::Result::accessDenied;
    }

    Entry* entry = getEntry(path);

    if (nullptr == entry)
    {
        return FileSystem::Result::notFound;
    }

    if (!(entry->permission & W))
    {
        return FileSystem::Result::accessDenied;
    }

    if (checkOpenedRecursive(*entry))
    {
        return FileSystem::Result::fileInUse;
    }

    Entry* parent = getParent(path);
    if (nullptr == parent)
    {
        return FileSystem::Result::invalidInput;  // tried removing root
    }

    if (!entry->isFile && entry->directoryEntries.size() > 0)
    {
        return FileSystem::Result::notEmpty;
    }

    if (!(parent->permission & W))
    {
        return FileSystem::Result::accessDenied;
    }

    for (auto it = parent->directoryEntries.begin(); it != parent->directoryEntries.end(); ++it)
    {
        if (&(*it) == entry)
        {
            parent->directoryEntries.erase(it);
            parent->modify = outpost::time::TimeEpochConverter<
                    outpost::time::SpacecraftElapsedTimeEpoch,
                    outpost::time::GpsEpoch>::convert(mClock.now());
            return FileSystem::Result::success;
        }
    }

    // can only happen if we have concurrent access
    return FileSystem::Result::other;
}

FileSystem::Result
FileSystemStub::remove(Directory& p, DirectoryEntry& e)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (!mMounted)
    {
        return FileSystem::Result::invalidState;
    }

    if (mReadOnly)
    {
        return FileSystem::Result::accessDenied;
    }

    if (p.data == 0 || e.data == 0)
    {
        return FileSystem::Result::invalidInput;
    }

    Entry* entry = reinterpret_cast<Entry*>(e.data);

    if (checkOpenedRecursive(*entry))
    {
        return FileSystem::Result::fileInUse;
    }

    if (!(entry->permission & W))
    {
        return FileSystem::Result::accessDenied;
    }

    Entry* parent = reinterpret_cast<DirectoryStub*>(p.data)->directory;

    if (!entry->isFile && entry->directoryEntries.size() > 0)
    {
        return FileSystem::Result::notEmpty;
    }

    if (!(parent->permission & W))
    {
        return FileSystem::Result::accessDenied;
    }

    for (auto it = parent->directoryEntries.begin(); it != parent->directoryEntries.end(); ++it)
    {
        if (&(*it) == entry)
        {
            parent->directoryEntries.erase(it);
            parent->modify = outpost::time::TimeEpochConverter<
                    outpost::time::SpacecraftElapsedTimeEpoch,
                    outpost::time::GpsEpoch>::convert(mClock.now());
            return FileSystem::Result::success;
        }
    }

    // p was not parent of e
    return FileSystem::Result::invalidInput;
}

FileSystem::Result
FileSystemStub::rename(const char* sourcePath, const char* targetPath)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (nullptr == sourcePath)
    {
        return FileSystem::Result::invalidInput;
    }
    if (nullptr == targetPath)
    {
        return FileSystem::Result::invalidInput;
    }
    FileSystem::Result r = copy(sourcePath, targetPath);
    if (r != FileSystem::Result::success)
    {
        return r;
    }

    // fix counting
    mCountToForce++;
    return remove(sourcePath);
}

FileSystem::Result
FileSystemStub::copy(const char* sourcePath, const char* targetPath)
{
    if (mForcedResult != Result::success)
    {
        if (mCountToForce > 0)
        {
            mCountToForce--;
        }
        else
        {
            Result tmp = mForcedResult;
            mForcedResult = Result::success;
            return tmp;
        }
    }

    if (nullptr == sourcePath)
    {
        return FileSystem::Result::invalidInput;
    }
    if (nullptr == targetPath)
    {
        return FileSystem::Result::invalidInput;
    }
    if (!mMounted)
    {
        return FileSystem::Result::invalidState;
    }

    if (mReadOnly)
    {
        return FileSystem::Result::accessDenied;
    }

    Entry* source = getEntry(sourcePath);
    if (nullptr == source)
    {
        return FileSystem::Result::notFound;
    }

    Entry* target = getEntry(targetPath);
    if (nullptr != target)
    {
        return FileSystem::Result::alreadyExists;
    }

    Entry* targetDir = getParent(targetPath);
    if (nullptr == targetDir)
    {
        return FileSystem::Result::notFound;
    }

    if (targetDir->isFile)
    {
        return FileSystem::Result::notADirectory;
    }

    if (!(targetDir->permission & W))
    {
        return FileSystem::Result::accessDenied;
    }

    Entry newEntry = *source;
    std::string path(targetPath);
    size_t pos = path.find_last_of('/');
    std::string name = path.substr(pos + 1);
    updateTime(newEntry,
               outpost::time::TimeEpochConverter<outpost::time::SpacecraftElapsedTimeEpoch,
                                                 outpost::time::GpsEpoch>::convert(mClock.now()));
    newEntry.name = name;
    targetDir->directoryEntries.push_back(newEntry);
    targetDir->modify =
            outpost::time::TimeEpochConverter<outpost::time::SpacecraftElapsedTimeEpoch,
                                              outpost::time::GpsEpoch>::convert(mClock.now());

    return FileSystem::Result::success;
}

void
FileSystemStub::updateTime(Entry& entry, outpost::time::GpsTime time)
{
    entry.modify = time;
    entry.creation = time;

    if (!entry.isFile)
    {
        for (auto it = entry.directoryEntries.begin(); it != entry.directoryEntries.end(); ++it)
        {
            updateTime(*it, time);
        }
    }
}

bool
FileSystemStub::existsFile(const std::string& path)
{
    Entry* entry = getEntry(path);
    return entry != nullptr && entry->isFile;
}

bool
FileSystemStub::existsDirectory(const std::string& path)

{
    Entry* entry = getEntry(path);
    return entry != nullptr && !entry->isFile;
}

bool
FileSystemStub::getPermission(const std::string& path, Permission& perm)
{
    Entry* entry = getEntry(path);
    if (nullptr == entry)
    {
        return false;
    }
    perm = entry->permission;
    return true;
}

bool
FileSystemStub::isFileContentEquals(const std::string& path,
                                    const outpost::Slice<const uint8_t>& data)
{
    Entry* entry = getEntry(path);
    if (entry == nullptr || !entry->isFile)
    {
        return false;
    }

    if (entry->fileData.size() != data.getNumberOfElements())
    {
        return false;
    }

    if (!data.getNumberOfElements())
    {
        // two empty-sized buffers are the same
        return true;
    }

    return memcmp(&entry->fileData[0], data.getDataPointer(), data.getNumberOfElements()) == 0;
}

void
FileSystemStub::reset()
{
    mRoot = Entry();
}

FileSystemStub::Entry*
FileSystemStub::getEntryInFolder(FileSystemStub::Entry& dir, const std::string& name)
{
    if (dir.isFile)
    {
        return nullptr;
    }
    for (auto it = dir.directoryEntries.begin(); it != dir.directoryEntries.end(); ++it)
    {
        if (it->name == name)
        {
            return &(*it);
        }
    }
    return nullptr;
}

FileSystemStub::Entry*
FileSystemStub::getEntry(std::string path)
{
    if (path[0] != '/')
    {
        // not a valid path
        return nullptr;
    }

    if (path == "/")
    {
        return &mRoot;
    }

    FileSystemStub::Entry* parent = getParent(path);
    if (nullptr == parent)
    {
        return nullptr;
    }
    else
    {
        size_t pos = path.find_last_of('/');
        return getEntryInFolder(*parent, path.substr(pos + 1));
    }
}

FileSystemStub::Entry*
FileSystemStub::getParent(std::string path)
{
    if (path[0] != '/')
    {
        // not a valid path
        return nullptr;
    }

    FileSystemStub::Entry* start = &mRoot;

    while (nullptr != start)
    {
        path = path.substr(1);  // remove leading '/'
        size_t index = path.find_first_of('/');
        if (index == std::string::npos)
        {
            return start;
        }
        FileSystemStub::Entry* next = getEntryInFolder(*start, path.substr(0, index));
        if (next == nullptr || next->isFile)
        {
            return nullptr;
        }

        start = next;
        path = path.substr(index);
    }
    return nullptr;
}

bool
FileSystemStub::checkOpenedRecursive(Entry& entry)
{
    if (entry.isFile)
    {
        return entry.isOpen;
    }
    else
    {
        for (auto it = entry.directoryEntries.begin(); it != entry.directoryEntries.end(); ++it)
        {
            if (checkOpenedRecursive(*it))
            {
                return true;
            }
        }
        return false;
    }
}

}  // namespace hal
}  // namespace unittest
