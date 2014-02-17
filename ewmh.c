#include <assert.h>
#include <unistd.h>
#include <regex.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/Xutil.h>
#include <X11/Xft/Xft.h>
#ifdef XRANDR
#include <X11/extensions/Xrandr.h>
#include <X11/extensions/randr.h>
#endif
#ifdef XINERAMA
#include <X11/extensions/Xinerama.h>
#endif
#ifdef SYNC
#include <X11/extensions/sync.h>
#endif
#include "adwm.h"
#include "config.h"

Atom atom[NATOMS];

/* keep in sync with enum in adwm.h */
char *atomnames[NATOMS] = {
	"MANAGER",
	"UTF8_STRING",
	"WM_PROTOCOLS",
	"WM_DELETE_WINDOW",
	"WM_SAVE_YOURSELF",
	"WM_STATE",
	"WM_CHANGE_STATE",
	"WM_TAKE_FOCUS",
	"_ECHINUS_LAYOUT",
	"_ECHINUS_SELTAGS",
	"_NET_RESTART",				/* TODO */
	"_NET_SHUTDOWN",			/* TODO */
	"_NET_DESKTOP_LAYOUT",			/* TODO */
	/* MWM/DTWM properties follow */
	"WM_DESKTOP",
	"_MOTIF_BINDINGS",
	"_MOTIF_DEFAULT_BINDINGS",
	"_MOTIF_WM_MESSAGES",
	"_MOTIF_WM_OFFSET",
	"_MOTIF_WM_HINTS",
	"_MOTIF_WM_MENU",
	"_MOTIF_WM_INFO",
	"_DT_WORKSPACE_HINTS",
	"_DT_WORKSPACE_PRESENCE",
	"_DT_WORKSPACE_LIST",
	"_DT_WORKSPACE_CURRENT",
	"_DT_WORKSPACE_INFO",
	"_DT_WM_HINTS",
	"_DT_WM_REQUEST",
	"_DT_WORKSPACE_EMBEDDED_CLIENTS",
	"_DT_WMSAVE_HINT",
	/* _WIN_PROTOCOLS following */
	"_WIN_APP_STATE",
	"_WIN_AREA_COUNT",
	"_WIN_AREA",
	"_WIN_CLIENT_LIST",
	"_WIN_CLIENT_MOVING",
	"_WIN_DESKTOP_BUTTON_PROXY",
	"_WIN_EXPANDED_SIZE",
	"_WIN_FOCUS",
	"_WIN_HINTS",
	"_WIN_ICONS",
	"_WIN_LAYER",
	"_WIN_MAXIMIZED_GEOMETRY",
	"_WIN_PROTOCOLS",
	"_WIN_RESIZE",				/* TODO */
	"_WIN_STATE",
	"_WIN_SUPPORTING_WM_CHECK",
	"_WIN_WORKAREA",
	"_WIN_WORKSPACE_COUNT",
	"_WIN_WORKSPACE_NAMES",
	"_WIN_WORKSPACE",
	"_WIN_WORKSPACES",
	"_SWM_VROOT",
	/* _NET_SUPPORTED following */
	"_NET_CLIENT_LIST",
	"_NET_ACTIVE_WINDOW",
	"_NET_WM_DESKTOP",
	"_NET_WM_DESKTOP_MASK",
	"_NET_NUMBER_OF_DESKTOPS",
	"_NET_DESKTOP_NAMES",
	"_NET_CURRENT_DESKTOP",
	"_NET_WORKAREA",
	"_NET_DESKTOP_VIEWPORT",
	"_NET_SHOWING_DESKTOP",
	"_NET_DESKTOP_GEOMETRY",
	"_NET_VISIBLE_DESKTOPS",
	"_NET_MONITOR_GEOMETRY",

	"_NET_DESKTOP_MODES",
	"_NET_DESKTOP_MODE_FLOATING",
	"_NET_DESKTOP_MODE_TILED",
	"_NET_DESKTOP_MODE_BOTTOM_TILED",
	"_NET_DESKTOP_MODE_MONOCLE",
	"_NET_DESKTOP_MODE_TOP_TILED",
	"_NET_DESKTOP_MODE_LEFT_TILED",

	"_NET_CLIENT_LIST_STACKING",
	"_NET_WM_WINDOW_OPACITY",
	"_NET_MOVERESIZE_WINDOW",
	"_NET_RESTACK_WINDOW",
	"_NET_WM_MOVERESIZE",
	"_NET_FRAME_EXTENTS",
	"_NET_WM_HANDLED_ICONS",
	"_NET_REQUEST_FRAME_EXTENTS",
	"_NET_VIRTUAL_ROOTS",

	"_NET_WM_WINDOW_TYPE",
	"_NET_WM_WINDOW_TYPE_DESKTOP",
	"_NET_WM_WINDOW_TYPE_DOCK",
	"_NET_WM_WINDOW_TYPE_TOOLBAR",
	"_NET_WM_WINDOW_TYPE_MENU",
	"_NET_WM_WINDOW_TYPE_UTILITY",
	"_NET_WM_WINDOW_TYPE_SPLASH",
	"_NET_WM_WINDOW_TYPE_DIALOG",
	"_NET_WM_WINDOW_TYPE_DROPDOWN_MENU",
	"_NET_WM_WINDOW_TYPE_POPUP_MENU",
	"_NET_WM_WINDOW_TYPE_TOOLTIP",
	"_NET_WM_WINDOW_TYPE_NOTIFICATION",
	"_NET_WM_WINDOW_TYPE_COMBO",
	"_NET_WM_WINDOW_TYPE_DND",
	"_NET_WM_WINDOW_TYPE_NORMAL",

	"_NET_WM_STRUT_PARTIAL",
	"_NET_WM_STRUT",
	"_NET_WM_PID",
	"_NET_WM_NAME",
	"_NET_WM_VISIBLE_NAME",
	"_NET_WM_ICON_NAME",
	"_NET_WM_VISIBLE_ICON_NAME",
	"_NET_WM_USER_TIME",
	"_NET_WM_USER_TIME_WINDOW",
	"_NET_STARTUP_ID",
	"_NET_STARTUP_INFO",
	"_NET_STARTUP_INFO_BEGIN",
	"_NET_WM_SYNC_REQUEST",
	"_NET_WM_SYNC_REQUEST_COUNTER",
	"_NET_WM_FULLSCREEN_MONITORS",

	"_NET_WM_STATE",
	"_NET_WM_STATE_MODAL",
	"_NET_WM_STATE_STICKY",
	"_NET_WM_STATE_MAXIMIZED_VERT",
	"_NET_WM_STATE_MAXIMIZED_HORZ",
	"_NET_WM_STATE_SHADED",
	"_NET_WM_STATE_SKIP_TASKBAR",
	"_NET_WM_STATE_SKIP_PAGER",
	"_NET_WM_STATE_HIDDEN",
	"_NET_WM_STATE_FULLSCREEN",
	"_NET_WM_STATE_ABOVE",
	"_NET_WM_STATE_BELOW",
	"_NET_WM_STATE_DEMANDS_ATTENTION",
	"_NET_WM_STATE_FOCUSED",
	"_NET_WM_STATE_FIXED",
	"_NET_WM_STATE_FLOATING",
	"_NET_WM_STATE_FILLED",

	"_NET_WM_ALLOWED_ACTIONS",
	"_NET_WM_ACTION_ABOVE",
	"_NET_WM_ACTION_BELOW",
	"_NET_WM_ACTION_CHANGE_DESKTOP",
	"_NET_WM_ACTION_CLOSE",
	"_NET_WM_ACTION_FULLSCREEN",
	"_NET_WM_ACTION_MAXIMIZE_HORZ",
	"_NET_WM_ACTION_MAXIMIZE_VERT",
	"_NET_WM_ACTION_MINIMIZE",
	"_NET_WM_ACTION_MOVE",
	"_NET_WM_ACTION_RESIZE",
	"_NET_WM_ACTION_SHADE",
	"_NET_WM_ACTION_STICK",
	"_NET_WM_ACTION_FLOAT",
	"_NET_WM_ACTION_FILL",

	"_NET_SUPPORTING_WM_CHECK",
	"_NET_CLOSE_WINDOW",
	"_NET_WM_PING",
	"_NET_SUPPORTED",

	"_KDE_NET_SYSTEM_TRAY_WINDOWS",
	"_KDE_NET_WM_FRAME_STRUT",
	"_KDE_NET_WM_SYSTEM_TRAY_WINDOW_FOR",
	"_KDE_NET_WM_WINDOW_TYPE_OVERRIDE",
	"_KDE_SPLASH_PROGRESS",
	"_KDE_WM_CHANGE_STATE"
};

#define _NET_WM_STATE_REMOVE	0
#define _NET_WM_STATE_ADD	1
#define _NET_WM_STATE_TOGGLE	2

#define MWM_STARTUP_STANDARD	(1<<0)
#define MWM_STARTUP_CUSTOM	(1<<1)

void
initewmh(Window win)
{
	char name[] = "adwm";
	long data[2];
	static Bool atoms_interned = False;

	if (!atoms_interned) {
		XInternAtoms(dpy, atomnames, NATOMS, False, atom);
		atoms_interned = True;
	}
	XChangeProperty(dpy, scr->root, _XA_NET_SUPPORTED, XA_ATOM, 32,
			PropModeReplace, (unsigned char *) &_XA_NET_CLIENT_LIST,
			NATOMS - ClientList);

	XChangeProperty(dpy, win, _XA_NET_WM_NAME, _XA_UTF8_STRING, 8,
			PropModeReplace, (unsigned char *) name, strlen(name));
	data[0] = getpid();
	XChangeProperty(dpy, win, _XA_NET_WM_PID, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) data, 1);
	XChangeProperty(dpy, scr->root, _XA_NET_SUPPORTING_WM_CHECK, XA_WINDOW, 32,
			PropModeReplace, (unsigned char *) &win, 1);
	XChangeProperty(dpy, win, _XA_NET_SUPPORTING_WM_CHECK, XA_WINDOW, 32,
			PropModeReplace, (unsigned char *) &win, 1);

	XChangeProperty(dpy, scr->root, _XA_WIN_PROTOCOLS, XA_ATOM, 32,
			PropModeReplace, (unsigned char *) &_XA_WIN_APP_STATE,
			ClientList - WinAppState);
	XChangeProperty(dpy, win, _XA_WIN_SUPPORTING_WM_CHECK, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) &win, 1);
	XChangeProperty(dpy, scr->root, _XA_WIN_SUPPORTING_WM_CHECK, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) &win, 1);
	XChangeProperty(dpy, win, _XA_WIN_DESKTOP_BUTTON_PROXY, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) &win, 1);
	XChangeProperty(dpy, scr->root, _XA_WIN_DESKTOP_BUTTON_PROXY, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) &win, 1);

	data[0] = MWM_STARTUP_CUSTOM;
	data[1] = win;
	XChangeProperty(dpy, scr->root, _XA_MOTIF_WM_INFO, _XA_MOTIF_WM_INFO, 32,
			PropModeReplace, (unsigned char *) data, 2);
	XChangeProperty(dpy, win, _XA_MOTIF_WM_INFO, _XA_MOTIF_WM_INFO, 32,
			PropModeReplace, (unsigned char *) data, 2);

	ewmh_update_net_client_list();
}

void
exitewmh(WithdrawCause cause)
{
	XDeleteProperty(dpy, scr->root, _XA_WIN_SUPPORTING_WM_CHECK);
	XDeleteProperty(dpy, scr->root, _XA_NET_SUPPORTING_WM_CHECK);
	XDeleteProperty(dpy, scr->root, _XA_WIN_DESKTOP_BUTTON_PROXY);

	XDeleteProperty(dpy, scr->root, _XA_WIN_PROTOCOLS);
	XDeleteProperty(dpy, scr->root, _XA_NET_SUPPORTED);

	switch (cause) {
		int i;

	default:
	case CauseQuitting:
	{
		Atom props[] = {
			_XA_WIN_AREA_COUNT,
			_XA_WIN_AREA,
			_XA_WIN_CLIENT_LIST,
			_XA_WIN_FOCUS,
			_XA_WIN_WORKSPACE_COUNT,
			_XA_WIN_WORKSPACE_NAMES,
			_XA_WIN_WORKSPACE,
			_XA_WIN_WORKSPACES,

			_XA_NET_ACTIVE_WINDOW,
			_XA_NET_CLIENT_LIST,
			_XA_NET_CLIENT_LIST_STACKING,
			_XA_NET_NUMBER_OF_DESKTOPS,
			_XA_NET_DESKTOP_NAMES,
			_XA_NET_CURRENT_DESKTOP,
			_XA_NET_DESKTOP_VIEWPORT,
			_XA_NET_SHOWING_DESKTOP,
			_XA_NET_VISIBLE_DESKTOPS,
			_XA_NET_MONITOR_GEOMETRY,
			_XA_NET_DESKTOP_MODES,
			/* _XA_NET_DESKTOP_LAYOUT, */
			/* _XA_NET_DESKTOP_STRUTS, */
			_XA_NET_RESTART,
			None
		};

		for (i = 0; props[i]; i++)
			XDeleteProperty(dpy, scr->root, props[i]);
		/* fall through */
	}
	case CauseSwitching:
	{
		Atom props[] = {
			_XA_ECHINUS_LAYOUT,
			_XA_ECHINUS_SELTAGS,

			_XA_WIN_WORKAREA,
			_XA_NET_WORKAREA,

			None
		};

		for (i = 0; props[i]; i++)
			XDeleteProperty(dpy, scr->root, props[i]);
		/* fall through */
	}
	case CauseRestarting:
	{
		Atom props[] = {
			_XA_SWM_VROOT,
			_XA_NET_VIRTUAL_ROOTS,
			_XA_NET_DESKTOP_GEOMETRY,
			_XA_NET_RESTART,

			None
		};

		for (i = 0; props[i]; i++)
			XDeleteProperty(dpy, scr->root, props[i]);
		break;
	}
	}
}

