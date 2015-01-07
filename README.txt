A scene renderer in OpenGL

Author: Ramona Seay
        rgseay@ncsu.edu

Renders a scene.  Pressing the space bar will switch between
using OpenGL to render and using ray tracing to render.  Pressing
space again will switch back.

The WASD keys and mouse can be used to move the camera around.
If the mouse is used then the left button must be held down while
moving to affect the camera.  In addition the Q and E keys can
be used to move the camera up and down.

The R key will refresh the ray traced render when it is in view.
When the program starts no render is available and R must be
pressed when switching to the view for the first time.

The camera can be moved in both modes.  If it is moved in the
ray tracing mode then the scene will be rendered at a lower
resolution until no input is received, at which point a full
resolution render will be provided.  In addition no rendering
will take place while moving the camera in the OpenGL mode.




Files:

window.txt:
  Lists the dimensions of the viewing window
  
view.txt:
  Specifies the location and orientation of the camera
  
project.txt:
  Specifies the viewing frustum

reflect.txt:
  Specifies the maximum number of reflections.  Defaults to
  0 for no reflections.

lights.txt:
  Specifies the locations and colors of the lights

world.txt:
  Describes the scene to render

The world file format can include .mtl files with the mtllib
line, such as "mtllib test.mtl".  The rest of the file is
used to place objects in the scene.  Each object has the form
"object <name> <attributes> done" where name is the name of
the object (currently has no use).  The first attribute must
be the type of shape the object is.  Accepted objects are:
  sphere
    A sphere of radius <radius> centered at the origin
  plane
    An infinite plane which intersects <point> and has
    normal <normal>
  cone
    A cone with a base of radius <radius> and height <height>
    with the base centered at the origin and parallel to
    the z-axis
  cylinder
    A cylinder with a base of radius <radius> and height
    <height> with the base centered at the origin and parallel
    to the z-axis
  torus
    A torus with the radius of the "tube" being <inner> and
    the distance from the center to the center of the tube
    being <outer>, with the torus centered at the origin and
    the axis through the center being paralle to the z-axis
  model
    A model read in from the .obj file <file>
Any <att> listed above is a shape attribute listed following
the declaration of the shape.  Attributes are finished when
done appears.  Other attributes include the material for the
object and any transformations necessary to place the shape
in its correct position and orientation.



Notes:

Vector normals are interpolated on .obj files if normals were
specified at the vertices.  If there are none then the normal
is assumed to be the normal of the face.  The input files
icosahedron.obj and ball.obj illustrate this--the latter
includes vertex normals while the former does not.  As a
result ball.obj has smooth shading while icosahedron.obj has
flat shading.

You can try using some of the other models but if you try to
ray trace them I suggest setting the resolution to low or
getting yourself a really nice cup of coffee.  Or only look
at them in OpenGL mode.

If you want a good example of moving the camera while ray
tracing I suggest only rendering a couple simple objects at
a time, such as a plane and a sphere or two.  This way the
intended effect can be seen more properly.  Also using a
computer better than mine helps.

Shadows are also implemented.
