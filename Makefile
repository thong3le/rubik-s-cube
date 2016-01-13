
final:	main.c Draw.h Draw.c TrackballUI.h TrackballUI.c Vec.h Vec.c Rubik.h Rubik.c
	cc main.c Draw.c Rubik.c TrackballUI.c Vec.c -framework OpenGL -framework GLUT -o final
