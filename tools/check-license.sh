#!/usr/bin/env bash

# Copyleft (C) 2013 - Sebastien Duthil <duthils@free.fr>
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


this_file_directory="$(dirname $(readlink -f $0))"

pattern_header_file="$this_file_directory/license_header.txt"

# The abstract is used to detect if the license is present at all
pattern_file_abstract="GPL HEADER"


function check_license_header_is_present_once {
    local source_file="$1"
    header_count=$(grep -i "$pattern_file_abstract" "$source_file" | wc -l | cut -f1 -d' ')
    if [ $header_count -ne 1 ] ; then
        if [ $header_count -eq 0 ] ; then
            echo "Missing license in $source_file"
        else
            echo "Multiple licenses in $source_file"
        fi
        exit 1
    fi
}

function check_license_header_is_correct {
    local source_file="$1"

    local pattern_header_length=$(wc -l "$pattern_header_file" | cut -f1 -d' ')
    local pattern_header="$(cat $pattern_header_file)"

    local actual_header="$(head -n$pattern_header_length $source_file)"

    if ! python ./compare_string_to_template.py "$pattern_header" "$actual_header" ; then
        echo "Wrong header in $source_file"
        exit 1
    fi
}

function main {
    for source_file in "$@" ; do
        echo "$source_file"
        check_license_header_is_present_once "$source_file"
        check_license_header_is_correct "$source_file"
    done
}

main "$@"
