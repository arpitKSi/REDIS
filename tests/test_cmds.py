# test_cmds.py
#!/usr/bin/env python3
"""
Redis Command Integration Test Suite
====================================

This script contains integration tests for the custom Redis server implementation.
It tests various Redis-compatible commands through the client interface:

Commands tested:
- ZSCORE: Get score of a member in a sorted set
- ZQUERY: Range queries on sorted sets  
- ZADD: Add members to sorted sets with scores
- ZREM: Remove members from sorted sets
- ZRANK: Get rank (position) of a member
- GET/SET: Basic key-value operations
- DEL: Delete keys
- TTL operations: Time-to-live functionality

The test cases are defined in a multi-line string format with expected
input/output pairs that can be processed by a test runner.

Format:
$ ./client <command> <args>
<expected_output>

Author: Custom Redis Project
"""

CASES = r'''
$ ./client zscore asdf n1
(nil)
$ ./client zquery xxx 1 asdf 1 10
(arr) len=0
(arr) end
$ ./client zadd zset 1 n1
(int) 1
$ ./client zadd zset 2 n2
(int) 1
$ ./client zadd zset 1.1 n1
(int) 0
$ ./client zscore zset n1
(dbl) 1.1
$ ./client zquery zset 1 "" 0 10
(arr) len=4
(str) n1
(dbl) 1.1
(str) n2
(dbl) 2
(arr) end
$ ./client zquery zset 1.1 "" 1 10
(arr) len=2
(str) n2
(dbl) 2
(arr) end
$ ./client zquery zset 1.1 "" 2 10
(arr) len=0
(arr) end
$ ./client zrem zset adsf
(int) 0
$ ./client zrem zset n1
(int) 1
$ ./client zquery zset 1 "" 0 10
(arr) len=2
(str) n2
(dbl) 2
(arr) end
'''


import shlex
import subprocess

cmds = []
outputs = []
lines = CASES.splitlines()
for x in lines:
    x = x.strip()
    if not x:
        continue
    if x.startswith('$ '):
        cmds.append(x[2:])
        outputs.append('')
    else:
        outputs[-1] = outputs[-1] + x + '\n'

assert len(cmds) == len(outputs)
for cmd, expect in zip(cmds, outputs):
    out = subprocess.check_output(shlex.split(cmd)).decode('utf-8')
    assert out == expect, f'cmd:{cmd} out:{out} expect:{expect}'
