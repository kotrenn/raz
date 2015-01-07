from math import *
from random import *

dims = 3

class vector(object):
    def __init__(self, *args):
        if len(args) == 0:
            self.v = [0.0] * dims
        elif len(args) == 1:
            v = args[0]
            self.v = list(v)
        elif len(args) == 2:
            x, y = args
            if dims == 2:
                self.v = [x, y]
            else:
                self.v = [x, y, 0.0]
        elif len(args) == 3:
            x, y, z = args
            self.v = [x, y, z]
    
    def __str__(self):
        return str(self.v)
    
    def __add__(self, v):
        return vector([self.v[i] + v.v[i] for i in range(dims)])

    def __sub__(self, v):
        return vector([self.v[i] - v.v[i] for i in range(dims)])

    def __mul__(self, s):
        return vector([self.v[i] * s for i in range(dims)])

    def __div__(self, s):
        return vector([self.v[i] / s for i in range(dims)])

    def __pow__(self, v):
        return sum([self.v[i] * v.v[i] for i in range(dims)])

    def __mod__(self, v):
        if dims == 2:
            return vector([0.0, 0.0,
                           self.v[0] * v.v[1] - v.v[1] * self.v[0]])
        else:
            return vector([self.v[1] * v.v[2] - self.v[2] * v.v[1],
                           self.v[2] * v.v[0] - self.v[0] * v.v[2],
                           self.v[0] * v.v[1] - self.v[1] * v.v[0]])

    def mag(self):
        return sqrt(self ** self)

    def norm(self):
        return self / self.mag()

def print_cone(r, h, base):
    x, y, z = base.v
    print 'object mountain'
    print '  shape cone'
    print '    radius {0:.3f}'.format(r)
    print '    height {0:.3f}'.format(h)
    print '  done'
    print '  material brown'
    print '  translate {0:.3f} {1:.3f} {2:.3f}'.format(x, y, z)
    print 'done'

def mountain(r0, h0, base, first):
    if r0 <= 0.1 or h0 <= 0.1:
        return
    print_cone(r0, h0, base)

    n = randint(0, 4)
    if first:
        n += randint(2, 3)
    for i in range(n):
        r = uniform(0.1, r0)
        h = uniform(0.1, (1.0 - r / r0) * h0)
        theta = uniform(0, 2 * pi)
        x = r * cos(theta)
        y = r * sin(theta)
        z = h
        new_base = base + vector(x, y, z)
        r = (r0 - r) * uniform(0, 1)
        h = (h0 - h) * uniform(0.3, 0.8)
        mountain(r, h, new_base, False)

mountain(15, 10, vector(20, 10, 0), True)