void
ewmh_add_client(Client *c)
{
}

void
ewmh_del_client(Client * c, WithdrawCause cause)
{
	switch (cause) {
		int i;

	default:
	case CauseDestroyed:
	case CauseReparented:
	case CauseUnmapped:
	{
		ewmh_update_net_client_list();
		if (sel == c) {
			focus(NULL);
			ewmh_update_net_active_window();
		}
		/* fall through */
	}
	case CauseQuitting:
	{
		Atom props[] = {
			_XA_WM_STATE,

			_XA_WIN_MAXIMIZED_GEOMETRY,

			_XA_NET_WM_DESKTOP,
			_XA_NET_WM_STATE,
			_XA_NET_WM_ALLOWED_ACTIONS,

			None
		};
		for (i = 0; props[i]; i++)
			XDeleteProperty(dpy, scr->root, props[i]);
		/* fall through */
	}
	case CauseSwitching:
	{
		Atom props[] = {
			_XA_NET_WM_DESKTOP_MASK,
			_XA_NET_FRAME_EXTENTS,
			_XA_NET_WM_VISIBLE_NAME,
			_XA_NET_WM_VISIBLE_ICON_NAME,
			_XA_NET_WM_FULLSCREEN_MONITORS,

			None
		};
		for (i = 0; props[i]; i++)
			XDeleteProperty(dpy, scr->root, props[i]);
		/* fall through */
	}
	case CauseRestarting:
	{
		Atom props[] = {
			_XA_KDE_NET_WM_FRAME_STRUT,

			None
		};
		for (i = 0; props[i]; i++)
			XDeleteProperty(dpy, scr->root, props[i]);
		/* fall through */
		break;
	}
	}
}

void
ewmh_update_echinus_layout_name()
{
	Monitor *cm;
	View *v;

	if (!(cm = selmonitor()))
		return;
	v = scr->views + cm->curtag;
	XChangeProperty(dpy, scr->root, _XA_ECHINUS_LAYOUT, XA_STRING, 8, PropModeReplace,
			(const unsigned char *) &v->layout->symbol, 1L);
}

void
ewmh_update_net_client_list_stacking() {
	Client *c;
	Window *wl = NULL;
	int i, n;

	XPRINTF("%s\n", "Updating _NET_CLIENT_LIST_STACKING");
	for (n = 0, c = scr->stack; c; n++, c = c->snext) ;
	if (n && (wl = ecalloc(n, sizeof(Window)))) {
		for (i = 0, c = scr->stack; c && i < n; c = c->snext)
			wl[i++] = c->win;
		assert(i == n);
	}
	XChangeProperty(dpy, scr->root, _XA_NET_CLIENT_LIST_STACKING, XA_WINDOW, 32,
			PropModeReplace, (unsigned char *) wl, n);
	free(wl);
	XFlush(dpy);		/* XXX: caller's responsibility */
}

void
ewmh_update_net_client_list() {
	Client *c;
	Window *wl = NULL;
	int i, n;

	XPRINTF("%s\n", "Updating _NET_CLIENT_LIST");
	for (n = 0, c = scr->clist; c; n++, c = c->cnext) ;
	if (n && (wl = calloc(n, sizeof(Window)))) {
		for (i = 0, c = scr->clist; c && i < n; c = c->cnext)
			wl[i++] = c->win;
		assert(i == n);
	}
	XChangeProperty(dpy, scr->root, _XA_NET_CLIENT_LIST, XA_WINDOW, 32,
			PropModeReplace, (unsigned char *) wl, n);
	XChangeProperty(dpy, scr->root, _XA_WIN_CLIENT_LIST, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) wl, n);
	free(wl);
	ewmh_update_net_client_list_stacking();
	XFlush(dpy);		/* XXX: caller's responsibility */
}

void
ewmh_update_net_desktop_viewport() {
	long *data;

	data = ecalloc(scr->ntags * 2, sizeof(data[0]));
	XChangeProperty(dpy, scr->root, _XA_NET_DESKTOP_VIEWPORT, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *)data, scr->ntags * 2);
	XChangeProperty(dpy, scr->root, _XA_WIN_AREA, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *)data, 2);
}

Bool names_synced = False;

void
ewmh_update_net_desktop_names() {
	char *buf, *pos;
	unsigned int i, len, slen;
	long *data;

	if (names_synced) {
		XPRINTF("%s\n", "Updating _NET_DESKTOP_NAMES: NOT!");
		return;
	}
	XPRINTF("%s\n", "Updating _NET_DESKTOP_NAMES");
	for (len = 0, i = 0; i < scr->ntags; i++)
		if (scr->tags[i])
			len += strlen(scr->tags[i]);
	len += scr->ntags;
	pos = buf = ecalloc(len, 1);
	for (i = 0; i < scr->ntags; i++) {
		if (scr->tags[i]) {
			slen = strlen(scr->tags[i]);
			memcpy(pos, scr->tags[i], slen);
			pos += slen;
		}
		*(pos++) = '\0';
	}
	XChangeProperty(dpy, scr->root, _XA_NET_DESKTOP_NAMES, _XA_UTF8_STRING, 8,
			PropModeReplace, (unsigned char *) buf, len);
	XChangeProperty(dpy, scr->root, _XA_WIN_WORKSPACE_NAMES, XA_STRING, 8,
			PropModeReplace, (unsigned char *) buf, len);
	data = ecalloc(scr->ntags, sizeof(*data));
	for (i = 0; i < scr->ntags; i++)
		data[i] = scr->dt_tags[i];
	XChangeProperty(dpy, scr->root, _XA_DT_WORKSPACE_LIST, XA_ATOM, 32,
			PropModeReplace, (unsigned char *) data, scr->ntags);
	XChangeProperty(dpy, scr->selwin, _XA_DT_WORKSPACE_LIST, XA_ATOM, 32,
			PropModeReplace, (unsigned char *) data, scr->ntags);
	free(data);
	free(buf);
	names_synced = True;
}

void
ewmh_update_echinus_seltags() {
	Monitor *m;
	long *seltags;
	unsigned int i;

	XPRINTF("%s\n", "Updating _ECHINUS_SELTAGS");
	seltags = ecalloc(scr->ntags, sizeof(seltags[0]));
	for (m = scr->monitors; m != NULL; m = m->next)
		for (i = 0; i < scr->ntags; i++)
			if (m->seltags[i])
				seltags[i] = True;
	XChangeProperty(dpy, scr->root, _XA_ECHINUS_SELTAGS, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) seltags, scr->ntags);
	free(seltags);
}

void
ewmh_update_net_desktop_layout() {
}

void
ewmh_process_net_desktop_layout()
{
	long *card, start = _NET_WM_TOPLEFT;
	unsigned long n = 0;
	int i, r, c;

	XPRINTF("Processing _NET_DESKTOP_LAYOUT\n");
	card = getcard(scr->root, _XA_NET_DESKTOP_LAYOUT, &n);
	if (n >= 4)
		start = card[3];
	if (n >= 3) {
		scr->layout.orient = card[0];
		scr->layout.cols = card[1];
		scr->layout.rows = card[2];
		scr->layout.start = start;
	}
	if (n > 0)
		XFree(card);
	switch (scr->layout.orient) {
	case _NET_WM_ORIENTATION_HORZ:
	case _NET_WM_ORIENTATION_VERT:
		break;
	default:
		scr->layout.orient = _NET_WM_ORIENTATION_HORZ;
		break;
	}
	switch (scr->layout.start) {
	case _NET_WM_TOPLEFT:
	case _NET_WM_TOPRIGHT:
	case _NET_WM_BOTTOMRIGHT:
	case _NET_WM_BOTTOMLEFT:
		break;
	default:
		scr->layout.start = _NET_WM_TOPLEFT;
		break;
	}
	if (!(scr->d.cols = scr->layout.cols) && !(scr->d.rows = scr->layout.rows)) {
		scr->d.rows = 2;
		scr->d.cols = 0;
	}
	if (!scr->d.rows)
		scr->d.rows = (scr->ntags + scr->d.cols - 1) / scr->d.cols;
	if (!scr->d.cols || scr->d.rows * scr->d.cols < scr->ntags)
		scr->d.cols = (scr->ntags + scr->d.rows - 1) / scr->d.rows;
	if (scr->d.rows < scr->m.rows)
		scr->d.rows = scr->m.rows;
	if (scr->d.cols < scr->m.cols)
		scr->d.cols = scr->m.cols;
	/* label row and col in views */
	for (i = 0, r = 0, c = 0; i < scr->ntags;
	     i++, r = i / scr->d.cols, c = i - r * scr->d.cols) {
		scr->views[i].index = i;
		scr->views[i].row = r;
		scr->views[i].col = c;
	}
}

void
ewmh_update_net_number_of_desktops() {
	long data = scr->ntags;

	XPRINTF("%s\n", "Updating _NET_NUMBER_OF_DESKTOPS");
	XChangeProperty(dpy, scr->root, _XA_NET_NUMBER_OF_DESKTOPS, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) &data, 1);
	XChangeProperty(dpy, scr->root, _XA_WIN_WORKSPACE_COUNT, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) &data, 1);
	ewmh_update_net_desktop_modes();
	ewmh_update_net_desktop_viewport();
	ewmh_update_net_desktop_names();
	ewmh_update_echinus_seltags();
}

void
ewmh_process_net_number_of_desktops() {
	long *card;
	unsigned long n = 0;

	card = getcard(scr->root, _XA_NET_NUMBER_OF_DESKTOPS, &n);
	if (n > 0) {
		scr->ntags = card[0];
		XFree(card);
	}
	if (1 > scr->ntags || scr->ntags > 64)
		scr->ntags = atoi(getresource("tags.number", "5"));
}

void
ewmh_process_net_showing_desktop() {
	long *card;
	unsigned long n = 0;

	XPRINTF("%s\n", "Processing _NET_SHOWING_DESKTOP");
	card = getcard(scr->root, _XA_NET_SHOWING_DESKTOP, &n);
	if (n > 0) {
		Bool show = card[0];

		XFree(card);
		if (!show != !scr->showing_desktop)
			toggleshowing();
	}
}


void
ewmh_update_net_showing_desktop() {
	long data = scr->showing_desktop ? 1 : 0;

	XPRINTF("%s\n", "Updating _NET_SHOWING_DESKTOP");
	XChangeProperty(dpy, scr->root, _XA_NET_SHOWING_DESKTOP, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *)&data, 1);
}

void
ewmh_update_net_desktop_geometry() {
	long data[2];

	XPRINTF("%s\n", "Updating _NET_DESKTOP_GEOMETRY");
	data[0] = DisplayWidth(dpy, scr->screen);
	data[1] = DisplayHeight(dpy, scr->screen);
	XChangeProperty(dpy, scr->root, _XA_NET_DESKTOP_GEOMETRY, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *)data, 2);
	data[0] = 1;
	data[1] = 1;
	XChangeProperty(dpy, scr->root, _XA_WIN_AREA_COUNT, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *)&data, 2);
}

void
ewmh_update_net_monitor_geometry(Client *c) {
	Monitor *m;
	unsigned int i, n;
	long *data;

	for (n = 0, m = scr->monitors; m; m = m->next, n++) ;
	data = ecalloc(4 * n, sizeof(*data));
	for (i = 0, m = scr->monitors; m; m = m->next) {
		data[i++] = m->sc.x;
		data[i++] = m->sc.y;
		data[i++] = m->sc.w;
		data[i++] = m->sc.h;
	}
	XChangeProperty(dpy, scr->root, _XA_NET_MONITOR_GEOMETRY, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *)data, i);
}

void
ewmh_update_net_virtual_roots() {
	XDeleteProperty(dpy, scr->root, _XA_NET_VIRTUAL_ROOTS);
}

