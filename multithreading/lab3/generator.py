#!/usr/bin/python3

import argparse, os, sys, time, struct
from random import randint, gauss, expovariate, choice

def get_distr_func(mode, args):
    if mode == 'uniform':
        a, b = args['a'], args['b']
        if a > b: 
            a, b = b, a
        return lambda: randint(a, b)

    if mode == 'normal':
        mu, sigma = args['mean'], args['deviation']
        return lambda: round( gauss(mu, sigma) )

    if mode == 'exponential':
        lambd = args['param']
        return lambda: round( expovariate(lambd) ) 

    raise ValueError


def generate_task(rand):
    fib, pow, sort = range(3)
    task_type = choice([fib, pow, sort])
    
    if task_type == fib:
        num_values = 1
    elif task_type == pow:
        num_values = 2
    else:
        num_values = max(10, rand())
    
    values = [rand() for _ in range(num_values)]

    if task_type == pow:
        if values[0] == 0 and values[1] < 0:
            raise ValueError('zero cannot be raised to a negative power')

    return task_type, values


#----------------------------#
#---Распознание-аргументов---#
#----------------------------#
parser = argparse.ArgumentParser('generator')
parser.add_argument('count', type=int, help='The amount of numbers to generate')
subparsers = parser.add_subparsers()

uniform_subparser = subparsers.add_parser('uniform', help='Uniform distribution')
uniform_subparser.add_argument('-a', '--min', dest='a', type=int, required=True, 
    help='Minimal value')
uniform_subparser.add_argument('-b', '--max', dest='b', type=int, required=True, 
    help='Maximal value')

normal_subparser = subparsers.add_parser('normal', help='Normal distribution')
normal_subparser.add_argument('-m', '--mu', '--mean', dest='mean', type=int, required=True, 
    help='Mean value')
normal_subparser.add_argument('-s', '-d', '--sigma', '--deviation', dest='deviation', type=int, required=True, 
    help='Standard deviation')

exp_subparser = subparsers.add_parser('exponential', help='Exponential distribution')
exp_subparser.add_argument('-l', '-p', '--lambda', '--parameter', dest='param', type=float, required=True,
    help='Exponential parameter')


#----------------------------#
#------Генерация-данных------#
#----------------------------#
def main():
    if len(sys.argv) < 3:
        parser.print_help()
        exit(1)

    mode = sys.argv[2]
    args = vars(parser.parse_args(sys.argv[1:]))
    rand = get_distr_func(mode, args)

    filepath = os.path.realpath(__file__)
    directory = os.path.dirname(filepath)

    with open(directory + "/errors.txt", 'w') as log:  
        for i in range(0, args['count']):
            try:
                task, values = generate_task(rand)
                size = len(values)

                format = '@ii' + 'i' * size
                msg = struct.pack(format, task, size, *values)
                os.write(1, msg)

                time.sleep(rand() / 1000)
            except ValueError as ex:
                log.write( 'Error in task %i: %s\n' % (i, ex) )

        stop_msg = struct.pack('@ii', 3, 0)
        os.write(1, stop_msg)

if __name__ == '__main__':
   main()
