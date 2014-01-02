/*
 *    EWMH atom support. initial implementation borrowed from
 *    awesome wm, then partially reworked.
 *
 *    Copyright © 2007-2008 Julien Danjou <julien@danjou.info>
 *    Copyright © 2008 Alexander Polakov <polachok@gmail.com>
 *
 */

#include <unistd.h>
#include <regex.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/Xutil.h>
#include <X11/Xft/Xft.h>
#include "echinus.h"
#include "config.h"

Atom atom[NATOMS];

/* keep in sync with enum in echinus.h */
char *atomnames[NATOMS] = {
	"MANAGER",
	"UTF8_STRING",
	"WM_PROTOCOLS",
	"WM_DELETE_WINDOW",
	"WM_NAME",
	"WM_STATE",
	"WM_CHANGE_STATE",
	"WM_TAKE_FOCUS",
	"_MOTIF_WM_HINTS",
	"_ECHINUS_LAYOUT",
	"_ECHINUS_SELTAGS",
	"_NET_WM_FULLSCREEN_MONITORS",
	"_NET_MOVERESIZE_WINDOW",
	"_NET_WM_MOVERESIZE",
	"_NET_DESKTOP_GEOMETRY",
	"_NET_DESKTOP_VIEWPORT",
	"_NET_SHOWING_DESKTOP",
	"_NET_RESTART",
	"_NET_SHUTDOWN",
	"_NET_REQUEST_FRAME_EXTENTS",
	"_NET_STARTUP_INFO_BEGIN",
	"_NET_STARTUP_INFO",
	"_NET_DESKTOP_LAYOUT",
	"_NET_WM_USER_TIME",
	"_NET_WM_USER_TIME_WINDOW",
	"_NET_WM_SYNC_REQUEST_COUNTER",
	"_NET_WM_HANDLED_ICONS",
	"_KDE_NET_WM_WINDOW_TYPE_OVERRIDE",
	/* _NET_SUPPORTED following */
	"_NET_CLIENT_LIST",
	"_NET_ACTIVE_WINDOW",
	"_NET_WM_DESKTOP",
	"_NET_WM_DESKTOP_MASK",
	"_NET_NUMBER_OF_DESKTOPS",
	"_NET_DESKTOP_NAMES",
	"_NET_CURRENT_DESKTOP",
	"_NET_WORKAREA",
	"_NET_DESKTOP_MODES",
	"_NET_DESKTOP_MODE_FLOATING",
	"_NET_DESKTOP_MODE_TILED",
	"_NET_DESKTOP_MODE_BOTTOM_TILED",
	"_NET_DESKTOP_MODE_MONOCLE",
	"_NET_CLIENT_LIST_STACKING",
	"_NET_WM_WINDOW_OPACITY",

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
	"_NET_SUPPORTED"
};

#define _NET_WM_STATE_REMOVE	0
#define _NET_WM_STATE_ADD	1
#define _NET_WM_STATE_TOGGLE	2

void
initewmh(Window win) {
	char name[] = "echinus";
	long data;

	XInternAtoms(dpy, atomnames, NATOMS, False, atom);
	XChangeProperty(dpy, root, atom[Supported], XA_ATOM, 32,
	    PropModeReplace, (unsigned char *) &atom[ClientList],
	    NATOMS - ClientList);

	XChangeProperty(dpy, win, atom[WindowName], atom[Utf8String], 8,
		       	PropModeReplace, (unsigned char*)name, strlen(name));
	data = getpid();
	XChangeProperty(dpy, win, atom[WindowPid], XA_CARDINAL, 32,
			PropModeReplace, (unsigned char*)&data, 1);
	XChangeProperty(dpy, root, atom[WMCheck], XA_WINDOW, 32,
		       	PropModeReplace, (unsigned char*)&win, 1);
	XChangeProperty(dpy, win, atom[WMCheck], XA_WINDOW, 32,
			PropModeReplace, (unsigned char*)&win, 1);
}

void
update_echinus_layout_name(Client *c) {
	XChangeProperty(dpy, root, atom[ELayout],
	    XA_STRING, 8, PropModeReplace,
	    (const unsigned char *) &views[curmontag].layout->symbol, 1L);
}