void
ewmh_update_net_current_desktop() {
	long *data;
	unsigned int i, n;
	Monitor *m;

	XPRINTF("%s\n", "Updating _NET_CURRENT_DESKTOP");
	for (n = 0, m = scr->monitors; m; m = m->next, n++) ;
	data = ecalloc(n, sizeof(*data));
	for (i = 0, m = scr->monitors; m; data[i++] = m->curtag, m = m->next) ;
	XChangeProperty(dpy, scr->root, _XA_NET_CURRENT_DESKTOP, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) data, n);
	XChangeProperty(dpy, scr->root, _XA_WIN_WORKSPACE, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) data, n);
	XChangeProperty(dpy, scr->root, _XA_WM_DESKTOP, _XA_WM_DESKTOP, 32,
			PropModeReplace, (unsigned char *) data, n);
	for (i = 0; i < n; i++)
		data[i] = scr->dt_tags[data[i]];
	XChangeProperty(dpy, scr->root, _XA_DT_WORKSPACE_CURRENT, XA_ATOM, 32,
			PropModeReplace, (unsigned char *) data, n);
	XChangeProperty(dpy, scr->selwin, _XA_DT_WORKSPACE_CURRENT, XA_ATOM, 32,
			PropModeReplace, (unsigned char *) data, n);
	free(data);
	ewmh_update_echinus_layout_name();
	ewmh_update_echinus_seltags();
}

void
ewmh_update_net_visible_desktops() {
	long *data;
	unsigned int i, n;
	Monitor *m;

	XPRINTF("%s\n", "Updating _NET_VISIBLE_DESKTOPS");
	for (n = 0, m = scr->monitors; m; m = m->next, n++) ;
	data = ecalloc(n, sizeof(*data));
	for (i = 0, m = scr->monitors; m; data[i++] = m->curtag, m = m->next) ;
	XChangeProperty(dpy, scr->root, _XA_NET_VISIBLE_DESKTOPS, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) data, n);
	free(data);
}

static Bool
isomni(Client *c) {
	unsigned int i;

	if (!c->is.sticky)
		for (i = 0; i < scr->ntags; i++)
			if (!c->tags[i])
				return False;
	return True;
}

static Bool
islost(Client *c) {
	unsigned int i;
	for (i = 0; i < scr->ntags; i++)
		if (c->tags[i])
			return False;
	return True;
}

#define DT_WORKSPACE_HINTS_WSFLAGS	(1<<0)
#define DT_WORKSPACE_HINTS_WORKSPACES	(1<<1)

#define DT_WORKSPACE_FLAGS_OCCUPY_ALL	(1<<0)

Bool
ewmh_process_net_window_desktop(Client *c)
{
	long desktop, *desktops = NULL;
	unsigned long n = 0, j, k;
	unsigned int i;
	Bool goodone;

	desktops = getcard(c->win, _XA_NET_WM_DESKTOP, &n);
	if (n > 0) {
		desktop = desktops[0];
		if ((desktop & 0xffffffff) == 0xffffffff) {
			for (i = 0; i < scr->ntags; i++)
				c->tags[i] = True;
			XFree(desktops);
			return True;
		} else {
			for (goodone = False, i = 0; !goodone && i < n; i++)
				if (0 <= desktops[i] && desktops[i] < scr->ntags)
					goodone = True;
			if (goodone) {
				for (i = 0; i < scr->ntags; i++)
					c->tags[i] = False;
				for (j = 0; j < n; j++)
					if (0 <= desktops[j] && desktops[j] < scr->ntags)
						c->tags[desktops[j]] = True;
				XFree(desktops);
				return True;
			}
		}
		XFree(desktops);
		return False;
	}
	desktops = getcard(c->win, _XA_WIN_WORKSPACE, &n);
	if (n > 0) {
		desktop = desktops[0];
		if ((desktop & 0xffffffff) == 0xffffffff) {
			for (i = 0; i < scr->ntags; i++)
				c->tags[i] = True;
			XFree(desktops);
			return True;
		} else {
			for (goodone = False, i = 0; !goodone && i < n; i++)
				if (0 <= desktops[i] && desktops[i] < scr->ntags)
					goodone = True;
			if (goodone) {
				for (i = 0; i < scr->ntags; i++)
					c->tags[i] = False;
				for (j = 0; j < n; j++)
					if (0 <= desktops[j] && desktops[j] < scr->ntags)
						c->tags[desktops[j]] = True;
				XFree(desktops);
				return True;
			}
		}
		XFree(desktops);
		return False;
	}
	desktops = getcard(c->win, _XA_DT_WORKSPACE_HINTS, &n);
	if (n >= 4) {
		if ((desktops[0] != 1)) {
			XFree(desktops);
			return False;
		}
		if ((desktops[1] & DT_WORKSPACE_HINTS_WSFLAGS) &&
		    (desktops[2] & DT_WORKSPACE_FLAGS_OCCUPY_ALL)) {
			for (i = 0; i < scr->ntags; i++)
				c->tags[i] = True;
			XFree(desktops);
			return True;
		} else
		    if ((desktops[1] & DT_WORKSPACE_HINTS_WORKSPACES) &&
			(n >= 4 + desktops[3])) {
			for (goodone = False, i = 4; !goodone && i < desktops[3] + 4; i++)
				for (k = 0; !goodone && k < scr->ntags; k++)
					if (scr->dt_tags[k] == desktops[i])
						goodone = True;
			if (goodone) {
				for (i = 0; i < scr->ntags; i++)
					c->tags[i] = False;
				for (j = 4; j < desktops[3] + 4; j++)
					for (k = 0; k < scr->ntags; k++)
						if (scr->dt_tags[k] == desktops[j])
							c->tags[k] = True;
				XFree(desktops);
				return True;
			}
		}
		XFree(desktops);
		return False;
	}
	return False;
}

void
ewmh_update_net_window_desktop_mask(Client *c) {
	unsigned int longs = (scr->ntags+31)>>5;
	unsigned long data[longs];
	unsigned int i, j, k, l;

	XPRINTF("Updating _NET_WM_DESKTOP_MASK for 0x%lx\n", c->win);
	for (j = 0, k = 0; j < longs; j++, k += 32)
		for (i = 0, l = k, data[j] = 0; i < 32 && l < scr->ntags; i++, l++)
			if (c->tags[l])
				data[j] |= (1<<i);
	XChangeProperty(dpy, c->win, _XA_WIN_WORKSPACES, XA_CARDINAL, 32,
		PropModeReplace, (unsigned char *)data, longs);
	XChangeProperty(dpy, c->win, _XA_NET_WM_DESKTOP_MASK, XA_CARDINAL, 32,
		PropModeReplace, (unsigned char *)data, longs);
}

void
ewmh_update_net_window_desktop(Client *c) {
	XPRINTF("Updating _NET_WM_DESKTOP for 0x%lx\n", c->win);
	if (isomni(c)) {
		long i = -1;

		XChangeProperty(dpy, c->win, _XA_NET_WM_DESKTOP, XA_CARDINAL, 32,
				PropModeReplace, (unsigned char *) &i, 1);
		XChangeProperty(dpy, c->win, _XA_WIN_WORKSPACE, XA_CARDINAL, 32,
				PropModeReplace, (unsigned char *) &i, 1);
		XChangeProperty(dpy, c->win, _XA_WM_DESKTOP, _XA_WM_DESKTOP, 32,
				PropModeReplace, (unsigned char *) &i, 1);
	} else {
		unsigned int i, j, n;
		long *data;

		for (n = 0, i = 0; i < scr->ntags; i++)
			if (c->tags[i])
				n++;
		data = calloc(n, sizeof(*data));
		for (j = 0, i = 0; i < scr->ntags; i++)
			if (c->tags[i])
				data[j++] = i;
		XChangeProperty(dpy, c->win, _XA_NET_WM_DESKTOP, XA_CARDINAL, 32,
				PropModeReplace, (unsigned char *) data, n);
		XChangeProperty(dpy, c->win, _XA_WIN_WORKSPACE, XA_CARDINAL, 32,
				PropModeReplace, (unsigned char *) data, n);
		XChangeProperty(dpy, c->win, _XA_WM_DESKTOP, _XA_WM_DESKTOP, 32,
				PropModeReplace, (unsigned char *) data, n);
		free(data);
	}
	{
		unsigned int i, j, n;
		long *data;

		for (n = 0, i = 0; i < scr->ntags; i++)
			if (c->tags[i])
				n++;
		data = ecalloc(n, sizeof(*data));
		for (j = 0, i = 0; i < scr->ntags; i++)
			if (c->tags[i])
				data[j++] = scr->dt_tags[i];
		XChangeProperty(dpy, c->win, _XA_DT_WORKSPACE_PRESENCE, XA_ATOM, 32,
				PropModeReplace, (unsigned char *) data, n);
		free(data);
	}
	ewmh_update_net_window_desktop_mask(c);
	ewmh_update_net_window_state(c);
}

Bool
ewmh_process_net_window_desktop_mask(Client *c) {
	unsigned int i, j, k, l;
	long *desktops = NULL;
	unsigned long n = 0;
	Bool *prev;

	desktops = getcard(c->win, _XA_WIN_WORKSPACES, &n);
	if (n > 0) {
		prev = ecalloc(scr->ntags, sizeof(*prev));
		memcpy(prev, c->tags, scr->ntags * sizeof(*prev));
		for (j = 0, k = 0; j < n; j++, k += 32)
			for (i = 0, l = k; i < 32 && l < scr->ntags; i++, l++)
				if (desktops[j] & (1<<i))
					c->tags[l] = True;
		for (i = 0; i < scr->ntags && !c->tags[i]; i++) ;
		if (i < scr->ntags) {
			free(prev);
			return True;
		}
		memcpy(c->tags, prev, scr->ntags * sizeof(*prev));
		free(prev);
	} else {
		if (desktops)
			XFree(desktops);
		desktops = getcard(c->win, _XA_NET_WM_DESKTOP_MASK, &n);
		if (n > 0) {
			prev = ecalloc(scr->ntags, sizeof(*prev));
			memcpy(prev, c->tags, scr->ntags * sizeof(*prev));
			for (j = 0, k = 0; j < n; j++, k += 32)
				for (i = 0, l = k; i < 32 && l < scr->ntags; i++, l++)
					if (desktops[j] & (1<<i))
						c->tags[l] = True;
			for (i = 0; i < scr->ntags && !c->tags[i]; i++) ;
			if (i < scr->ntags) {
				free(prev);
				return True;
			}
			memcpy(c->tags, prev, scr->ntags * sizeof(*prev));
			free(prev);
		}
	}
	if (desktops)
		XFree(desktops);
	return False;
}

void
ewmh_update_net_work_area() {
	long *geoms, longs = scr->ntags * 4;
	int i, j, x, y, w, h, l = 0, r = 0, t = 0, b = 0;
	Monitor *m;
	long workarea[4];

	XPRINTF("%s\n", "Updating _NET_WORKAREA");
	geoms = ecalloc(longs, sizeof(geoms[0]));

	for (m = scr->monitors; m; m = m->next) {
		l = max(l, m->struts[LeftStrut]);
		r = max(r, m->struts[RightStrut]);
		t = max(t, m->struts[TopStrut]);
		b = max(b, m->struts[BotStrut]);
	}
	x = l;
	y = t;
	w = DisplayWidth(dpy, scr->screen) - (l + r);
	h = DisplayHeight(dpy, scr->screen) - (t + b);
	for (i = 0, j = 0; i < scr->ntags; i++) {
		geoms[j++] = x;
		geoms[j++] = y;
		geoms[j++] = w;
		geoms[j++] = h;
	}
	XChangeProperty(dpy, scr->root, _XA_NET_WORKAREA, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) geoms, longs);
	free(geoms);
	workarea[0] = x;
	workarea[1] = y;
	workarea[2] = x + w;
	workarea[3] = y + h;
	XChangeProperty(dpy, scr->root, _XA_WIN_WORKAREA, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) workarea, 4L);
}

void
ewmh_process_net_desktop_names() {
	int format, status;
	Atom real;
	unsigned long nitems = 0, extra = 0, bytes = 1;
	char *ret = NULL, *pos, *str;
	unsigned int i, len;

	names_synced = True;
	do {
		if (ret)
			XFree((unsigned char *) ret);
		bytes += 4 * extra;
		status = XGetWindowProperty(dpy, scr->root, _XA_NET_DESKTOP_NAMES, 0L, bytes, False,
				       _XA_UTF8_STRING, &real, &format, &nitems, &extra,
				       (unsigned char **) &ret);
		if (status != Success) {
			names_synced = False;
			break;
		}
	} while (extra && bytes == 1);

	for (pos = ret, i = 0; nitems && i < scr->ntags; i++) {
		if ((len = strnlen(pos, nitems))) {
			if ((str = strndup(pos, nitems))) {
				XPRINTF("Assigning name '%s' to tag %u\n", str, i);
				free(scr->tags[i]);
				scr->tags[i] = str;
			}
		} else {
			free(scr->tags[i]);
			inittag(i);
		}
		nitems -= len;
		pos += len;
		if (nitems) {
			--nitems;
			++pos;
		}
	}
	for (; i < scr->ntags; i++) {
		free(scr->tags[i]);
		inittag(i);
		names_synced = False;
	}
	if (ret)
		XFree(ret);
	ewmh_update_net_desktop_names();
}

