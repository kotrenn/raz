#include <windows.h>

#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

/*--------------------------------------------------------------------------*/
/*  GLUT_DEMO.C    */
/*    Demonstration of GLUT program with an OpenGL canvas running under    */
/*    Windows 2000    */
/*    */
/*- Modification History: --------------------------------------------------*/
/*  When:Who:Comments:    */
/*  12-Jun-98Christopher G. HealeyInitial implementation    */
/*--------------------------------------------------------------------------*/

//  Module global prototypes

void  display_obj( void );
void  draw_string( float, float, float, char * );
void  handle_menu( int );
void  handle_motion( int, int );
void  handle_mouse( int, int, int, int );

//  Module global variables

typedef enum {
	TRANSLATE,
	ROTATE_X,
	ROTATE_Y,
	  ROTATE_Z
} mode;

int    btn[ 3 ] = { 0 };// Current button state
mode   cur_mode = TRANSLATE;// Current mouse mode
float  translate[ 3 ] = { 0 };// Current translation values
float  rotate[ 3 ] = { 0 };// Current rotation values
int    mouse_x, mouse_y;// Current mouse position


void display_obj( void )

//  This routine displays a Goroud-shaded RGB cube
{
	float c[ 4 ][ 3 ] = {// Axis colours
		{ 1, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 1 },
		{ 1, 1, 1 }
	};
	int i;// Loop counters
	char  *txt[ 3 ] = {// Axis labels
		"+X", "+Y", "+Z"
	};
	float  v[ 4 ][ 3 ] = {// Vertex positions
		{ .5,  0,  0 },
		{  0, .5,  0 },
		{  0,  0, .5 },
		{  0,  0,  0 }
	};


	glMatrixMode( GL_MODELVIEW );// Setup model transformations
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glPushMatrix();
	glTranslatef( translate[ 0 ], translate[ 1 ], translate[ 2 ] );
	glRotatef( rotate[ 0 ], 1, 0, 0 );
	glRotatef( rotate[ 1 ], 0, 1, 0 );
	glRotatef( rotate[ 2 ], 0, 0, 1 );

	glColor3f( 1, 1, 0 );
	glutWireCube( 2.0 );

	for( i = 0; i < 3; i++ ) {// Draw axis at center of cube
		glBegin( GL_LINES );
		glColor3fv( c[ i ] );
		glVertex3fv( v[ i ] );
		glColor3fv( c[ 3 ] );
		glVertex3fv( v[ 3 ] );
		glEnd();
	}

	glColor3f( 1, 1, 1 );
	draw_string( v[ 0 ][ 0 ], v[ 0 ][ 1 ], v[ 0 ][ 2 ], txt[ 0 ] );
	draw_string( v[ 1 ][ 0 ], v[ 1 ][ 1 ], v[ 1 ][ 2 ], txt[ 1 ] );
	draw_string( v[ 2 ][ 0 ], v[ 2 ][ 1 ], v[ 2 ][ 2 ], txt[ 2 ] );

	glPopMatrix();
	glFlush();// Flush OpenGL queue

	glutSwapBuffers();// Display back buffer
}// End routine display_obj


void draw_string( float x, float y, float z, char *txt )

//  This routine draws the text string at the given (x,y,z) position
//
//  x,y,z:  Raster position for text
//  txt:    String to draw
{
	glRasterPos3f( x, y, z );
	while( *txt != '\0' ) {
		glutBitmapCharacter( GLUT_BITMAP_9_BY_15, *txt );
		txt++;
	}
}// End routine draw_string


void handle_menu( int ID )

//  This routine handles popup menu selections
//
//  ID:  Menu entry ID
{
	switch( ID ) {
	case 0:// XY translation
		cur_mode = TRANSLATE;
		break;
	case 1:// X rotation
		cur_mode = ROTATE_X;
		break;
	case 2:// Y rotation
		cur_mode = ROTATE_Y;
		break;
	case 3:// Z rotation
		cur_mode = ROTATE_Z;
		break;
	case 4:// Quit
		exit( 0 );
	}
}// End routine handle_menu


void handle_motion( int x, int y )

//  This routine acts as a callback for GLUT mouse motion events
//
//  x, y:  Cursor position
{
	float x_ratchet;// X ratchet value
	float y_ratchet;// Y ratchet value


	if ( !btn[ 0 ] ) {// Left button not depressed?
		return;
	}

	x_ratchet = glutGet( GLUT_WINDOW_WIDTH ) / 10.0;
	y_ratchet = glutGet( GLUT_WINDOW_HEIGHT ) / 10.0;

	//  Windows XP has y = 0 at top, GL has y = 0 at bottom, so reverse y

	y = glutGet( GLUT_WINDOW_HEIGHT ) - y;

	switch( cur_mode ) {
	case TRANSLATE:// XY translation
		translate[ 0 ] += (float) ( x - mouse_x ) / x_ratchet;
		translate[ 1 ] += (float) ( y - mouse_y ) / y_ratchet;
		break;
	case ROTATE_X:// X rotation
		x_ratchet /= 10.0;
		rotate[ 0 ] += (float) ( x - mouse_x ) / x_ratchet;
		break;
	case ROTATE_Y:// Y rotation
		x_ratchet /= 10.0;
		rotate[ 1 ] += (float) ( x - mouse_x ) / x_ratchet;
		break;
	case ROTATE_Z:// Z rotation
		x_ratchet /= 10.0;
		rotate[ 2 ] += (float) ( x - mouse_x ) / x_ratchet;
		break;
	}

	mouse_x = x;// Update cursor position
	mouse_y = y;

	glutPostRedisplay();
}// End routine handle_motion


void handle_mouse( int b, int s, int x, int y )

//  This routine acts as a callback for GLUT mouse events
//
//  b:     Mouse button (left, middle, or right)
//  s:     State (button down or button up)
//  x, y:  Cursor position
{
	if ( s == GLUT_DOWN ) {// Store button state if mouse down
		btn[ b ] = 1;
	} else {
		btn[ b ] = 0;
	}

	mouse_x = x;
	mouse_y = glutGet( GLUT_WINDOW_HEIGHT ) - y;
}// End routine handle_mouse


int main( int argc, char *argv[] )
{
	glutInit( &argc, argv );// Initialize GLUT
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutCreateWindow( "GLUT/XP Demo" );

	glutDisplayFunc( display_obj );// Setup GLUT callbacks
	glutMotionFunc( handle_motion );
	glutMouseFunc( handle_mouse );

	glutCreateMenu( handle_menu );// Setup GLUT popup menu
	glutAddMenuEntry( "Translate", 0 );
	glutAddMenuEntry( "Rotate X", 1 );
	glutAddMenuEntry( "Rotate Y", 2 );
	glutAddMenuEntry( "Rotate Z", 3 );
	glutAddMenuEntry( "Quit", 4 );
	glutAttachMenu( GLUT_RIGHT_BUTTON );

	glMatrixMode( GL_PROJECTION );// Setup perspective projection
	glLoadIdentity();
	gluPerspective( 70, 1, 1, 40 );

	glMatrixMode( GL_MODELVIEW );// Setup model transformations
	glLoadIdentity();
	gluLookAt( 0, 0, 5, 0, 0, -1, 0, 1, 0 );

	glShadeModel( GL_SMOOTH );

	glClearDepth( 1.0 );// Setup background colour
	glClearColor( 0, 0, 0, 0 );
	glEnable( GL_DEPTH_TEST );

	glutMainLoop();// Enter GLUT main loop
}// End mainline
