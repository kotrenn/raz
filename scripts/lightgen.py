from math import *

colors = [(1, 0, 0),
          (1, 1, 0),
          (0, 1, 0),
          (0, 1, 1),
          (0, 0, 1),
          (1, 0, 1)]

center = [0, 0, -4]
for i in range(6):
    theta = (1.0 * i) / 6
    theta *= 2 * pi
    r = 2
    x = r * cos(theta) + center[0]
    y = center[1] + 2
    z = r * sin(theta) + center[2]
    r, g, b = map(lambda x: x * 0.2, colors[i])
    print 'pointlight'
    print '  loc {0:.3f} {1:.3f} {2:.3f}'.format(x, y, z)
    print '  ambient {0:.1f} {1:.1f} {2:.1f}'.format(r, g, b)
    print '  diffuse {0:.1f} {1:.1f} {2:.1f}'.format(r, g, b)
    print '  specular {0:.1f} {1:.1f} {2:.1f}'.format(r, g, b)
    print 'done'
    print ''