void
ewmh_update_net_active_window() {
	Window win;

	XPRINTF("%s\n", "Updating _NET_ACTIVE_WINDOW");
	win = sel ? sel->win : None;
	XChangeProperty(dpy, scr->root, _XA_NET_ACTIVE_WINDOW, XA_WINDOW, 32,
			PropModeReplace, (unsigned char *) &win, 1);
	XChangeProperty(dpy, scr->root, _XA_WIN_FOCUS, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) &win, 1);
}

void
ewmh_update_net_desktop_modes() {
	long *data;
	unsigned int i;

	XPRINTF("%s\n", "Updating _NET_DESKTOP_MODES");
	data = ecalloc(scr->ntags, sizeof(data[0]));
	for (i = 0; i < scr->ntags; i++) {
		switch (scr->views[i].layout->symbol) {
		case 'i':
		case 'f':
			data[i] = _XA_NET_DESKTOP_MODE_FLOATING;
			break;
		case 't':
			data[i] = _XA_NET_DESKTOP_MODE_TILED;
			break;
		case 'b':
			data[i] = _XA_NET_DESKTOP_MODE_BOTTOM_TILED;
			break;
		case 'm':
			data[i] = _XA_NET_DESKTOP_MODE_MONOCLE;
			break;
		case 'u':
			data[i] = _XA_NET_DESKTOP_MODE_TOP_TILED;
			break;
		case 'l':
			data[i] = _XA_NET_DESKTOP_MODE_LEFT_TILED;
			break;
		default:
			data[i] = None;
			break;
		}
	}
	XChangeProperty(dpy, scr->root, _XA_NET_DESKTOP_MODES, XA_ATOM, 32,
			PropModeReplace, (unsigned char *) data, scr->ntags);
	free(data);
	ewmh_update_echinus_layout_name();
}

#define MWM_HINTS_FUNCTIONS	(1<<0)
#define MWM_HINTS_DECORATIONS	(1<<1)
#define MWM_HINTS_INPUT_MODE	(1<<2)
#define MWM_HINTS_STATUS	(1<<3)

#define MWM_FUNC_ALL		(1<<0)
#define MWM_FUNC_RESIZE		(1<<1)
#define MWM_FUNC_MOVE		(1<<2)
#define MWM_FUNC_MINIMIZE	(1<<3)
#define MWM_FUNC_MAXIMIZE	(1<<4)
#define MWM_FUNC_CLOSE		(1<<5)
#define MWM_FUNC_STANDARD	(MWM_FUNC_ALL|MWM_FUNC_RESIZE|MWM_FUNC_MOVE|\
				 MWM_FUNC_MINIMIZE|MWM_FUNC_MAXIMIZE|MWM_FUNC_CLOSE)
#define MWM_FUNC_SHADE		(1<<6)
#define MWM_FUNC_STICK		(1<<7)
#define MWM_FUNC_FULLSCREEN	(1<<8)
#define MWM_FUNC_ABOVE		(1<<9)
#define MWM_FUNC_BELOW		(1<<10)
#define MWM_FUNC_MAXIMUS	(1<<11)
#define MWM_FUNC_NONSTD		(MWM_FUNC_SHADE|MWM_FUNC_STICK|MWM_FUNC_FULLSCREEN|\
				 MWM_FUNC_ABOVE|MWM_FUNC_BELOW|MWM_FUNC_MAXIMUS)

#define MWM_DECOR_ALL		(1<<0)
#define MWM_DECOR_BORDER	(1<<1)
#define MWM_DECOR_RESIZEH	(1<<2)
#define MWM_DECOR_TITLE		(1<<3)
#define MWM_DECOR_MENU		(1<<4)
#define MWM_DECOR_MINIMIZE	(1<<5)
#define MWM_DECOR_MAXIMIZE	(1<<6)
#define MWM_DECOR_STANDARD	(MWM_DECOR_ALL|MWM_DECOR_BORDER|MWM_DECOR_RESIZEH|\
				 MWM_DECOR_TITLE|MWM_DECOR_MENU|MWM_DECOR_MINIMIZE\
				 MWM_DECOR_MAXIMIZE)
#define MWM_DECOR_CLOSE		(1<<7)
#define MWM_DECOR_RESIZE	(1<<8)
#define MWM_DECOR_SHADE		(1<<9)
#define MWM_DECOR_STICK		(1<<10)
#define MWM_DECOR_MAXIMUS	(1<<11)
#define MWM_DECOR_NONSTD	(MWM_DECOR_CLOSE|MWM_DECOR_RESIZE|MWM_DECOR_SHADE|\
				 MWM_DECOR_STICK|MWM_DECOR_MAXIMUS)h

#define MWM_INPUT_MODELESS		 (1<<0)
#define MWM_INPUT_APPLICATION_MODAL	 (1<<1)
#define MWM_INPUT_SYSTEM_MODAL		 (1<<2)
#define MWM_INPUT_FULL_APPLICATION_MODAL (1<<3)

#define MWM_TEAROFF_WINDOW	(1<<0)

void
mwmh_process_motif_wm_hints(Client *c)
{
	long hint, *hints = NULL;
	unsigned long n = 0;

	hints = getcard(c->win, _XA_MOTIF_WM_HINTS, &n);
	if (n > 1) {
		if ((hints[0] & MWM_HINTS_FUNCTIONS) && n >= 2 &&
		    !((hint = hints[1]) & MWM_FUNC_ALL)) {
			if (!(hint & MWM_FUNC_MOVE))
				c->can.move = False;
			if (!(hint & MWM_FUNC_RESIZE)) {
				c->can.size = False;
				c->can.fill = False;
				c->can.max = False;
				c->can.fill = False;
			}
			if (!(hint & MWM_FUNC_MINIMIZE)) {
				c->can.min = False;
				c->can.shade = False;
			}
			if (!(hint & MWM_FUNC_MAXIMIZE)) {
				c->can.max = False;
				c->can.fill = False;
				c->can.fs = False;
			}
			if (!(hint & MWM_FUNC_CLOSE))
				c->can.close = False;
		}
		if ((hints[0] & MWM_HINTS_DECORATIONS) && n >= 3 &&
		    !((hint = hints[2]) & MWM_DECOR_ALL)) {
			if (!(hint & MWM_DECOR_BORDER))
				c->has.border = False;
			if (!(hint & MWM_DECOR_RESIZEH)) {
				c->has.grips = False;
				c->has.but.size = False;
				c->has.but.max = False;
				c->has.but.fill = False;
			}
			if (!(hint & MWM_DECOR_TITLE))
				c->has.title = False;
			if (!(hint & MWM_DECOR_MENU))
				c->has.but.menu = False;
			if (!(hint & MWM_DECOR_MINIMIZE))
				c->has.but.min = False;
			if (!(hint & MWM_DECOR_MAXIMIZE))
				c->has.but.max = False;
		}
		if ((hints[0] & MWM_HINTS_INPUT_MODE) && n >= 4 &&
				(hint = hints[3])) {
			/* handled by ICCCM 2.0 */
			if (hint & MWM_INPUT_MODELESS) {
				// c->is.modal = ModalModeless;
			} else if (hint & MWM_INPUT_APPLICATION_MODAL) {
				// c->is.modal = ModalPrimary;
			} else if (hint & MWM_INPUT_SYSTEM_MODAL) {
				// c->is.modal = ModalSystem;
			} else if (hint & MWM_INPUT_FULL_APPLICATION_MODAL) {
				// c->is.modal = ModalGroup;
				c->is.grptrans = True;
			}

		}
		if ((hints[0] & MWM_HINTS_STATUS) && n >= 5 &&
				(hint = hints[4])) {
			if (hint & MWM_TEAROFF_WINDOW) {
				/* TODO: set window type */
			}
		}

	}
	if (hints)
		XFree(hints);
}

#define DTWM_HINTS_FUNCTIONS	    (1<<0)
#define DTWM_HINTS_BEHAVIORS	    (1<<1)
#define DTWM_HINTS_ATTACH_WINDOW    (1<<2)

#define DTWM_FUNCTION_ALL	    (1<<0)
#define DTWM_FUNCTION_OCCUPY_WS	    (1<<1)

#define DTWM_BEHAVIOR_PANEL	    (1<<1)
#define DTWM_BEHAVIOR_SUBPANEL	    (1<<2)
#define DTWM_BEHAVIOR_SUB_RESTORED  (1<<3)

void
mwmh_process_dt_wm_hints(Client *c)
{
	long hint, *hints = NULL;
	unsigned long n = 0;

	hints = getcard(c->win, _XA_DT_WM_HINTS, &n);
	if (n > 1) {
		if ((hints[0] & DTWM_HINTS_FUNCTIONS) && n >= 2 &&
				!((hint = hints[1]) && DTWM_FUNCTION_ALL)) {
			if (!(hint & DTWM_FUNCTION_OCCUPY_WS))
				c->can.tag = False;
		}
		if ((hints[0] & DTWM_HINTS_BEHAVIORS) && n >= 3) {
			/* TODO treat subpanel stuff like a dock app */
		}
		if ((hints[0] & DTWM_HINTS_ATTACH_WINDOW) && n >= 4) {
			/* TODO treat subpanel stuff like a dock app */
		}
	}
	if (hints)
		XFree(hints);
}

void
getmwmhints(Window win, Bool *title, Bool *grips, int *border) {
	long *hints = NULL;
	unsigned long n = 0;

	hints = getcard(win, _XA_MOTIF_WM_HINTS, &n);
	if (n > 0) {
		if ((hints[0] & MWM_HINTS_DECORATIONS) && n > 1) {
			*border = (hints[1] & (MWM_DECOR_ALL|MWM_DECOR_BORDER)) ? scr->style.border : 0;
			*title = (hints[1] & (MWM_DECOR_ALL|MWM_DECOR_TITLE)) ? True : False;
			*grips = (hints[1] & (MWM_DECOR_ALL|MWM_DECOR_RESIZEH)) ? True : False;
		}
		XFree(hints);
	}
}

#define WIN_LAYER_DESKTOP      0
#define WIN_LAYER_BELOW        2
#define WIN_LAYER_NORMAL       4
#define WIN_LAYER_ONTOP        6
#define WIN_LAYER_DOCK         8
#define WIN_LAYER_ABOVE_DOCK  10
#define WIN_LAYER_MENU        12

void
wmh_update_win_layer(Client * c)
{
	unsigned long layer = WIN_LAYER_NORMAL;

	if (WTCHECK(c, WindowTypeDesk))
		layer = WIN_LAYER_DESKTOP;
	else if (WTCHECK(c, WindowTypeMenu))
		layer = WIN_LAYER_MENU;
	else if (c->is.below)
		layer = WIN_LAYER_BELOW;
	else if (WTCHECK(c, WindowTypeDock) && c->is.above)
		layer = WIN_LAYER_ABOVE_DOCK;
	else if (WTCHECK(c, WindowTypeDock) && !c->is.above)
		layer = WIN_LAYER_DOCK;
	else if (c->wintype & ~(WTFLAG(WindowTypeNormal) | WTFLAG(WindowTypeDesk) |
				WTFLAG(WindowTypeDock) | WTFLAG(WindowTypeMenu)))
		layer = 13;
	else
		layer = WIN_LAYER_NORMAL;
	XChangeProperty(dpy, c->win, _XA_WIN_LAYER, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) &layer, 1L);
}

void
ewmh_update_net_window_actions(Client *c) {
	long action[32];
	int actions = 0;

	XPRINTF("Updating _NET_WM_ALLOWED_ACTIONS for 0x%lx\n", c->win);
	if (c->can.move)
		action[actions++] = _XA_NET_WM_ACTION_MOVE;
	if (c->can.size)
		action[actions++] = _XA_NET_WM_ACTION_RESIZE;
	if (c->can.min)
		action[actions++] = _XA_NET_WM_ACTION_MINIMIZE;
	if (c->can.shade)
		action[actions++] = _XA_NET_WM_ACTION_SHADE;
	if (c->can.stick)
		action[actions++] = _XA_NET_WM_ACTION_STICK;
	if (c->can.maxh || c->can.max)
		action[actions++] = _XA_NET_WM_ACTION_MAXIMIZE_HORZ;
	if (c->can.maxv || c->can.max)
		action[actions++] = _XA_NET_WM_ACTION_MAXIMIZE_VERT;
	if (c->can.fs)
		action[actions++] = _XA_NET_WM_ACTION_FULLSCREEN;
	if (c->can.tag)
		action[actions++] = _XA_NET_WM_ACTION_CHANGE_DESKTOP;
	if (c->can.close)
		action[actions++] = _XA_NET_WM_ACTION_CLOSE;
	if (c->can.above)
		action[actions++] = _XA_NET_WM_ACTION_ABOVE;
	if (c->can.below)
		action[actions++] = _XA_NET_WM_ACTION_BELOW;
	/* following two are non-standard */
	if (c->can.fill)
		action[actions++] = _XA_NET_WM_ACTION_FILL;
	if (c->can.floats)
		action[actions++] = _XA_NET_WM_ACTION_FLOAT;

	XChangeProperty(dpy, c->win, _XA_NET_WM_ALLOWED_ACTIONS, XA_ATOM, 32,
		PropModeReplace, (unsigned char *) action, actions);
}

