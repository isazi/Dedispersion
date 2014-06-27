#!/usr/bin/env python
# Copyright 2014 Alessio Sclocco <a.sclocco@vu.nl>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import sys
import pymysql
import config
import manage
import export

if len(sys.argv) == 1:
    print("Supported commands are: create, delete, load, tune, tuneNoReuse, statistics")
    sys.exit(1)

COMMAND = sys.argv[1]

DB_CONN = pymysql.connect(host=config.MY_HOST, port=config.MY_PORT, user=config.MY_USER, passwd=config.MY_PASS, db=config.MY_DB)
QUEUE = DB_CONN.cursor()

if COMMAND == "create":
    if len(sys.argv) < 3 or len(sys.argv) > 4:
        print("Usage: " + sys.argv[0] + " create <table> [opencl]")
        QUEUE.close()
        DB_CONN.close()
        sys.exit(1)
    try:
        if 'opencl' in sys.argv:
            manage.create_table(QUEUE, sys.argv[2], True)
        else:
            manage.create_table(QUEUE, sys.argv[2], False)
    except pymysql.err.InternalError:
        pass
elif COMMAND == "delete":
    if len(sys.argv) != 3:
        print("Usage: " + sys.argv[0] + " delete <table>")
        QUEUE.close()
        DB_CONN.close()
        sys.exit(1)
    try:
        manage.delete_table(QUEUE, sys.argv[2])
    except pymysql.err.InternalError:
        pass
elif COMMAND == "load":
    if len(sys.argv) < 4 or len(sys.argv) > 5:
        print("Usage: " + sys.argv[0] + " load <table> <input_file> [opencl]")
        QUEUE.close()
        DB_CONN.close()
        sys.exit(1)
    INPUT_FILE = open(sys.argv[3])
    try:
        if "opencl" in sys.argv:
            manage.load_file(QUEUE, sys.argv[2], INPUT_FILE, True)
        else:
            manage.load_file(QUEUE, sys.argv[2], INPUT_FILE, False)
    except:
        print(sys.exc_info())
        sys.exit(1)
elif COMMAND == "tune":
    if len(sys.argv) < 6 or len(sys.argv) > 8:
        print("Usage: " + sys.argv[0] + " tune <table> <operator> <channels> <samples> [opencl] [all|local]")
        QUEUE.close()
        DB_CONN.close()
        sys.exit(1)
    try:
        FLAGS = [False, False, False]
        if "opencl" in sys.argv:
            FLAGS[0] = True
            if "all" in sys.argv:
                FLAGS[1] = True
            elif "local" in sys.argv:
                FLAGS[2] = True
        CONFS = export.tune(QUEUE, sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5], FLAGS)
        export.print_results(CONFS)
    except:
        print(sys.exc_info())
elif COMMAND == "tuneNoReuse":
    if len(sys.argv) < 6 or len(sys.argv) > 7:
        print("Usage: " + sys.argv[0] + " tuneNoReuse <table> <operator> <channels> <samples> [opencl]")
        QUEUE.close()
        DB_CONN.close()
        sys.exit(1)
    try:
        if "opencl" in sys.argv:
            CONFS = export.tune_no_reuse(QUEUE, sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5], True)
        else:
            CONFS = export.tune_no_reuse(QUEUE, sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5], False)
        export.print_results(CONFS)
    except pymysql.err.ProgrammingError:
        pass
    except:
        print(sys.exc_info())
        sys.exit(1)
elif COMMAND == "statistics":
    if len(sys.argv) != 5:
        print("Usage: " + sys.argv[0] + " statistics <table>, <channels>, <samples>")
        QUEUE.close()
        DB_CONN.close()
        sys.exit(1)
    try:
        CONFS = export.statistics(QUEUE, sys.argv[2], sys.argv[3], sys.argv[4])
        export.print_results(CONFS)
    except pymysql.err.ProgrammingError:
        pass
    except:
        print(sys.exc_info())
        sys.exit(1)
else:
    print("Unknown command.")
    print("Supported commands are: create, delete, load, tune, tuneNoReuse, statistics")

QUEUE.close()
DB_CONN.commit()
DB_CONN.close()
sys.exit(0)

