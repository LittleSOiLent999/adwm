/* See COPYING file for copyright and license details. */

#include "adwm.h"
#include "ewmh.h"
#include "layout.h"
#include "tags.h"
#include "actions.h"
#include "config.h"
#include "icons.h"
#include "draw.h"
#include "image.h"
#if 1
#include "ximage.h"
#else
#include "xlib.h"
#endif
#include "xcairo.h" /* verification */

#ifdef XCAIRO

void
xcairo_removepixmap(AdwmPixmap *p)
{
	if (p->pixmap.surf) {
		cairo_surface_destroy(p->pixmap.surf);
		p->pixmap.surf = NULL;
	}
	if (p->pixmap.clip) {
		cairo_surface_destroy(p->pixmap.clip);
		p->pixmap.clip = NULL;
	}
	if (p->bitmap.surf) {
		cairo_surface_destroy(p->bitmap.surf);
		p->bitmap.surf = NULL;
	}
	if (p->bitmap.clip) {
		cairo_surface_destroy(p->bitmap.clip);
		p->bitmap.clip = NULL;
	}
}

static Bool
xcairo_createicon(AScreen *ds, Client *c, cairo_surface_t **surf, cairo_surface_t **clip, Bool cropscale)
{
	/* for now */
	return (False);
}

Bool
xcairo_createbitmapicon(AScreen *ds, Client *c, Pixmap icon, Pixmap mask, unsigned w, unsigned h)
{
	Screen *screen;
	XRenderPictFormat *format;
	cairo_surface_t *surf = NULL, *clip = NULL;
	Bool result = False;

	format = XRenderFindStandardFormat(dpy, PictStandardA1);
	screen = ScreenOfDisplay(dpy, scr->screen);
#if 1
	surf = cairo_xlib_surface_create_with_xrender_format(dpy, icon, screen, format, w, h);
#else
	surf = cairo_xlib_surface_create_for_bitmap(dpy, icon, screen, w, h);
#endif
	if (!surf) {
		EPRINTF("could not create surface\n");
		goto error;
	}
	if (mask) {
#if 1
		clip = cairo_xlib_surface_create_with_xrender_format(dpy, mask, screen, format, w, h);
#else
		clip = cairo_xlib_surface_create_for_bitmap(dpy, mask, screen, format, w, h);
#endif
		if (!clip) {
			EPRINTF("could not create surface\n");
			goto error;
		}
	}
	result = xcairo_createicon(ds, c, &surf, &clip, False);
      error:
	if (surf)
		cairo_surface_destroy(surf);
	if (clip)
		cairo_surface_destroy(clip);
	return (result);
}

Bool
xcairo_createpixmapicon(AScreen *ds, Client *c, Pixmap icon, Pixmap mask, unsigned w, unsigned h, unsigned d)
{
	Screen *screen;
	XRenderPictFormat *format;
	cairo_surface_t *surf = NULL, *clip = NULL;
	Bool result = False;

	format = XRenderFindStandardFormat(dpy, PictStandardRGB24);
	screen = ScreenOfDisplay(dpy, scr->screen);
#if 1
	surf = cairo_xlib_surface_create_with_xrender_format(dpy, icon, screen, format, w, h);
#else
	surf = cairo_xlib_surface_create_for_bitmap(dpy, icon, screen, w, h);
#endif
	if (!surf) {
		EPRINTF("could not create surface\n");
		goto error;
	}
	if (mask) {
		format = XRenderFindStandardFormat(dpy, PictStandardA1);
#if 1
		clip = cairo_xlib_surface_create_with_xrender_format(dpy, mask, screen, format, w, h);
#else
		clip = cairo_xlib_surface_create_for_bitmap(dpy, mask, screen, format, w, h);
#endif
		if (!clip) {
			EPRINTF("could not create surface\n");
			goto error;
		}
	}
	result = xcairo_createicon(ds, c, &surf, &clip, False);
      error:
	if (surf)
		cairo_surface_destroy(surf);
	if (clip)
		cairo_surface_destroy(clip);
	return (result);
}