#define WIN_STATE_STICKY          (1<<0) /* everyone knows sticky */
#define WIN_STATE_MINIMIZED       (1<<1) /* Reserved - definition is unclear */
#define WIN_STATE_MAXIMIZED_VERT  (1<<2) /* window in maximized V state */
#define WIN_STATE_MAXIMIZED_HORIZ (1<<3) /* window in maximized H state */
#define WIN_STATE_HIDDEN          (1<<4) /* not on taskbar but window visible */
#define WIN_STATE_SHADED          (1<<5) /* shaded (MacOS / Afterstep style) */
#define WIN_STATE_HID_WORKSPACE   (1<<6) /* not on current desktop */
#define WIN_STATE_HID_TRANSIENT   (1<<7) /* owner of transient is hidden */
#define WIN_STATE_FIXED_POSITION  (1<<8) /* window is fixed in position even */
#define WIN_STATE_ARRANGE_IGNORE  (1<<9) /* ignore for auto arranging */

void
ewmh_update_net_window_state(Client *c)
{
	long winstate[16];
	int states = 0;
	unsigned long state = 0;

	/* do not update until we have finished reading it */
	if (!c->is.managed)
		return;

	XPRINTF("Updating _NET_WM_STATE for 0x%lx\n", c->win);
	if (c->is.modal)
		winstate[states++] = _XA_NET_WM_STATE_MODAL;
	if (c->is.sticky)
		winstate[states++] = _XA_NET_WM_STATE_STICKY;
	if (c->is.maxv)
		winstate[states++] = _XA_NET_WM_STATE_MAXIMIZED_VERT;
	if (c->is.maxh)
		winstate[states++] = _XA_NET_WM_STATE_MAXIMIZED_HORZ;
	if (c->is.shaded)
		winstate[states++] = _XA_NET_WM_STATE_SHADED;
	if (c->skip.taskbar)
		winstate[states++] = _XA_NET_WM_STATE_SKIP_TASKBAR;
	if (c->skip.pager)
		winstate[states++] = _XA_NET_WM_STATE_SKIP_PAGER;
	if (c->is.icon || c->is.hidden)
		winstate[states++] = _XA_NET_WM_STATE_HIDDEN;
	/* FIXME: should be is.fs not is.max */
	if (c->is.max)
		winstate[states++] = _XA_NET_WM_STATE_FULLSCREEN;
	if (c->is.above)
		winstate[states++] = _XA_NET_WM_STATE_ABOVE;
	if (c->is.below)
		winstate[states++] = _XA_NET_WM_STATE_BELOW;
	if (c->is.attn)
		winstate[states++] = _XA_NET_WM_STATE_DEMANDS_ATTENTION;
	if (c->is.focused)
		winstate[states++] = _XA_NET_WM_STATE_FOCUSED;

	/* following are non-standard */
	if (!c->can.move)
		winstate[states++] = _XA_NET_WM_STATE_FIXED;
	if (c->skip.arrange)
		winstate[states++] = _XA_NET_WM_STATE_FLOATING;
	if (c->is.fill)
		winstate[states++] = _XA_NET_WM_STATE_FILLED;

	XChangeProperty(dpy, c->win, _XA_NET_WM_STATE, XA_ATOM, 32,
			PropModeReplace, (unsigned char *) winstate, states);

	XPRINTF("Updating _WIN_STATE for 0x%lx\n", c->win);
	if (c->is.sticky)
		state |= WIN_STATE_STICKY;
	if (c->is.icon)
		state |= WIN_STATE_MINIMIZED;
	if (c->is.maxv)
		state |= WIN_STATE_MAXIMIZED_VERT;
	if (c->is.maxh)
		state |= WIN_STATE_MAXIMIZED_HORIZ;
	if (c->skip.taskbar || c->is.hidden)	/* not sure which */
		state |= WIN_STATE_HIDDEN;
	if (c->is.shaded)
		state |= WIN_STATE_SHADED;
	if (c->is.banned)
		state |= WIN_STATE_HID_WORKSPACE;
	if (c->is.icon && c->transfor)
		state |= WIN_STATE_HID_TRANSIENT;
	if (!c->can.move)
		state |= WIN_STATE_FIXED_POSITION;
	if (c->skip.arrange)
		state |= WIN_STATE_ARRANGE_IGNORE;

	XChangeProperty(dpy, c->win, _XA_WIN_STATE, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) &state, 1);
	wmh_update_win_layer(c);
	ewmh_update_net_window_actions(c);
}

void
wmh_process_state_mask(Client *c, unsigned int mask, unsigned int change)
{
	if (mask & WIN_STATE_STICKY)
		if (((change & WIN_STATE_STICKY) && !c->is.sticky) ||
		    (!(change & WIN_STATE_STICKY) && c->is.sticky))
			togglesticky(c);
	if (mask & WIN_STATE_MINIMIZED)
		if (((change & WIN_STATE_MINIMIZED) && !c->is.icon) ||
		    (!(change & WIN_STATE_MINIMIZED) && c->is.icon))
			togglemin(c);
	if (mask & WIN_STATE_MAXIMIZED_VERT)
		if (((change & WIN_STATE_MAXIMIZED_VERT) && !c->is.maxv) ||
		    (!(change & WIN_STATE_MAXIMIZED_VERT) && c->is.maxv))
			togglemaxv(c);
	if (mask & WIN_STATE_MAXIMIZED_HORIZ)
		if (((change & WIN_STATE_MAXIMIZED_HORIZ) && !c->is.maxh) ||
		    (!(change & WIN_STATE_MAXIMIZED_HORIZ) && c->is.maxh))
			togglemaxh(c);
	if (mask & WIN_STATE_HIDDEN)
		if (((change & WIN_STATE_HIDDEN) && !c->is.hidden) ||
		    (!(change & WIN_STATE_HIDDEN) && c->is.hidden))
			togglehidden(c);
	if (mask & WIN_STATE_SHADED)
		if (((change & WIN_STATE_SHADED) && !c->is.shaded) ||
		    (!(change & WIN_STATE_SHADED) && c->is.shaded))
			toggleshade(c);
	if (mask & WIN_STATE_HID_WORKSPACE) {
		/* read-only */
	}
	if (mask & WIN_STATE_HID_TRANSIENT) {
		/* read-only */
	}
	if (mask & WIN_STATE_FIXED_POSITION)
		if (((change & WIN_STATE_FIXED_POSITION) && !c->can.move) ||
		    (!(change & WIN_STATE_FIXED_POSITION) && c->can.move)) {
			c->can.move = !c->can.move;
			arrange(NULL);
		}
	if (mask & WIN_STATE_ARRANGE_IGNORE)
		if (((change & WIN_STATE_ARRANGE_IGNORE) && !c->skip.arrange) ||
		    (!(change & WIN_STATE_ARRANGE_IGNORE) && c->skip.arrange))
			togglefloating(c);
}

void
ewmh_process_state_atom(Client *c, Atom state, int set) {
	if (0) {
	} else if (state == _XA_NET_WM_STATE_MODAL) {
		/* _NET_WM_STATE_MODAL indicates that this is a modal dialog box.  If the
		   WM_TRANSIENT_FOR hint is set to another toplevel window, the dialog is
		   modal for that window; if WM_TRANSIENT_FOR is not set or set to the
		   root window the dialog is modal for its window group */
		if ((set == _NET_WM_STATE_ADD && !c->is.modal) ||
		    (set == _NET_WM_STATE_REMOVE && c->is.modal) ||
		    (set == _NET_WM_STATE_TOGGLE))
			togglemodal(c);
	} else if (state == _XA_NET_WM_STATE_STICKY) {
		if ((set == _NET_WM_STATE_ADD && !c->is.sticky) ||
		    (set == _NET_WM_STATE_REMOVE && c->is.sticky) ||
		    (set == _NET_WM_STATE_TOGGLE))
			togglesticky(c);
	} else if (state == _XA_NET_WM_STATE_MAXIMIZED_VERT) {
		if ((set == _NET_WM_STATE_ADD && !c->is.maxv) ||
		    (set == _NET_WM_STATE_REMOVE && c->is.maxv) ||
		    (set == _NET_WM_STATE_TOGGLE))
			togglemaxv(c);
	} else if (state == _XA_NET_WM_STATE_MAXIMIZED_HORZ) {
		if ((set == _NET_WM_STATE_ADD && !c->is.maxh) ||
		    (set == _NET_WM_STATE_REMOVE && c->is.maxh) ||
		    (set == _NET_WM_STATE_TOGGLE))
			togglemaxh(c);
	} else if (state == _XA_NET_WM_STATE_SHADED) {
		if ((set == _NET_WM_STATE_ADD && !c->is.shaded) ||
		    (set == _NET_WM_STATE_REMOVE && c->is.shaded) ||
		    (set == _NET_WM_STATE_TOGGLE))
			toggleshade(c);
	} else if (state == _XA_NET_WM_STATE_SKIP_TASKBAR) {
		if ((set == _NET_WM_STATE_ADD && !c->skip.taskbar) ||
		    (set == _NET_WM_STATE_REMOVE && c->skip.taskbar) ||
		    (set == _NET_WM_STATE_TOGGLE))
			toggletaskbar(c);
	} else if (state == _XA_NET_WM_STATE_SKIP_PAGER) {
		if ((set == _NET_WM_STATE_ADD && !c->skip.pager) ||
		    (set == _NET_WM_STATE_REMOVE && c->skip.pager) ||
		    (set == _NET_WM_STATE_TOGGLE))
			togglepager(c);
	} else if (state == _XA_NET_WM_STATE_HIDDEN) {
		/* Implementation note: if an Application asks to toggle
		   _NET_WM_STATE_HIDDEN the Window Manager should probably just ignore
		   the request, since _NET_WM_STATE_HIDDEN is a function of some other
		   aspect of the window such as minimization, rather than an independent
		   state. */
		if ((set == _NET_WM_STATE_ADD && !c->is.hidden) ||
		    (set == _NET_WM_STATE_REMOVE && c->is.hidden) ||
		    (set == _NET_WM_STATE_TOGGLE))
			togglehidden(c);
	} else if (state == _XA_NET_WM_STATE_FULLSCREEN) {
		/* FIXME: this needs to be fullscreen not maximize */
		if ((set == _NET_WM_STATE_ADD || set == _NET_WM_STATE_TOGGLE)
		    && !c->is.max) {
			c->wasfloating = c->skip.arrange;
			if (!c->skip.arrange)
				togglefloating(c);
			togglemax(c);
		} else if ((set == _NET_WM_STATE_REMOVE ||
			    set == _NET_WM_STATE_TOGGLE) && c->is.max) {
			togglemax(c);
			if (!c->wasfloating)
				togglefloating(c);
		}
		DPRINT;
		arrange(curmonitor());
		XPRINTF("%s: x%d y%d w%d h%d\n", c->name, c->c.x, c->c.y, c->c.w, c->c.h);
	} else if (state == _XA_NET_WM_STATE_ABOVE) {
		if ((set == _NET_WM_STATE_ADD && !c->is.above) ||
		    (set == _NET_WM_STATE_REMOVE && c->is.above) ||
		    (set == _NET_WM_STATE_TOGGLE))
			toggleabove(c);
	} else if (state == _XA_NET_WM_STATE_BELOW) {
		if ((set == _NET_WM_STATE_ADD && !c->is.below) ||
		    (set == _NET_WM_STATE_REMOVE && c->is.below) ||
		    (set == _NET_WM_STATE_TOGGLE))
			togglebelow(c);
	} else if (state == _XA_NET_WM_STATE_DEMANDS_ATTENTION) {
		if ((set == _NET_WM_STATE_ADD && !c->is.attn) ||
		    (set == _NET_WM_STATE_REMOVE && c->is.attn) ||
		    (set == _NET_WM_STATE_TOGGLE)) {
			c->is.attn = !c->is.attn;
		}
	} else if (state == _XA_NET_WM_STATE_FOCUSED) {
		/* _NET_WM_STATE_FOCUSED indicates whether the window's decorations are
		   drawn in an active state.  Clients MUST regard it as a read-only hint. 
		   It cannot be set at map time or changed via a _NET_WM_STATE client
		   message. */
	} else if (state == _XA_NET_WM_STATE_FIXED) {
		/* _NET_WM_STATE_FIXED is a read-only state. */
	} else if (state == _XA_NET_WM_STATE_FLOATING) {
		if ((set == _NET_WM_STATE_ADD && !c->skip.arrange) ||
		    (set == _NET_WM_STATE_REMOVE && c->skip.arrange) ||
		    (set == _NET_WM_STATE_TOGGLE)) {
			togglefloating(c);
		}
	} else if (state == _XA_NET_WM_STATE_FILLED) {
		if ((set == _NET_WM_STATE_ADD && !c->is.fill) ||
		    (set == _NET_WM_STATE_REMOVE && c->is.fill) ||
		    (set == _NET_WM_STATE_TOGGLE)) {
			togglefill(c);
		}
	}
}

