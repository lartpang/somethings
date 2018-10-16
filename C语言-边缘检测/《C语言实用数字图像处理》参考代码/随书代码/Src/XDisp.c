#include 	<stdio.h>
#include 	<stdlib.h>
#include 	<X11/Xlib.h>
#include 	<X11/Xutil.h>
#include 	"Params.h"

#define WMAIN	0    	/* ���͉摜		*/
#define WPREV	1    	/* �o�͉摜		*/
#define WSUB	2    	/* ���[�N�摜	*/
#define	WKEY	3    	/* �L�[�摜		*/

#define	WMAIN_POSX	10		/* ���͉摜�̉������\���ʒu		*/
#define	WMAIN_POSY	400		/* ���͉摜�̏c�����\���ʒu		*/
#define	WPREV_POSX	145		/* �o�͉摜�̉������\���ʒu		*/
#define	WPREV_POSY	400		/* �o�͉摜�̏c�����\���ʒu		*/
#define	WSUB_POSX	280		/* ���[�N�摜�̉������\���ʒu	*/
#define	WSUB_POSY	400		/* ���[�N�摜�̏c�����\���ʒu	*/
#define	WKEY_POSX	415		/* �L�[�摜�̉������\���ʒu		*/
#define	WKEY_POSY	400		/* �L�[�摜�̏c�����\���ʒu		*/

#define	GRAY	1	/* ���m�N���[���[�h	*/
#define	COLOR	2	/* �J���[���[�h		*/

typedef struct _ximage {	/* 	�摜�p�\���� */
	Window 	win;
	XImage 	*im;
	Pixmap 	pix;
} XIMAGE;
static int 	error[X_SIZE], error1[X_SIZE], error2[X_SIZE]; 

void InitVisual(void);
void InitWindows(void);
void InitWindow(XIMAGE *ximage, int posi_x, int posi_y);
void ColorDisplay(XIMAGE ximage, unsigned char imager[Y_SIZE][X_SIZE],
	unsigned char imageg[Y_SIZE][X_SIZE], \
	unsigned char imageb[Y_SIZE][X_SIZE], char wtitle[]);
void dither(unsigned char image_in[Y_SIZE][X_SIZE],
	unsigned char image_out[Y_SIZE][X_SIZE], int nq);
void d_quantize(int in, unsigned char *pout, int nq);
int d_iquantize(unsigned char in, int nq);

Display *d;				/* �\����ʗp�|�C���^				*/
Visual 	*vis;			/* �\����ʂ̏��					*/
GC 		gc;				/* �`���							*/
int		depth;			/* ��f������̃r�b�g��				*/

XIMAGE	ximage_main;		/* ���͉摜�̍\����		*/
XIMAGE 	ximage_prev;		/* �o�͉摜�̍\����		*/
XIMAGE	ximage_sub;	/* ���[�N�摜�̍\����	*/
XIMAGE	ximage_key;		/* �L�[�摜�̍\����		*/

/*--- InitVisual --- �J���[��ʂ̐ݒ�----------------------------------------*/
void InitVisual(void)
{
	int n;
	unsigned long vmask;
	XVisualInfo *xvinfo, vtemp;

	d = XOpenDisplay(NULL);
	if (d == NULL) {
		printf("cannot open Display. \n");
		exit(0);
	}
	vtemp.class = TrueColor;
	vtemp.depth = 32;
	xvinfo = XGetVisualInfo(d, VisualClassMask | VisualDepthMask, &vtemp, &n);
	if (n) {
		vis = xvinfo->visual;
		depth = xvinfo->depth;
	} else {
		vis = DefaultVisual(d, 0);
		depth = DefaultDepth(d, 0);
	}
}

/*--- InitDisplay --- ��ʕ\���̏������i���m�N���j---------------------------*/
void InitDisplay(void)
{
	InitVisual();
	if (vis->class != TrueColor && vis->class != DirectColor) {
		printf("--Not supported X-window mode !\n");
		exit(0);
	}
	InitWindows();
}

