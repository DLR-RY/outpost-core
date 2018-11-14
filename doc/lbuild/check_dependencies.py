#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Copyright (c) 2018, German Aerospace Center (DLR)
#
# This file is part of the development version of OUTPOST.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#
# Authors:
# - 2018, Fabian Greif (DLR RY-AVS)

import os
import colorful
import subprocess

import lbuild

FILEPATH = os.path.dirname(os.path.realpath(__file__))


class Result:
    """
    Result of build operation for a given module
    """
    module = None
    similar = None
    modules = []
    success = False


def lbuild_query_modules(module):
    """
    Query the required set of modules to build the given module.

    Returns:
        List of module names.
    """
    builder = lbuild.api.Builder(config="project.xml")
    builder.load()
    modules = builder.validate(module)
    return lbuild.utils.listrify(modules)


def build(module):
    try:
        subprocess.run(["rm", "-rf", "source"],
                       cwd=FILEPATH, stdout=subprocess.DEVNULL).check_returncode()

        # Generate source code blob through lbuild
        builder = lbuild.api.Builder(config="project.xml")
        builder.load()
        builder.build(outpath="source", modules=module)

        # Compile generated sources
        subprocess.run(["scons", "-Q", "-j3", "-Csource"], cwd=FILEPATH).check_returncode()

        # Run unittests
        subprocess.run(["source/build/test/unittest/runner"],
                       cwd=FILEPATH).check_returncode()
    except subprocess.CalledProcessError or lbuild.excpetion.LbuildException:
        return False
    else:
        return True


if __name__ == '__main__':
    all_modules = lbuild_query_modules(":*")

    results = []
    for module in all_modules:
        print(colorful.bold("\n###### Check module '{}' ######\n".format(module)))

        result = Result()
        result.module = module
        result.modules = lbuild_query_modules(module)

        # Check previous results
        for previous in results:
            if result.modules == previous.modules:
                print("Similar to {}".format(previous.module))
                result.similar = previous.module
                result.success = previous.success
                break
        else:
            # Unknown combination of modules, build the source code
            result.success = build(module)

        results.append(result)

    print(colorful.bold("\n\nResults:\n"))
    for result in results:
        result_output = colorful.green("SUCCESS") if result.success else colorful.red("FAIL")
        print("- {} - {}".format(result.module, result_output))
        if result.similar is not None:
            print(colorful.dimmed("    Same modules as {}".format(result.similar)))
        else:
            print(colorful.dimmed("    {}".format(
                ", ".join([x.replace("outpost", "") for x in result.modules]))))