void
ewmh_update_net_client_list(Client *c) {
	Window *wins = NULL;
	int i, n = 0;

	for (c = stack; c; c = c->snext)
		n++;
	if (!n) {
		XChangeProperty(dpy, root, atom[ClientList], XA_WINDOW, 32,
			       	PropModeReplace, (unsigned char *) wins, n);
		XChangeProperty(dpy, root, atom[ClientListStacking], XA_WINDOW,
			       	32, PropModeReplace, (unsigned char *) wins, n);
		return;
	}
	wins = malloc(sizeof(Window) * n);
	for (i = 0, c = stack; c; c = c->snext)
		wins[i++] = c->win;
	XChangeProperty(dpy, root,
	    atom[ClientListStacking], XA_WINDOW, 32, PropModeReplace,
	    (unsigned char *) wins, n);
	for (i = 0, c = clients; c; c = c->next)
		wins[i++] = c->win;
	XChangeProperty(dpy, root,
	    atom[ClientList], XA_WINDOW, 32, PropModeReplace, (unsigned char *) wins, n);
	free(wins);
	XFlush(dpy);
}

void
ewmh_update_net_number_of_desktops(Client *c) {
	XChangeProperty(dpy, root,
	    atom[NumberOfDesk], XA_CARDINAL, 32, PropModeReplace,
	    (unsigned char *) &ntags, 1);
}

void
ewmh_update_net_current_desktop(Client *c) {
	Monitor *m;
	unsigned long *seltags;
	unsigned int i;

	seltags = emallocz(ntags * sizeof(unsigned long));
	for (m = monitors; m != NULL; m = m->next) {
		for (i = 0; i < ntags; i++)
			seltags[i] |= m->seltags[i];
	}
	XChangeProperty(dpy, root,
	    atom[ESelTags], XA_CARDINAL, 32, PropModeReplace,
	    (unsigned char *) seltags, ntags);
	XChangeProperty(dpy, root, atom[CurDesk], XA_CARDINAL, 32,
	    PropModeReplace, (unsigned char *) &curmontag, 1);
	update_echinus_layout_name(NULL);
	free(seltags);
}

static Bool
issticky(Client *c) {
	unsigned int i;

	for (i = 0; i < ntags; i++)
		if (!c->tags[i])
			return False;
	return True;
}

static Bool
islost(Client *c) {
	unsigned int i;
	for (i = 0; i < ntags; i++)
		if (c->tags[i])
			return False;
	return True;
}

void
ewmh_update_net_window_desktop(Client *c) {
	long i = -1;

	if (!issticky(c))
		for (i = 0; i < ntags && !c->tags[i]; i++);
	XChangeProperty(dpy, c->win, atom[WindowDesk], XA_CARDINAL, 32,
		PropModeReplace, (unsigned char *) &i, 1);
}

void
ewmh_update_net_window_desktop_mask(Client *c) {
	unsigned int longs = (ntags+31)>>5;
	unsigned long data[longs];
	unsigned int i, j, k, l;

	for (j = 0, k = 0; j < longs; j++, k += 32)
		for (i = 0, l = k, data[j] = 0; i < 32 && l < ntags; i++, l++)
			if (c->tags[l])
				data[j] |= (1<<i);
	XChangeProperty(dpy, c->win, atom[WindowDeskMask], XA_CARDINAL, 32,
		PropModeReplace, (unsigned char *)data, longs);
}

void
ewmh_update_net_work_area(Client *c) {
	unsigned long *geoms;
	Monitor *m = monitors;
	int i, x, y, w, h;

	geoms = malloc(sizeof(unsigned long)*4*ntags);
	x = m->wax - m->sx;
	y = m->way - m->sy;
	w = m->waw;
	h = m->wah;
	for (m = m->next; m != NULL; m = m->next) {
		x = max(x, m->wax - m->sx);
		y = max(y, m->way - m->sy);
		w = min(w, m->waw);
		h = min(h, m->wah);
	}
	for (i = 0; i < ntags; i++) {
		geoms[i*4] = x;
		geoms[i*4+1] = y;
		geoms[i*4+2] = w;
		geoms[i*4+3] = h;
	}
	XChangeProperty(dpy, root,
	    atom[WorkArea], XA_CARDINAL, 32, PropModeReplace, (unsigned char *) geoms, ntags*4);
	free(geoms);
}

void
ewmh_update_net_desktop_names(Client *c) {
	char buf[1024], *pos;
	unsigned int i;
	int len = 0;

	pos = buf;
	for (i = 0; i < ntags; i++) {
		snprintf(pos, strlen(tags[i]) + 1, "%s", tags[i]);
		pos += (strlen(tags[i]) + 1);
	}
	len = pos - buf;

	XChangeProperty(dpy, root,
	    atom[DeskNames], atom[Utf8String], 8, PropModeReplace,
	    (unsigned char *) buf, len);
}