/*--- InitCDisplay --- ��ʕ\���̏������i�J���[�j----------------------------*/
void InitCDisplay(void)
{
	InitVisual();
	if (vis->class != TrueColor && vis->class != DirectColor) {
		printf("--Not supported X-window mode !\n");
		exit(0);
	}
	InitWindows();
}

/*--- DisplayImage --- ��ʂ̕\���i���m�N���j----------------------------------
	image:		�摜�f�[�^
	position:	�\���E�B���h�E
-----------------------------------------------------------------------------*/
void DisplayImage(unsigned char image[Y_SIZE][X_SIZE], int position)
{
	switch (position) {
		case WMAIN:		/*	���͉摜�p�E�B���h�E	*/
			ColorDisplay(ximage_main, image, image, image, "MAIN");
			break;
		case WPREV:		/*	�o�͉摜�p�E�B���h�E	*/
			ColorDisplay(ximage_prev, image, image, image, "PREV");
			break;
		case WSUB:		/*	���[�N�摜�p�E�B���h�E	*/
			ColorDisplay(ximage_sub, image, image, image, "SUB");
			break;
		case WKEY:		/*	�L�[�摜�p�E�B���h�E	*/
			ColorDisplay(ximage_key, image, image, image, "KEY");
			break;
	}
}

/*--- DisplayCImage --- ��ʂ̕\���i�J���[�j-----------------------------------
	image:		�J���[�摜�f�[�^
	position:	�\���E�B���h�E
-----------------------------------------------------------------------------*/
void DisplayCImage(unsigned char image[3][Y_SIZE][X_SIZE], int position)
{
	switch (position) {
		case WMAIN:		/*	���͉摜�p�E�B���h�E	*/
			ColorDisplay(ximage_main, image[0], image[1], image[2], "MAIN");
			break;
		case WPREV:		/*	�o�͉摜�p�E�B���h�E	*/
			ColorDisplay(ximage_prev, image[0], image[1], image[2], "PREV");
			break;
		case WSUB:		/*	���[�N�摜�p�E�B���h�E	*/
			ColorDisplay(ximage_sub, image[0], image[1], image[2], "SUB");
			break;
		case WKEY:		/*	�L�[�摜�p�E�B���h�E	*/
			ColorDisplay(ximage_key, image[0], image[1], image[2], "KEY");
			break;
 	}
}

/*---InitWindows --- �S�̃E�B���h�E������������ ---------------------------*/
void InitWindows(void)
{
	unsigned char 	*image;

	image = (unsigned char *)calloc(X_SIZE*Y_SIZE, 1);
	if (image == NULL) {
		printf("--Memory allocation error !");
		exit(0);
	}
	InitWindow(&ximage_main, WMAIN_POSX, WMAIN_POSY);
	XMapWindow(d, ximage_main.win);
	DisplayImage((unsigned char (*)[X_SIZE])image, WMAIN);
	InitWindow(&ximage_prev, WPREV_POSX, WPREV_POSY);
	XMapWindow(d, ximage_prev.win);
	DisplayImage((unsigned char (*)[X_SIZE])image, WPREV);
	InitWindow(&ximage_sub, WSUB_POSX, WSUB_POSY);
	XMapWindow(d, ximage_sub.win);
	DisplayImage((unsigned char (*)[X_SIZE])image, WSUB);
	InitWindow(&ximage_key, WKEY_POSX, WKEY_POSY);
	XMapWindow(d, ximage_key.win);
	DisplayImage((unsigned char (*)[X_SIZE])image, WKEY);
	free(image);
}

