#!/bin/python

import sys, os, argparse

parser = argparse.ArgumentParser()
parser.add_argument('directory', type = str, help = "Directory with executables. Path must be relative to project parent directory.")
parser.add_argument('sum_results', type = str, help = "Results filename for SUM.")
parser.add_argument('update_results', type = str, help = "Results filename for UPDATE.")
args = parser.parse_args()

types = [
    "st",

    # "sts_16u",
    "sts_64u",
    "sts_256u",

    "ft",

    # "ftb_16u",
    # "ftb_64u",
    # "ftb_256u",

    # "ftt_16u",
    "ftt_64u",
    "ftt_256u"
]

def run(operation, results):
    for type in types:
        os.system("../" + args.directory + "/perf " + type + " " + operation +  " 2>> " + results)

run("sum", args.sum_results)
run("update", args.update_results)