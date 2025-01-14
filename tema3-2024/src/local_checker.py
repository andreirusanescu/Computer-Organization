#!/usr/bin/env python3

import argparse
import subprocess
import os
import sys
import re

#============================== ARGUMENTS ==============================#

argParser = argparse.ArgumentParser(description="Python Checker for PCLP2 Homework 3", prog="python3 checker.py")
group = argParser.add_mutually_exclusive_group()
group.add_argument("-t", "--task", help="Run tests for a certain task.")
group.add_argument("--all", action="store_true", help="Run all tasks.")
group.add_argument("--zip", action="store_true", help="Make zip file for VMChecker.")
group.add_argument("--linter", action="store_true", help="Run only the linter.")
argParser.add_argument("--no_clean", action="store_false", help="Do not clean outputs after run.")
args = argParser.parse_args()

if len(sys.argv) == 1:
    argParser.print_help()
    exit(0)

#============================== CONSTANTS ==============================#

tasksNo = 3

runExec = "./"
checker = "checker"
taskDir = "task-"
bonusDir = "bonus"
linterDir = "linter"
readme_path = "README"

zipName = "VMChecker_Homework_3"

useShell = True

header = "======================= Tema 3 PCLP2 =======================\n"
#============================== FUNCTIONS ==============================#

def test_task(taskNo):
    global points

    taskString = f"{taskDir}{taskNo}/"
    procString = runExec + taskString + checker
    rc = subprocess.call(f"make -C {taskString} > /dev/null 2> /dev/null", shell=useShell)

    if rc != 0:
        sys.stderr.write("make failed with status %d\n" % rc)
        return

    if not os.path.exists(procString):
        sys.stderr.write("The file %s is missing and could not be created with \'make\'" % (taskString + checker))
        return

    checkerOutput = str(subprocess.check_output(f"cd {taskString} && ./checker", shell=useShell), encoding='utf-8')

    print(checkerOutput)
    taskScore = re.findall(r'\d+\.\d+', re.findall(fr'TASK {taskNo} SCORE: \d+\.\d+', checkerOutput)[0])[0]

    points += float(taskScore)

    if args.no_clean:
        rc = subprocess.call(f"make -C {taskString} clean > /dev/null 2> /dev/null", shell=useShell)

def test_bonus():
    global points

    taskString = f"{bonusDir}/"
    procString = runExec + taskString + checker
    rc = subprocess.call(f"make -C {taskString} > /dev/null 2> /dev/null", shell=useShell)

    if rc != 0:
        sys.stderr.write("make failed with status %d\n" % rc)
        return

    if not os.path.exists(procString):
        sys.stderr.write("The file %s is missing and could not be created with \'make\'" % (taskString + checker))
        return

    checkerOutput = str(subprocess.check_output(f"cd {taskString} && ./checker", shell=useShell), encoding='utf-8')

    print(checkerOutput)
    taskScore = re.findall(r'\d+\.\d+', re.findall(fr'TASK BONUS SCORE: \d+\.\d+', checkerOutput)[0])[0]

    points += float(taskScore)

    if args.no_clean:
        rc = subprocess.call(f"make -C {taskString} clean > /dev/null 2> /dev/null", shell=useShell)

def run_linter():
    global points
    linter_points= 5.0
    source_list = ['task-1/paranthesinator.asm', 'task-2/subtask_1.asm', 'task-2/subtask_2.asm', 'task-3/task_3.asm', 'bonus/functional.asm']

    print("======================= Linter =======================\n")

    for source in source_list:
        checkerOutput = str(subprocess.check_output(f"cd {linterDir} && ./linter-script-file ../{source}", shell=useShell), encoding='utf-8')
        if len(checkerOutput) > 0:
            linter_points = 0.0
            print(f'Linter errors in: {source}')
            print(checkerOutput)

    print(f"LINTER SCORE: {linter_points} / 5.00\n")
    points += linter_points

def check_readme():
    global points
    readme_points= 5.0

    print("======================= README =======================\n")

    if not os.path.isfile(readme_path):
        readme_points = 0.0

    print(f"LINTER SCORE: {readme_points} / 5.00\n")
    points += readme_points
#=======================================================================#

if args.zip:
    rc = subprocess.call(f"zip -r {zipName} */*.asm README", shell=useShell)
    exit(rc)

points = 0

if args.linter:
    run_linter()
    exit(0)

print("\n" + header)



#================================ TESTS ================================#

if args.task == None and args.all:
    for task in range(1, tasksNo + 1):
        test_task(task)
    test_bonus()
    run_linter()
    check_readme()
elif args.task == 'b':
    test_bonus()
elif args.task != None:
    test_task(re.findall(r'\d', args.task)[0])


print(f"====================== Total: {round(points)} / 130 ======================\n")
