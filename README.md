#Raz
Timeline:  Fall 2012, Winter 2015

A 3d scene renderer which supports interactive viewing in OpenGL and realistic rendering with ray tracing.

During my time at Clemson, I learned a great deal about computer graphics.  It was one of the big things our department focused on.  A result of this is that I ended up enjoying making ray tracers.  They make pretty pictures.  They also work well for showing off.

When I went to graduate school at NC State, I took the graduate level computer graphics course for funsies.  Plus it counted for core requirements.  The class gave me more experience working with OpenGL through various projects.  One such project was the ray tracer project, where students were required to make their own rendering programs.  The basic setup was that we had to render a scene in both OpenGL and a ray tracer method, and users could press a key to switch between the two rendering modes.  I was one of the lucky couple who had experience with ray tracing, so the project basics were simple at the time.

Thankfully the professor provided many opportunities for extra credit.  Particularly, he laid out several features to implement, from setting up a camera to describing a scene with various objects.  We did still have to be able to read in and render models in .obj files, but there was opportunity to also support .mtl files, which describe surface materials.  I had fun with this, and put in my own little nifty things as well.

My main original contribution was being able to move around in the scene and look at it from different angles.  In the OpenGL mode, I based the controls around the first person controls which had become common in the game industry.  Additionally, I supported these in the ray tracing mode as well.  The result was that users could fly around a scene just like before, but it would be a little more "photo realistic" since it was being rendered using ray tracing rather than basic OpenGL methods.  The downside is that ray tracing is *slow*.  To counter this, I implemented a technique we saw in a video early on in the semester.  The idea was that when moving around, we can render everything in a much lower resolution.  Once the user has stopped moving, we can then use this time to render everything in finer detail.

A couple years later I wanted to revisit my computer graphics days and create some actual nice art.  During my undergrad I spent a night or two preparing a simple iceberg in a low-poly style reminiscent of computer graphics from the 1980's.  The resulting image can be found at http://cs.clemson.edu/~rseay/iceberg.png.  Unfortunately I no longer have access to the source code or the scene data involved.

Thus Raz was born.  I would take my accumulated knowledge of computer graphics along with my maturity as a programmer to make a general rendering program that I could use for my own artistic endeavors.  I wanted to start with the graduate course project as a simple code base, since I liked the idea of a preview mode with OpenGL.

The main goals were:

- Incorporate standard image files (.png, .jpg, ...)
- Support industry standard files (.obj, .mtl, ...)
- Experiment with computer graphics research such as volumetric rendering
- Develop an interface for ease of editing
- Add support for creating animations

School and real life put the project on indefinite hiatus.  I definitely wish to return to the project once I have the time.  Computer graphics is definitely an interesting artistic medium and the technical side provides many interesting challenges.  For now though, it will have to wait for the future.


The remainder of this readme is documentation on the controls and files provided.  This was written a while back and may not be the best.  One should also keep in mind that the current form of the codebase is INOPERABLE.  I did not finish reorganizing everything.  It is possible to hack some portions together to make it work, but at the moment, not so much.  For now, it provides some examples of my C++ code.


===================


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
