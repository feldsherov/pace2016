#!/usr/bin/env python
import os
import subprocess
import sys
import threading

from random import random
from sys import stdout
from optparse import OptionParser
import logging

FORMAT = 'test_num:%(test_num)s %(message)s'
logging.basicConfig(format=FORMAT, level=logging.INFO)

class TestingError(Exception):
     def __init__(self, value):
         self.value = value
     def __str__(self):
         return repr(self.value)


def gen_rand_graph(fout, n, p):
    for i in range(n):
        for j in range(i + 1, n):
            if random() < p:
                print(i, j, file=fout)
    fout.close()

def start_stress(n):
    FNULL = open(os.devnull, 'w')

    test_num = 0
    indent = threading.get_ident()

    logger = logging.getLogger('thread-{}'.format(indent))
    while True:
        for p in (0.1 * k for k in range(1, 10)):
            test_file_name = "stress_th{}test{}.graph".format(indent, test_num)
            ans_file_name = "stress_th{}test{}.ans".format(indent, test_num)
            slow_file_name = "stress_th{}test{}.slow_ans".format(indent, test_num)
            fout = open(test_file_name, "w")
            gen_rand_graph(fout, n, p)

            solve_args = ("../src/fsolve", )
            test_in = open(test_file_name, "r")
            test_out = open(ans_file_name, "w")
            solve_proc = subprocess.Popen(solve_args, stdin=test_in, stdout=test_out, stderr=FNULL)

            test_in = open(test_file_name, "r")
            slow_out = open(slow_file_name, "w")
            slow_args = ("../src/fslow", )
            slow_proc = subprocess.Popen(slow_args, stdin=test_in, stdout=slow_out)

            slow_proc.wait()
            solve_proc.wait()

            check_args = ("../src/fcheck", test_file_name, ans_file_name)
            check_proc = subprocess.Popen(check_args, stdout=FNULL, stderr=FNULL)
            check_proc.wait()

            extra_data = {"test_num":test_num}
            if check_proc.returncode != 0 or solve_proc.returncode != 0:
                format_str = "Thread-%s: solve_proc return %s || check_proc return %s"
                logger.critical(format_str, indent, solve_proc.returncode, check_proc.returncode, extra=extra_data)
                raise TestingError("Checker return not 0")

            slow_ans = open(slow_file_name, "r")
            ans = open(ans_file_name, "r")

            ans_size = len(ans.readlines())
            slow_ans_size = int(slow_ans.readline())
            if ans_size != slow_ans_size:
                format_str = "Thread-%s: ans_size %s || slow_ans_size %s"
                logger.critical(format_str, indent, ans_size, slow_ans_size, extra=extra_data)
                raise TestingError("fsolve and fslow have different answer size")

            format_str = "Thread-%s: OK"
            logger.info(format_str, indent, extra=extra_data)
            logger
            test_num += 1


if __name__ == "__main__":
    os.chdir("../var")

    parser = OptionParser("usage: %prog filename")
    parser.add_option("-j", dest="proc_count",
                    type=int,
                    help="count of process for testing", default=2)
    parser.add_option("-n", dest="n",
                    type=int,
                    help="size of graph",
                    default=10)

    options, args = parser.parse_args()

    thread_pool = list()
    
    for i in range(options.proc_count):
        thread_pool.append(threading.Thread(target=start_stress,
                                            args=(options.n, )) )
        thread_pool[-1].start()
