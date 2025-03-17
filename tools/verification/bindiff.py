#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Compare the files of the same name within different directories for equivelence.

The purpose of this is to verify the build outputs are binary equivelent, even
in the case that the directory structures are not the same (like sdk prebuilt
targets and the output of a normal build).
"""
################################################################################
###
### @file       bindiff.py
###
### @project    930x
###
### @classification  Confidential
###
################################################################################
###
################################################################################
###
### @copyright Copyright (C) 2022 EM Microelectronic
### @cond
###
### All rights reserved.
###
### Redistribution and use in source and binary forms, with or without
### modification, are permitted provided that the following conditions are met:
### 1. Redistributions of source code must retain the above copyright notice,
### this list of conditions and the following disclaimer.
### 2. Redistributions in binary form must reproduce the above copyright notice,
### this list of conditions and the following disclaimer in the documentation
### and/or other materials provided with the distribution.
###
################################################################################
###
### THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
### AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
### IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
### ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
### LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
### CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
### SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
### INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
### CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
### ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
### POSSIBILITY OF SUCH DAMAGE.
### @endcond
################################################################################

import os
import sys
import argparse

if __name__ == "__main__":
    HELP_STR = \
"""
This program is designed to be used to compare the outputs of two different
firmware builds for binary equivelence.

This program does a diff of files recursively between different folders. If a
file in one folder shares a name (not a path) with a file in the other folder,
then a diff is done between the files. This does not work if file names are
not unique within the same directory tree.

The purpose of this is to verify the build outputs are binary equivelent, even
in the case that the directory structures are not the same (like sdk prebuilt
targets and the output of a normal build).

example:
  $ .\\init.bat
  $ cd build
  $ cmake --build . --target all
  $ python ..\\tools\\bindiff.py . ..\\projects
"""
    parser = argparse.ArgumentParser(description=HELP_STR,
        formatter_class=argparse.RawTextHelpFormatter)
    parser.add_argument('dir1', help='directory tree to compare against')
    parser.add_argument('dir2', help='directory tree to compare against')
    parser.add_argument('extension',
        help='extension of files we want to compare (.hex by default)',
        nargs='?',
        default='.hex')
    parser.add_argument('-b', '--blacklist', action="append",
        help='files containing the supplied prefix will be ignored by the script')
    parser.add_argument('-i', '--ignore-missing', action='store_true',
        help='ignore files that are only present in one directory or the other UNLESS there are no files in the directories')
    args = parser.parse_args()

    if not os.path.isdir(args.dir1):
        print("{} is not a directory".format(args.dir1), file=sys.stderr)
        sys.exit(-1)
    if not os.path.isdir(args.dir2):
        print("{} is not a directory".format(args.dir2), file=sys.stderr)
        sys.exit(-1)


    def in_blacklist(file_name):
        if args.blacklist:
            for item in args.blacklist:
                if item in file_name:
                    return True

    d1 = {}
    d2 = {}
    different = False
    for root, subFolder, files in os.walk(args.dir1):
        for file in files:
            if file.endswith(args.extension) and not in_blacklist(file):
                d1[file] = root

    for root, subFolder, files in os.walk(args.dir2):
        for file in files:
            if file.endswith(args.extension) and not in_blacklist(file):
                d2[file] = root

    shared_keys = d1.keys() & d2.keys()
    d1_only = d1.keys() - d2.keys()
    d2_only = d2.keys() - d1.keys()
    results_dict = {}

    for key in shared_keys:
        contentsA = open(os.path.join(d1[key], key), 'r').read()
        contentsB = open(os.path.join(d2[key], key), 'r').read()
        results_dict[key] = contentsB == contentsA

    if not args.ignore_missing:
        different = len(d1_only) and len(d2_only)
        for key in d1_only:
            different = True
            print('file "{}" only in "{}"'.format(os.path.join(key), args.dir1))
        for key in d2_only:
            different = True
            print('file "{}" only in "{}"'.format(os.path.join(key), args.dir2))

    # ensure the directories contain some files
    if len(d1.keys()) == 0:
        different = True
        print('No {} files in {}'.format(args.extension, args.dir1))
    if len(d2.keys()) == 0:
        different = True
        print('No {} files in {}'.format(args.extension, args.dir2))


    for key, result in results_dict.items():
        if not result:
            different = True
            print('files "{}" and "{}" are different'.format(
                os.path.join(d1[key], key), os.path.join(d2[key], key)))

    if different:
        sys.exit(-1)
    else:
        print('No differences found in all "{}" files'.format(args.extension))
        sys.exit(0)
