/*
 * Copyright (c) 2013-2017, German Aerospace Center (DLR)
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

#include <outpost/time/time_epoch.h>

#include <unittest/hal/file_system_stub.h>
#include <unittest/harness.h>
#include <unittest/time/testing_clock.h>

using outpost::hal::FileSystem;
using unittest::hal::FileSystemStub;
using unittest::time::TestingClock;

TEST(FileSystemStubTest, mounting)
{
    TestingClock clk;
    FileSystemStub fs(clk);

    EXPECT_EQ(false, fs.isMounted());
    EXPECT_EQ(FileSystem::Result::success, fs.mount(false));
    EXPECT_EQ(true, fs.isMounted());
    EXPECT_NE(FileSystem::Result::success, fs.mount(false));

    char dir1[] = "/a";
    EXPECT_EQ(FileSystem::Result::success, fs.mkDir(dir1, FileSystem::R | FileSystem::W));

    EXPECT_EQ(FileSystem::Result::success, fs.unmount());
    EXPECT_EQ(false, fs.isMounted());
    EXPECT_NE(FileSystem::Result::success, fs.unmount());

    EXPECT_EQ(FileSystem::Result::success, fs.mount(true));
    EXPECT_EQ(true, fs.isMounted());

    char dir2[] = "/b";
    EXPECT_NE(FileSystem::Result::success, fs.mkDir(dir2, FileSystem::R | FileSystem::W));
}

TEST(FileSystemStubTest, forceError)
{
    TestingClock clk;
    FileSystemStub fs(clk);

    fs.forceError(FileSystem::Result::alreadyExists, 0);
    EXPECT_EQ(FileSystem::Result::alreadyExists, fs.mount(false));

    // no its works
    EXPECT_EQ(FileSystem::Result::success, fs.mount(false));

    // delayed
    fs.forceError(FileSystem::Result::alreadyExists, 2);
    char dir1[] = "/a";
    EXPECT_EQ(FileSystem::Result::success, fs.mkDir(dir1, FileSystem::R | FileSystem::W));

    char dir2[] = "/b";
    EXPECT_EQ(FileSystem::Result::success, fs.mkDir(dir2, FileSystem::R | FileSystem::W));

    char dir3[] = "/c";
    EXPECT_EQ(FileSystem::Result::alreadyExists, fs.mkDir(dir3, FileSystem::R | FileSystem::W));

    // now it works again
    EXPECT_EQ(FileSystem::Result::success, fs.mkDir(dir3, FileSystem::R | FileSystem::W));
}

TEST(FileSystemStubTest, readWriteSeek)
{
    TestingClock clk;
    FileSystemStub fs(clk);

    EXPECT_EQ(FileSystem::Result::success, fs.mount(false));

    char file[] = "/a";

    FileSystem::File handle;
    FileSystem::OpenMask mask = FileSystem::READ | FileSystem::WRITE | FileSystem::CREATE;
    EXPECT_EQ(FileSystem::Result::success, fs.open(file, mask, handle));

    const uint8_t data[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    auto input = outpost::asSlice(data);

    EXPECT_EQ(FileSystem::Result::success, fs.write(handle, input));

    uint8_t readBuffer[10];

    auto output = outpost::asSlice(readBuffer);

    EXPECT_EQ(FileSystem::Result::endOfData, fs.read(handle, output));

    EXPECT_EQ(FileSystem::Result::success, fs.seek(handle, -5, FileSystem::SeekMode::current));

    EXPECT_EQ(FileSystem::Result::success, fs.read(handle, output));
    EXPECT_EQ(5u, output.getNumberOfElements());

    for (size_t i = 0; i < output.getNumberOfElements(); i++)
    {
        EXPECT_EQ(data[i + 5], output[i]);
    }

    EXPECT_EQ(FileSystem::Result::success, fs.seek(handle, 5, FileSystem::SeekMode::set));
    EXPECT_EQ(FileSystem::Result::success, fs.write(handle, input));

    EXPECT_EQ(FileSystem::Result::success, fs.seek(handle, -15, FileSystem::SeekMode::end));

    uint8_t readBuffer2[15];
    output = outpost::asSlice(readBuffer2);
    EXPECT_EQ(FileSystem::Result::success, fs.read(handle, output));
    EXPECT_EQ(15u, output.getNumberOfElements());

    for (size_t i = 0; i < 10; i++)
    {
        EXPECT_EQ(data[i], output[i + 5]);
    }

    for (size_t i = 0; i < 5; i++)
    {
        EXPECT_EQ(data[i], output[i]);
    }

    EXPECT_EQ(FileSystem::Result::success, fs.close(handle));

    mask = FileSystem::READ | FileSystem::WRITE;
    EXPECT_EQ(FileSystem::Result::success, fs.open(file, mask, handle));
    EXPECT_EQ(FileSystem::Result::success, fs.close(handle));

    char file2[] = "/b";  // not existing file
    EXPECT_EQ(FileSystem::Result::notFound, fs.open(file2, mask, handle));

    EXPECT_EQ(FileSystem::Result::success, fs.unmount());
}

TEST(FileSystemStubTest, mkDirCreateFileRenameCopyAndRemove)
{
    TestingClock clk;
    FileSystemStub fs(clk);

    EXPECT_EQ(FileSystem::Result::success, fs.mount(false));

    char dirA[] = "/A";
    char dirB[] = "/B";
    char file[] = "/A/a";

    FileSystem::Permission perm = FileSystem::R | FileSystem::W | FileSystem::X;

    EXPECT_EQ(FileSystem::Result::success, fs.mkDir(dirA, perm));
    EXPECT_EQ(FileSystem::Result::success, fs.mkDir(dirB, perm));
    EXPECT_EQ(FileSystem::Result::success, fs.createFile(file, perm));

    FileSystem::File handle;
    FileSystem::OpenMask mask = FileSystem::READ | FileSystem::WRITE;
    EXPECT_EQ(FileSystem::Result::success, fs.open(file, mask, handle));

    const uint8_t data[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    auto input = outpost::asSlice(data);

    EXPECT_EQ(FileSystem::Result::success, fs.write(handle, input));

    EXPECT_EQ(FileSystem::Result::success, fs.close(handle));

    char file2[] = "/B/b";

    EXPECT_EQ(FileSystem::Result::success, fs.rename(file, file2));

    uint8_t readBuffer[10];

    auto output = outpost::asSlice(readBuffer);
    EXPECT_EQ(FileSystem::Result::success, fs.open(file2, mask, handle));
    EXPECT_EQ(FileSystem::Result::success, fs.read(handle, output));
    EXPECT_EQ(10u, output.getNumberOfElements());
    for (size_t i = 0; i < output.getNumberOfElements(); i++)
    {
        EXPECT_EQ(data[i], output[i]);
    }
    EXPECT_EQ(FileSystem::Result::success, fs.close(handle));
    EXPECT_NE(FileSystem::Result::success, fs.open(file, mask, handle));

    EXPECT_EQ(FileSystem::Result::success, fs.copy(file2, file));

    outpost::asSlice(readBuffer);
    EXPECT_EQ(FileSystem::Result::success, fs.open(file2, mask, handle));
    EXPECT_EQ(FileSystem::Result::success, fs.read(handle, output));
    EXPECT_EQ(10u, output.getNumberOfElements());
    for (size_t i = 0; i < output.getNumberOfElements(); i++)
    {
        EXPECT_EQ(data[i], output[i]);
    }
    EXPECT_EQ(FileSystem::Result::success, fs.close(handle));

    outpost::asSlice(readBuffer);
    EXPECT_EQ(FileSystem::Result::success, fs.open(file, mask, handle));
    EXPECT_EQ(FileSystem::Result::success, fs.read(handle, output));
    EXPECT_EQ(10u, output.getNumberOfElements());
    for (size_t i = 0; i < output.getNumberOfElements(); i++)
    {
        EXPECT_EQ(data[i], output[i]);
    }
    EXPECT_EQ(FileSystem::Result::success, fs.close(handle));

    EXPECT_EQ(FileSystem::Result::success, fs.remove(file));
    EXPECT_NE(FileSystem::Result::success, fs.open(file, mask, handle));

    EXPECT_EQ(FileSystem::Result::success, fs.unmount());
}

TEST(FileSystemStubTest, openDirObjectInfoChmodAndTruncat)
{
    TestingClock clk;
    FileSystemStub fs(clk);
    auto time1 = outpost::time::TimeEpochConverter<outpost::time::SpacecraftElapsedTimeEpoch,
                                                   outpost::time::GpsEpoch>::convert(clk.now());

    EXPECT_EQ(FileSystem::Result::success, fs.mount(false));

    char dirA[] = "/A";
    char dirB[] = "/B";
    char dirC[] = "/C";
    char file1[] = "/1";
    char file2[] = "/2";
    char file3[] = "/3";
    char subFile1[] = "/B/x";
    char subFile2[] = "/B/a";
    constexpr uint64_t sizeFile1 = 10;
    uint64_t sizeFile2 = 5;

    FileSystem::Permission perm = FileSystem::R | FileSystem::W | FileSystem::X;

    EXPECT_EQ(FileSystem::Result::success, fs.mkDir(dirA, perm));
    EXPECT_EQ(FileSystem::Result::success, fs.mkDir(dirB, perm));
    EXPECT_EQ(FileSystem::Result::success, fs.createFile(file1, perm));
    EXPECT_EQ(FileSystem::Result::success, fs.createFile(file2, perm));

    FileSystem::File handle;
    FileSystem::OpenMask mask = FileSystem::READ | FileSystem::WRITE;
    EXPECT_EQ(FileSystem::Result::success, fs.open(file1, mask, handle));

    const uint8_t data[sizeFile1] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    auto input = outpost::asSlice(data);
    EXPECT_EQ(FileSystem::Result::success, fs.write(handle, input));
    EXPECT_EQ(FileSystem::Result::success, fs.close(handle));

    clk.incrementBy(outpost::time::Seconds(1));
    auto time2 = outpost::time::TimeEpochConverter<outpost::time::SpacecraftElapsedTimeEpoch,
                                                   outpost::time::GpsEpoch>::convert(clk.now());

    // modify file after creating
    EXPECT_EQ(FileSystem::Result::success, fs.open(file2, mask, handle));
    EXPECT_EQ(FileSystem::Result::success, fs.write(handle, input));
    EXPECT_EQ(FileSystem::Result::success, fs.close(handle));

    EXPECT_EQ(FileSystem::Result::success, fs.truncate(file2, sizeFile2));
    FileSystem::Permission perm2 = FileSystem::R;
    EXPECT_EQ(FileSystem::Result::success,
              fs.chmod(file2, perm2));  // correct behavior checked below

    // modify dir after creating, also this setup the test of remove later
    EXPECT_EQ(FileSystem::Result::success, fs.createFile(subFile1, perm));
    EXPECT_EQ(FileSystem::Result::success, fs.mkDir(subFile2, perm));
    EXPECT_EQ(FileSystem::Result::success, fs.remove(subFile2));

    EXPECT_EQ(FileSystem::Result::success, fs.mkDir(dirC, perm));
    EXPECT_EQ(FileSystem::Result::success, fs.createFile(file3, perm));

    bool isFile;
    bool isDirectory;
    FileSystem::Permission permission;
    outpost::time::GpsTime creation;
    outpost::time::GpsTime modify;
    uint64_t size;
    FileSystem::Info info;
    EXPECT_EQ(FileSystem::Result::success, fs.getInfo(dirB, info));

    EXPECT_EQ(FileSystem::Result::success, fs.isFile(info, isFile));
    EXPECT_EQ(FileSystem::Result::success, fs.isDirectory(info, isDirectory));
    EXPECT_EQ(FileSystem::Result::success, fs.getPermissions(info, permission));
    EXPECT_EQ(FileSystem::Result::success, fs.getCreationTime(info, creation));
    EXPECT_EQ(FileSystem::Result::success, fs.getModifyTime(info, modify));

    EXPECT_EQ(isFile, false);
    EXPECT_EQ(isDirectory, true);
    EXPECT_EQ(permission, perm);
    EXPECT_EQ(creation, time1);
    EXPECT_EQ(modify, time2);

    EXPECT_EQ(FileSystem::Result::success, fs.releaseInfo(info));

    EXPECT_EQ(FileSystem::Result::success, fs.getInfo(file2, info));

    EXPECT_EQ(FileSystem::Result::success, fs.isFile(info, isFile));
    EXPECT_EQ(FileSystem::Result::success, fs.isDirectory(info, isDirectory));
    EXPECT_EQ(FileSystem::Result::success, fs.getPermissions(info, permission));
    EXPECT_EQ(FileSystem::Result::success, fs.getCreationTime(info, creation));
    EXPECT_EQ(FileSystem::Result::success, fs.getModifyTime(info, modify));
    EXPECT_EQ(FileSystem::Result::success, fs.getSize(info, size));

    EXPECT_EQ(isFile, true);
    EXPECT_EQ(isDirectory, false);
    EXPECT_EQ(permission, perm2);  // checks chmod
    EXPECT_EQ(creation, time1);
    EXPECT_EQ(modify, time2);
    EXPECT_EQ(size, sizeFile2);

    EXPECT_EQ(FileSystem::Result::success, fs.releaseInfo(info));

    FileSystem::DirectoryEntry dirEntry;
    FileSystem::Directory dir;

    char root[] = "/";

    EXPECT_EQ(FileSystem::Result::success, fs.openDir(root, dir));
    bool found[6] = {false, false, false, false, false};
    for (int i = 0; i < 6; i++)
    {
        EXPECT_EQ(FileSystem::Result::success, fs.readDir(dir, dirEntry));

        outpost::Slice<const char> name = outpost::Slice<const char>::empty();
        EXPECT_EQ(FileSystem::Result::success, fs.getName(dirEntry, name));
        EXPECT_EQ(1u, name.getNumberOfElements());
        FileSystem::Directory subDir;
        FileSystem::DirectoryEntry subEntry;
        outpost::Slice<const char> name2 = outpost::Slice<const char>::empty();
        switch (name[0])
        {
            case 'A':
                found[0] = true;
                EXPECT_EQ(FileSystem::Result::success, fs.getInfo(dirEntry, info));

                EXPECT_EQ(FileSystem::Result::success, fs.isFile(info, isFile));
                EXPECT_EQ(FileSystem::Result::success, fs.isDirectory(info, isDirectory));
                EXPECT_EQ(FileSystem::Result::success, fs.getPermissions(info, permission));
                EXPECT_EQ(FileSystem::Result::success, fs.getCreationTime(info, creation));
                EXPECT_EQ(FileSystem::Result::success, fs.getModifyTime(info, modify));

                EXPECT_EQ(isFile, false);
                EXPECT_EQ(isDirectory, true);
                EXPECT_EQ(permission, perm);
                EXPECT_EQ(creation, time1);
                EXPECT_EQ(modify, time1);

                EXPECT_EQ(FileSystem::Result::success, fs.releaseInfo(info));
                break;
            case 'B':
                found[1] = true;
                EXPECT_EQ(FileSystem::Result::success, fs.getInfo(dirEntry, info));

                EXPECT_EQ(FileSystem::Result::success, fs.isFile(info, isFile));
                EXPECT_EQ(FileSystem::Result::success, fs.isDirectory(info, isDirectory));
                EXPECT_EQ(FileSystem::Result::success, fs.getPermissions(info, permission));
                EXPECT_EQ(FileSystem::Result::success, fs.getCreationTime(info, creation));
                EXPECT_EQ(FileSystem::Result::success, fs.getModifyTime(info, modify));

                EXPECT_EQ(isFile, false);
                EXPECT_EQ(isDirectory, true);
                EXPECT_EQ(permission, perm);
                EXPECT_EQ(creation, time1);
                EXPECT_EQ(modify, time2);

                EXPECT_EQ(FileSystem::Result::success, fs.releaseInfo(info));

                // test openDir with Direntry
                EXPECT_EQ(FileSystem::Result::success, fs.openDir(dirEntry, subDir));
                EXPECT_EQ(FileSystem::Result::success, fs.readDir(subDir, subEntry));
                EXPECT_EQ(FileSystem::Result::success, fs.getName(subEntry, name2));
                EXPECT_EQ('x', name2[0]);
                // Test remove with Direnty
                EXPECT_EQ(FileSystem::Result::success, fs.remove(subDir, subEntry));
                EXPECT_EQ(FileSystem::Result::success, fs.closeDir(subDir));

                EXPECT_EQ(FileSystem::Result::success, fs.openDir(dirEntry, subDir));
                // this is the test that both removes were successful
                EXPECT_EQ(FileSystem::Result::endOfData, fs.readDir(subDir, subEntry));
                EXPECT_EQ(FileSystem::Result::success, fs.closeDir(subDir));

                break;
            case 'C':
                found[2] = true;
                EXPECT_EQ(FileSystem::Result::success, fs.getInfo(dirEntry, info));

                EXPECT_EQ(FileSystem::Result::success, fs.isFile(info, isFile));
                EXPECT_EQ(FileSystem::Result::success, fs.isDirectory(info, isDirectory));
                EXPECT_EQ(FileSystem::Result::success, fs.getPermissions(info, permission));
                EXPECT_EQ(FileSystem::Result::success, fs.getCreationTime(info, creation));
                EXPECT_EQ(FileSystem::Result::success, fs.getModifyTime(info, modify));

                EXPECT_EQ(isFile, false);
                EXPECT_EQ(isDirectory, true);
                EXPECT_EQ(permission, perm);
                EXPECT_EQ(creation, time2);
                EXPECT_EQ(modify, time2);

                EXPECT_EQ(FileSystem::Result::success, fs.releaseInfo(info));
                break;
            case '1':
                found[3] = true;
                EXPECT_EQ(FileSystem::Result::success, fs.getInfo(dirEntry, info));

                EXPECT_EQ(FileSystem::Result::success, fs.isFile(info, isFile));
                EXPECT_EQ(FileSystem::Result::success, fs.isDirectory(info, isDirectory));
                EXPECT_EQ(FileSystem::Result::success, fs.getPermissions(info, permission));
                EXPECT_EQ(FileSystem::Result::success, fs.getCreationTime(info, creation));
                EXPECT_EQ(FileSystem::Result::success, fs.getModifyTime(info, modify));
                EXPECT_EQ(FileSystem::Result::success, fs.getSize(info, size));

                EXPECT_EQ(isFile, true);
                EXPECT_EQ(isDirectory, false);
                EXPECT_EQ(permission, perm);
                EXPECT_EQ(creation, time1);
                EXPECT_EQ(modify, time1);
                EXPECT_EQ(size, sizeFile1);

                EXPECT_EQ(FileSystem::Result::success, fs.releaseInfo(info));
                break;
            case '2':
                found[4] = true;
                EXPECT_EQ(FileSystem::Result::success, fs.getInfo(dirEntry, info));

                EXPECT_EQ(FileSystem::Result::success, fs.isFile(info, isFile));
                EXPECT_EQ(FileSystem::Result::success, fs.isDirectory(info, isDirectory));
                EXPECT_EQ(FileSystem::Result::success, fs.getPermissions(info, permission));
                EXPECT_EQ(FileSystem::Result::success, fs.getCreationTime(info, creation));
                EXPECT_EQ(FileSystem::Result::success, fs.getModifyTime(info, modify));
                EXPECT_EQ(FileSystem::Result::success, fs.getSize(info, size));

                EXPECT_EQ(isFile, true);
                EXPECT_EQ(isDirectory, false);
                EXPECT_EQ(permission, perm2);
                EXPECT_EQ(creation, time1);
                EXPECT_EQ(modify, time2);
                EXPECT_EQ(size, sizeFile2);

                EXPECT_EQ(FileSystem::Result::success, fs.releaseInfo(info));
                break;
            case '3':
                found[5] = true;

                // open with direnty
                EXPECT_EQ(FileSystem::Result::success, fs.open(dirEntry, mask, handle));
                EXPECT_EQ(FileSystem::Result::success, fs.write(handle, input));
                EXPECT_EQ(FileSystem::Result::success, fs.close(handle));

                EXPECT_EQ(FileSystem::Result::success, fs.getInfo(dirEntry, info));

                EXPECT_EQ(FileSystem::Result::success, fs.isFile(info, isFile));
                EXPECT_EQ(FileSystem::Result::success, fs.isDirectory(info, isDirectory));
                EXPECT_EQ(FileSystem::Result::success, fs.getPermissions(info, permission));
                EXPECT_EQ(FileSystem::Result::success, fs.getCreationTime(info, creation));
                EXPECT_EQ(FileSystem::Result::success, fs.getModifyTime(info, modify));
                EXPECT_EQ(FileSystem::Result::success, fs.getSize(info, size));

                EXPECT_EQ(isFile, true);
                EXPECT_EQ(isDirectory, false);
                EXPECT_EQ(permission, perm);
                EXPECT_EQ(creation, time2);
                EXPECT_EQ(modify, time2);
                EXPECT_EQ(size, sizeFile1);  // this checks also that the write was succesful

                EXPECT_EQ(FileSystem::Result::success, fs.releaseInfo(info));

                break;
            default: EXPECT_TRUE(false); break;
        }
    }
    EXPECT_TRUE(found[0] && found[1] && found[2] && found[3] && found[4] && found[5]);
    EXPECT_EQ(FileSystem::Result::endOfData, fs.readDir(dir, dirEntry));
    EXPECT_EQ(FileSystem::Result::success, fs.rewindDir(dir));
    EXPECT_EQ(FileSystem::Result::success, fs.readDir(dir, dirEntry));
    EXPECT_EQ(FileSystem::Result::success, fs.closeDir(dir));

    EXPECT_EQ(FileSystem::Result::success, fs.unmount());
}