void
ewmh_update_net_active_window(Client *c) {
	Window win;

	win = sel ? sel->win : None;
	XChangeProperty(dpy, root,
	    atom[ActiveWindow], XA_WINDOW, 32, PropModeReplace,
	    (unsigned char *) &win, 1);
}

void
ewmh_update_net_desktop_modes(Client *c) {
	long data[ntags];
	int i;

	for (i = 0; i < ntags; i++) {
		switch (views[i].layout->symbol) {
		case 'i':
		case 'f':
			data[i] = atom[DeskModeFloating];
			break;
		case 't':
			data[i] = atom[DeskModeTiled];
			break;
		case 'b':
			data[i] = atom[DeskModeBottomTiled];
			break;
		case 'm':
			data[i] = atom[DeskModeMonocle];
			break;
		default:
			data[i] = None;
			break;
		}
	}
	XChangeProperty(dpy, root,
	    atom[DeskModes], XA_ATOM, 32, PropModeReplace,
	    (unsigned char *)data, ntags);
}

void
mwm_process_atom(Client *c) {
	Atom real;
	int format;
	unsigned char *data = NULL;
	long *hint;
	unsigned long n, extra;
#define MWM_HINTS_ELEMENTS 5
#define MWM_DECOR_ALL(x) ((x) & (1L << 0))
#define MWM_DECOR_TITLE(x) ((x) & (1L << 3))
#define MWM_DECOR_BORDER(x) ((x) & (1L << 1))
#define MWM_HINTS_DECOR(x) ((x) & (1L << 1))
	if (XGetWindowProperty(dpy, c->win, atom[MWMHints], 0L, 20L, False,
		atom[MWMHints], &real, &format, &n, &extra,
		(unsigned char **) &data) == Success && n >= MWM_HINTS_ELEMENTS) {
		hint = (long *) data;
		if (MWM_HINTS_DECOR(hint[0]) && !(MWM_DECOR_ALL(hint[2]))) {
			c->title = MWM_DECOR_TITLE(hint[2]) ? root : (Window) NULL;
			c->border = MWM_DECOR_BORDER(hint[2]) ? style.border : 0;
		}
	}
	if (data)
		XFree(data);
}

void
ewmh_update_net_window_state(Client *c)
{
	long winstate[16];
	int states = 0;

	if (c->isicon)
		winstate[states++] = atom[WindowStateHidden];
	if (c->ismax)
		winstate[states++] = atom[WindowStateFs];
	if (c->isfixed)
		winstate[states++] = atom[WindowStateFixed];
	if (c->isfloating)
		winstate[states++] = atom[WindowStateFloating];
	if (c == sel)
		winstate[states++] = atom[WindowStateFocused];
	if (c->isfill)
		winstate[states++] = atom[WindowStateFilled];
	if (issticky(c))
		winstate[states++] = atom[WindowStateSticky];

	XChangeProperty(dpy, c->win, atom[WindowState], XA_ATOM, 32,
		PropModeReplace, (unsigned char *) winstate, states);
}

void
ewmh_update_net_window_actions(Client *c)
{
	long action[32];
	int actions = 0;

	if (!c->isbastard) {
		// action[actions++] = atom[WindowActionAbove];
		// action[actions++] = atom[WindowActionBelow];
		action[actions++] = atom[WindowActionChangeDesk];
	}
	action[actions++] = atom[WindowActionClose];
	if (!c->isbastard) {
		if (c->isfloating) {
			action[actions++] = atom[WindowActionFs];
			// action[actions++] = atom[WindowActionMaxH];
			// action[actions++] = atom[WindowActionMaxV];
		}
		action[actions++] = atom[WindowActionMin];
		action[actions++] = atom[WindowActionMove];
		if (!c->isfixed)
			action[actions++] = atom[WindowActionResize];
		action[actions++] = atom[WindowActionStick];
		action[actions++] = atom[WindowActionFloat];
		if (c->isfloating) {
			// action[actions++] = atom[WindowActionShade];
			action[actions++] = atom[WindowActionFill];
		}
	}

	XChangeProperty(dpy, c->win, atom[WindowActions], XA_ATOM, 32,
		PropModeReplace, (unsigned char *) action, actions);
}