/*---InitWindow --- �E�B���h�E������������ ------------------------------------
	ximage:	�摜�̍\����
	posi_x:	�E�B���h�E�̉������\���ʒu
	posi_y:	�E�B���h�E�̏c�����\���ʒu
-----------------------------------------------------------------------------*/
void InitWindow(XIMAGE *ximage, int posi_x, int posi_y)
{
	unsigned long vmask = 0;
	XSetWindowAttributes attr;
	XWindowAttributes watr;
	Colormap colormap;

	attr.background_pixel = 0;
	attr.border_pixel = 0;
	attr.background_pixmap = 0;
	attr.border_pixmap = 0;
	colormap = XCreateColormap(d, RootWindow(d, 0), vis, AllocNone);
	attr.colormap = colormap;
	vmask = CWBackPixel | CWBorderPixel | CWColormap ;
        ximage->win = XCreateWindow(d, RootWindow(d, 0),
        posi_x, posi_y, X_SIZE, Y_SIZE, 0, depth, InputOutput,
		vis, vmask, &attr);
	gc = XCreateGC(d, ximage->win, 0, 0);
	ximage->im = XCreateImage(d, vis, depth,
		ZPixmap, 0, 0, X_SIZE, Y_SIZE, 32, 0);
	(ximage->im)->data = (char *)calloc(1, X_SIZE*Y_SIZE*4);
	if ((ximage->im)->data  == NULL) {
		printf("--Memory allocation error !\n");
		exit(0);
	}
	ximage->pix = XCreatePixmap(d, ximage->win, X_SIZE, Y_SIZE, depth);
	XPutImage(d, ximage->pix, gc, ximage->im, 0, 0, 0, 0, X_SIZE, Y_SIZE);
	XSetWindowBackgroundPixmap(d, ximage->win, ximage->pix);
	XFlush(d);
}

/*--- ColorDisplay --- �E�B���h�E�ɃJ���[�摜��\������ -----------------------
	ximage:	�摜�\����
	imager:	�摜�f�[�^�i�q�j
	imageg:	�摜�f�[�^�i�f�j
	imageb:	�摜�f�[�^�i�a�j
	wtitle:	�E�B���h�E�̃^�C�g��
-----------------------------------------------------------------------------*/
void ColorDisplay(XIMAGE ximage, unsigned char imager[Y_SIZE][X_SIZE],
		unsigned char imageg[Y_SIZE][X_SIZE], 
		unsigned char imageb[Y_SIZE][X_SIZE], char wtitle[])
{
	int 			*pd;
	short int 		*pds;
	unsigned char 	*pdc;
	int 			i, j;
	unsigned char 	*psr0, *psg0, *psb0;
	unsigned char 	*psr, *psg, *psb;

	XStoreName(d, ximage.win, wtitle);
	pd = (int *)ximage.im->data; 
	pds = (short int *)ximage.im->data; 
	psg = (unsigned char *)imageg;
	if (vis->blue_mask == 31) {
		psr = (unsigned char *)imager;
		psb = (unsigned char *)imageb;
	}
	else if (vis->blue_mask == 255) {
		psr = (unsigned char *)imager;
		psb = (unsigned char *)imageb;
	} else {
		psb = (unsigned char *)imager;
		psr = (unsigned char *)imageb;
	}
	if (vis->blue_mask == 31) {
		for (i=0 ; i<X_SIZE*Y_SIZE ; i++) {
			*pds++ = ((int)*psb++ >> 3 & 0x1f) 
				+ ((int)*psg++ << 3 & 0x7e0) 
				+ ((int)*psr++ << 8 & 0xf800);
		}
	} else {
		for (i=0 ; i<X_SIZE*Y_SIZE ; i++) {
			*pd++ = ((int)*psb++ & 0xff) 
				+ ((int)*psg++ << 8 & 0xff00) 
				+ ((int)*psr++ << 16 & 0xff0000);
		}
	}
	XPutImage(d, ximage.pix, gc, ximage.im, 0, 0, 0, 0, X_SIZE, Y_SIZE);
	XCopyArea(d, ximage.pix, ximage.win, gc, 0, 0, X_SIZE, Y_SIZE, 0, 0);
	XRaiseWindow(d, ximage.win);
	XFlush(d);
}