void
ewmh_update_net_window_extents(Client *c) {
	long data[4] = {
		c->c.b, /* left */
		c->c.b, /* right */
		c->c.b + c->th, /* top */
		c->c.b + c->gh, /* bottom */
	};

	XPRINTF("Updating _NET_WM_FRAME_EXTENTS for 0x%lx\n", c->win);
	XChangeProperty(dpy, c->win, _XA_NET_FRAME_EXTENTS, XA_CARDINAL, 32,
		PropModeReplace, (unsigned char *) data, 4L);
	XPRINTF("Updating _KDE_NET_WM_FRAME_STRUT for 0x%lx\n", c->win);
	XChangeProperty(dpy, c->win, _XA_KDE_NET_WM_FRAME_STRUT, XA_CARDINAL, 32,
		PropModeReplace, (unsigned char *) data, 4L);
}

void
wmh_update_win_maximized_geometry(Client *c)
{
	long data[4] = { c->c.x, c->c.y, c->c.w, c->c.h };

	XChangeProperty(dpy, c->win, _XA_WIN_MAXIMIZED_GEOMETRY, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) data, 4L);
}

void
ewmh_update_net_window_visible_name(Client *c) {
	XTextProperty prop;

	if (c->name) {
		if (XmbTextListToTextProperty(dpy, &c->name, 1, XUTF8StringStyle, &prop) == Success)
			XSetTextProperty(dpy, c->win, &prop, _XA_NET_WM_VISIBLE_NAME);
	} else {
		XDeleteProperty(dpy, c->win, _XA_NET_WM_VISIBLE_NAME);
	}
}

void
ewmh_update_net_window_visible_icon_name(Client *c) {
	XTextProperty prop;

	if (c->icon_name) {
		if (XmbTextListToTextProperty(dpy, &c->icon_name, 1, XUTF8StringStyle, &prop) == Success)
			XSetTextProperty(dpy, c->win, &prop, _XA_NET_WM_VISIBLE_ICON_NAME);
	} else {
		XDeleteProperty(dpy, c->win, _XA_NET_WM_VISIBLE_ICON_NAME);
	}
}

#ifdef STARTUP_NOTIFICATION
SnStartupSequence *
find_startup_seq(Client *c) {
	Notify *n = NULL;
	SnStartupSequence *seq = NULL;
	XClassHint ch;
	char **argv;
	int argc;
	const char *binary, *wmclass;


	if ((seq = c->seq))
		return seq;
	if (c->startup_id) {
		for (n = notifies; n; n = n->next)
			if (!strcmp(c->startup_id, sn_startup_sequence_get_id(n->seq)))
				break;
		if (!n) {
			DPRINTF("Cannot find startup id '%s'!\n", c->startup_id);
			return NULL;
		} 
		goto found_it;
	}
	if (XGetClassHint(dpy, c->win, &ch)) {
		for (n = notifies; n; n = n->next) {
			if (n->assigned)
				continue;
			if (sn_startup_sequence_get_completed(n->seq))
				continue;
			if ((wmclass = sn_startup_sequence_get_wmclass(n->seq))) {
				if (ch.res_name && !strcmp(wmclass, ch.res_name))
					break;
				if (ch.res_class && !strcmp(wmclass, ch.res_class))
					break;
			}
		}
		if (ch.res_class)
			XFree(ch.res_class);
		if (ch.res_name)
			XFree(ch.res_name);
		if (n)
			goto found_it;
	}
	if (XGetCommand(dpy, c->win, &argv, &argc)) {
		for (n = notifies; n; n = n->next) {
			if (n->assigned)
				continue;
			if (sn_startup_sequence_get_completed(n->seq))
				continue;
			if ((binary = sn_startup_sequence_get_binary_name(n->seq))) {
				if (argv[0] && !strcmp(binary, argv[0]))
					break;
			}
		}
		if (argv)
			XFreeStringList(argv);
		if (n)
			goto found_it;
	}
	if (XGetClassHint(dpy, c->win, &ch)) {
		/* try again, case insensitive */
		for (n = notifies; n; n = n->next) {
			if (n->assigned)
				continue;
			if (sn_startup_sequence_get_completed(n->seq))
				continue;
			if ((wmclass = sn_startup_sequence_get_wmclass(n->seq))) {
				if (ch.res_name && !strcasecmp(wmclass, ch.res_name))
					break;
				if (ch.res_class && !strcasecmp(wmclass, ch.res_class))
					break;
			}
		}
		if (ch.res_class)
			XFree(ch.res_class);
		if (ch.res_name)
			XFree(ch.res_name);
		if (n)
			goto found_it;
	}
	return NULL;
found_it:
	n->assigned = True;
	seq = n->seq;
	sn_startup_sequence_ref(seq);
	sn_startup_sequence_complete(seq);
	c->seq = seq;
	return seq;
}
#endif

void
push_client_time(Client *c, Time time)
{
	if (!time)
		return;
	if (c->with.time) {
		if ((c->user_time != CurrentTime) &&
		    (int) ((int) time - (int) c->user_time) > 0)
			c->user_time = time;
	} else {
		c->user_time = time;
		c->with.time = True;
	}
	if ((user_time == CurrentTime) ||
	    ((int) ((int) time - (int) user_time) > 0))
		user_time = time;
}

void
ewmh_process_net_startup_id(Client *c)
{
#ifdef STARTUP_NOTIFICATION
	SnStartupSequence *seq;
#endif
	if (c->startup_id)
		return;
	if (!gettextprop(c->win, _XA_NET_STARTUP_ID, &c->startup_id))
		if (c->leader != None && c->leader != c->win)
			gettextprop(c->leader, _XA_NET_STARTUP_ID, &c->startup_id);
	if (c->startup_id) {
		char *p, *q, *copy;
		char *launcher, *launchee;
		long pid, sequence, timestamp = 0;

		do {
			p = q = copy = strdup(c->startup_id);
			if (!(p = strchr(q, '/')))
				break;
			*p++ = '\0';
			launcher = q;
			q = p;
			if (!(p = strchr(q, '/')))
				break;
			*p++ = '\0';
			launchee = q;
			q = p;
			if (!(p = strchr(q, '-')))
				break;
			*p++ = '\0';
			pid = atol(q);
			q = p;
			if (!(p = strchr(q, '_')))
				break;
			*p++ = '\0';
			sequence = atol(q);
			q = p;
			if (!(p = strstr(q, "TIME")) || p != q)
				break;
			q = p + 4;
			timestamp = atol(q);

			/* Note: when xdg-launch is the launcher, the startup id is:
			   %s/%s/%d-%d_TIME%lu, launcher, launchee, pid, sequence, ts
			   where the sequence is in fact the monitor number. */

			if (strstr(launcher, "xdg-launch") &&
			    0 <= sequence && sequence < scr->nmons)
				c->monitor = sequence + 1;

			(void) launchee;
			(void) pid;
		} while (0);
		free(copy);

		if (!timestamp && (p = strstr(c->startup_id, "_TIME")))
			timestamp = atol(p + 5);
		push_client_time(c, timestamp);
	}
#ifdef STARTUP_NOTIFICATION
	if ((seq = find_startup_seq(c))) {
		int i, workspace;

		if (!c->startup_id) {
			c->startup_id = strdup(sn_startup_sequence_get_id(seq));
			XChangeProperty(dpy, c->win, _XA_NET_STARTUP_ID,
					_XA_UTF8_STRING, 8, PropModeReplace,
					(unsigned char *) c->startup_id,
					strlen(c->startup_id) + 1);
		}
		push_client_time(c, sn_startup_sequence_get_timestamp(seq));
		/* NOTE: should not override _NET_WM_DESKTOP */
		workspace = sn_startup_sequence_get_workspace(seq);
		if (0 <= workspace && workspace < scr->ntags) {
			for (i = 0; i < scr->ntags; i++)
				c->tags[i] = False;
			c->tags[workspace] = True;
		}
		/* TODO: use screen number to select monitor */
	}
#endif
}

void
ewmh_process_net_window_user_time(Client *c) {
	long *time = NULL;
	unsigned long n = 0;
	Window win;

	if ((win = c->time_window) == None)
		win = c->win;

	time = getcard(win, _XA_NET_WM_USER_TIME, &n);
	if (n > 0) {
		c->with.time = True;
		c->user_time = time[0];
		if (user_time == CurrentTime) {
			user_time = c->user_time;
		} else if (c->user_time != CurrentTime) {
			if ((int)((int)c->user_time - (int)user_time) > 0)
				user_time = c->user_time;
		}
		XFree(time);
	} else {
		c->with.time = False;
		c->user_time = CurrentTime;
	}
}

void
ewmh_release_user_time_window(Client *c) {
	Window win;
	
	if ((win = c->time_window) == None)
		return;

	c->time_window = None;
	XSelectInput(dpy, win, NoEventMask);
	XDeleteContext(dpy, win, context[ClientTimeWindow]);
	XDeleteContext(dpy, win, context[ClientAny]);
}

Window *getwind(Window win, Atom atom, unsigned long *nitems);

void
ewmh_process_net_window_user_time_window(Client * c) {
	Window *wins = NULL, win = None;
	unsigned long n = 0;

	wins = getwind(c->win, _XA_NET_WM_USER_TIME_WINDOW, &n);
	if (n > 0) {
		Window other = wins[0];

		XFree(wins);
		/* check recursive */
		wins = getwind(other, _XA_NET_WM_USER_TIME_WINDOW, &n);
		if (n > 0) {
			Window again = wins[0];

			XFree(wins);
			win = (other == again) ? again : None;
		}
	}
	if (win == c->time_window)
		return;
	ewmh_release_user_time_window(c);
	if (win == None)
		return;

	c->time_window = win;
	XSelectInput(dpy, win, PropertyChangeMask);
	XSaveContext(dpy, win, context[ClientTimeWindow], (XPointer)c);
	XSaveContext(dpy, win, context[ClientAny], (XPointer)c);

	ewmh_process_net_window_user_time(c);
}

Atom *getatom(Window win, Atom atom, unsigned long *nitems);

#define WIN_HINTS_SKIP_FOCUS      (1<<0) /* "alt-tab" skips this win */
#define WIN_HINTS_SKIP_WINLIST    (1<<1) /* do not show in window list */
#define WIN_HINTS_SKIP_TASKBAR    (1<<2) /* do not show on taskbar */
#define WIN_HINTS_GROUP_TRANSIENT (1<<3) /* Reserved - definition is unclear */
#define WIN_HINTS_FOCUS_ON_CLICK  (1<<4) /* app only accepts focus if clicked */

void
wmh_process_win_window_hints(Client * c) {
	long *state;
	unsigned long n = 0;

	state = getcard(c->win, _XA_WIN_HINTS, &n);
	if (n > 0) {
		c->skip.focus = (state[0] & WIN_HINTS_SKIP_FOCUS) ? True : c->skip.focus;
		c->skip.winlist = (state[0] & WIN_HINTS_SKIP_WINLIST) ? True : c->skip.winlist;
		c->skip.taskbar = (state[0] & WIN_HINTS_SKIP_TASKBAR) ? True : c->skip.taskbar;
		c->skip.sloppy = (state[0] & WIN_HINTS_FOCUS_ON_CLICK) ? True : c->skip.sloppy;
		c->is.grptrans = (state[0] & WIN_HINTS_GROUP_TRANSIENT) ? True : c->is.grptrans;
	}
}

void
wmh_process_layer(Client *c, unsigned int layer)
{
	switch (layer) {
	case WIN_LAYER_DESKTOP:
		c->wintype |= WTFLAG(WindowTypeDesk);
		break;
	case 1:
	case WIN_LAYER_BELOW:
		if (!c->is.below)
			togglebelow(c);
		break;
	case 3:
	case WIN_LAYER_NORMAL:
		c->wintype |= WTFLAG(WindowTypeNormal);
		break;
	case 5:
	case WIN_LAYER_ONTOP:
		if (!c->is.above)
			toggleabove(c);
		break;
	case 7:
	case WIN_LAYER_DOCK:
		c->wintype |= WTFLAG(WindowTypeDock);
		break;
	case 9:
	case WIN_LAYER_ABOVE_DOCK:
		c->wintype |= WTFLAG(WindowTypeDock);
		if (!c->is.above)
			toggleabove(c);
		break;
	case 11:
	case WIN_LAYER_MENU:
		c->wintype |= WTFLAG(WindowTypeMenu);
		break;
	case 13:
	default:
		c->wintype |= WTFLAG(WindowTypePopup);
		break;
	}
}

