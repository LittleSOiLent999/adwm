/* image.c */

#ifndef __LOCAL_IMAGE_H__
#define __LOCAL_IMAGE_H__

int XReadBitmapFileImage(Display *display, Visual * visual, const char *file,
			 unsigned *width, unsigned *height, XImage **image_return,
			 int *x_hot, int *y_hot);
#ifdef LIBPNG
XImage *png_read_file_to_ximage(Display *display, Visual * visual, const char *file);
#endif
void renderimage(AScreen *ds, const ARGB *argb, const unsigned width,
		 const unsigned height);
void initimage(void);

#endif				/* __LOCAL_IMAGE_H__ */
