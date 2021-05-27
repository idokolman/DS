#!/bin/python
import os, sys
from subprocess import call
from shutil import copyfile

prefix = ''
def run_test_suite(program):
    work_dir = os.getcwd()
    test_dir = os.path.join(work_dir, 'tests')
    if not os.path.isdir(test_dir): return

    def cmp(path1, path2):
        with open(path1, 'r') as f1, open(path2, 'r') as f2:
            identical = f1.readlines() == f2.readlines()
        return identical
    
    def run_and_diff(program, newput, input, output = ''):
        diff = ''
        if not os.path.isfile(output):
            with open(input, 'r') as infile, open(output, 'w') as outfile:
                print(f'[{os.path.basename(input)}] ATTENTION: CREATING NEW TEST!')
                call(os.path.join(work_dir, program), stdin=infile, stdout=outfile)
            return (0, 1, diff)

        with open(input, 'r') as infile, open(newput, 'w') as newfile:
            call(os.path.join(work_dir, program), stdin=infile, stdout=newfile)

        if not cmp(output, newput):
            diff += f'[{os.path.basename(input)}]'.center(24, '_') + ' test failed!\n'
            #with open(input, 'r')  as f: diff += ''.join(f.readlines())+'\n\n'
            #diff += f'[{os.path.basename(newput)}]'.center(24) + '\n'
            #with open(newput, 'r') as f: diff += ''.join(f.readlines())+'\n\n'
            #diff += f'[{os.path.basename(output)}]'.center(24) + '\n'
            #with open(output, 'r') as f: diff += ''.join(f.readlines())+'\n\n'
            return (0, 0, diff)
        return (1, 0, diff)
    
    if os.path.isfile(program):
        diff_output = ''
        counter = 0
        new_counter = 0
        success_counter = 0
        for f in os.listdir(test_dir):
            if f.startswith(prefix + "in") and f.endswith(".txt"):
                counter += 1
                input  = os.path.join(test_dir, f)
                newput = os.path.join(test_dir, f.replace("in", "my"))
                output = os.path.join(test_dir, f.replace("in", "out"))
                success, new, diff = run_and_diff(program, newput, input, output)
                #if success:
                #    os.remove(newput)
                success_counter += success; new_counter += new; diff_output += diff

        if counter == (success_counter + new_counter):
            print(f'passed {success_counter} out of {counter} tests, {new_counter} new tests'.ljust(50, '.') + ('OK' if success_counter > 0 else 'N/A').rjust(10,'.'))
        else:
            print(f'passed {success_counter} out of {counter} tests, {new_counter} new tests'.ljust(50, '.') + 'Oh No!'.rjust(10,'.'))

        print(diff_output, end='')
                    
if __name__ == '__main__':
    if len(sys.argv) == 2:
        run_test_suite(sys.argv[1])
    elif len(sys.argv) == 3:
        prefix = sys.argv[2]
        run_test_suite(sys.argv[1])
    else:
        print(f'Usage: autotest <executable> [test_prefix]')
        exit(1)
