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

#ifndef OUTPOST_HAL_FILE_SYSTEM_H_
#define OUTPOST_HAL_FILE_SYSTEM_H_

#include <outpost/base/slice.h>
#include <outpost/time.h>

#include <stdint.h>

namespace outpost
{
namespace hal
{
class FileSystem
{
public:
    /**
     * A general result list, that should include most used result types
     */
    enum class Result : uint8_t
    {
        success,
        endOfData,          // EOF or no further directory entries
        notFound,           // File/directory not found
        accessDenied,       // no permission
        noSpace,            // file / directory / partition full
        readOnly,           // cannot write
        writeOnly,          // cannot read
        alreadyExists,      // file/directory already exists
        notEmpty,           // removed directory is not empty
        notAFile,           // functions requires a file
        notADirectory,      // function requires a directory
        invalidInput,       // function parameter misformed
        resourceExhausted,  // some internal resource is exhausted
        invalidState,       // Filesytem is in a state that not allows the function to be called
        IOError,            // Error in underlying driver/hardware
        notImplemented,     // function not supported for this filesystem
        fileInUse,  // Operation cannot be done because related file is open by other operation
        other       // a failure not caught by any above
    };

    class Permission
    {
        friend class FileSystem;

    public:
        Permission() = default;

        bool
        operator==(const Permission& o) const
        {
            return value == o.value;
        }

        bool
        operator!=(const Permission& o) const
        {
            return value != o.value;
        }

        Permission
        operator&(const Permission& o) const
        {
            Permission ret;
            ret.value = value & o.value;
            return ret;
        }

        Permission
        operator|(const Permission& o) const
        {
            Permission ret;
            ret.value = value | o.value;
            return ret;
        }

        /**
         * Allow short functions like if(permission & X) instead of if((permission & X)==X)
         */
        operator bool() const
        {
            return value != 0;
        }

    private:
        explicit Permission(uint8_t v) : value{v} {};
        uint8_t value = 0;
    };

    static const Permission R;
    static const Permission W;
    static const Permission X;

    class OpenMask
    {
        friend class FileSystem;

    public:
        constexpr OpenMask() = default;

        OpenMask(const OpenMask& o) : value(o.value)
        {
        }

        OpenMask&
        operator=(const OpenMask& o)
        {
            value = o.value;
            return *this;
        }

        bool
        operator==(const OpenMask& o) const
        {
            return value == o.value;
        }

        bool
        operator!=(const OpenMask& o) const
        {
            return value != o.value;
        }

        OpenMask
        operator&(const OpenMask& o) const
        {
            OpenMask ret;
            ret.value = value & o.value;
            return ret;
        }

        OpenMask
        operator|(const OpenMask& o) const
        {
            OpenMask ret;
            ret.value = value | o.value;
            return ret;
        }

        /**
         * Allow short functions like if(OpenMask & read) instead of if((OpenMask & read)==read)
         */
        operator bool() const
        {
            return value != 0;
        }

    private:
        explicit constexpr OpenMask(uint8_t v) : value(v){};
        uint8_t value = 0;
    };

    static const OpenMask READ;
    static const OpenMask WRITE;
    static const OpenMask EXECUTE;
    static const OpenMask APPEND;
    static const OpenMask CREATE;

    // usage of data implementation specific.
    // put enough memory space in that a pointer can be used
    struct Directory
    {
        uintptr_t data = 0;
    };

    struct DirectoryEntry
    {
        uintptr_t data = 0;
    };

    struct File
    {
        uintptr_t data = 0;
    };

    struct Info
    {
        uintptr_t data = 0;
    };

    enum class SeekMode
    {
        set,
        current,
        end
    };

    FileSystem() = default;
    virtual ~FileSystem() = default;

    FileSystem&
    operator=(const FileSystem&) = delete;
    FileSystem(const FileSystem&) = delete;

    /**
     * @return whether the filesystem is mounted
     */
    virtual bool
    isMounted() const = 0;

