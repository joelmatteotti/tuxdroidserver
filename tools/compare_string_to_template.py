# -*- coding: utf-8 -*-

# Copyright 2013 - Sebastien Duthil <duthils@free.fr>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.

import re
import sys

token_source_file_name = 'SOURCEFILENAME'
token_copyright_years = 'COPYRIGHTYEARS'

def make_regexp(pattern_line):
    pattern_line = re.escape(pattern_line)
    pattern_line = pattern_line.replace(token_source_file_name, '\w+\.\w+')
    pattern_line = pattern_line.replace(token_copyright_years, '\d{4}(-\d{4})?')
    return re.compile(pattern_line)

def pattern_matches(pattern, string_to_compare):
    pattern_lines = pattern.split('\n')
    compared_lines = string_to_compare.split('\n')
    assert len(pattern_lines) == len(compared_lines)

    for line_index in range(len(pattern_lines)):
        pattern_line = pattern_lines[line_index]
        compared_line = compared_lines[line_index]
        if not _line_matches(pattern_line, compared_line):
            print('expected: {0}'.format(pattern_line))
            print('actual: {0}'.format(compared_line))
            return False
    return True

def _line_matches(pattern_line, compared_line):
    line_regexp = make_regexp(pattern_line)
    return line_regexp.match(compared_line)

if __name__ == '__main__':
    pattern = sys.argv[1]
    string_to_compare = sys.argv[2]
    if pattern_matches(pattern, string_to_compare):
        exit(0)
    else:
        exit(1)
