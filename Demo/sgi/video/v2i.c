/* Convert the first image of a CMIF video movie file to SGI .rgb format.
   usage: v2i videofile imagefile [planemask]
   link with -limage
*/

#include <stdio.h>
#include <gl/image.h>

long bm[1280];
short rb[1280], gb[1280], bb[1280];
long w, h, pf;

#define R(comp) ((comp) & 0xff)
#define G(comp) (((comp)>>8) & 0xff)
#define B(comp) (((comp)>>16) & 0xff)

main(argc, argv)
    char **argv;
{
    char lbuf[100];
    int x, y;
    int i;
    IMAGE * of;
    int pmask;

    if( argc != 3 && argc != 4) {
       fprintf(stderr, "Usage: v2i videofile imgfile [planemask]\n");
       exit(2);
    }
    if ( argc == 4)
	pmask = atoi(argv[3]);
    else
	pmask = 7;
    if ( freopen(argv[1], "r", stdin) == NULL ) {
	perror(argv[1]);
	exit(1);
    }
    if (fgets(lbuf, sizeof lbuf, stdin) == NULL) {
	    fprintf(stderr, "Immediate EOF\n");
	    exit(1);
    }
    if (strncmp(lbuf, "CMIF", 4) == 0) {
	    /* Skip optional header line */
	    if (fgets(lbuf, sizeof lbuf, stdin) == NULL) {
		    fprintf(stderr, "Immediate EOF after header\n");
		    exit(1);
	    }
    }
    pf = 2; /* Default */
    if ( sscanf(lbuf, "(%d,%d,%d)", &w, &h, &pf) < 2) {
	fprintf(stderr, "%s: bad size spec: %s\n", argv[0], lbuf);
	exit(1);
    }
    fgets(lbuf, sizeof lbuf, stdin); /* Skip time info */
    if ( w > 1280 ) {
	fprintf(stderr, "%s: Sorry, too wide\n", argv[0]);
	exit(1);
    }
    if ( (of=iopen(argv[2], "w", RLE(1), 3, w, h, 3)) == 0) {
	perror(argv[2]);
	exit(1);
    }
    for( y=0; y<h; y++) {
	if( fread(bm, sizeof(long), w, stdin) != w) {
	    fprintf(stderr, "%s: short read\n", argv[0]);
	    exit(1);
	}
	for( x=0; x<w; x++) {
	    if ( pmask & 1) rb[x] = R(bm[x]);
	    if ( pmask & 2) gb[x] = G(bm[x]);
	    if ( pmask & 4) bb[x] = B(bm[x]);
	}
	putrow(of, rb, y, 0);
	putrow(of, gb, y, 1);
	putrow(of, bb, y, 2);
    }
    iclose(of);
    exit(0);
}