void
wmh_process_win_layer(Client * c)
{
	long *layer;
	unsigned long n;

	layer = getcard(c->win, _XA_WIN_LAYER, &n);
	if (n)
		wmh_process_layer(c, layer[0]);
	if (layer)
		XFree(layer);
}

void
wmh_process_win_state(Client *c) {
	long *state = NULL;
	unsigned long n = 0;

	state = getcard(c->win, _XA_WIN_STATE, &n);
	if (n)
		wmh_process_state_mask(c, 0xffffffff, state[0]);
	if (state)
		XFree(state);
}

void
wmh_process_win_window_state(Client *c)
{
	// wmh_process_win_window_hints(c);
	wmh_process_win_layer(c);
	wmh_process_win_state(c);
}

void
ewmh_process_net_window_state(Client *c) {
	Atom *state;
	unsigned long i, n = 0;

	wmh_process_win_window_state(c);

	state = getatom(c->win, _XA_NET_WM_STATE, &n);
	for (i = 0; i < n; i++)
		ewmh_process_state_atom(c, state[i], _NET_WM_STATE_ADD);
	if (state)
		XFree(state);
}

void
ewmh_process_net_window_sync_request_counter(Client *c) {
	int format, status;
	long *data = NULL;
	unsigned long extra, nitems = 0;
	Atom real;
	

	if (!checkatom(c->win, _XA_WM_PROTOCOLS, _XA_NET_WM_SYNC_REQUEST))
		return;
	status = XGetWindowProperty(dpy, c->win, _XA_NET_WM_SYNC_REQUEST_COUNTER, 0L, 1L,
			False, AnyPropertyType, &real, &format, &nitems, &extra,
			(unsigned char **)&data);
	if (status == Success && nitems > 0) {
#ifdef SYNC
		XSyncAlarmAttributes aa;

		c->sync.counter = data[0];

		if (haveext[XsyncBase]) {
			XSyncIntToValue(&c->sync.val, 0);
			XSyncSetCounter(dpy, c->sync.counter, c->sync.val);

			aa.trigger.counter = c->sync.counter;
			aa.trigger.wait_value = c->sync.val;
			aa.trigger.value_type = XSyncAbsolute;
			aa.trigger.test_type = XSyncPositiveTransition;
			aa.events = True;
			XSyncIntToValue(&aa.delta, 1);

			c->sync.alarm = XSyncCreateAlarm(dpy,
					XSyncCACounter|XSyncCAValueType|XSyncCAValue|XSyncCATestType|
					XSyncCADelta|XSyncCAEvents, &aa);

			if (c->sync.alarm) {
				CPRINTF(c, "allocated alarm 0x%08lx\n", c->sync.alarm);
				XSaveContext(dpy, c->sync.alarm, context[ClientSync], (XPointer) c);
				XSaveContext(dpy, c->sync.alarm, context[ClientAny], (XPointer) c);
				XSaveContext(dpy, c->sync.alarm, context[ScreenContext], (XPointer) scr);

			} else
				CPRINTF(c, "could not allocate alarm!\n");
		} else
			DPRINTF("SYNC extension not present\n");
#else
		DPRINTF("SYNC extension not supported\n");
#endif
	}
	if (data)
		XFree(data);
}

void
ewmh_update_net_window_fs_monitors(Client *c) {
	Monitor *m;
	long mons[4] = { 0, };

	if (!c->is.max)
		return;
	for (m = scr->monitors; m && c->c.y != m->sc.y; m = m->next) ;
	if (!m)
		return;
	mons[0] = m->num;
	for (m = scr->monitors; m && c->c.y + c->c.h != m->sc.y + m->sc.h; m = m->next) ;
	if (!m)
		return;
	mons[1] = m->num;
	for (m = scr->monitors; m && c->c.x != m->sc.x; m = m->next) ;
	if (!m)
		return;
	mons[2] = m->num;
	for (m = scr->monitors; m && c->c.x + c->c.w != m->sc.x + m->sc.w; m = m->next) ;
	if (!m)
		return;
	mons[3] = m->num;
	XChangeProperty(dpy, c->win, _XA_NET_WM_FULLSCREEN_MONITORS, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *)mons, 4L);
}

void
ewmh_process_kde_net_window_type_override(Client *c) {
	int format, status;
	long *data = NULL;
	unsigned long extra, nitems = 0;
	Atom real;

	status = XGetWindowProperty(dpy, c->win, _XA_KDE_NET_WM_WINDOW_TYPE_OVERRIDE, 0L, 1L,
			False, AnyPropertyType, &real, &format, &nitems, &extra,
			(unsigned char **)&data);
	if (status == Success && real != None) {
		/* no decorations or functionality */
		c->is.bastard = True;
		c->is.floater = True;
		c->skip.skip = -1U; /* skip everything */
		c->can.can = 0; /* no functionality */
		c->has.has = 0; /* no decorations */
	}
	if (data)
		XFree(data);
}

void
ewmh_update_kde_splash_progress() {
	XEvent ev;

	ev.xclient.display = dpy;
	ev.xclient.type = ClientMessage;
	ev.xclient.window = scr->root;
	ev.xclient.message_type = _XA_KDE_SPLASH_PROGRESS;
	ev.xclient.format = 8;
	strcpy(ev.xclient.data.b, "wm started");
	XSendEvent(dpy, scr->root, False, SubstructureNotifyMask, &ev);
}

Bool
clientmessage(XEvent *e)
{
	XClientMessageEvent *ev = &e->xclient;
	Client *c;
	Atom message_type = ev->message_type;

	if ((c = getclient(ev->window, ClientWindow))) {
		if (0) {
		} else if (message_type == _XA_NET_CLOSE_WINDOW) {
			killclient(c);
		} else if (message_type == _XA_NET_ACTIVE_WINDOW ||
			   message_type == _XA_WIN_FOCUS) {
			c->is.icon = False;
			c->is.hidden = False;
			focus(c);
			arrange(clientmonitor(c));
		} else if (message_type == _XA_NET_WM_STATE) {
			ewmh_process_state_atom(c, (Atom) ev->data.l[1], ev->data.l[0]);
			if (ev->data.l[2])
				ewmh_process_state_atom(c,
				    (Atom) ev->data.l[2], ev->data.l[0]);
			ewmh_update_net_window_state(c);
		} else if (message_type == _XA_WIN_LAYER) {
			wmh_process_layer(c, ev->data.l[0]);
			ewmh_update_net_window_state(c);
		} else if (message_type == _XA_WIN_STATE) {
			wmh_process_state_mask(c, ev->data.l[0], ev->data.l[1]);
			ewmh_update_net_window_state(c);
		} else if (message_type == _XA_WM_CHANGE_STATE) {
			if (ev->data.l[0] == IconicState)
				iconify(c);
		} else if (message_type == _XA_NET_WM_DESKTOP ||
			   message_type == _XA_WIN_WORKSPACE) {
			int index = ev->data.l[0];

			if (-1 <= index && index < scr->ntags)
				tag(c, index);
		} else if (message_type == _XA_NET_WM_DESKTOP_MASK ||
			   message_type == _XA_WIN_WORKSPACES) {
			unsigned index = ev->data.l[0];
			unsigned mask = ev->data.l[1], oldmask = 0;
			unsigned num = (scr->ntags+31)>>5;
			unsigned i, j;

			if (0 > index || index >= num)
				return False;
			for (i = 0, j = index<<5; j<scr->ntags; i++, j++) {
				if (c->tags[j])
					oldmask |= (1<<i);
				c->tags[j] = (mask & (1<<i)) ? True : False;
			}
			if (islost(c))
				for (i = 0, j = index<<5; j<scr->ntags; i++, j++)
					c->tags[j] = (oldmask & (1<<i)) ? True : False;
			else {
				/* what toggletag does */
				ewmh_update_net_window_desktop(c);
				drawclient(c);
				arrange(NULL);
			}
			/* TODO */
		} else if (message_type == _XA_WM_PROTOCOLS) {
			/* TODO */
		} else if (message_type == _XA_NET_WM_FULLSCREEN_MONITORS) {
			if (c->is.max) {
				Monitor *mt, *mb, *ml, *mr;
				int t, b, l, r;
				ClientGeometry g;

				mt = findmonbynum(ev->data.l[0]);
				mb = findmonbynum(ev->data.l[1]);
				ml = findmonbynum(ev->data.l[2]);
				mr = findmonbynum(ev->data.l[3]);
				if (!mt || !mb || !ml || !mr) {
					ewmh_update_net_window_fs_monitors(c);
					return False;
				}
				t = mt->sc.y;
				b = mb->sc.y + mb->sc.h;
				l = ml->sc.x;
				r = mr->sc.x + mr->sc.w;
				if (t >= b || r >= l) {
					ewmh_update_net_window_fs_monitors(c);
					return False;
				}
				g.x = l;
				g.y = t;
				g.w = r - l;
				g.h = b - t;
				g.b = 0;
				g.t = 0;
				g.g = 0;
				reconfigure(c, &g);
			}
			XChangeProperty(dpy, c->win, _XA_NET_WM_FULLSCREEN_MONITORS, XA_CARDINAL, 32,
					PropModeReplace, (unsigned char *)&ev->data.l[0], 4L);
		} else if (message_type == _XA_NET_MOVERESIZE_WINDOW) {
			unsigned flags = (unsigned) ev->data.l[0];
			unsigned source = ((flags >> 12) & 0x0f);
			unsigned gravity = flags & 0xff;
			ClientGeometry g;

			if (!isfloating(c, NULL))
				return False;
			if (source != 0 && source != 2)
				return False;

			g.x = ((flags & (1 << 8)) && c->can.move) ? ev->data.l[1] : c->c.x;
			g.y = ((flags & (1 << 9)) && c->can.move) ? ev->data.l[2] : c->c.y;
			g.w = ((flags & (1 << 10)) && c->can.sizeh) ? ev->data.l[3] : c->c.w;
			g.h = ((flags & (1 << 11)) && c->can.sizev) ? ev->data.l[4] : c->c.h;
			g.b = c->c.b;
			g.t = c->th;
			g.g = c->gh;
			if (gravity == 0)
				gravity = c->gravity;
			applygravity(c, &g, gravity);
			/* FIXME: this just resizes and moves the window, it does
			 * handle changing monitors */
			reconfigure(c, &g);
		} else if (message_type == _XA_NET_WM_MOVERESIZE) {
			int x_root = (int) ev->data.l[0];
			int y_root = (int) ev->data.l[1];
			int direct = (int) ev->data.l[2];
			int button = (int) ev->data.l[3];
			int source = (int) ev->data.l[4];
			XButtonEvent bev;

			if (source != 0 && source != 1 && source != 2)
				return False;
			if (direct < 0 || direct > 11)
				return False;
			bev.type = ButtonPress;
			bev.serial = 0;
			bev.send_event = True;
			bev.display = dpy;
			bev.window = c->frame;
			bev.root = scr->root;
			bev.subwindow = c->win;
			bev.time = CurrentTime;
			bev.x = 0;
			bev.y = 0;
			bev.x_root = x_root;
			bev.y_root = y_root;
			bev.state = 0;
			bev.button = button;
			bev.same_screen = XQueryPointer(dpy, c->frame, &bev.root,
					&bev.subwindow, &bev.x_root, &bev.y_root,
					&bev.x, &bev.y, &bev.state);
			switch (direct) {
			case 0: /* _NET_WM_MOVERESIZE_SIZE_TOPLEFT */
			case 1: /* _NET_WM_MOVERESIZE_SIZE_TOP */
			case 2: /* _NET_WM_MOVERESIZE_SIZE_TOPRIGHT */
			case 3: /* _NET_WM_MOVERESIZE_SIZE_RIGHT */
			case 4: /* _NET_WM_MOVERESIZE_SIZE_BOTTOMRIGHT */
			case 5: /* _NET_WM_MOVERESIZE_SIZE_BOTTOM */
			case 6: /* _NET_WM_MOVERESIZE_SIZE_BOTTOMLEFT */
			case 7: /* _NET_WM_MOVERESIZE_SIZE_LEFT */
				mouseresize_from(c, direct, (XEvent *)&bev, False);
				break;
			case 8: /* _NET_WM_MOVERESIZE_MOVE */
				mousemove(c, (XEvent *)&bev, False);
				break;
			case 9: /* _NET_WM_MOVERESIZE_SIZE_KEYBOARD */
				/* TODO */
				break;
			case 10: /* _NET_WM_MOVERESIZE_MOVE_KEYBOARD */
				/* TODO */
				break;
			case 11: /* _NET_WM_MOVERESIZE_CANCEL */
				/* intercepted while moving or resizing */
				break;
			}
		} else if (message_type == _XA_NET_RESTACK_WINDOW) {
			unsigned source = ev->data.l[0];
			Window sibling = ev->data.l[1];
			unsigned detail = ev->data.l[2];
			Client *o = NULL;
			if (sibling)
				if (!(o = getclient(sibling, ClientAny)))
					return False;
			if (source == 1)
				return False;
			restack_client(c, detail, o);
		} else if (message_type == _XA_NET_REQUEST_FRAME_EXTENTS) {
			ewmh_update_net_window_extents(c);
		} else
			return False;
	} else if (ev->window == scr->root) {
		if (0) {
		} else if (message_type == _XA_NET_NUMBER_OF_DESKTOPS) {
			settags(ev->data.l[0]);
		} else if (message_type == _XA_NET_DESKTOP_GEOMETRY ||
			   message_type == _XA_WIN_AREA_COUNT) {
			/* Ignore desktop geometry changes but change the property in response */
			ewmh_update_net_desktop_geometry();
		} else if (message_type == _XA_NET_DESKTOP_VIEWPORT ||
			   message_type == _XA_WIN_AREA) {
			/* Ignore viewport changes but change the property in response. */
			ewmh_update_net_desktop_viewport();
		} else if (message_type == _XA_NET_CURRENT_DESKTOP ||
			   message_type == _XA_WIN_WORKSPACE) {
			int tag = ev->data.l[0];
			if (0 <= tag && tag < scr->ntags)
				view(tag);
		} else if (message_type == _XA_NET_SHOWING_DESKTOP) {
			if (!ev->data.l[0] != !scr->showing_desktop)
				toggleshowing();
		} else if (message_type == _XA_NET_RESTART) {
			/* TODO */
		} else if (message_type == _XA_NET_SHUTDOWN) {
			/* TODO */
		} else if (message_type == _XA_MANAGER) {
			/* TODO */
		} else if (message_type == _XA_NET_STARTUP_INFO_BEGIN) {
#ifdef STARTUP_NOTIFICATION
			sn_display_process_event(sn_dpy, e);
#endif
		} else if (message_type == _XA_NET_STARTUP_INFO) {
#ifdef STARTUP_NOTIFICATION
			sn_display_process_event(sn_dpy, e);
#endif
		} else if (message_type == _XA_WM_PROTOCOLS) {
			if (0) {
			} else if (ev->data.l[0] == _XA_NET_WM_PING) {
				if ((c = getclient(ev->data.l[2], ClientPing)))
					XDeleteContext(dpy, c->win, context[ClientPing]);
				if ((c = getclient(ev->data.l[2], ClientDead)))
					XDeleteContext(dpy, c->win, context[ClientDead]);
			}
		}
	} else {
		if (0) {
		} else if (message_type == _XA_NET_REQUEST_FRAME_EXTENTS) {
			Window win = ev->window;
			unsigned int wintype;
			Bool title = True;
			Bool grips = True;
			int th, gh, border = scr->style.border;
			long data[4];

			if (win == None)
				return False;
			wintype = getwintype(win);
			if (!(WTTEST(wintype, WindowTypeNormal)))
				if (WTTEST(wintype, WindowTypeDesk) ||
				    WTTEST(wintype, WindowTypeDock) ||
				    WTTEST(wintype, WindowTypeDrop) ||
				    WTTEST(wintype, WindowTypePopup) ||
				    WTTEST(wintype, WindowTypeTooltip) ||
				    WTTEST(wintype, WindowTypeNotify) ||
				    WTTEST(wintype, WindowTypeCombo) ||
				    WTTEST(wintype, WindowTypeDnd)) {
					title = False;
					grips = False;
					border = 0;
				}
			getmwmhints(win, &title, &grips, &border);
			th = title ? scr->style.titleheight : 0;
			gh = grips ? scr->style.gripsheight : 0;
			data[0] = border;
			data[1] = border;
			data[2] = border + th;
			data[3] = border + gh;
			XChangeProperty(dpy, win, _XA_NET_FRAME_EXTENTS, XA_CARDINAL, 32,
					PropModeReplace, (unsigned char *) &data, 4L);
		} else
			return False;
	}
	return True;
}

