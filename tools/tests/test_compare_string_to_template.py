# -*- coding: utf-8 -*-

import re

import unittest
from mock import patch

import compare_string_to_template


class TestMakeRegexp(unittest.TestCase):
    def setUp(self):
        pass

    def test_make_regexp_empty(self):
        regexp = compare_string_to_template.make_regexp('')

        self.assertIsNotNone(regexp.match(''))
        self.assertIsNotNone(regexp.match('abc'))

    def test_make_regexp_something(self):
        regexp = compare_string_to_template.make_regexp('abc')

        self.assertIsNone(regexp.match(''))
        self.assertIsNone(regexp.match('ab'))
        self.assertIsNotNone(regexp.match('abc'))
        self.assertIsNotNone(regexp.match('abcd'))
        self.assertIsNone(regexp.match('bcd'))

    def test_make_regexp_special_characters(self):
        regexp = compare_string_to_template.make_regexp('.*')

        self.assertIsNone(regexp.match(''))
        self.assertIsNone(regexp.match('abc'))
        self.assertIsNotNone(regexp.match('.*'))

    def test_make_regexp_source_file_name(self):
        regexp = compare_string_to_template.make_regexp(
            compare_string_to_template.token_source_file_name)

        self.assertIsNone(regexp.match(''))
        self.assertIsNone(regexp.match('abc'))
        self.assertIsNone(regexp.match('file.'))
        self.assertIsNone(regexp.match('.extension'))
        self.assertIsNotNone(regexp.match('file.extension'))

    def test_make_regexp_source_copyright_years(self):
        regexp = compare_string_to_template.make_regexp(
            compare_string_to_template.token_copyright_years)

        self.assertIsNone(regexp.match(''))
        self.assertIsNone(regexp.match('abc'))
        self.assertIsNone(regexp.match('1'))
        self.assertIsNotNone(regexp.match('2011'))
        self.assertIsNotNone(regexp.match('2011-2012'))


class TestPatternMatches(unittest.TestCase):

    @patch('compare_string_to_template.make_regexp')
    def test_pattern_matches_empty(self, make_regexp):
        make_regexp.return_value = re.compile('')
        result = compare_string_to_template.pattern_matches('', '')
        self.assertTrue(result)
        result = compare_string_to_template.pattern_matches('', 'abc')
        self.assertTrue(result)

    def test_pattern_matches_not_same_length(self):
        multiline_pattern = '''abc
                            def'''
        matched = 'abc'
        self.assertRaises(AssertionError,
                          compare_string_to_template.pattern_matches,
                          multiline_pattern,
                          matched)

    @patch('compare_string_to_template.make_regexp')
    def test_pattern_matches_multiline_right(self, make_regexp):
        pattern_first_line = 'abcd?'
        pattern_second_line = '.ef'
        pattern = '\n'.join([pattern_first_line, pattern_second_line])
        matched = '\n'.join(['abc', 'def'])

        make_regexp.side_effect = [re.compile(pattern_first_line),
                                   re.compile(pattern_second_line)]
        result = compare_string_to_template.pattern_matches(pattern, matched)
        self.assertTrue(result)

    @patch('compare_string_to_template.make_regexp')
    def test_pattern_matches_multiline_wrong(self, make_regexp):
        pattern_first_line = 'abc'
        pattern_second_line = 'def'
        pattern = '\n'.join([pattern_first_line, pattern_second_line])
        matched = '\n'.join(['123', '456'])

        make_regexp.side_effect = [re.compile(pattern_first_line),
                                   re.compile(pattern_second_line)]
        result = compare_string_to_template.pattern_matches(pattern, matched)
        self.assertFalse(result)
