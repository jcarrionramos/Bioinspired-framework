# coding=utf-8
import pandas
import os.path
import statistics
"""Para usar este script, modificar 3 cosas: el sub_path que es el path
donde está ubicado el csv a procesar, el n con el que se llama la funcion
write_file(n) que es el numero de csv a procesar en el path y el path en la
variable f que es el path donde se guardara el csv con las respuestas."""
def last_value(l):
    return l[len(l) - 1]

def first_index(l):
    for i in range(len( l)):
        if l[i] > 0:
            return i
    return 0

sub_path = "/Users/jcarrion/Documents/codes/EvoGro/C++/Crossover/10/"
def n_last_gfp(n):
    l = [] 
    extension = ".csv"
    for i in range(n):
        path = sub_path + "simu" + str(i + 1) + extension
        df = pandas.read_csv(path, names = ['t', 'gfp'])
        l.append(last_value(df.gfp))
    return l

def n_first_gfp(n):
    l = []
    extension = ".csv"
    for i in range(n):
        path = sub_path + "simu" +  str(i + 1) + extension
        df = pandas.read_csv(path, names = ['t', 'gfp'])
        l.append(first_index(df.gfp))
    return l


def write_output(n):
    f = open(os.path.expanduser(os.path.join(sub_path,"out" + ".csv")), "a+")
    l1 = n_last_gfp(n)
    l3 = n_first_gfp(n)
    media1 = statistics.mean(l1)
    std1 = statistics.stdev(l1)
    media1 = format(media1, '.3f')
    std1 = format(std1, '.3f')
    media3 = statistics.mean(l3)
    media3 = format(media3, '.3f')
    s = str(media1) +', '+str(media3)+', '+str(std1)+'\n'
    f.write(s)
    
write_output(30)