void
ewmh_process_state_atom(Client *c, Atom state, int set) {
	if (0) {
	} else if (state == atom[WindowStateModal]) {
		focus(c);
	} else if (state == atom[WindowStateSticky]) {
		if ((set == _NET_WM_STATE_ADD || set == _NET_WM_STATE_TOGGLE)
				&& !issticky(c)) {
			tag(c, -1);
		} else
		if ((set == _NET_WM_STATE_REMOVE || set == _NET_WM_STATE_TOGGLE)
				&& issticky(c)) {
			static char buf[512];
			unsigned int i, j;
			int matched = 0;
			regmatch_t tmp;
			XClassHint ch = { 0, };


			/* reapply rules */
			XGetClassHint(dpy, c->win, &ch);
			snprintf(buf, sizeof(buf), "%s:%s:%s",
			    ch.res_class ? ch.res_class : "", ch.res_name ? ch.res_name : "", c->name);
			buf[LENGTH(buf)-1] = 0;
			for (i = 0; i < nrules; i++) {
				if (rules[i]->propregex && !regexec(rules[i]->propregex, buf, 1, &tmp, 0)) {
					for (j = 0; rules[i]->tagregex && j < ntags; j++) {
						if (!regexec(rules[i]->tagregex, tags[j], 1, &tmp, 0)) {
							matched = j;
							c->tags[j] = True;
						}
					}
				}
			}
			if (ch.res_class)
				XFree(ch.res_class);
			if (ch.res_name)
				XFree(ch.res_name);
			tag(c, matched);
		}

	} else if (state == atom[WindowStateMaxV]) {
	} else if (state == atom[WindowStateMaxH]) {
	} else if (state == atom[WindowStateShaded]) {
	} else if (state == atom[WindowStateNoTaskbar]) {
	} else if (state == atom[WindowStateNoPager]) {
	} else if (state == atom[WindowStateHidden]) {
	} else if (state == atom[WindowStateFs]) {
		if ((set == _NET_WM_STATE_ADD || set == _NET_WM_STATE_TOGGLE)
				&& !c->ismax) {
			c->wasfloating = c->isfloating;
			if (!c->isfloating)
				togglefloating(c);
			togglemax(c);
		} else if ((set == _NET_WM_STATE_REMOVE ||
				set == _NET_WM_STATE_TOGGLE) && c->ismax) {
			togglemax(c);
			if (!c->wasfloating)
				togglefloating(c);
		}
		DPRINT;
		arrange(curmonitor());
		DPRINTF("%s: x%d y%d w%d h%d\n", c->name, c->x, c->y, c->w, c->h);
	} else if (state == atom[WindowStateAbove]) {
	} else if (state == atom[WindowStateBelow]) {
	} else if (state == atom[WindowStateAttn]) {
	} else if (state == atom[WindowStateFocused]) {
	} else if (state == atom[WindowStateFixed]) {
	} else if (state == atom[WindowStateFloating]) {
		if ((set == _NET_WM_STATE_ADD || set == _NET_WM_STATE_TOGGLE)
				&& !c->isfloating) {
		} else
		if ((set == _NET_WM_STATE_REMOVE || set == _NET_WM_STATE_TOGGLE)
				&& c->isfloating) {
		}
	} else if (state == atom[WindowStateFilled]) {
	}
}

Atom *getatom(Window win, Atom atom, unsigned long *nitems);

void
ewmh_process_net_window_state(Client *c) {
	Atom *state;
	unsigned long i, n = 0;

	state = getatom(c->win, atom[WindowState], &n);
	for (i = 0; i < n; i++)
		ewmh_process_state_atom(c, state[i], _NET_WM_STATE_ADD);
	if (state)
		XFree(state);
	ewmh_update_net_window_state(c);
}

