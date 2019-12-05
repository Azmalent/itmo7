#!/usr/bin/python3

import generator, io, os, struct, subprocess

filepath = os.path.realpath(__file__)
directory = os.path.dirname(filepath)    
error_log = directory + '/errors.txt'

fib, pow, sort, stop = range(4)


def run_generator(args):
    cmd = ['./generator.py'] + args
    return subprocess.Popen(cmd, stdout=subprocess.PIPE, universal_newlines=True)


def read_messages(args):
    process = run_generator(args)

    messages = []
    while True:
        header_size = struct.calcsize('<ii')
        header_bytes = bytes( process.stdout.read(header_size), encoding='ascii' )
        msg_type, msg_argc = struct.unpack('<ii', header_bytes)

        if msg_argc > 0:
            arg_format = 'i' * msg_argc
            arg_size = struct.calcsize(arg_format)
            arg_bytes = bytes( process.stdout.read(arg_size), encoding='ascii' )
            msg_argv = list( struct.unpack(arg_format, arg_bytes) )
        else:
            msg_argv = []

        msg = (msg_type, msg_argv)
        messages.append(msg)
        
        if msg_type == stop:
            break

    process.stdout.close()
    return messages


def validate_messages(messages):
    for msg in messages:
        msg_type, msg_args = msg
        assert fib <= msg_type <= stop

        if msg_type == fib:
            assert len(msg_args) == 1
        elif msg_type == pow:
            assert len(msg_args) == 2
            b, p = msg_args
            assert not (b == 0 and p < 0)  
        elif msg_type == sort:
            assert len(msg_args) >= 10
        elif msg_type == stop:
            assert len(msg_args) == 0

    tasks, last_message = messages[:-1], messages[-1]
    assert all(msg_type != stop for msg_type, _ in tasks)
    assert last_message[0] == stop


def line_count(filename):
     with open(filename) as file:
         return sum(1 for line in file)  


#---------------#
#-----Тесты-----#
#---------------#
def test_errors():
    args = '10 uniform -a -200 -b -100'.split()
    process = run_generator(args)
    process.wait()

    assert os.path.isfile(error_log)
    assert line_count(error_log) == 10


def test_generation():
    args = '10 exponential -p 1'.split()
    messages = read_messages(args)

    assert len(messages) == 11
    validate_messages(messages)

    assert os.path.isfile(error_log)
    assert line_count(error_log) == 0
    