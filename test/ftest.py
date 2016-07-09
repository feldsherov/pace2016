#!/usr/bin/env python
import os
import subprocess
import sys

from sys import stdout
from optparse import OptionParser


def start_instance_testing(s, e, timeout):
    FNULL = open(os.devnull, 'w')
    for test_num in range(s, e):
        args = "./check_instance.sh", "{:03}".format(test_num)
        tester_proc = subprocess.Popen(args, stdout=FNULL, stderr=FNULL)
        try:
            tester_proc.communicate(timeout=timeout)
        except subprocess.SubprocessError:
            print("TL {:03}".format(test_num))
            os.system("pkill -TERM -P {}".format(tester_proc.pid))
        else:
            if tester_proc.returncode != 0:
                print("FAIL {:03}".format(test_num))
                return
            else:
                print("OK {:03}".format(test_num))

        stdout.flush()



if __name__ == "__main__":
    parser = OptionParser("usage: %prog filename")
    parser.add_option("-j", dest="proc_count",
                    help="count of process for testing", default=2)
    parser.add_option("-i", "--instances", dest="instances",
                    help="test on pace instances", action="store_true",
                    default=False)
    parser.add_option("-s", dest="s",
                    help="start index in range of instances",
                    type=int,
                    default=0)
    parser.add_option("-e", dest="e",
                help="start index in range of instances",
                type=int,
                default=100)
    parser.add_option("-t", dest="timeout",
                help="timeout in seconds",
                type=int,
                default=60)


    options, args = parser.parse_args()

    print(options.s, options.e)
    
    if len(args) > 1:
        testset_directory = args[0]

    if options.instances:
        start_instance_testing(options.s, options.e, options.timeout)
    else:
        print("Functionality of testing on custom testset is not implemented")
