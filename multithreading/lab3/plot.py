#!/usr/bin/python3

import matplotlib.pyplot as plt
from matplotlib.figure import Figure
import sys


def init_subplot(sp, points, label, color):
    xs, ys = zip(*points)

    sp.set_xlabel('Время (мс)')
    sp.set_ylabel('Верхний квартиль (мкс)')
    sp.bar(xs, ys, label=label, color=color)
    sp.legend()


def main():
    if len(sys.argv) < 2 or len(sys.argv) > 3:
        print("Использование: plot.py <файл метрик> [<выходной файл>].")
        exit(1)

    filename = sys.argv[1]
    if len(sys.argv) == 3:
        out_file = sys.argv[2]
    else:
        out_file = 'plots.png'

    lists = ([], [], [], [], [])
    with open(filename, 'r') as metric_file:  
        for line in metric_file:
            time, *metrics = line.strip().split()
            values = map(lambda x: (time, int(x)), metrics)
            for l, v in zip(lists, values):
                l.append(v)

    r, wp, p, ww, w = lists

    gridsize = (3, 2)

    f = plt.figure(figsize=(3*8, 2*8))

    ax_r = plt.subplot2grid(gridsize, (0, 0))
    init_subplot(ax_r, r, 'Чтение', 'green')

    ax_p = plt.subplot2grid(gridsize, (1, 0))
    init_subplot(ax_p, p, 'Обработка', 'goldenrod')

    ax_wp = plt.subplot2grid(gridsize, (1, 1))
    init_subplot(ax_wp, wp, 'Ожидание обработки', 'orange')

    ax_w = plt.subplot2grid(gridsize, (2, 0))
    init_subplot(ax_w, w, 'Запись', 'teal')

    ax_ww = plt.subplot2grid(gridsize, (2, 1))
    init_subplot(ax_ww, ww, 'Ожидание записи', 'navy')
    
    plt.savefig('plots/%s' % out_file, dpi=120, quality=95)


if __name__ == '__main__':
   main()