    /**
     * Mounts the filesystem
     *
     * @param readOnly If true the filesystem cannot be modified
     * @return Operation result
     */
    virtual Result
    mount(bool readOnly) = 0;

    /**
     * Unmounts the filesystem
     *
     * @return Operation result
     */
    virtual Result
    unmount() = 0;

    /**
     * Creates a directory at a given path
     *
     * @param fullPath the absolute path of the directory
     * @param mask the permission with which the folder should be created
     * @return Operation result
     */
    virtual Result
    mkDir(const char* fullPath, Permission mask) = 0;

    /**
     * Opens a directory to iterate its content
     *
     * @param path path of the directory
     * @param folder the directory object
     * @return Operation result
     */
    virtual Result
    openDir(const char* path, Directory& folder) = 0;

    /**
     * Opens a directory to iterate its content
     *
     * @param entry entry to open
     * @param folder the directory object
     * @return Operation result
     */
    virtual Result
    openDir(DirectoryEntry& entry, Directory& folder) = 0;

    /**
     * Gives the next element in a directory
     *
     * @param dir directory to iterate
     * @param entry Position to store the next entry
     * @return Operation result
     *         endOfData: directory iteration already done, entry not valid
     */
    virtual Result
    readDir(Directory dir, DirectoryEntry& entry) = 0;

    /**
     * Get the name of a directory entry.
     *
     * @param entry the entry for which the name should be requested
     * @param name place to store the position of the name
     * @return Operation result
     */
    virtual Result
    getName(DirectoryEntry& entry, outpost::Slice<const char>& name) = 0;

    /**
     * Closes a directory, also closes the corresponding DirectoryEntry objects
     * @param dir the directory to close
     * @return Operation result
     */
    virtual Result
    closeDir(Directory& dir) = 0;

    /**
     * Resets the iteration pointer for a directory
     * @param dir Directory to reset its iteration
     * @return Operation result
     */
    virtual Result
    rewindDir(Directory& dir) = 0;

    /**
     * Opens a file
     *
     * @param path absolute path to the file
     * @param mask flags for opening
     * @param file Object to store file pointer
     * @return Operation result
     */
    virtual Result
    open(const char* path, OpenMask mask, File& file) = 0;

    /**
     * Opens a file
     *
     * @param dir File entry pointing to the file to open
     * @param mask flags for opening
     * @param file Object to store file pointer
     * @return Operation result
     */
    virtual Result
    open(DirectoryEntry& dir, OpenMask mask, File& file) = 0;

    /**
     * Creates a new file
     *
     * @param path absolute of the file to create
     * @param permission the permission of the file after creation
     * @return Operation result
     */
    virtual Result
    createFile(const char* path, Permission permission) = 0;

    /**
     * Closes a file
     *
     * @param file the file to close
     * @return Operation result
     */
    virtual Result
    close(File& file) = 0;

    /**
     * Gets the information object for a file/directory
     *
     * @param fullPath absolute path of the element to open
     * @param info the info object to store the data
     * @return Operation result
     */
    virtual Result
    getInfo(const char* fullPath, Info& info) = 0;

    /**
     * Gets the information object for a file/directory
     *
     * @param entry Entry pointing to the element to get the information
     * @param info the info object to store the data
     * @return Operation result
     */
    virtual Result
    getInfo(DirectoryEntry& entry, Info& info) = 0;

    /**
     * Frees the resources of a info object
     *
     * @param info the information object to free
     * @return Operation result
     */
    virtual Result
    releaseInfo(Info& info) = 0;

    /**
     * Checks whether a info points to a file
     *
     * @param info the info to check
     * @param answer storage for the answer
     * @return Operation result
     */
    virtual Result
    isFile(Info& info, bool& answer) = 0;

    /**
     * Checks whether a info points to a directory
     *
     * @param info the info to check
     * @param answer storage for the answer
     * @return Operation result
     */
    virtual Result
    isDirectory(Info& info, bool& answer) = 0;

    /**
     * Gets the size of a file
     *
     * @param info the info to check
     * @param size storage for the answer
     * @return Operation result
     */
    virtual Result
    getSize(Info& info, uint64_t& size) = 0;