void
setopacity(Client * c, unsigned int opacity) {
	/* TODO: This is not quite right: the client is responsible for setting 
	   opacity on the client window, the WM should only propagate that
	   opacity to the frame. */
	if (opacity == OPAQUE) {
		XDeleteProperty(dpy, c->win, _XA_NET_WM_WINDOW_OPACITY);
		XDeleteProperty(dpy, c->frame, _XA_NET_WM_WINDOW_OPACITY);
	} else {
		long data = opacity;

		XChangeProperty(dpy, c->win, _XA_NET_WM_WINDOW_OPACITY, XA_CARDINAL, 32,
				PropModeReplace, (unsigned char *) &data, 1L);
		XChangeProperty(dpy, c->frame, _XA_NET_WM_WINDOW_OPACITY, XA_CARDINAL, 32,
				PropModeReplace, (unsigned char *) &data, 1L);
	}
}

Atom *
getatom(Window win, Atom atom, unsigned long *nitems) {
	int format, status;
	Atom *ret = NULL;
	unsigned long extra;
	Atom real;

	status = XGetWindowProperty(dpy, win, atom, 0L, 64L, False, XA_ATOM,
			&real, &format, nitems, &extra, (unsigned char **)&ret);
	if (status != Success) {
		*nitems = 0;
		return NULL;
	}
	return ret;
}

long *
getcard(Window win, Atom atom, unsigned long *nitems) {
	int format, status;
	long *ret = NULL;
	unsigned long extra;
	Atom real;

	status = XGetWindowProperty(dpy, win, atom, 0L, 64L, False, XA_CARDINAL,
			&real, &format, nitems, &extra, (unsigned char **)&ret);
	if (status != Success) {
		*nitems = 0;
		return NULL;
	}
	return ret;
}

Window *
getwind(Window win, Atom atom, unsigned long *nitems) {
	int format, status;
	Window *ret = NULL;
	unsigned long extra;
	Atom real;

	status = XGetWindowProperty(dpy, win, atom, 0L, 64L, False, XA_WINDOW,
			&real, &format, nitems, &extra, (unsigned char **)&ret);
	if (status != Success) {
		*nitems = 0;
		return NULL;
	}
	return ret;
}

Bool
checkatom(Window win, Atom bigatom, Atom smallatom) {
	Atom *state;
	unsigned long i, n;
	Bool ret = False;

	state = getatom(win, bigatom, &n);
	for (i = 0; i < n; i++) {
		if (state[i] == smallatom) {
			ret = True;
			break;
		}
	}
	if (state)
		XFree(state);
	return ret;
}

unsigned int
getwintype(Window win) {
	Atom *state;
	unsigned long i, j, n = 0;
	unsigned int ret = 0;
	long *layer;

	state = getatom(win, _XA_NET_WM_WINDOW_TYPE, &n);
	for (i = 0; i < n; i++)
		for (j = WindowTypeDesk; j <= WindowTypeNormal; j++)
			if (state[i] == atom[j])
				ret |= WTFLAG(j);
	layer = getcard(win, _XA_WIN_LAYER, &n);
	if (n) {
		switch ((unsigned int)layer[0]) {
		case WIN_LAYER_DESKTOP:
			ret |= WTFLAG(WindowTypeDesk);
			break;
		case 1:
		case WIN_LAYER_BELOW:
		case 3:
		case WIN_LAYER_NORMAL:
		case 5:
		case WIN_LAYER_ONTOP:
			ret |= WTFLAG(WindowTypeNormal);
			break;
		case 7:
		case WIN_LAYER_DOCK:
		case 9:
		case WIN_LAYER_ABOVE_DOCK:
			ret |= WTFLAG(WindowTypeDock);
			break;
		case 11:
		case WIN_LAYER_MENU:
			ret |= WTFLAG(WindowTypeMenu);
			break;
		case 13:
		default:
			ret |= WTFLAG(WindowTypePopup);
			break;
		}
	}
	if (ret == 0)
		ret = WTFLAG(WindowTypeNormal);
	if (state)
		XFree(state);
	if (layer)
		XFree(layer);
	return ret;
}

void
ewmh_process_net_window_type(Client *c)
{
	c->wintype = getwintype(c->win);
	if (!WTCHECK(c, WindowTypeNormal)) {
		if (WTCHECK(c, WindowTypeDesk) ||
		    WTCHECK(c, WindowTypeDock) ||
		    WTCHECK(c, WindowTypeSplash) ||
		    WTCHECK(c, WindowTypeDrop) ||
		    WTCHECK(c, WindowTypePopup) ||
		    WTCHECK(c, WindowTypeTooltip) ||
		    WTCHECK(c, WindowTypeNotify) ||
		    WTCHECK(c, WindowTypeCombo) ||
		    WTCHECK(c, WindowTypeDnd)) {
			c->is.bastard = True;
			c->skip.skip = -1U; /* skip everything */
			c->can.can = 0; /* no functionality */
			c->has.has = 0; /* no decorations */
			c->is.floater = True;
		}
		if (WTCHECK(c, WindowTypeDialog) ||
		    WTCHECK(c, WindowTypeMenu)) {
			c->skip.arrange = True;
			c->is.floater = True;
			c->can.arrange = False;
			c->can.size = False;
			c->has.grips = False;
			// c->can.move = False;
		}
		if (WTCHECK(c, WindowTypeToolbar) ||
		    WTCHECK(c, WindowTypeUtil)) {
			c->skip.arrange = True;
		}
	}
}

Bool
checkwintype(Window win, int wintype) {
	Atom *state;
	unsigned long i, n = 0;
	Bool ret = False;

	state = getatom(win, _XA_NET_WM_WINDOW_TYPE, &n);
	if (n == 0) {
		if (wintype == WindowTypeNormal)
			ret = True;
	} else {
		for (i = 0; i < n; i++) {
			if (state[i] == atom[wintype]) {
				ret = True;
				break;
			}
		}
	}
	if (state)
		XFree(state);
	return ret;
}

static Bool
strut_overlap(long min1, long max1, long min2, long max2) {
	long tmp;

	if (min1 > max1) { tmp = min1; min1 = max1; max1 = tmp; }
	if (min2 > max2) { tmp = min2; min2 = max2; max2 = tmp; }
	if (min1 <= min2 && max1 >= min2)
		return True;
	if (min1 <= max2 && max1 >= max2)
		return True;
	if (min2 <= min1 && max2 >= min1)
		return True;
	if (min2 <= max1 && max2 >= max1)
		return True;
	return False;
}

int
getstrut(Client * c, Atom atom) {
	long *prop, dw, dh, strut;
	Monitor *m;
	unsigned long n = 0;
	struct {
		long l, r, t, b, ly1, ly2, ry1, ry2, tx1, tx2, bx1, bx2;
	} s;

	dw = DisplayWidth(dpy, scr->screen);
	dh = DisplayHeight(dpy, scr->screen);

	s.l = s.r = s.t = s.b = 0;
	s.ly1 = s.ry1 = 0;
	s.ly2 = s.ry2 = dh;
	s.tx1 = s.bx1 = 0;
	s.tx2 = s.bx2 = dw;

	prop = getcard(c->win, atom, &n);
	if (n == 0)
		return n;

	memcpy(&s, prop, n * sizeof(long));
	XFree(prop);

	for (m = scr->monitors; m; m = m->next) {
		if ((strut = s.l - m->sc.x) > 0)
			if (strut_overlap(m->sc.y, m->sc.y + m->sc.h, s.ly1, s.ly2))
				m->struts[LeftStrut] = max(strut, m->struts[LeftStrut]);
		if ((strut = m->sc.x + m->sc.w - dw + s.r) > 0)
			if (strut_overlap(m->sc.y, m->sc.y + m->sc.h, s.ry1, s.ry2))
				m->struts[RightStrut] = max(strut, m->struts[RightStrut]);
		if ((strut = s.t - m->sc.y) > 0)
			if (strut_overlap(m->sc.x, m->sc.x + m->sc.w, s.tx1, s.tx2))
				m->struts[TopStrut] = max(strut, m->struts[TopStrut]);
		if ((strut = m->sc.y + m->sc.h - dh + s.b) > 0)
			if (strut_overlap(m->sc.x, m->sc.x + m->sc.w, s.bx1, s.bx2))
				m->struts[BotStrut] = max(strut, m->struts[BotStrut]);
	}
	return n;
}

int getstruts(Client *c) {
	return (getstrut(c, _XA_NET_WM_STRUT_PARTIAL) || getstrut(c, _XA_NET_WM_STRUT));
}