Bool
xcairo_createdataicon(AScreen *ds, Client *c, unsigned w, unsigned h, long *data)
{
	/* for now */
	return (False);
}

static cairo_t *
xcairo_create_context(AScreen *ds, Window win, int w, int h)
{
	cairo_t *cr = NULL;
	Screen *screen;
	cairo_surface_t *surf;
	
	screen = ScreenOfDisplay(dpy, ds->screen);
	if (!(surf = cairo_xlib_surface_create_with_xrender_format(dpy, win, screen, ds->format, w, h))) {
		EPRINTF("could not create surface\n");
	} else if (!(cr = cairo_create(surf))) {
		cairo_surface_destroy(surf);
		EPRINTF("could not create context\n");
	}
	return (cr);
}

void
xcairo_drawnormal(AScreen *ds, Client *c)
{
	cairo_t *cctx;

	ds->dc.x = ds->dc.y = 0;
	ds->dc.w = c->c.w;
	ds->dc.h = ds->style.titleheight;
	if (ds->dc.draw.w < ds->dc.w)
		ds->dc.draw.w = ds->dc.w;

	if (!(cctx = c->cctx.title) &&
	    !(cctx = xcairo_create_context(ds, c->title, ds->dc.draw.w, ds->dc.draw.h)))
		return;
	c->cctx.title = cctx;
	ds->dc.draw.cctx = cctx;

}

Bool
xcairo_initpng(char *path, AdwmPixmap *px)
{
#ifdef LIBPNG
	/* for now */
	return (False);
#else				/* LIBPNG */
	return (False);
#endif				/* LIBPNG */
}

Bool
xcairo_initjpg(char *path, AdwmPixmap *px)
{
#ifdef LIBJPEG
	/* for now */
	return (False);
#else				/* LIBJPEG */
	return (False);
#endif				/* LIBJPEG */
}

Bool
xcairo_initsvg(char *path, AdwmPixmap *px)
{
#ifdef LIBSVG
	/* for now */
	return (False);
#else				/* LIBSVG */
	return (False);
#endif				/* LIBSVG */
}

Bool
xcairo_initxpm(char *path, AdwmPixmap *px)
{
	Pixmap draw = None, mask = None;
	Screen *screen;
	cairo_surface_t *surf = NULL, *clip = NULL;
	int status;
	XRenderPictFormat *format;
	XpmAttributes xa = {
		.visual = DefaultVisual(dpy, scr->screen),
		.colormap = DefaultColormap(dpy, scr->screen),
		.depth = DefaultDepth(dpy, scr->screen),
		.valuemask = XpmVisual| XpmColormap| XpmDepth,
	};

	status = XpmReadFileToPixmap(dpy, scr->drawable, path, &draw, &mask, &xa);
	if (status != XpmSuccess || !draw) {
		EPRINTF("could not load xpm file %s\n", path);
		goto error;
	}
	XpmFreeAttributes(&xa);
	format = XRenderFindStandardFormat(dpy, PictStandardRGB24);
	screen = ScreenOfDisplay(dpy, scr->screen);
#if 1
	surf = cairo_xlib_surface_create_with_xrender_format(dpy, draw, screen, format, xa.width, xa.height);
#else
	surf = cairo_xlib_surface_create(dpy, draw, xa.visual, xa.width, xa.height);
#endif
	if (!surf) {
		EPRINTF("could not create surface\n");
		goto error;
	}
	if (mask) {
		format = XRenderFindStandardFormat(dpy, PictStandardA1);
#if 1
		clip = cairo_xlib_surface_create_with_xrender_format(dpy, mask, screen, format, xa.width, xa.height);
#else
		clip = cairo_xlib_surface_create_for_bitmap(dpy, mask, screen, xa.width, xa.height);
#endif
		if (!clip) {
			EPRINTF("could not create surface\n");
			goto error;
		}
	}
	if (px->bitmap.surf) {
		cairo_surface_destroy(px->bitmap.surf);
		px->bitmap.surf = NULL;
	}
	if (px->bitmap.clip) {
		cairo_surface_destroy(px->bitmap.clip);
		px->bitmap.clip = NULL;
	}
	if (px->file) {
		free(px->file);
		px->file = NULL;
	}
	px->pixmap.surf = surf;
	px->pixmap.clip = clip;
	px->file = path;
	px->x = px->y = px->b = 0;
	px->w = xa.width;
	px->h = xa.height;
	px->d = xa.depth;
      error:
	if (draw)
		XFreePixmap(dpy, draw);
	if (mask)
		XFreePixmap(dpy, mask);
	return (surf ? True : False);
}