    /**
     * Gets the permission for a file or directory
     *
     * @param info the info to check
     * @param permission storage for the answer
     * @return Operation result
     */
    virtual Result
    getPermissions(Info& info, Permission& permission) = 0;

    /**
     * Gets the creation time of an element
     *
     * @param info the info to check
     * @param time storage for the answer
     * @return Operation result
     */
    virtual Result
    getCreationTime(Info& info, outpost::time::GpsTime& time) = 0;

    /**
     * Gets the modify time of an element
     *
     * @param info the info to check
     * @param time storage for the answer
     * @return Operation result
     */
    virtual Result
    getModifyTime(Info& info, outpost::time::GpsTime& time) = 0;

    /**
     * Reads the contents of a file
     *
     * @param file the file to read from
     * @param data the place to read the data to, if not enough data is left in file the size of the
     * slice is reduced
     * @return Operation result
     *         endOfData: Only when the end of file was reach before the request.
     */
    virtual Result
    read(File& file, outpost::Slice<uint8_t>& data) = 0;

    /**
     * Writes data to a file
     *
     * @param file the file to read from
     * @param data data to write to the file
     * @return Operation result
     */
    virtual Result
    write(File& file, outpost::Slice<const uint8_t>& data) = 0;

    /**
     * Moves the read/writer pointer of a file
     *
     * @param file filepointer for which the position should be changed
     * @param diff amount in bytes of which the pointer should be moved
     * @param mode position from which the pointer should be moved
     * @return Operation result
     */
    virtual Result
    seek(File& file, int64_t diff, SeekMode mode) = 0;

    /**
     * Flushes a file
     *
     * @param file the file to flush
     * @return Operation result
     */
    virtual Result
    flush(File& file) = 0;

    /**
     * Flushes a filesystem
     * @return Operation result
     */
    virtual Result
    flush() = 0;

    /**
     * Sets the size of a file to a given value
     *
     * Note: implementation may support size reduction only
     * @param path absolute path to the file to truncate
     * @param newLength new length of the file
     * @return Operation result
     */
    virtual Result
    truncate(const char* path, uint64_t newLength) = 0;

    /**
     * Sets the size of a file to a given value
     *
     * Note: implementation may support size reduction only
     * @param entry the file to truncate
     * @param newLength new length of the file
     * @return Operation result
     */
    virtual Result
    truncate(DirectoryEntry& entry, uint64_t newLength) = 0;

    /**
     * Renames a element in the filesystem
     *
     * @param sourePath Absolute path of the object to rename
     * @param targetPath Absolute path which the element should have after operation
     * @return Operation result
     */
    virtual Result
    rename(const char* sourcePath, const char* targetPath) = 0;

    /**
     * Copies a element
     *
     * Note: implementation may support file copy only
     * @param sourePath Absolute path of the object to copy
     * @param targetPath Absolute path of the copied element
     * @return Operation result
     */
    virtual Result
    copy(const char* sourcePath, const char* targetPath) = 0;

    /**
     * Changes the permission of an element
     *
     * @param path absolute path of the element
     * @param perm the new permission
     * @return Operation result
     */
    virtual Result
    chmod(const char* path, Permission perm) = 0;

    /**
     * Changes the permission of an element
     *
     * @param entry the elemt to change
     * @param perm the new permission
     * @return Operation result
     */
    virtual Result
    chmod(DirectoryEntry& entry, Permission perm) = 0;

    /**
     * Removes an element
     *
     * Note: Implementation may require directories to be empty
     * @param path absolute path to the element to remove
     * @return Operation result
     */
    virtual Result
    remove(const char* path) = 0;

    /**
     * Removes an element
     *
     * Note: Implementation may require directories to be empty
     * @param parant the directory from which the entry should be removed
     * @param entry the element to remove
     * @return Operation result
     */
    virtual Result
    remove(Directory& parent, DirectoryEntry& entry) = 0;
};

}  // namespace hal
}  // namespace outpost

#endif
