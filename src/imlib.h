/* imlib.c */

#ifndef __LOCAL_IMLIB_H__
#define __LOCAL_IMLIB_H__
#if defined IMLIB2
void imlib2_removepixmap(AdwmPixmap *p);
Bool imlib2_createbitmapicon(AScreen *ds, Client *c, Pixmap icon, Pixmap mask, unsigned w, unsigned h);
Bool imlib2_createpixmapicon(AScreen *ds, Client *c, Pixmap icon, Pixmap mask, unsigned w, unsigned h, unsigned d);
Bool imlib2_createdataicon(AScreen *ds, Client *c, unsigned w, unsigned h, long *data);
Bool imlib2_createpngicon(AScreen *ds, Client *c, const char *file);
Bool imlib2_createsvgicon(AScreen *ds, Client *c, const char *file);
Bool imlib2_createxpmicon(AScreen *ds, Client *c, const char *file);
Bool imlib2_createxbmicon(AScreen *ds, Client *c, const char *file);
const char *imlib2_error_string(Imlib_Load_Error error);
#ifdef DAMAGE
Bool imlib2_drawdamage(Client *c, XDamageNotifyEvent *ev);
#endif
int imlib2_drawbutton(AScreen *ds, Client *c, ElementType type, XftColor *col, int x);
int imlib2_drawtext(AScreen *ds, const char *text, Drawable drawable, XftDraw *xftdraw, XftColor *col, int hilite, int x, int y, int mw);
int imlib2_drawsep(AScreen *ds, const char *text, Drawable drawable, XftDraw *xftdraw, XftColor *col, int hilite, int x, int y, int w);
void imlib2_drawdockapp(AScreen *ds, Client *c);
void imlib2_drawnormal(AScreen *ds, Client *c);
Bool imlib2_initpng(char *path, AdwmPixmap *p);
Bool imlib2_initsvg(char *path, AdwmPixmap *p);
Bool imlib2_initxpm(char *path, AdwmPixmap *p);
Bool imlib2_initxbm(char *path, AdwmPixmap *p);
Bool imlib2_initxbmdata(const unsigned char *bits, int width, int height, AdwmPixmap *px);
Bool imlib2_initpixmap(char *path, AdwmPixmap *p);
Bool imlib2_initbitmap(char *path, AdwmPixmap *p);
void imlib2_getpixmap(const char *file, AdwmPixmap *p);
#endif				/* defined IMLIB2 */
#endif				/* __LOCAL_IMLIB_H__ */
