#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
check that the names of the compiled libraries follow the naming rules agreed on
with the customer (all libs contain a suffix)
"""
################################################################################
###
### @file       libnamecheck.py
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
from pathlib import PurePath

if __name__ == "__main__":
    HELP_STR = \
"""
This program walks through the supplied directory and checks that files with a
certain extension have an acceptable suffix.

Example:
python tools/libnamecheck.py --blacklist libs/third_party/Metaware/bin --blacklist libs/security --suffix _fpga_dev
"""
    parser = argparse.ArgumentParser(description=HELP_STR,
        formatter_class=argparse.RawTextHelpFormatter)
    parser.add_argument('dir', help='directory tree to serch for files with the given extension')
    parser.add_argument('extension',
        help='extension of files we want to compare (.hex by default)',
        nargs='?',
        default='.a')
    parser.add_argument('-b', '--blacklist', action="append",
        help='files with a path in the blacklist will not be checked. Note: the blacklist can only contain folders or files, no globs or partial names')
    parser.add_argument('-s', '--suffix', action='append')
    args = parser.parse_args()

    if not os.path.isdir(args.dir):
        print("{} is not a directory".format(args.dir), file=sys.stderr)
        sys.exit(-1)

    if args.suffix is None:
        print("missing suffix")
        sys.exit(-1)

    blacklist = [PurePath(item) for item in args.blacklist] if args.blacklist else []

    def in_blacklist(file_dir, file_name):
        for item in blacklist:
            if set(item.parts) <= set(file_dir.parts):
                return True
        return False

    def good_suffix(file):
        for suffix in args.suffix:
            if file.endswith(suffix + args.extension):
                return True
        return False

    exit = 0
    for root, subFolder, files in os.walk(args.dir):
        root = PurePath(root)
        for file in files:
            if file.endswith(args.extension) and not in_blacklist(root, file):
                if not good_suffix(file):
                    print("Library has bad suffix: {}".format(root.joinpath(file)))
                    exit = -1

    sys.exit(exit)
