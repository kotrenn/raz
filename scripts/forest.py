from random import *

R = 20
N = 30

def tree():
    x = uniform(-R, R)
    y = uniform(-R, R)
    h_bot = uniform(1.5, 3.0)
    h_top = uniform(1.2, 1.8) * h_bot
    r1 = uniform(0.2, 0.6)
    r2 = uniform(2.0, 2.2) * r1
    print 'object tree_bot'
    print '  shape cylinder'
    print '    radius {0:.3f}'.format(r1)
    print '    height {0:.3f}'.format(h_bot)
    print '  done'
    print '  material brown'
    print '  translate {0:.3f} {1:.3f} {2:.3f}'.format(x, y, 0)
    print 'done'
    print ''
    print 'object tree_top'
    print '  shape cone'
    print '    radius {0:.3f}'.format(r2)
    print '    height {0:.3f}'.format(h_top)
    print '  done'
    print '  material green'
    print '  translate {0:.3f} {1:.3f} {2:.3f}'.format(x, y, h_bot)
    print 'done'
    print ''

for i in range(N):
    tree()