Bool
xcairo_initxbm(char *path, AdwmPixmap *px)
{
	Pixmap draw;
	Screen *screen;
	XRenderPictFormat *format;
	cairo_surface_t *surf = NULL;
	int status;

	status = XReadBitmapFile(dpy, scr->root, path, &px->w, &px->h, &draw, &px->x, &px->y);
	if (status != BitmapSuccess || !draw) {
		EPRINTF("could not load xbm file %s\n", path);
		goto error;
	}
	format = XRenderFindStandardFormat(dpy, PictStandardA1);
	screen = ScreenOfDisplay(dpy, scr->screen);
#if 1
	surf = cairo_xlib_surface_create_with_xrender_format(dpy, draw, screen, format, px->w, px->h);
#else
	surf = cairo_xlib_surface_create_for_bitmap(dpy, draw, screen, px->w, px->h);
#endif
	if (!surf) {
		EPRINTF("could not create surface\n");
		goto error;
	}
	if (px->bitmap.surf) {
		cairo_surface_destroy(px->bitmap.surf);
		px->bitmap.surf = NULL;
	}
	if (px->bitmap.clip) {
		cairo_surface_destroy(px->bitmap.clip);
		px->bitmap.clip = NULL;
	}
	if (px->file) {
		free(px->file);
		px->file = NULL;
	}
	px->bitmap.surf = surf;
	px->file = path;
	px->x = px->y = px->b = 0;
	px->w = px->w;
	px->h = px->h;
error:
	if (draw)
		XFreePixmap(dpy, draw);
	return (surf ? True : False);
}

Bool
xcairo_initxbmdata(const unsigned char *bits, int w, int h, AdwmPixmap *px)
{
	Pixmap draw;
	Screen *screen;
	XRenderPictFormat *format;
	cairo_surface_t *surf = NULL;

	if (!(draw = XCreateBitmapFromData(dpy, scr->drawable, (char *) bits, w, h))) {
		EPRINTF("could not load xbm data\n");
		goto error;
	}
	format = XRenderFindStandardFormat(dpy, PictStandardA1);
	screen = ScreenOfDisplay(dpy, scr->screen);
#if 1
	surf = cairo_xlib_surface_create_with_xrender_format(dpy, draw, screen, format, w, h);
#else
	surf = cairo_xlib_surface_create_for_bitmap(dpy, draw, screen, w, h);
#endif
	if (!surf) {
		EPRINTF("could not create surface\n");
		goto error;
	}
	if (px->bitmap.surf) {
		cairo_surface_destroy(px->bitmap.surf);
		px->bitmap.surf = NULL;
	}
	if (px->bitmap.clip) {
		cairo_surface_destroy(px->bitmap.clip);
		px->bitmap.clip = NULL;
	}
	if (px->file) {
		free(px->file);
		px->file = NULL;
	}
	px->bitmap.surf = surf;
	px->x = px->y = px->b = 0;
	px->w = w;
	px->h = h;
error:
	if (draw)
		XFreePixmap(dpy, draw);
	return (surf ? True : False);
}

#endif				/* XCAIRO */

