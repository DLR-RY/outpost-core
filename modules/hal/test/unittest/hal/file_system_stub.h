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

#ifndef UNITTEST_HAL_FILE_SYSTEM_STUB_H_
#define UNITTEST_HAL_FILE_SYSTEM_STUB_H_

#include <outpost/hal/file_system.h>
#include <outpost/time/clock.h>

#include <list>
#include <string>
#include <vector>

namespace unittest
{
namespace hal
{
class FileSystemStub : public outpost::hal::FileSystem
{
    /**
     * To let the std::vector handle all the heap stuff we have an entry that can be either a file
     * or a folder
     */
    struct Entry
    {
        std::string name = "";
        outpost::hal::FileSystem::Permission permission = outpost::hal::FileSystem::R
                                                          | outpost::hal::FileSystem::W
                                                          | outpost::hal::FileSystem::X;
        bool isFile = false;
        std::list<Entry> directoryEntries;
        std::vector<uint8_t> fileData;
        outpost::time::GpsTime creation = outpost::time::GpsTime::startOfEpoch();
        outpost::time::GpsTime modify = outpost::time::GpsTime::startOfEpoch();
        bool isOpen = false;
    };

public:
    explicit FileSystemStub(outpost::time::Clock& clock) :
        mClock(clock), mForcedResult(Result::success), mCountToForce(0)
    {
    }

    bool
    isMounted() const override
    {
        return mMounted;
    }

    Result
    mount(bool readOnly) override;

    Result
    unmount() override;

    Result
    mkDir(const char* fullPath, Permission mask) override;

    Result
    createFile(const char* fullPath, Permission permission) override;

    Result
    openDir(const char* path, Directory& folder) override;

    Result
    openDir(DirectoryEntry& entry, Directory& folder) override;

    Result
    readDir(Directory dir, DirectoryEntry& entry) override;

    Result
    getName(DirectoryEntry& entry, outpost::Slice<const char>& name) override;

    Result
    closeDir(Directory& dir) override;

    Result
    rewindDir(Directory& dir) override;

    Result
    open(const char* path, OpenMask mask, File& file) override;

    Result
    open(DirectoryEntry& dir, OpenMask mask, File& file) override;

    Result
    close(File& file) override;

    Result
    read(File& file, outpost::Slice<uint8_t>& data) override;

    Result
    write(File& file, outpost::Slice<const uint8_t>& data) override;

    Result
    seek(File& file, int64_t diff, SeekMode mode) override;

    Result
    truncate(const char* path, uint64_t newLength) override;

    Result
    truncate(DirectoryEntry& file, uint64_t newLength) override;

    Result
    getInfo(const char* fullPath, Info& info) override;

    Result
    getInfo(DirectoryEntry& entry, Info& info) override;

    Result
    releaseInfo(Info& info) override;

    Result
    isFile(Info& info, bool& answer) override;

    Result
    isDirectory(Info& info, bool& answer) override;

    Result
    getSize(Info& info, uint64_t& size) override;

    Result
    getPermissions(Info& info, Permission& permission) override;

    Result
    getCreationTime(Info& info, outpost::time::GpsTime& time) override;

    Result
    getModifyTime(Info& info, outpost::time::GpsTime& time) override;

    Result
    chmod(const char* path, Permission perm) override;

    Result
    chmod(DirectoryEntry& obj, Permission perm) override;

    Result
    remove(const char* path) override;

    Result
    remove(Directory& p, DirectoryEntry& e) override;

    Result
    rename(const char* sourcePath, const char* targetPath) override;

    Result
    copy(const char* sourcePath, const char* targetPath) override;

    Result
    flush(File&) override
    {
        return Result::success;
    }

    Result
    flush() override
    {
        return Result::success;
    }

    /**
     * Will cause an error after a given amount of calls.
     * Only one may be queued at a time, set another will override the old
     *
     * @param error the error which should be caused, Result::success will disable
     * @param delay how many calls between now and the error is allowed
     */
    void
    forceError(Result error, size_t delay)
    {
        mForcedResult = error;
        mCountToForce = delay;
    }

    // some helper function for unit test
    /**
     * checks whether file exist
     * @param path the path to file
     * @return true if path exists and is a file
     */
    bool
    existsFile(const std::string& path);

    /**
     * checks whether directory exist
     * @param path the path to director
     * @return true if path exists and is a directory
     */
    bool
    existsDirectory(const std::string& path);

    /**
     * checks whether file exist with an defined content
     * @param path the path to file
     * @param the expected contents of the file
     * @return true if path exists, is a file and it data is as expected
     */
    bool
    isFileContentEquals(const std::string& path, const outpost::Slice<const uint8_t>& data);

    /**
     * Provided the permission of a given entry
     * @param path the path to file/dirctory
     * @param perm place to store der permission
     * @return true if path exists
     */
    bool
    getPermission(const std::string& path, Permission& perm);

    /**
     * restarts with empty file system, not only use when all handles are freed
     */
    void
    reset();

private:
    struct DirectoryStub
    {
        Entry* directory;
        std::list<Entry>::iterator iterator;
    };

    struct FileStub
    {
        Entry* file;
        Permission perm;
        uint64_t pos;
    };

    /**
     * Opens a file
     *
     * @param entry the file to open
     * @param mask flags for opening
     * @param file Object to store file pointer
     * @return Operation result
     */
    Result
    open(Entry* entry, OpenMask mask, File& file);

    /**
     * Opens a directory to iterate its content
     *
     * @param entry the folder to open
     * @param folder the directory object
     * @return Operation result
     */
    Result
    openDir(Entry* entry, Directory& folder);

    /**
     * Sets the size of a file to a given value
     * implementation supports size reduction only
     *
     * @param entry the file to truncate
     * @param newLength new length of the file
     * @return Operation result
     */
    Result
    truncate(Entry* entry, uint64_t newLength);

    /**
     * Get the Entry of a given name in a folder
     * @param dir the director to search
     * @param name the name
     * @return Pointer to do entry or nullptr if not exists
     */
    Entry*
    getEntryInFolder(Entry& dir, const std::string& name);  // cppcheck-suppress functionStatic

    /**
     * Get the Entry for a given path
     * @param path the path to search
     * @return Pointer to do entry or nullptr if not exists
     */
    Entry*
    getEntry(std::string path);

    /**
     * Get the parent directory for a given absolute path
     * @param path the path to search
     * @return Pointer to do entry or nullptr if not exists
     */
    Entry*
    getParent(std::string path);

    /**
     * Set all time of entry and child to a given time
     * @param entry the entry
     * @param time the time to set
     */
    void
    updateTime(Entry& entry, outpost::time::GpsTime time);

    /**
     * Checks recursive whether a element or one of its children are opended.
     *
     * @param entry The entry to check
     * @return true if element or any of its children is opened
     */
    bool
    checkOpenedRecursive(Entry& entry);

    bool mMounted = false;
    bool mReadOnly = false;
    Entry mRoot;  // default values of entry are fine.

    outpost::time::Clock& mClock;

    Result mForcedResult;
    size_t mCountToForce;
};

}  // namespace hal
}  // namespace unittest

#endif