void
clientmessage(XEvent *e) {
	XClientMessageEvent *ev = &e->xclient;
	Client *c;
	Atom message_type = ev->message_type;

	c = getclient(ev->window, clients, ClientWindow);

	if (c) {
		if (0) {
		} else if (message_type == atom[CloseWindow]) {
			killclient(c);
		} else if (message_type == atom[ActiveWindow]) {
			if (c->isicon)
				c->isicon = False;
			focus(c);
			arrange(curmonitor());
		} else if (message_type == atom[WindowState]) {
			ewmh_process_state_atom(c, (Atom) ev->data.l[1], ev->data.l[0]);
			if (ev->data.l[2])
				ewmh_process_state_atom(c,
				    (Atom) ev->data.l[2], ev->data.l[0]);
			ewmh_update_net_window_state(c);
		} else if (message_type == atom[WMChangeState]) {
			if (ev->data.l[0] == IconicState) {
				iconify(c);
			}
		} else if (message_type == atom[WindowDesk]) {
			int index = ev->data.l[0];

			if (-1 <= index && index < ntags) {
				tag(c, index);
				ewmh_update_net_window_desktop(c);
				ewmh_update_net_window_desktop_mask(c);
				ewmh_update_net_window_state(c);
			}
		} else if (message_type == atom[WindowDeskMask]) {
			unsigned index = ev->data.l[0];
			unsigned mask = ev->data.l[1], oldmask = 0;
			unsigned num = (ntags+31)>>5;
			unsigned i, j;

			if (0 <= index && index < num) {
				for (i = 0, j = index<<5; j<ntags; i++, j++) {
					if (c->tags[j])
						oldmask |= (1<<i);
					c->tags[j] = (mask & (1<<i)) ? True : False;
				}
				if (islost(c))
					for (i = 0, j = index<<5; j<ntags; i++, j++)
						c->tags[j] = (oldmask & (1<<i)) ? True : False;
				else {
					ewmh_update_net_window_desktop(c);
					ewmh_update_net_window_desktop_mask(c);
					ewmh_update_net_window_state(c);
					/* what toggletag does */
					drawclient(c);
					arrange(NULL);
				}
			}
			/* TODO */
		} else if (message_type == atom[WMProto]) {
			/* TODO */
		} else if (message_type == atom[WindowFsMonitors]) {
			/* TODO */
		} else if (message_type == atom[MoveResizeWindow]) {
			/* TODO */
		} else if (message_type == atom[WindowMoveResize]) {
			/* TODO */
		}
	} else if (ev->window == root) {
		if (0) {
		} else if (message_type == atom[NumberOfDesk]) {
			/* TODO */
		} else if (message_type == atom[DeskGeometry]) {
			/* TODO */
		} else if (message_type == atom[DeskViewport]) {
			/* TODO */
		} else if (message_type == atom[CurDesk]) {
			int tag = ev->data.l[0];
			if (0 <= tag && tag < ntags)
				view(tag);
		} else if (message_type == atom[ShowingDesktop]) {
			/* TODO */
		} else if (message_type == atom[WMRestart]) {
			/* TODO */
		} else if (message_type == atom[WMShutdown]) {
			/* TODO */
		} else if (message_type == atom[Manager]) {
			/* TODO */
		} else if (message_type == atom[RequestFrameExt]) {
			/* TODO */
		} else if (message_type == atom[StartupInfoBegin]) {
			/* TODO */
		} else if (message_type == atom[StartupInfo]) {
			/* TODO */
		}
	}
}

void
setopacity(Client *c, unsigned int opacity) {
	if (opacity == OPAQUE) {
		XDeleteProperty(dpy, c->win, atom[WindowOpacity]);
		XDeleteProperty(dpy, c->frame, atom[WindowOpacity]);
	} else {
		XChangeProperty(dpy, c->win, atom[WindowOpacity],
		    XA_CARDINAL, 32, PropModeReplace, (unsigned char *) &opacity, 1L);
		XChangeProperty(dpy, c->frame, atom[WindowOpacity],
		    XA_CARDINAL, 32, PropModeReplace, (unsigned char *) &opacity, 1L);

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

Bool
checkwintype(Window win, int wintype) {
	Atom *state;
	unsigned long i, n = 0;
	Bool ret = False;

	state = getatom(win, atom[WindowType], &n);
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

int
getstrut(Client *c, Atom strut) {
	long *state;
	int ret = 0;
	Monitor *m;
	unsigned long i, n;

	if (!(m = clientmonitor(c)))
		return ret;

	state = getcard(c->win, strut, &n);
	if (n) {
		for (i = LeftStrut; i < LastStrut; i++)
			m->struts[i] = max(state[i], m->struts[i]);
		ret = 1;
	}
	if (state)
		XFree(state);
	return ret;
}

int getstruts(Client *c) {
	return (getstrut(c, atom[StrutPartial]) || getstrut(c, atom[Strut]));
}

void (*updateatom[]) (Client *) = {
	[ClientList] = ewmh_update_net_client_list,
	[ActiveWindow] = ewmh_update_net_active_window,
	[WindowDesk] = ewmh_update_net_window_desktop,
	[WindowDeskMask] = ewmh_update_net_window_desktop_mask,
	[NumberOfDesk] = ewmh_update_net_number_of_desktops,
	[DeskNames] = ewmh_update_net_desktop_names,
	[CurDesk] = ewmh_update_net_current_desktop,
	[ELayout] = update_echinus_layout_name,
	[WorkArea] = ewmh_update_net_work_area,
	[DeskModes] = ewmh_update_net_desktop_modes,
	[WindowState] = ewmh_update_net_window_state,
	[WindowActions] = ewmh_update_net_window_actions,
};
