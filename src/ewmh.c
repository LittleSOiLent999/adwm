/* See COPYING file for copyright and license details. */

#include "adwm.h"
#include "layout.h"
#include "draw.h"
#include "tags.h"
#include "config.h"
#include "ewmh.h" /* verification */

extern AdwmPlaces config;

Atom atom[NATOMS];

/* keep in sync with enum in adwm.h */
char *atomnames[NATOMS] = {
	"MANAGER",
	"UTF8_STRING",
	"SM_CLIENT_ID",
	"WM_PROTOCOLS",
	"WM_DELETE_WINDOW",
	"WM_SAVE_YOURSELF",
	"WM_STATE",
	"WM_CHANGE_STATE",
	"WM_TAKE_FOCUS",
	"WM_CLIENT_LEADER",
	"WM_WINDOW_ROLE",
	"WM_COLORMAP_WINDOWS",
	"WM_COLORMAP_NOTIFY",
	"_ECHINUS_LAYOUT",
	"_ECHINUS_SELTAGS",
	"_ADWM_RCFILE",
	"_ADWM_PRVDIR",
	"_ADWM_RUNDIR",
	"_ADWM_USRDIR",
	"_ADWM_XDGDIR",
	"_ADWM_SYSDIR",
	"_ADWM_STYLE_NAME",
	"_ADWM_THEME_NAME",
	"_ADWM_ICON_THEME_NAME",
	"_ADWM_CURSOR_THEME_NAME",
	"_ADWM_STYLE",
	"_ADWM_THEME",
	"_ADWM_ICON_THEME",
	"_ADWM_CURSOR_THEME",
	"_ADWM_CHECK",
	"_NET_RELOAD",
	"_NET_RESTART",
	"_NET_SHUTDOWN",
	"_NET_DESKTOP_LAYOUT",
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
	"_WIN_RESIZE",		/* TODO */
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
	"_NET_DESKTOP_MODE_GRID",

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
	"_NET_WM_ICON",
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
	"_NET_WM_STATE_MAXIMUS_LEFT",
	"_NET_WM_STATE_MAXIMUS_RIGHT",

	"_NET_WM_ALLOWED_ACTIONS",
	"_NET_WM_ACTION_MOVE",
	"_NET_WM_ACTION_RESIZE",
	"_NET_WM_ACTION_MINIMIZE",
	"_NET_WM_ACTION_SHADE",
	"_NET_WM_ACTION_STICK",
	"_NET_WM_ACTION_MAXIMIZE_HORZ",
	"_NET_WM_ACTION_MAXIMIZE_VERT",
	"_NET_WM_ACTION_FULLSCREEN",
	"_NET_WM_ACTION_CHANGE_DESKTOP",
	"_NET_WM_ACTION_CLOSE",
	"_NET_WM_ACTION_ABOVE",
	"_NET_WM_ACTION_BELOW",
	"_NET_WM_ACTION_FLOAT",
	"_NET_WM_ACTION_FILL",
	"_NET_WM_ACTION_MAXIMUS_LEFT",
	"_NET_WM_ACTION_MAXIMUS_RIGHT",

	"_NET_SUPPORTING_WM_CHECK",
	"_NET_CLOSE_WINDOW",
	"_NET_WM_PING",
	"_NET_SUPPORTED",

	"KWM_WIN_ICON",
	"_KDE_NET_SYSTEM_TRAY_WINDOWS",
	"_KDE_NET_WM_FRAME_STRUT",
	"_KDE_NET_WM_SYSTEM_TRAY_WINDOW_FOR",
	"_KDE_NET_WM_WINDOW_TYPE_OVERRIDE",
	"_KDE_SPLASH_PROGRESS",
	"_KDE_WM_CHANGE_STATE",

	"_OPENBOX_PID",
	"_OB_CONTROL",

	"_OB_CONFIG_FILE",
	"_OB_THEME",
	"_OB_VERSION",

	"_OB_APP_GROUP_CLASS",
	"_OB_APP_GROUP_NAME",
	"_OB_APP_CLASS",
	"_OB_APP_NAME",
	"_OB_APP_ROLE",
	"_OB_APP_TITLE",
	"_OB_APP_TYPE",

	"_OB_WM_STATE_UNDECORATED",
	"_OB_WM_ACTION_UNDECORATE",

	"_NET_APP_APPLICATION_ID",
	"_NET_APP_LAUNCHER",
	"_NET_APP_LAUNCHEE",
	"_NET_APP_HOSTNAME",
	"_NET_APP_PID",
	"_NET_APP_SEQUENCE",
	"_NET_APP_TIMESTAMP",
	"_NET_APP_NAME",
	"_NET_APP_DESCRIPTION",
	"_NET_APP_ICON_NAME",
	"_NET_APP_BINARY_NAME",
	"_NET_APP_WMCLASS",
	"_NET_APP_SCREEN",
	"_NET_APP_WORKSPACE"
};

#define _NET_WM_STATE_REMOVE	0
#define _NET_WM_STATE_ADD	1
#define _NET_WM_STATE_TOGGLE	2

#define MWM_STARTUP_STANDARD	(1<<0)
#define MWM_STARTUP_CUSTOM	(1<<1)

static Bool
parse_startup_id(const char *id, char **launcher_p, char **launchee_p, char **hostname_p,
		 pid_t *pid_p, long *sequence_p, Time *timestamp_p)
{
	long sequence = 0, timestamp = 0;
	pid_t pid = 0;
	const char *p;
	char *endptr = NULL;

	do {
		const char *q;
		char *tmp, *b;

		p = q = id;
		if (!(p = strchr(q, '/')))
			break;
		if (launcher_p) {
			*launcher_p = strndup(q, p - q);
			while ((b = strchr(*launcher_p, '|'))) *b = '/';
		}
		q = p + 1;
		if (!(p = strchr(q, '/')))
			break;
		if (launchee_p) {
			*launchee_p = strndup(q, p - q);
			while ((b = strchr(*launchee_p, '|'))) *b = '/';
		}
		q = p + 1;
		if (!(p = strchr(q, '-')))
			break;
		tmp = strndup(q, p - q);
		pid = strtoul(tmp, &endptr, 10);
		if (endptr && *endptr) {
			free(tmp);
			break;
		}
		free(tmp);
		if (pid_p)
			*pid_p = pid;
		q = p + 1;
		if (!(p = strchr(q, '-')))
			break;
		tmp = strndup(q, p - q);
		sequence = strtoul(tmp, &endptr, 10);
		if (endptr && *endptr) {
			free(tmp);
			break;
		}
		free(tmp);
		if (sequence_p)
			*sequence_p = sequence;
		q = p + 1;
		if (!(p = strstr(q, "_TIME")))
			break;
		if (hostname_p)
			*hostname_p = strndup(q, p - q);
		q = p + 5;
		timestamp = strtoul(q, &endptr, 10);
		if (endptr && *endptr)
			break;
		if (timestamp_p)
			*timestamp_p = timestamp;
		return (True);
	}
	while (0);

	if (!timestamp && (p = strstr(id, "_TIME"))) {
		timestamp = strtoul(p + 5, &endptr, 10);
		if (!endptr || !*endptr)
			if (timestamp_p)
				*timestamp_p = timestamp;
	}
	return (False);
}

#ifdef STARTUP_NOTIFICATION
static Notify *notifies = NULL;

struct SnStartupSequence
{
	int refcount;
	SnDisplay *display;
	int screen;
	char *id;
	char *name;
	char *description;
	char *wmclass;
	int workspace;
	Time timestamp;
	char *binary_name;
	char *icon_name;
	char *application_id;
	unsigned int completed : 1;
	unsigned int canceled : 1;
	unsigned int timestamp_set : 1;
	int creation_serial;
	struct timeval initiation_time;
};

static Notify **
n_find_n(Notify * n)
{
	Notify *np, **npp = NULL;

	if (!n) {
		EPRINTF("Passed a null pointer\n");
		return (npp);
	}
	for (npp = &notifies; (np = *npp) && np != n; npp = &np->next) ;
	if (!np || np != n) {
		EPRINTF("Could not find notify %p\n", n);
		return (NULL);
	}
	return (npp);
}

static Notify *
n_find_seq(SnStartupSequence *seq)
{
	Notify *n = NULL;

	if (!seq) {
		EPRINTF("Passed a nulil pointer\n");
		return (n);
	}
	for (n = notifies; n && n->seq != seq; n = n->next) ;
	if (!n) {
		const char *id = sn_startup_sequence_get_id(seq);

		EPRINTF("Could not find sequence %p : %s\n", seq, id);
		/* let's try harder */
		for (n = notifies; n && strcmp(n->id, id); n = n->next) ;
		if (!n)
			EPRINTF("Still couldn't find sequence %p : %s\n", seq, id);
		else
			EPRINTF("Found sequence by id!\n");
	}
	return (n);
}

static Client *
c_find_seq(SnStartupSequence *seq)
{
	Client *c = NULL;

	if (!seq) {
		EPRINTF("Passed a nulil pointer\n");
		return (c);
	}
	for (c = scr->clients; c && c->seq != seq; c = c->next) ;
	if (!c) {
		const char *id = sn_startup_sequence_get_id(seq);

		EPRINTF("Could not find sequence %p : %s\n", seq, id);
		/* let's try harder */
		for (c = scr->clients; c; c = c->next) {
			const char *id2;

			if (!c->seq)
				continue;
			id2 = sn_startup_sequence_get_id(c->seq);
			if (!strcmp(id, id2))
				break;
		}
		if (!c)
			EPRINTF("Still couldn't find sequence %p : %s\n", seq, id);
		else
			EPRINTF("Found sequence by id!\n");
	}
	return (c);
}

static void
n_new_notify(SnStartupSequence *seq)
{
	const char *text;
	Notify *n;
	Time timestamp;

	text = sn_startup_sequence_get_id(seq);
	n = emallocz(sizeof(*n));
	n->seq = seq;
	n->id = strdup(text);
	n->pid = -1;
	n->sequence = -1;
	n->timestamp = -1;
	sn_startup_sequence_ref(seq);
	parse_startup_id(n->id, &n->launcher, &n->launchee, &n->hostname,
			 &n->pid, &n->sequence, &n->timestamp);
	if (!n->timestamp)
		if ((timestamp = sn_startup_sequence_get_timestamp(seq)) != -1)
			n->timestamp = timestamp;
	XPRINTF("NOTIFY: NEW: %s\n", n->id);
	n->next = notifies;
	notifies = n;
}

static void ewmh_update_sn_app_props(Client *c, Notify *n);

static void
n_chg_notify(SnStartupSequence *seq)
{
	Notify *n = n_find_seq(seq);

	if (n) {
		XPRINTF("NOTIFY: CHANGE: %s\n", n->id);
		if (n->assigned) {
			Client *c;

			if ((c = c_find_seq(seq)))
				ewmh_update_sn_app_props(c, n);
		}
	} else {
		/* may be a sequence that we never say the start of */
		/* jsut skip it for now */
	}
}

static void
n_del_notify(Notify *n)
{
	Notify **np;

	np = n_find_n(n);
	assert(np != NULL);
	*np = n->next;
	sn_startup_sequence_unref(n->seq);
	free(n->id);
	free(n->launcher);
	free(n->launchee);
	free(n->hostname);
	memset(n, 0, sizeof(*n));
	free(n);
}

static void
n_end_notify(SnStartupSequence *seq)
{
	Notify *n = n_find_seq(seq);

	if (n) {
		XPRINTF("NOTIFY: END: %s\n", n->id);
		n->complete = True;
		/* Cannot delete a notification that has completed before it is assigned
		   to a client.  This is because the first thing that most toolkits (such 
		   as GTK) do is completed the sequence even before creating any window.
		   */
		if (n->assigned)
			n_del_notify(n);
	} else {
		/* May be completiion of a sqeuence that we never got the begin for */
		/* we just won't ref it and were are done with it */
	}
}

static SnStartupSequence *
n_set_notify(Notify *n, Client *c)
{
	SnStartupSequence *seq;

	if ((seq = c->seq)) {
		XPRINTF("sequence already assigned to client\n");
		return (seq);
	}
	seq = c->seq = n->seq;
	sn_startup_sequence_ref(seq);
	if (n->assigned)
		XPRINTF("notify already assigned\n");
	n->assigned = True;
	if (!n->complete) {
		sn_startup_sequence_complete(seq);
		n->complete = True;
	}
	if (n->complete)
		n_del_notify(n);
	else
		XPRINTF("could not find sequence\n");
	updateappl(c);
	return (seq);
}

static void
sn_handler(SnMonitorEvent * event, void *dummy)
{
	SnStartupSequence *seq = NULL;

	seq = sn_monitor_event_get_startup_sequence(event);

	switch (sn_monitor_event_get_type(event)) {
	case SN_MONITOR_EVENT_INITIATED:
		XPRINTF("sn initiated id '%s'\n",
			sn_startup_sequence_get_id(seq));
		n_new_notify(seq);
		break;
	case SN_MONITOR_EVENT_CHANGED:
		XPRINTF("sn changed id '%s'\n",
			sn_startup_sequence_get_id(seq));
		n_chg_notify(seq);
		break;
	case SN_MONITOR_EVENT_COMPLETED:
	case SN_MONITOR_EVENT_CANCELED:
		XPRINTF("sn completed/canceled id '%s'\n",
			sn_startup_sequence_get_id(seq));
		n_end_notify(seq);
		break;
	}
}
#endif

void
initewmh(char *name)
{
	long data[6];
	static Bool atoms_interned = False;
	Window swin = scr->selwin;
	Window root = scr->root;

#ifdef STARTUP_NOTIFICATION
	if (!scr->ctx) {
		scr->ctx = sn_monitor_context_new(sn_dpy, scr->screen, &sn_handler, NULL, NULL);
		XPRINTF("startup notification on screen %d\n", scr->screen);
	}
#else
	XPRINTF("startup notification not supported screen %d\n", scr->screen);
#endif

	if (!atoms_interned) {
		XInternAtoms(dpy, atomnames, NATOMS, False, atom);
		atoms_interned = True;
	}
	XChangeProperty(dpy, root, _XA_NET_SUPPORTED, XA_ATOM, 32,
			PropModeReplace, (unsigned char *) &_XA_NET_CLIENT_LIST,
			NATOMS - ClientList);

	XChangeProperty(dpy, swin, _XA_NET_WM_NAME, _XA_UTF8_STRING, 8,
			PropModeReplace, (unsigned char *) name, strlen(name));
	data[0] = getpid();
	XChangeProperty(dpy, swin, _XA_NET_WM_PID, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) data, 1);
#if 0
	/* Problem with doing this is that we might fool others into thinking that
	   we really are openbox. */
	const char *version = VERSION;

	XChangeProperty(dpy, root, _XA_OPENBOX_PID, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) data, 1);
	XChangePropertY(dpy, root, _XA_OB_VERSION, _XA_UTF8_STRING, 8,
			PropModeReplace, (unsigned char *) version, strlen(version));
#endif
	XChangeProperty(dpy, root, _XA_NET_SUPPORTING_WM_CHECK, XA_WINDOW, 32,
			PropModeReplace, (unsigned char *) &swin, 1);
	XChangeProperty(dpy, swin, _XA_NET_SUPPORTING_WM_CHECK, XA_WINDOW, 32,
			PropModeReplace, (unsigned char *) &swin, 1);

	XChangeProperty(dpy, root, _XA_WIN_PROTOCOLS, XA_ATOM, 32,
			PropModeReplace, (unsigned char *) &_XA_WIN_APP_STATE,
			ClientList - WinAppState);
	XChangeProperty(dpy, swin, _XA_WIN_SUPPORTING_WM_CHECK, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) &swin, 1);
	XChangeProperty(dpy, root, _XA_WIN_SUPPORTING_WM_CHECK, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) &swin, 1);
	XChangeProperty(dpy, swin, _XA_WIN_DESKTOP_BUTTON_PROXY, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) &swin, 1);
	XChangeProperty(dpy, root, _XA_WIN_DESKTOP_BUTTON_PROXY, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) &swin, 1);

	data[0] = MWM_STARTUP_CUSTOM;
	data[1] = swin;
	XChangeProperty(dpy, root, _XA_MOTIF_WM_INFO, _XA_MOTIF_WM_INFO, 32,
			PropModeReplace, (unsigned char *) data, 2);
	XChangeProperty(dpy, swin, _XA_MOTIF_WM_INFO, _XA_MOTIF_WM_INFO, 32,
			PropModeReplace, (unsigned char *) data, 2);

	XChangeProperty(dpy, root, _XA_ADWM_CHECK, XA_WINDOW, 32,
			PropModeReplace, (unsigned char *) &swin, 1);
	XChangeProperty(dpy, swin, _XA_ADWM_CHECK, XA_WINDOW, 32,
			PropModeReplace, (unsigned char *) &swin, 1);

	ewmh_update_net_client_lists();


	if (config.stylename)
		XChangeProperty(dpy, root, _XA_ADWM_STYLE_NAME, _XA_UTF8_STRING, 8,
				PropModeReplace, (unsigned char *) config.stylename,
				strlen(config.stylename));
	else
		XDeleteProperty(dpy, root, _XA_ADWM_STYLE_NAME);

	if (config.stylefile)
		XChangeProperty(dpy, root, _XA_ADWM_STYLE, _XA_UTF8_STRING, 8,
				PropModeReplace, (unsigned char *) config.stylefile,
				strlen(config.stylefile));
	else
		XDeleteProperty(dpy, root, _XA_ADWM_STYLE);

	if (config.themename)
		XChangeProperty(dpy, root, _XA_ADWM_THEME_NAME, _XA_UTF8_STRING, 8,
				PropModeReplace, (unsigned char *) config.themename,
				strlen(config.themename));
	else
		XDeleteProperty(dpy, root, _XA_ADWM_THEME_NAME);

	if (config.themefile)
		XChangeProperty(dpy, root, _XA_ADWM_THEME, _XA_UTF8_STRING, 8,
				PropModeReplace, (unsigned char *) config.themefile,
				strlen(config.themefile));
	else
		XDeleteProperty(dpy, root, _XA_ADWM_THEME);

	XChangeProperty(dpy, root, _XA_ADWM_RCFILE, _XA_UTF8_STRING, 8,
			PropModeReplace, (unsigned char *) config.rcfile,
			strlen(config.rcfile));
	XChangeProperty(dpy, root, _XA_ADWM_PRVDIR, _XA_UTF8_STRING, 8,
			PropModeReplace, (unsigned char *) config.pdir,
			strlen(config.pdir));
	XChangeProperty(dpy, root, _XA_ADWM_RUNDIR, _XA_UTF8_STRING, 8,
			PropModeReplace, (unsigned char *) config.rdir,
			strlen(config.rdir));
	XChangeProperty(dpy, root, _XA_ADWM_USRDIR, _XA_UTF8_STRING, 8,
			PropModeReplace, (unsigned char *) config.udir,
			strlen(config.udir));
	XChangeProperty(dpy, root, _XA_ADWM_XDGDIR, _XA_UTF8_STRING, 8,
			PropModeReplace, (unsigned char *) config.xdir,
			strlen(config.xdir));
	XChangeProperty(dpy, root, _XA_ADWM_SYSDIR, _XA_UTF8_STRING, 8,
			PropModeReplace, (unsigned char *) config.sdir,
			strlen(config.sdir));

	if (config.iconname)
		XChangeProperty(dpy, root, _XA_ADWM_ICON_THEME_NAME, _XA_UTF8_STRING, 8,
				PropModeReplace, (unsigned char *) config.iconname,
				strlen(config.iconname));
	else
		XDeleteProperty(dpy, root, _XA_ADWM_ICON_THEME_NAME);

	if (config.iconfile)
		XChangeProperty(dpy, root, _XA_ADWM_ICON_THEME, _XA_UTF8_STRING, 8,
				PropModeReplace, (unsigned char *) config.iconfile,
				strlen(config.iconfile));
	else
		XDeleteProperty(dpy, root, _XA_ADWM_ICON_THEME);
}

void
exitewmh(WithdrawCause cause)
{
	XDeleteProperty(dpy, scr->root, _XA_WIN_SUPPORTING_WM_CHECK);
	XDeleteProperty(dpy, scr->root, _XA_NET_SUPPORTING_WM_CHECK);
	XDeleteProperty(dpy, scr->root, _XA_WIN_DESKTOP_BUTTON_PROXY);
	XDeleteProperty(dpy, scr->root, _XA_MOTIF_WM_INFO);

	XDeleteProperty(dpy, scr->root, _XA_WIN_PROTOCOLS);
	XDeleteProperty(dpy, scr->root, _XA_NET_SUPPORTED);

	XDeleteProperty(dpy, scr->root, XA_WM_ICON_SIZE);

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

			_XA_NET_WM_NAME,
			_XA_NET_WM_PID,
			_XA_OPENBOX_PID,
			_XA_OB_VERSION,
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

			_XA_NET_WM_NAME,
			_XA_NET_WM_PID,
			_XA_OPENBOX_PID,
			_XA_OB_VERSION,

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
ewmh_del_client(Client *c, WithdrawCause cause)
{
	switch (cause) {
		int i;

	default:
	case CauseDestroyed:
	case CauseReparented:
	case CauseUnmapped:
	{
		ewmh_update_net_client_lists();
		if (took == c) {
			focus(findfocus(c));
			ewmh_update_net_active_window();
		} else if (sel == c) {
			focus(findselect(c));
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

			_XA_OB_APP_GROUP_CLASS,
			_XA_OB_APP_GROUP_NAME,
			_XA_OB_APP_CLASS,
			_XA_OB_APP_NAME,
			_XA_OB_APP_ROLE,
			_XA_OB_APP_TITLE,
			_XA_OB_APP_TYPE,

			None
		};
		for (i = 0; props[i]; i++)
			XDeleteProperty(dpy, c->win, props[i]);
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

			_XA_OB_APP_GROUP_CLASS,
			_XA_OB_APP_GROUP_NAME,
			_XA_OB_APP_CLASS,
			_XA_OB_APP_NAME,
			_XA_OB_APP_ROLE,
			_XA_OB_APP_TITLE,
			_XA_OB_APP_TYPE,

			None
		};
		for (i = 0; props[i]; i++)
			XDeleteProperty(dpy, c->win, props[i]);
		/* fall through */
	}
	case CauseRestarting:
	{
		Atom props[] = {
			_XA_KDE_NET_WM_FRAME_STRUT,

			None
		};
		for (i = 0; props[i]; i++)
			XDeleteProperty(dpy, c->win, props[i]);
		/* fall through */
		break;
	}
	}
}

void
ewmh_update_ob_app_props(Client *c)
{
	XClassHint ch = { 0, };

	if (getgrpclasshint(c, &ch)) {
		if (ch.res_class) {
			XChangeProperty(dpy, c->win, _XA_OB_APP_GROUP_CLASS,
					_XA_UTF8_STRING, 8, PropModeReplace,
					(unsigned char *) ch.res_class,
					strlen(ch.res_class) + 1);
			XFree(ch.res_class);
			ch.res_class = NULL;
		}
		if (ch.res_name) {
			XChangeProperty(dpy, c->win, _XA_OB_APP_GROUP_NAME,
					_XA_UTF8_STRING, 8, PropModeReplace,
					(unsigned char *) ch.res_name,
					strlen(ch.res_name) + 1);
			XFree(ch.res_name);
			ch.res_name = NULL;
		}
	}
	if (c->ch.res_name || c->ch.res_class) {
		if (c->ch.res_class) {
			XChangeProperty(dpy, c->win, _XA_OB_APP_CLASS,
					_XA_UTF8_STRING, 8, PropModeReplace,
					(unsigned char *) c->ch.res_class,
					strlen(c->ch.res_class) + 1);
		}
		if (c->ch.res_name) {
			XChangeProperty(dpy, c->win, _XA_OB_APP_NAME,
					_XA_UTF8_STRING, 8, PropModeReplace,
					(unsigned char *) c->ch.res_name,
					strlen(c->ch.res_name) + 1);
		}
	}
	{
		const char *title = c->name ? : "Untitled Window";

		XChangeProperty(dpy, c->win, _XA_OB_APP_TITLE,
				_XA_UTF8_STRING, 8, PropModeReplace,
				(unsigned char *) title,
				strlen(title) + 1);
	}
	{
		char *role = NULL;

		if (gettextprop(c->win, _XA_WM_WINDOW_ROLE, &role) && role) {
			XChangeProperty(dpy, c->win, _XA_OB_APP_ROLE,
					_XA_UTF8_STRING, 8, PropModeReplace,
					(unsigned char *) role,
					strlen(role) + 1);
			free(role);
		}
	}
	{
		const char *typename[] = {
			[WTINDEX(WindowTypeDesk)]	= "desktop",
			[WTINDEX(WindowTypeDock)]	= "dock",
			[WTINDEX(WindowTypeToolbar)]	= "toolbar",
			[WTINDEX(WindowTypeMenu)]	= "menu",
			[WTINDEX(WindowTypeUtil)]	= "utility",
			[WTINDEX(WindowTypeSplash)]	= "splash",
			[WTINDEX(WindowTypeDialog)]	= "dialog",
			[WTINDEX(WindowTypeDrop)]	= "dropdown menu",
			[WTINDEX(WindowTypePopup)]	= "popup menu",
			[WTINDEX(WindowTypeTooltip)]	= "tooltip",
			[WTINDEX(WindowTypeNotify)]	= "notification",
			[WTINDEX(WindowTypeCombo)]	= "combo",
			[WTINDEX(WindowTypeDnd)]	= "dnd",
			[WTINDEX(WindowTypeNormal)]	= "normal",
		};
		const char *type = "unknown";
		int i;

		for (i = WindowTypeDesk; i <= WindowTypeNormal; i++) {
			if (WTCHECK(c, i)) {
				type = typename[WTINDEX(i)];
				break;
			}
		}
		XChangeProperty(dpy, c->win, _XA_OB_APP_TYPE,
				_XA_UTF8_STRING, 8, PropModeReplace,
				(unsigned char *) type,
				strlen(type) + 1);

	}
}

void
ewmh_update_echinus_layout_name()
{
	View *v;

	if (!(v = selview()))
		return;
	XChangeProperty(dpy, scr->root, _XA_ECHINUS_LAYOUT, XA_STRING, 8, PropModeReplace,
			(const unsigned char *) &v->layout->symbol, 1L);
}

void
ewmh_update_net_client_list_stacking()
{
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
ewmh_update_net_client_list()
{
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
	XFlush(dpy);		/* XXX: caller's responsibility */
}

void
ewmh_update_net_client_lists()
{
	ewmh_update_net_client_list();
	ewmh_update_net_client_list_stacking();
	XFlush(dpy);		/* XXX: caller's responsibility */
}

void
ewmh_update_net_desktop_viewport()
{
	long *data;

	data = ecalloc(scr->ntags * 2, sizeof(*data));
	XChangeProperty(dpy, scr->root, _XA_NET_DESKTOP_VIEWPORT, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) data, scr->ntags * 2);
	XChangeProperty(dpy, scr->root, _XA_WIN_AREA, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) data, 2);
	free(data);
}

Bool names_synced = False;

void
ewmh_update_net_desktop_names()
{
	char *buf, *pos;
	unsigned int i, len, slen;
	long *data;
	Atom dt;

	if (names_synced) {
		XPRINTF("%s\n", "Updating _NET_DESKTOP_NAMES: NOT!");
		return;
	}
	XPRINTF("%s\n", "Updating _NET_DESKTOP_NAMES");
	for (len = 0, i = 0; i < scr->ntags; i++)
		if (scr->tags[i].name)
			len += strlen(scr->tags[i].name);
	len += scr->ntags;
	pos = buf = ecalloc(len, sizeof(*buf));
	for (i = 0; i < scr->ntags; i++) {
		if (scr->tags[i].name) {
			slen = strlen(scr->tags[i].name);
			memcpy(pos, scr->tags[i].name, slen);
			pos += slen;
		}
		*pos++ = '\0';
	}
	XChangeProperty(dpy, scr->root, _XA_NET_DESKTOP_NAMES, _XA_UTF8_STRING, 8,
			PropModeReplace, (unsigned char *) buf, len);
	XChangeProperty(dpy, scr->root, _XA_WIN_WORKSPACE_NAMES, XA_STRING, 8,
			PropModeReplace, (unsigned char *) buf, len);
	data = ecalloc(scr->ntags, sizeof(*data));
	for (i = 0; i < scr->ntags; i++) {
		/* don't intern more than we need */
		if (!(dt = scr->tags[i].dt))
			dt = scr->tags[i].dt = XInternAtom(dpy, scr->tags[i].name, False);
		data[i] = dt;
	}
	XChangeProperty(dpy, scr->root, _XA_DT_WORKSPACE_LIST, XA_ATOM, 32,
			PropModeReplace, (unsigned char *) data, scr->ntags);
	XChangeProperty(dpy, scr->selwin, _XA_DT_WORKSPACE_LIST, XA_ATOM, 32,
			PropModeReplace, (unsigned char *) data, scr->ntags);
	free(data);
	free(buf);
	names_synced = True;
}

void
ewmh_update_echinus_seltags()
{
	unsigned long long tags;
	Monitor *m;
	long *seltags;
	unsigned int i;

	XPRINTF("%s\n", "Updating _ECHINUS_SELTAGS");
	seltags = ecalloc(scr->ntags, sizeof(*seltags));
	for (m = scr->monitors; m != NULL; m = m->next)
		for (tags = 1, i = 0; i < scr->ntags; i++, tags <<= 1)
			if (m->curview->seltags & tags)
				seltags[i] = True;
	XChangeProperty(dpy, scr->root, _XA_ECHINUS_SELTAGS, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) seltags, scr->ntags);
	free(seltags);
}

void
ewmh_update_net_desktop_layout()
{
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
ewmh_update_net_number_of_desktops()
{
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
ewmh_process_net_number_of_desktops()
{
	long *card;
	unsigned long n = 0;
	unsigned i;

	card = getcard(scr->root, _XA_NET_NUMBER_OF_DESKTOPS, &n);
	if (n > 0) {
		if (0 < card[0] && card[0] <= MAXTAGS) {
			scr->ntags = card[0];
		}
		XFree(card);
	}
	for (i = 0; i < scr->ntags; i++)
		if (!scr->tags[i].dt)
			scr->tags[i].dt = XInternAtom(dpy, scr->tags[i].name, False);
}

void
ewmh_process_net_showing_desktop()
{
	long *card;
	unsigned long n = 0;

	XPRINTF("%s\n", "Processing _NET_SHOWING_DESKTOP");
	card = getcard(scr->root, _XA_NET_SHOWING_DESKTOP, &n);
	if (n > 0) {
		Bool show = card[0];

		XFree(card);
		if (!show != !scr->options.showdesk)
			toggleshowing();
	}
}

void
ewmh_update_net_showing_desktop()
{
	long data = scr->options.showdesk ? 1 : 0;

	XPRINTF("%s\n", "Updating _NET_SHOWING_DESKTOP");
	XChangeProperty(dpy, scr->root, _XA_NET_SHOWING_DESKTOP, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) &data, 1);
}

void
ewmh_update_net_desktop_geometry()
{
	long data[2];

	XPRINTF("%s\n", "Updating _NET_DESKTOP_GEOMETRY");
	data[0] = DisplayWidth(dpy, scr->screen);
	data[1] = DisplayHeight(dpy, scr->screen);
	XChangeProperty(dpy, scr->root, _XA_NET_DESKTOP_GEOMETRY, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) data, 2);
	data[0] = 1;
	data[1] = 1;
	XChangeProperty(dpy, scr->root, _XA_WIN_AREA_COUNT, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) &data, 2);
}

void
ewmh_update_net_monitor_geometry(Client *c)
{
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
			PropModeReplace, (unsigned char *) data, i);
}

void
ewmh_update_net_virtual_roots()
{
	XDeleteProperty(dpy, scr->root, _XA_NET_VIRTUAL_ROOTS);
}

void
ewmh_update_net_current_desktop()
{
	long *data;
	unsigned int i, n;
	Monitor *m;
	Atom dt;

	XPRINTF("%s\n", "Updating _NET_CURRENT_DESKTOP");
	for (n = 0, m = scr->monitors; m; m = m->next, n++) ;
	data = ecalloc(n, sizeof(*data));
	for (i = 0, m = scr->monitors; m; data[i++] = m->curview->index, m = m->next) ;
	XChangeProperty(dpy, scr->root, _XA_NET_CURRENT_DESKTOP, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) data, n);
	XChangeProperty(dpy, scr->root, _XA_WIN_WORKSPACE, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) data, n);
	XChangeProperty(dpy, scr->root, _XA_WM_DESKTOP, _XA_WM_DESKTOP, 32,
			PropModeReplace, (unsigned char *) data, n);
	for (i = 0; i < n; i++) {
		/* don't intern more than we need */
		if (!(dt = scr->tags[data[i]].dt))
			dt = scr->tags[data[i]].dt =
			    XInternAtom(dpy, scr->tags[data[i]].name, False);
		data[i] = dt;
	}
	XChangeProperty(dpy, scr->root, _XA_DT_WORKSPACE_CURRENT, XA_ATOM, 32,
			PropModeReplace, (unsigned char *) data, n);
	XChangeProperty(dpy, scr->selwin, _XA_DT_WORKSPACE_CURRENT, XA_ATOM, 32,
			PropModeReplace, (unsigned char *) data, n);
	free(data);
	ewmh_update_echinus_layout_name();
	ewmh_update_echinus_seltags();
}

void
ewmh_update_net_visible_desktops()
{
	long *data;
	unsigned int i, n;
	Monitor *m;

	XPRINTF("%s\n", "Updating _NET_VISIBLE_DESKTOPS");
	for (n = 0, m = scr->monitors; m; m = m->next, n++) ;
	data = ecalloc(n, sizeof(*data));
	for (i = 0, m = scr->monitors; m; data[i++] = m->curview->index, m = m->next) ;
	XChangeProperty(dpy, scr->root, _XA_NET_VISIBLE_DESKTOPS, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) data, n);
	free(data);
}

static Bool
isomni(Client *c)
{
	if (!c->is.sticky) {
		unsigned long long alltags = (1ULL << scr->ntags) - 1;

		if ((c->tags & alltags) != alltags)
			return False;
	}
	return True;
}

static Bool
islost(Client *c)
{
	if (c->is.sticky)
		return False;
	if (c->tags & ((1ULL << scr->ntags) - 1))
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
			c->tags = ((1ULL << scr->ntags) - 1);
			XFree(desktops);
			return True;
		} else {
			for (goodone = False, i = 0; !goodone && i < n; i++)
				if (0 <= desktops[i] && desktops[i] < scr->ntags)
					goodone = True;
			if (goodone) {
				c->tags = 0;
				for (j = 0; j < n; j++)
					if (0 <= desktops[j] && desktops[j] < scr->ntags)
						c->tags |= (1ULL << desktops[j]);
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
			c->tags = ((1ULL << scr->ntags) - 1);
			XFree(desktops);
			return True;
		} else {
			for (goodone = False, i = 0; !goodone && i < n; i++)
				if (0 <= desktops[i] && desktops[i] < scr->ntags)
					goodone = True;
			if (goodone) {
				c->tags = 0;
				for (j = 0; j < n; j++)
					if (0 <= desktops[j] && desktops[j] < scr->ntags)
						c->tags |= (1ULL << desktops[j]);
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
			c->tags = ((1ULL << scr->ntags) - 1);
			XFree(desktops);
			return True;
		} else
		    if ((desktops[1] & DT_WORKSPACE_HINTS_WORKSPACES) &&
			(n >= 4 + desktops[3])) {
			/* don't intern more than we need */
			for (k = 0; k < scr->ntags; k++)
				if (!scr->tags[k].dt)
					scr->tags[k].dt =
					    XInternAtom(dpy, scr->tags[k].name, False);
			for (goodone = False, i = 4; !goodone && i < desktops[3] + 4; i++)
				for (k = 0; !goodone && k < scr->ntags; k++)
					if (scr->tags[k].dt == desktops[i])
						goodone = True;
			if (goodone) {
				c->tags = 0;
				for (j = 4; j < desktops[3] + 4; j++)
					for (k = 0; k < scr->ntags; k++)
						if (scr->tags[k].dt == desktops[j])
							c->tags |= (1ULL << k);
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
ewmh_update_net_window_desktop_mask(Client *c)
{
	unsigned int longs = (scr->ntags + 31) >> 5;
	unsigned long data[longs];
	unsigned int i, j, k, l;

	XPRINTF("Updating _NET_WM_DESKTOP_MASK for 0x%lx\n", c->win);
	for (j = 0, k = 0; j < longs; j++, k += 32)
		for (i = 0, l = k, data[j] = 0; i < 32 && l < scr->ntags; i++, l++)
			if (c->tags & (1ULL << l))
				data[j] |= (1 << i);
	XChangeProperty(dpy, c->win, _XA_WIN_WORKSPACES, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) data, longs);
	XChangeProperty(dpy, c->win, _XA_NET_WM_DESKTOP_MASK, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) data, longs);
}

void
ewmh_update_net_window_desktop(Client *c)
{
	XPRINTF(c, "Updating _NET_WM_DESKTOP\n");
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
			if (c->tags & (1ULL << i))
				n++;
		data = calloc(n, sizeof(*data));
		for (j = 0, i = 0; i < scr->ntags; i++)
			if (c->tags & (1ULL << i))
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
		Atom dt;

		for (n = 0, i = 0; i < scr->ntags; i++)
			if (c->tags & (1ULL << i))
				n++;
		data = ecalloc(n, sizeof(*data));
		for (j = 0, i = 0; i < scr->ntags; i++)
			if (c->tags & (1ULL << i)) {
				/* don't intern more than we need */
				if (!(dt = scr->tags[i].dt))
					dt = scr->tags[i].dt =
					    XInternAtom(dpy, scr->tags[i].name, False);
				data[j++] = dt;
			}
		XChangeProperty(dpy, c->win, _XA_DT_WORKSPACE_PRESENCE, XA_ATOM, 32,
				PropModeReplace, (unsigned char *) data, n);
		free(data);
	}
	ewmh_update_net_window_desktop_mask(c);
	ewmh_update_net_window_state(c);
}

Bool
ewmh_process_net_window_desktop_mask(Client *c)
{
	unsigned int i, j, k, l;
	long *desktops = NULL;
	unsigned long n = 0;
	unsigned long long prev;

	desktops = getcard(c->win, _XA_WIN_WORKSPACES, &n);
	if (n > 0) {
		prev = c->tags;
		for (j = 0, k = 0; j < n; j++, k += 32)
			for (i = 0, l = k; i < 32 && l < scr->ntags; i++, l++)
				if (desktops[j] & (1 << i))
					c->tags |= (1ULL << l);
		for (i = 0; i < scr->ntags && !(c->tags & (1ULL << i)); i++) ;
		if (i < scr->ntags)
			return True;
		c->tags = prev;
	} else {
		if (desktops)
			XFree(desktops);
		desktops = getcard(c->win, _XA_NET_WM_DESKTOP_MASK, &n);
		if (n > 0) {
			prev = c->tags;
			for (j = 0, k = 0; j < n; j++, k += 32)
				for (i = 0, l = k; i < 32 && l < scr->ntags; i++, l++)
					if (desktops[j] & (1 << i))
						c->tags |= (1ULL << l);
			for (i = 0; i < scr->ntags && !(c->tags & (1ULL << i)); i++) ;
			if (i < scr->ntags) {
				return True;
			}
			c->tags = prev;
		}
	}
	if (desktops)
		XFree(desktops);
	return False;
}

void
ewmh_update_net_work_area()
{
	long *geoms, longs = scr->ntags * 4;
	int i, j, x, y, w, h, l = 0, r = 0, t = 0, b = 0;
	Monitor *m;
	long workarea[4];

	XPRINTF("%s\n", "Updating _NET_WORKAREA");
	geoms = ecalloc(longs, sizeof(*geoms));

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
ewmh_process_net_desktop_names()
{
	int format, status;
	Atom real;
	unsigned long nitems = 0, extra = 0, bytes = 1;
	char *ret = NULL, *pos;
	unsigned int i, len;

	names_synced = True;
	do {
		if (ret)
			XFree((unsigned char *) ret);
		bytes += 4 * extra;
		status =
		    XGetWindowProperty(dpy, scr->root, _XA_NET_DESKTOP_NAMES, 0L, bytes,
				       False, _XA_UTF8_STRING, &real, &format, &nitems,
				       &extra, (unsigned char **) &ret);
		if (status != Success) {
			names_synced = False;
			break;
		}
	} while (extra && bytes == 1);

	for (pos = ret, i = 0; nitems && i < scr->ntags; i++) {
		if ((len = strnlen(pos, nitems))) {
			strncpy(scr->tags[i].name, pos, nitems);
			XPRINTF("Assigning name '%s' to tag %u\n", scr->tags[i].name, i);
		} else
			names_synced = False;
		nitems -= len;
		pos += len;
		if (nitems) {
			--nitems;
			++pos;
		}
	}
	if (i < scr->ntags)
		names_synced = False;
	if (ret)
		XFree(ret);
	ewmh_update_net_desktop_names();
}

void
ewmh_update_net_active_window()
{
	Window win;

	/* little bit of a question whether we set on the dockapp icon or win */
	XPRINTF("%s\n", "Updating _NET_ACTIVE_WINDOW");
	win = sel ? sel->win : None;
	XChangeProperty(dpy, scr->root, _XA_NET_ACTIVE_WINDOW, XA_WINDOW, 32,
			PropModeReplace, (unsigned char *) &win, 1);
	XChangeProperty(dpy, scr->root, _XA_WIN_FOCUS, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) &win, 1);
}

void
ewmh_update_net_desktop_modes()
{
	long *data;
	unsigned int i;

	XPRINTF("%s\n", "Updating _NET_DESKTOP_MODES");
	data = ecalloc(scr->ntags, sizeof(*data));
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
		case 'g':
			data[i] = _XA_NET_DESKTOP_MODE_GRID;
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
				 MWM_DECOR_STICK|MWM_DECOR_MAXIMUS)

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
				c->can.full = False;
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
				c->has.but.half = False;
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
		if ((hints[0] & MWM_HINTS_INPUT_MODE) && n >= 4 && (hint = hints[3])) {
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
		if ((hints[0] & MWM_HINTS_STATUS) && n >= 5 && (hint = hints[4])) {
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
getmwmhints(Window win, Bool *title, Bool *grips, int *border)
{
	long *hints = NULL;
	unsigned long n = 0;

	hints = getcard(win, _XA_MOTIF_WM_HINTS, &n);
	if (n > 0) {
		if ((hints[0] & MWM_HINTS_DECORATIONS) && n > 1) {
			*border =
			    (hints[1] & (MWM_DECOR_ALL | MWM_DECOR_BORDER)) ? scr->style.
			    border : 0;
			*title =
			    (hints[1] & (MWM_DECOR_ALL | MWM_DECOR_TITLE)) ? True : False;
			*grips =
			    (hints[1] & (MWM_DECOR_ALL | MWM_DECOR_RESIZEH)) ? True :
			    False;
		}
		XFree(hints);
	}
}

#define WIN_LAYER_DESKTOP      0 /* windows with _NET_WM_TYPE_DESKTOP */
#define WIN_LAYER_BELOW        2 /* windows with _NET_WM_STATE_BELOW */
#define WIN_LAYER_NORMAL       4 /* windows not belonging to any other layer */
#define WIN_LAYER_ONTOP        6
#define WIN_LAYER_DOCK         8 /* windows of type _NET_WM_TYPE_DOCK unless _NET_WM_STATE_BELOW */
#define WIN_LAYER_ABOVE_DOCK  10 /* window with _NET_WM_STATE_ABOVE */
#define WIN_LAYER_MENU        12 /* focused windows with _NET_WM_STATE_FULLSCREEN */

#define WIN_LAYER_FULLSCREEN  14 /* hack */
#define WIN_LAYER_ABOVE_ALL   15 /* for taskbar autohide */

unsigned long
get_layer(Client *c)
{
	unsigned long layer;

	if (window_stack.modal_transients && c->is.modal == ModalSystem)
		layer = WIN_LAYER_MENU;
	else if (WTCHECK(c, WindowTypeDesk))
		layer = WIN_LAYER_DESKTOP;
	else if (sel == c && c->is.full)
		layer = WIN_LAYER_FULLSCREEN;
	else if (sel && ((sel->is.dockapp && c->is.dockapp)
			 || (WTCHECK(sel, WindowTypeDock) && sel == c)))
		layer = WIN_LAYER_ABOVE_ALL;
	else if (WTCHECK(c, WindowTypeDock) && !c->is.below)
		layer = WIN_LAYER_DOCK;
	else if (c->is.above)
		layer = WIN_LAYER_ABOVE_DOCK;
	else if (!WTCHECK(c, WindowTypeDock) && !c->is.below)
		layer = WIN_LAYER_NORMAL;
	else
		layer = WIN_LAYER_BELOW;
	return (layer);
}

unsigned long
get_full_layer(Client *c)
{
	unsigned long layer;
	Window w;
	Client *l;

	layer = get_layer(c);
	if (c->is.grptrans && (w = c->leader) && (l = findclient(w)))
		layer = max(layer, get_layer(l));
	if (c->is.transient && (w = c->transfor) && (l = findclient(w)))
		layer = max(layer, get_layer(l));
	return (layer);
}

void
wmh_update_win_layer(Client *c)
{
	unsigned long layer = get_full_layer(c);

	XChangeProperty(dpy, c->win, _XA_WIN_LAYER, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) &layer, 1L);
}

void
ewmh_update_net_window_actions(Client *c)
{
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
	if (c->can.full)
		action[actions++] = _XA_NET_WM_ACTION_FULLSCREEN;
	if (c->can.tag)
		action[actions++] = _XA_NET_WM_ACTION_CHANGE_DESKTOP;
	if (c->can.close)
		action[actions++] = _XA_NET_WM_ACTION_CLOSE;
	if (c->can.above)
		action[actions++] = _XA_NET_WM_ACTION_ABOVE;
	if (c->can.below)
		action[actions++] = _XA_NET_WM_ACTION_BELOW;
	/* following four are non-standard */
	if (c->can.fill)
		action[actions++] = _XA_NET_WM_ACTION_FILL;
	if (c->can.floats)
		action[actions++] = _XA_NET_WM_ACTION_FLOAT;
	if (c->can.size)
		action[actions++] = _XA_NET_WM_ACTION_MAXIMUS_LEFT;
	if (c->can.size)
		action[actions++] = _XA_NET_WM_ACTION_MAXIMUS_RIGHT;
	/* following is openbox specific (but a good idea) libwnck++ observes */
	if (c->can.undec)
		action[actions++] = _XA_OB_WM_ACTION_UNDECORATE;

	XChangeProperty(dpy, c->win, _XA_NET_WM_ALLOWED_ACTIONS, XA_ATOM, 32,
			PropModeReplace, (unsigned char *) action, actions);
}

#define WIN_STATE_STICKY          (1<< 0)	/* everyone knows sticky */
#define WIN_STATE_MINIMIZED       (1<< 1)	/* Reserved - definition is unclear */
#define WIN_STATE_MAXIMIZED_VERT  (1<< 2)	/* window is maximized V state */
#define WIN_STATE_MAXIMIZED_HORIZ (1<< 3)	/* window is maximized H state */
#define WIN_STATE_HIDDEN          (1<< 4)	/* not on taskbar but window visible */
#define WIN_STATE_SHADED          (1<< 5)	/* shaded (MacOS / Afterstep style) */
#define WIN_STATE_HID_WORKSPACE   (1<< 6)	/* not on current desktop */
#define WIN_STATE_HID_TRANSIENT   (1<< 7)	/* owner of transient is hidden */
#define WIN_STATE_FIXED_POSITION  (1<< 8)	/* window is fixed in position even */
#define WIN_STATE_ARRANGE_IGNORE  (1<< 9)	/* ignore for auto arranging */

#define WIN_STATE_FILLED	  (1<<13)	/* window fills available space */
#define WIN_STATE_MAXIMUS_LEFT    (1<<14)	/* window is maximus L state */
#define WIN_STATE_MAXIMUS_RIGHT   (1<<15)	/* window is maximus R state */
#define WIN_STATE_UNDECORATED	  (1<<16)	/* window is undecorated */

/* additionals from IceWM */

#define WIN_STATE_FOCUSED	  (1<<21)	/* has the focus */
#define WIN_STATE_URGENT	  (1<<22)	/* demands attention */
#define WIN_STATE_SKIP_PAGER	  (1<<23)	/* skip pager */
#define WIN_STATE_SKIP_TASKBAR	  (1<<24)	/* skip taskbar */
#define WIN_STATE_MODAL		  (1<<25)	/* modal */
#define WIN_STATE_BELOW		  (1<<26)	/* below layer */
#define WIN_STATE_ABOVE		  (1<<27)	/* above layer */
#define WIN_STATE_FULLSCREEN	  (1<<28)	/* fullscreen (no layout limits) */
#define WIN_STATE_WAS_HIDDEN	  (1<<29)	/* was hidden when parent minimized/hidden */
#define WIN_STATE_WAS_MINIMIZED	  (1<<30)	/* was minimized when parent minimized/hidden */
#define WIN_STATE_WITHDRAWN	  (1<<31)	/* managed, but not available to user */


void
ewmh_update_net_window_state(Client *c)
{
	long winstate[20];
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
	if (c->is.maxv || c->is.max)
		winstate[states++] = _XA_NET_WM_STATE_MAXIMIZED_VERT;
	if (c->is.maxh || c->is.max)
		winstate[states++] = _XA_NET_WM_STATE_MAXIMIZED_HORZ;
	if (c->is.shaded)
		winstate[states++] = _XA_NET_WM_STATE_SHADED;
	if (c->skip.taskbar)
		winstate[states++] = _XA_NET_WM_STATE_SKIP_TASKBAR;
	if (c->skip.pager)
		winstate[states++] = _XA_NET_WM_STATE_SKIP_PAGER;
	if (c->is.icon || c->is.hidden)
		winstate[states++] = _XA_NET_WM_STATE_HIDDEN;
	if (c->is.full)
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
	if (c->skip.arrange || c->is.floater)
		winstate[states++] = _XA_NET_WM_STATE_FLOATING;
	if (c->is.fill)
		winstate[states++] = _XA_NET_WM_STATE_FILLED;
	if (c->is.lhalf)
		winstate[states++] = _XA_NET_WM_STATE_MAXIMUS_LEFT;
	if (c->is.rhalf)
		winstate[states++] = _XA_NET_WM_STATE_MAXIMUS_RIGHT;

	/* following is openbox specific */
	if (c->is.undec)
		winstate[states++] = _XA_OB_WM_STATE_UNDECORATED;

	XChangeProperty(dpy, c->win, _XA_NET_WM_STATE, XA_ATOM, 32,
			PropModeReplace, (unsigned char *) winstate, states);

	XPRINTF("Updating _WIN_STATE for 0x%lx\n", c->win);
	if (c->is.sticky)
		state |= WIN_STATE_STICKY;
	if (c->is.icon)
		state |= WIN_STATE_MINIMIZED;
	if (c->is.maxv || c->is.max)
		state |= WIN_STATE_MAXIMIZED_VERT;
	if (c->is.maxh || c->is.max)
		state |= WIN_STATE_MAXIMIZED_HORIZ;
	if (c->skip.taskbar)
		state |= WIN_STATE_HIDDEN;
	if (c->is.shaded)
		state |= WIN_STATE_SHADED;
	if (c->is.banned)
		state |= WIN_STATE_HID_WORKSPACE;
	if (c->is.icon && c->transfor)
		state |= WIN_STATE_HID_TRANSIENT;
	if (!c->can.move)
		state |= WIN_STATE_FIXED_POSITION;
	if (c->skip.arrange || c->is.floater)
		state |= WIN_STATE_ARRANGE_IGNORE;
	/* the following are ADWM specific extensions */
	if (c->is.fill)
		state |= WIN_STATE_FILLED;
	if (c->is.lhalf)
		state |= WIN_STATE_MAXIMUS_LEFT;
	if (c->is.rhalf)
		state |= WIN_STATE_MAXIMUS_RIGHT;
	if (c->is.undec)
		state |= WIN_STATE_UNDECORATED;
	/* the following are IceWM specific extensions */
	if (c->is.focused)
		state |= WIN_STATE_FOCUSED;
	if (c->is.attn)
		state |= WIN_STATE_URGENT;
	if (c->skip.pager)
		state |= WIN_STATE_SKIP_PAGER;
	if (c->skip.taskbar)
		state |= WIN_STATE_SKIP_TASKBAR;
	if (c->is.modal)
		state |= WIN_STATE_MODAL;
	if (c->is.below)
		state |= WIN_STATE_BELOW;
	if (c->is.above)
		state |= WIN_STATE_ABOVE;
	if (c->is.full)
		state |= WIN_STATE_FULLSCREEN;
#if 0
	if (c->was.hidden)
		state |= WIN_STATE_WAS_HIDDEN;
	if (c->was.icon)
		state |= WIN_STATE_WAS_MINIMIZED;
#endif
	if (c->is.dockapp)
		state |= WIN_STATE_WITHDRAWN;

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
			if (c->can.stick)
				togglesticky(c);
	if (mask & WIN_STATE_MINIMIZED)
		if (((change & WIN_STATE_MINIMIZED) && !c->is.icon) ||
		    (!(change & WIN_STATE_MINIMIZED) && c->is.icon))
			if (c->can.min)
				togglemin(c);
	if (mask & WIN_STATE_MAXIMIZED_VERT)
		if (((change & WIN_STATE_MAXIMIZED_VERT) && !c->is.maxv) ||
		    (!(change & WIN_STATE_MAXIMIZED_VERT) && c->is.maxv))
			if (c->can.maxv)
				togglemaxv(c);
	if (mask & WIN_STATE_MAXIMIZED_HORIZ)
		if (((change & WIN_STATE_MAXIMIZED_HORIZ) && !c->is.maxh) ||
		    (!(change & WIN_STATE_MAXIMIZED_HORIZ) && c->is.maxh))
			if (c->can.maxh)
				togglemaxh(c);
	if (mask & WIN_STATE_HIDDEN) {
		if (((change & WIN_STATE_HIDDEN) && !c->skip.taskbar) ||
		    (!(change & WIN_STATE_HIDDEN) && c->skip.taskbar))
			toggletaskbar(c);
	}
	if (mask & WIN_STATE_SHADED)
		if (((change & WIN_STATE_SHADED) && !c->is.shaded) ||
		    (!(change & WIN_STATE_SHADED) && c->is.shaded))
			if (c->can.shade)
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
			if (c->can.arrange)
				togglefloating(c);
	/* the following are ADWM specific extensions */
	if (mask & WIN_STATE_FILLED)
		if (((change & WIN_STATE_FILLED) && !c->is.fill) ||
		    (!(change & WIN_STATE_FILLED) && c->is.fill)) {
			if (c->can.fill)
				togglefill(c);
			arrange(NULL);
		}
	if (mask & WIN_STATE_MAXIMUS_LEFT)
		if (((change & WIN_STATE_MAXIMUS_LEFT) && !c->is.lhalf) ||
		    (!(change & WIN_STATE_MAXIMUS_LEFT) && c->is.lhalf)) {
			if (c->can.size)
				togglelhalf(c);
			arrange(NULL);
		}
	if (mask & WIN_STATE_MAXIMUS_RIGHT)
		if (((change & WIN_STATE_MAXIMUS_RIGHT) && !c->is.rhalf) ||
		    (!(change & WIN_STATE_MAXIMUS_RIGHT) && c->is.rhalf)) {
			if (c->can.size)
				togglerhalf(c);
			arrange(NULL);
		}
	if (mask & WIN_STATE_UNDECORATED)
		if (((change & WIN_STATE_UNDECORATED) && !c->is.undec) ||
		    (!(change & WIN_STATE_UNDECORATED) && c->is.undec)) {
			if (c->can.undec)
				toggleundec(c);
			arrange(NULL);
		}
	/* the following are IceWM specific extensions */
	if (mask & WIN_STATE_FOCUSED) {
		/* read-only */
	}
	if (mask & WIN_STATE_URGENT) {
		/* read-only */
	}
	if (mask & WIN_STATE_SKIP_PAGER)
		if (((change & WIN_STATE_SKIP_PAGER) && !c->skip.pager) ||
		    (!(change & WIN_STATE_SKIP_PAGER) && c->skip.pager)) {
			togglepager(c);
			arrange(NULL);
		}
	if (mask & WIN_STATE_SKIP_TASKBAR)
		if (((change & WIN_STATE_SKIP_TASKBAR) && !c->skip.taskbar) ||
		    (!(change & WIN_STATE_SKIP_TASKBAR) && c->skip.taskbar)) {
			toggletaskbar(c);
			arrange(NULL);
		}
	if (mask & WIN_STATE_MODAL) {
		/* read-only */
	}
	if (mask & WIN_STATE_BELOW)
		if (((change & WIN_STATE_BELOW) && !c->is.below) ||
		    (!(change & WIN_STATE_BELOW) && c->is.below)) {
			if (c->can.below)
				togglebelow(c);
			arrange(NULL);
		}
	if (mask & WIN_STATE_ABOVE)
		if (((change & WIN_STATE_ABOVE) && !c->is.above) ||
		    (!(change & WIN_STATE_ABOVE) && c->is.above)) {
			if (c->can.above)
				toggleabove(c);
			arrange(NULL);
		}
	if (mask & WIN_STATE_FULLSCREEN)
		if (((change & WIN_STATE_FULLSCREEN) && !c->is.full) ||
		    (!(change & WIN_STATE_FULLSCREEN) && c->is.full)) {
			if (c->can.full)
				togglefull(c);
			arrange(NULL);
		}
#if 0
	if (mask & WIN_STATE_WAS_HIDDEN) {
		/* read-only */
	}
	if (mask & WIN_STATE_WAS_MINIMIZED) {
		/* read-only */
	}
#endif
	if (mask & WIN_STATE_WITHDRAWN) {
		/* read-only */
	}
}

void
ewmh_process_state_atom(Client *c, Atom state, int set)
{
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
			if (c->can.stick)
				togglesticky(c);
	} else if (state == _XA_NET_WM_STATE_MAXIMIZED_VERT) {
		if ((set == _NET_WM_STATE_ADD && !c->is.maxv) ||
		    (set == _NET_WM_STATE_REMOVE && c->is.maxv) ||
		    (set == _NET_WM_STATE_TOGGLE))
			if (c->can.maxv)
				togglemaxv(c);
	} else if (state == _XA_NET_WM_STATE_MAXIMIZED_HORZ) {
		if ((set == _NET_WM_STATE_ADD && !c->is.maxh) ||
		    (set == _NET_WM_STATE_REMOVE && c->is.maxh) ||
		    (set == _NET_WM_STATE_TOGGLE))
			if (c->can.maxh)
				togglemaxh(c);
	} else if (state == _XA_NET_WM_STATE_SHADED) {
		if ((set == _NET_WM_STATE_ADD && !c->is.shaded) ||
		    (set == _NET_WM_STATE_REMOVE && c->is.shaded) ||
		    (set == _NET_WM_STATE_TOGGLE))
			if (c->can.shade)
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
	} else if (state == _XA_NET_WM_STATE_FULLSCREEN) {
		if ((set == _NET_WM_STATE_ADD || set == _NET_WM_STATE_TOGGLE)
		    && !c->is.full) {
			if (c->can.full)
				togglefull(c);
		} else if ((set == _NET_WM_STATE_REMOVE ||
			    set == _NET_WM_STATE_TOGGLE) && c->is.full) {
			if (c->can.full)
				togglefull(c);
		}
		DPRINT;
		arrange(nearview());
		XPRINTF("%s: x%d y%d w%d h%d\n", c->name, c->c.x, c->c.y, c->c.w, c->c.h);
	} else if (state == _XA_NET_WM_STATE_ABOVE) {
		if ((set == _NET_WM_STATE_ADD && !c->is.above) ||
		    (set == _NET_WM_STATE_REMOVE && c->is.above) ||
		    (set == _NET_WM_STATE_TOGGLE))
			if (c->can.above)
				toggleabove(c);
	} else if (state == _XA_NET_WM_STATE_BELOW) {
		if ((set == _NET_WM_STATE_ADD && !c->is.below) ||
		    (set == _NET_WM_STATE_REMOVE && c->is.below) ||
		    (set == _NET_WM_STATE_TOGGLE))
			if (c->can.below)
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
			if (c->can.floats)
				togglefloating(c);
		}
	} else if (state == _XA_NET_WM_STATE_FILLED) {
		if ((set == _NET_WM_STATE_ADD && !c->is.fill) ||
		    (set == _NET_WM_STATE_REMOVE && c->is.fill) ||
		    (set == _NET_WM_STATE_TOGGLE)) {
			if (c->can.fill)
				togglefill(c);
		}
	} else if (state == _XA_NET_WM_STATE_MAXIMUS_LEFT) {
		if ((set == _NET_WM_STATE_ADD && !c->is.lhalf) ||
		    (set == _NET_WM_STATE_REMOVE && c->is.lhalf) ||
		    (set == _NET_WM_STATE_TOGGLE)) {
			if (c->can.size)
				togglelhalf(c);
		}
	} else if (state == _XA_NET_WM_STATE_MAXIMUS_RIGHT) {
		if ((set == _NET_WM_STATE_ADD && !c->is.rhalf) ||
		    (set == _NET_WM_STATE_REMOVE && c->is.rhalf) ||
		    (set == _NET_WM_STATE_TOGGLE)) {
			if (c->can.size)
				togglerhalf(c);
		}
	} else if (state == _XA_OB_WM_STATE_UNDECORATED) {
		if ((set == _NET_WM_STATE_ADD && !c->is.undec) ||
		    (set == _NET_WM_STATE_REMOVE && c->is.undec) ||
		    (set == _NET_WM_STATE_TOGGLE)) {
			if (c->can.undec)
				toggleundec(c);
		}
	}
}

void
ewmh_update_net_window_extents(Client *c)
{
	int t = c->c.b + c->c.v + c->c.t;
	int l = c->c.b + c->c.v;
	int r = c->c.b + c->c.v;
	int b = c->c.b + c->c.v + c->c.g;
	Bool change = False;

	if (c->e.t != t) {
		c->e.t = t;
		change = True;
	}
	if (c->e.l != l) {
		c->e.l = l;
		change = True;
	}
	if (c->e.r != r) {
		c->e.r = r;
		change = True;
	}
	if (c->e.b != b) {
		c->e.b = b;
		change = True;
	}
	if (!change)
		return;

	long data[4] = { c->e.l, c->e.r, c->e.t, c->e.b };

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
ewmh_update_net_window_visible_name(Client *c)
{
	if (c->name) {
		XTextProperty prop = { NULL, };
		char *list[2] = { c->name, NULL };

		if (Xutf8TextListToTextProperty(dpy, list, 1, XUTF8StringStyle, &prop) == Success) {
			XSetTextProperty(dpy, c->win, &prop, _XA_NET_WM_VISIBLE_NAME);
			if (prop.value)
				XFree(prop.value);
		}
	} else {
		XDeleteProperty(dpy, c->win, _XA_NET_WM_VISIBLE_NAME);
	}
}

void
ewmh_update_net_window_visible_icon_name(Client *c)
{

	if (c->icon_name) {
		XTextProperty prop = { NULL, };
		char *list[2] = { c->icon_name, NULL };

		if (Xutf8TextListToTextProperty(dpy, list, 1, XUTF8StringStyle, &prop) == Success) {
			XSetTextProperty(dpy, c->win, &prop, _XA_NET_WM_VISIBLE_ICON_NAME);
			if (prop.value)
				XFree(prop.value);
		}
	} else {
		XDeleteProperty(dpy, c->win, _XA_NET_WM_VISIBLE_ICON_NAME);
	}
}

#ifdef STARTUP_NOTIFICATION
static void
ewmh_update_sn_app_props(Client *c, Notify *n)
{
	SnStartupSequence *seq = n->seq;
	const char *text;
	Window win = None;
	long data;

	if (!win && c->session && (c->session != c->win))
		win = c->session;
	if (!win && c->leader && (c->leader != c->win))
		win = c->leader;
	if (!win)
		win = c->win;

	XPRINTF(c, "%s updating startup properties on window 0x%lx\n", n->id, win);
	if ((text = sn_startup_sequence_get_id(seq))) {
		XChangeProperty(dpy, win, _XA_NET_STARTUP_ID,
				_XA_UTF8_STRING, 8, PropModeReplace,
				(unsigned char *) text, strlen(text) + 1);
	} else
		XPRINTF(c, "%s sequence has no id!\n", n->id);
	if ((text = n->launcher)) {
		XChangeProperty(dpy, win, _XA_NET_APP_LAUNCHER,
				_XA_UTF8_STRING, 8, PropModeReplace,
				(unsigned char *) text, strlen(text) + 1);
	} else
		XPRINTF(c, "%s sequence has no launcher!\n", n->id);
	if ((text = n->launchee)) {
		XChangeProperty(dpy, win, _XA_NET_APP_LAUNCHEE,
				_XA_UTF8_STRING, 8, PropModeReplace,
				(unsigned char *) text, strlen(text) + 1);
	} else
		XPRINTF(c, "%s sequence has no launchee!\n", n->id);
	if ((text = n->hostname)) {
		XChangeProperty(dpy, win, _XA_NET_APP_HOSTNAME,
				_XA_UTF8_STRING, 8, PropModeReplace,
				(unsigned char *) text, strlen(text) + 1);
	} else
		XPRINTF(c, "%s sequence has no hostname!\n", n->id);
	if (n->pid != -1) {
		data = n->pid;
		XChangeProperty(dpy, win, _XA_NET_APP_PID,
				XA_CARDINAL, 32, PropModeReplace,
				(unsigned char *) &data, 1);
	} else
		XPRINTF(c, "%s sequence has no pid!\n", n->id);
	if (n->sequence != -1) {
		data = n->sequence;
		XChangeProperty(dpy, win, _XA_NET_APP_SEQUENCE,
				XA_CARDINAL, 32, PropModeReplace,
				(unsigned char *) &data, 1);
	} else
		XPRINTF(c, "%s sequence has no sequence!\n", n->id);
	if (n->timestamp != -1) {
		data = n->timestamp;
		XChangeProperty(dpy, win, _XA_NET_APP_TIMESTAMP,
				XA_CARDINAL, 32, PropModeReplace,
				(unsigned char *) &data, 1);
	} else
		XPRINTF(c, "%s sequence has no timestamp!\n", n->id);
	if ((text = sn_startup_sequence_get_application_id(seq))) {
		XChangeProperty(dpy, win, _XA_NET_APP_APPLICATION_ID,
				_XA_UTF8_STRING, 8, PropModeReplace,
				(unsigned char *) text, strlen(text) + 1);
	} else
		XPRINTF(c, "%s sequence has no application id!\n", n->id);
	if ((text = sn_startup_sequence_get_name(seq))) {
		XChangeProperty(dpy, win, _XA_NET_APP_NAME,
				_XA_UTF8_STRING, 8, PropModeReplace,
				(unsigned char *) text, strlen(text) + 1);
	} else
		XPRINTF(c, "%s sequence has no name!\n", n->id);
	if ((text = sn_startup_sequence_get_description(seq))) {
		XChangeProperty(dpy, win, _XA_NET_APP_DESCRIPTION,
				_XA_UTF8_STRING, 8, PropModeReplace,
				(unsigned char *) text, strlen(text) + 1);
	} else
		XPRINTF(c, "%s sequence has no description!\n", n->id);
	if ((text = sn_startup_sequence_get_icon_name(seq))) {
		XChangeProperty(dpy, win, _XA_NET_APP_ICON_NAME,
				_XA_UTF8_STRING, 8, PropModeReplace,
				(unsigned char *) text, strlen(text) + 1);
	} else
		XPRINTF(c, "%s sequence has no icon name!\n", n->id);
	if ((text = sn_startup_sequence_get_binary_name(seq))) {
		XChangeProperty(dpy, win, _XA_NET_APP_BINARY_NAME,
				_XA_UTF8_STRING, 8, PropModeReplace,
				(unsigned char *) text, strlen(text) + 1);
	} else
		XPRINTF(c, "%s sequence has no binary name!\n", n->id);
	if ((text = sn_startup_sequence_get_wmclass(seq))) {
		XChangeProperty(dpy, win, _XA_NET_APP_WMCLASS,
				_XA_UTF8_STRING, 8, PropModeReplace,
				(unsigned char *) text, strlen(text) + 1);
	} else
		XPRINTF(c, "%s sequence has no wmclass!\n", n->id);
	if (sn_startup_sequence_get_screen(seq) != -1) {
		data = sn_startup_sequence_get_screen(seq);
		XChangeProperty(dpy, win, _XA_NET_APP_SCREEN,
				XA_CARDINAL, 32, PropModeReplace,
				(unsigned char *) &data, 1);
	} else
		XPRINTF(c, "%s sequence has no screen!\n", n->id);
	if (sn_startup_sequence_get_workspace(seq) != -1) {
		data = sn_startup_sequence_get_workspace(seq);
		XChangeProperty(dpy, win, _XA_NET_APP_WORKSPACE,
				XA_CARDINAL, 32, PropModeReplace,
				(unsigned char *) &data, 1);
	} else
		XPRINTF(c, "%s sequence has no workspace!\n", n->id);
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
	if ((user_time == CurrentTime) || ((int) ((int) time - (int) user_time) > 0))
		user_time = time;
}

#ifdef STARTUP_NOTIFICATION
SnStartupSequence *
find_startup_seq(Client *c)
{
	Notify *n = NULL, **np;
	SnStartupSequence *seq = NULL;
	char **argv = NULL, *machine = NULL, *startup_id = NULL;
	int argc;
	const char *binary, *wmclass;
	pid_t pid = 0;

	np = &notifies;
	while ((n = *np)) {
		/* 1500 seconds old is just plain too old */
		if ((user_time > n->timestamp) && user_time - n->timestamp > 1500000) {
			XPRINTF("%s %lu - %lu > 15000: deleting as too old\n", n->id, user_time, n->timestamp);
			n_del_notify(n);
			assert(*np != n); /* to be sure */
		} else
			/* yes, this works while deleting */
			np = &n->next;
	}

	if ((seq = c->seq))
		return seq;
	do {
		if ((startup_id = getstartupid(c))) {
			for (n = notifies; n; n = n->next) {
				XPRINTF("comparing '%s' with '%s'\n",
					startup_id, sn_startup_sequence_get_id(n->seq));
				if (!strcmp(startup_id, sn_startup_sequence_get_id(n->seq)))
					break;
			}
			if (n)
				break;
			XPRINTF("cannot find startup id '%s'!\n",
				startup_id);
		}
		if ((pid = getnetpid(c)) && (machine = getclientmachine(c))) {
			for (n = notifies; n; n = n->next) {

				XPRINTF("checking _NET_WM_PID and WM_CLIENT_MACHINE for '%s'\n",
					sn_startup_sequence_get_id(n->seq));
				if (n->hostname && strcasecmp(machine, n->hostname))
					continue;	/* wrong host */
				if (pid == n->pid)
					break;
			}
			if (n)
				break;
		}
		if (c->ch.res_name || c->ch.res_class) {
			for (n = notifies; n; n = n->next) {
				XPRINTF("checking WM_CLASS for '%s'\n",
					sn_startup_sequence_get_id(n->seq));
				if (machine && n->hostname && strcasecmp(machine, n->hostname))
					continue;	/* wrong host */
				if ((wmclass = sn_startup_sequence_get_wmclass(n->seq))) {
					if (c->ch.res_name && !strcmp(wmclass, c->ch.res_name))
						break;
					if (c->ch.res_class && !strcmp(wmclass, c->ch.res_class))
						break;
				}
			}
			if (n)
				break;
			XPRINTF("cannot find startup for (%s,%s)\n",
				c->ch.res_name, c->ch.res_class);
		}
		if (getcommand(c, &argv, &argc)) {
			for (n = notifies; n; n = n->next) {
				XPRINTF("checking WM_COMMAND for '%s'\n",
					sn_startup_sequence_get_id(n->seq));
				if (machine && n->hostname && strcasecmp(machine, n->hostname))
					continue;	/* wrong host */
				if ((binary = sn_startup_sequence_get_binary_name(n->seq))) {
					if (argv[0] && !strcmp(binary, argv[0]))
						break;
				}
			}
			if (n)
				break;
			XPRINTF("cannot find startup for !%s\n", argv[0]);
		}
		if (c->ch.res_name || c->ch.res_class) {
			/* try again, case insensitive */
			for (n = notifies; n; n = n->next) {
				XPRINTF("checking WM_CLASS for '%s'\n",
					sn_startup_sequence_get_id(n->seq));
				if (machine && n->hostname && strcasecmp(machine, n->hostname))
					continue;	/* wrong host */
				if ((wmclass = sn_startup_sequence_get_wmclass(n->seq))) {
					if (c->ch.res_name && !strcasecmp(wmclass, c->ch.res_name))
						break;
					if (c->ch.res_class && !strcasecmp(wmclass, c->ch.res_class))
						break;
				}
			}
			if (n)
				break;
			XPRINTF("cannot find startup for (%s,%s) (no case)\n",
				c->ch.res_name, c->ch.res_class);
		}
	}
	while (0);
	if (n) {
		long workspace;

		XPRINTF(c, "FOUND STARTUP ID '%s'!\n", n->id);
		if (!startup_id)
			setstartupid(c, n->id);
		/* Note that if the window has mapped itself on the wrong workspace, we
		   should move it there. */
		if (sn_startup_sequence_get_workspace(n->seq) != -1) {
			workspace = sn_startup_sequence_get_workspace(n->seq);
			if (0 <= workspace && workspace < scr->ntags) {
				if (c->is.managed) {
					XPRINTF(c, "moving to workspace %ld for sequence '%s'\n", workspace, n->id);
					tagonly(c, workspace);
				} else {
					XPRINTF(c, "marking for workspace %ld for sequence '%s'\n", workspace, n->id);
					c->tags = (1ULL << workspace);
					/* likely have not been read yet */
					XChangeProperty(dpy, c->win, _XA_NET_WM_DESKTOP,
							XA_CARDINAL, 32, PropModeReplace,
							(unsigned char *) &workspace, 1);
					XChangeProperty(dpy, c->win, _XA_WIN_WORKSPACE,
							XA_CARDINAL, 32, PropModeReplace,
							(unsigned char *) &workspace, 1);
				}
			}
		} else
			XPRINTF(c, "workspace not defined in sequence '%s'\n", n->id);
		if (strstr(n->id, "xdg-launch") == n->id)
			if (0 <= n->sequence && n->sequence < scr->nmons)
				c->monitor = n->sequence + 1;
		if (n->timestamp && n->timestamp != -1)
			push_client_time(c, n->timestamp);
		ewmh_update_sn_app_props(c, n);
		seq = n_set_notify(n, c);
	}
	if (startup_id)
		free(startup_id);
	if (machine)
		free(machine);
	if (argv)
		XFreeStringList(argv);
	return (seq);
}
#endif

void
ewmh_process_net_startup_id(Client *c)
{
#ifdef STARTUP_NOTIFICATION
	find_startup_seq(c);
#else
	char *startup_id;

	if (c->is.managed)
		return;
	if ((startup_id = getstartupid(c))) {
		char *launcher = NULL, *launchee = NULL, *hostname = NULL;
		pid_t pid = 0;
		long sequence = 0, timestamp = 0;

		if (parse_startup_id(startup_id, &launcher, &launchee,
				     &hostname, &pid, &sequence, &timestamp)) {

			/* Note: when xdg-launch is the launcher, the startup id is:
			   %s/%s/%d-%d-%s_TIME%lu, launcher, launchee, pid, sequence,
			   %hostname, ts where the sequence is in fact the monitor
			   %number. */

			if (strstr(launcher, "xdg-launch") &&
			    0 <= sequence && sequence < scr->nmons)
				c->monitor = sequence + 1;
		}
		if (timestamp)
			push_client_time(c, timestamp);
		free(launcher);
		free(launchee);
		free(hostname);
	}
	free(startup_id);
#endif
}

void
ewmh_process_net_window_user_time(Client *c)
{
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
			if ((int) ((int) c->user_time - (int) user_time) > 0)
				user_time = c->user_time;
		}
		XFree(time);
	} else {
		c->with.time = False;
		c->user_time = CurrentTime;
	}
}

void
ewmh_release_user_time_window(Client *c)
{
	Window win;

	if ((win = c->time_window) == None)
		return;

	c->time_window = None;
	XSelectInput(dpy, win, NoEventMask);
	XDeleteContext(dpy, win, context[ClientTimeWindow]);
	XDeleteContext(dpy, win, context[ClientAny]);
}

Window getrecwin(Client *c, Atom atom);

void
ewmh_process_net_window_user_time_window(Client *c)
{
	Window win;

	win = getrecwin(c, _XA_NET_WM_USER_TIME_WINDOW);
	if (win == c->time_window)
		return;
	ewmh_release_user_time_window(c);
	if (win == None)
		return;

	c->time_window = win;
	if (win != c->win) {
		XSelectInput(dpy, win, PropertyChangeMask);
		XSaveContext(dpy, win, context[ClientTimeWindow], (XPointer) c);
		XSaveContext(dpy, win, context[ClientAny], (XPointer) c);
	}
	ewmh_process_net_window_user_time(c);
}

void
ewmh_process_net_window_icon(Client *c)
{
	unsigned long n = 0;
	long *card;
	Pixmap *pixmap;
	ButtonImage *bi;

	if ((bi = getbutton(c)))
		return;
	if ((card = getcard(c->win, _XA_NET_WM_ICON, &n))) {
		if (n < 2 || n < 2 + card[0] * card[1])
			XFree(card);
		else if (createneticon(c, card, n)) {
			XPRINTF(c, "using _NET_WM_ICON for icon\n");
			return;
		}
	}
	if ((pixmap = getpixmaps(c->win, _XA_KWM_WIN_ICON, &n))) {
		if (n < 2)
			XFree(pixmap);
		else if (createkwmicon(c, pixmap, n)) {
			XPRINTF(c, "using KWM_WM_ICON for icon\n");
			return;
		}
	}
	if (createwmicon(c)) {
		XPRINTF(c, "using WM_HINTS icon for icon\n");
		return;
	}
	if (createappicon(c)) {
		XPRINTF(c, "using app icon for icon\n");
		return;
	}
}

void
ewmh_process_net_window_opacity(Client *c)
{
	long *opacity;
	unsigned long n = 0;

	if ((opacity = getcard(c->win, _XA_NET_WM_WINDOW_OPACITY, &n))) {
		c->opacity = opacity[0] & 0xffffffff;
		XChangeProperty(dpy, c->frame, _XA_NET_WM_WINDOW_OPACITY, XA_CARDINAL, 32,
				PropModeReplace, (unsigned char *) opacity, 1);
		XFree(opacity);
	} else {
		c->opacity = OPAQUE;
		XDeleteProperty(dpy, c->frame, _XA_NET_WM_WINDOW_OPACITY);
	}
}

Atom *getatom(Window win, Atom atom, unsigned long *nitems);

#define WIN_HINTS_SKIP_FOCUS      (1<<0)	/* "alt-tab" skips this win */
#define WIN_HINTS_SKIP_WINLIST    (1<<1)	/* do not show in window list */
#define WIN_HINTS_SKIP_TASKBAR    (1<<2)	/* do not show on taskbar */
#define WIN_HINTS_GROUP_TRANSIENT (1<<3)	/* Reserved - definition is unclear */
#define WIN_HINTS_FOCUS_ON_CLICK  (1<<4)	/* app only accepts focus if clicked */

/* additional from IceWM */

#define WIN_HINTS_DO_NOT_COVER	  (1<<5)	/* attempt not to cover this window */
#define WIN_HINTS_DOCK_HORIZONTAL (1<<6)	/* docked horizontally */

void
wmh_process_win_window_hints(Client *c)
{
	long *state;
	unsigned long n = 0;

	state = getcard(c->win, _XA_WIN_HINTS, &n);
	if (n > 0) {
		c->skip.focus = (state[0] & WIN_HINTS_SKIP_FOCUS) ? True : c->skip.focus;
		c->skip.winlist =
		    (state[0] & WIN_HINTS_SKIP_WINLIST) ? True : c->skip.winlist;
		c->skip.taskbar =
		    (state[0] & WIN_HINTS_SKIP_TASKBAR) ? True : c->skip.taskbar;
		c->skip.sloppy =
		    (state[0] & WIN_HINTS_FOCUS_ON_CLICK) ? True : c->skip.sloppy;
		c->is.grptrans =
		    (state[0] & WIN_HINTS_GROUP_TRANSIENT) ? True : c->is.grptrans;
		c->is.above =
		    (state[0] & WIN_HINTS_DO_NOT_COVER) ? True : c->is.above;
		XFree(state);
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
		if (c->transfor)
			c->wintype |= WTFLAG(WindowTypeDialog);
		else
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
		if (c->is.below)
			togglebelow(c);
		break;
	case 9:
	case WIN_LAYER_ABOVE_DOCK:
		if (!c->is.above)
			toggleabove(c);
		break;
	case 11:
	case WIN_LAYER_MENU:
		if (c->transfor)
			c->wintype |= WTFLAG(WindowTypeMenu);
		else
			c->wintype |= WTFLAG(WindowTypeToolbar);
		break;
	case 13:
	case WIN_LAYER_FULLSCREEN:
		if (!c->is.full)
			togglefull(c);
		break;
	case WIN_LAYER_ABOVE_ALL:
	default:
		c->wintype |= WTFLAG(WindowTypePopup);
		break;
	}
}

void
wmh_process_win_layer(Client *c)
{
	long *layer;
	unsigned long n;

	layer = getcard(c->win, _XA_WIN_LAYER, &n);
	if (n > 0) {
		wmh_process_layer(c, layer[0]);
		XFree(layer);
	}
}

void
wmh_process_win_state(Client *c)
{
	long *state = NULL;
	unsigned long n = 0;

	state = getcard(c->win, _XA_WIN_STATE, &n);
	if (n > 0) {
		wmh_process_state_mask(c, 0xffffffff, state[0]);
		XFree(state);
	}
}

void
wmh_process_win_window_state(Client *c)
{
	// wmh_process_win_window_hints(c);
	// wmh_process_win_layer(c);
	wmh_process_win_state(c);
}

void
ewmh_process_net_window_state(Client *c)
{
	Atom *state;
	unsigned long i, n = 0;

	wmh_process_win_window_state(c);

	if ((state = getatom(c->win, _XA_NET_WM_STATE, &n))) {
		for (i = 0; i < n; i++)
			ewmh_process_state_atom(c, state[i], _NET_WM_STATE_ADD);
		XFree(state);
	}
}

void
ewmh_process_net_window_sync_request_counter(Client *c)
{
	int format, status;
	long *data = NULL;
	unsigned long extra, nitems = 0;
	Atom real;

	/* ICCCM 2.0/4.1.9: Window managers will ignore any WM_PROTOCOLS properties they
	   find on icon windows. */
	if (!checkatom(c->win, _XA_WM_PROTOCOLS, _XA_NET_WM_SYNC_REQUEST))
		return;
	status = XGetWindowProperty(dpy, c->win, _XA_NET_WM_SYNC_REQUEST_COUNTER, 0L, 1L,
				    False, AnyPropertyType, &real, &format, &nitems,
				    &extra, (unsigned char **) &data);
	if (status == Success && nitems > 0) {
#ifdef SYNC
		XSyncAlarmAttributes aa;

		c->sync.counter = data[0];

		if (einfo[XsyncBase].have) {
			XSyncIntToValue(&c->sync.val, 0);
			XSyncSetCounter(dpy, c->sync.counter, c->sync.val);

			aa.trigger.counter = c->sync.counter;
			aa.trigger.wait_value = c->sync.val;
			aa.trigger.value_type = XSyncAbsolute;
			aa.trigger.test_type = XSyncPositiveTransition;
			aa.events = True;
			XSyncIntToValue(&aa.delta, 1);

			c->sync.alarm = XSyncCreateAlarm(dpy,
							 XSyncCACounter | XSyncCAValueType
							 | XSyncCAValue | XSyncCATestType
							 | XSyncCADelta | XSyncCAEvents,
							 &aa);

			if (c->sync.alarm) {
				XPRINTF(c, "allocated alarm 0x%08lx\n", c->sync.alarm);
				XSaveContext(dpy, c->sync.alarm, context[ClientAny],
					     (XPointer) c);
				XSaveContext(dpy, c->sync.alarm, context[ScreenContext],
					     (XPointer) scr);

			} else
				XPRINTF(c, "could not allocate alarm!\n");
		} else
			XPRINTF("SYNC extension not present\n");
#else
		XPRINTF("SYNC extension not supported\n");
#endif
	}
	if (data)
		XFree(data);
}

void
ewmh_update_net_window_fs_monitors(Client *c)
{
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
			PropModeReplace, (unsigned char *) mons, 4L);
}

void
ewmh_process_kde_net_window_type_override(Client *c)
{
	int format, status;
	long *data = NULL;
	unsigned long extra, nitems = 0;
	Atom real;

	status =
	    XGetWindowProperty(dpy, c->win, _XA_KDE_NET_WM_WINDOW_TYPE_OVERRIDE, 0L, 1L,
			       False, AnyPropertyType, &real, &format, &nitems, &extra,
			       (unsigned char **) &data);
	if (status == Success && real != None) {
		/* no decorations or functionality */
		c->is.bastard = True;
		c->is.floater = True;
		c->skip.skip = -1U;	/* skip everything */
		c->skip.sloppy = False;
		c->can.can = 0;	/* no functionality */
		c->has.has = 0;	/* no decorations */
		c->needs.has = 0;	/* no decorations */
	}
	if (data)
		XFree(data);
}

void
ewmh_update_kde_splash_progress()
{
	XEvent ev = { 0. };

	ev.xclient.display = dpy;
	ev.xclient.type = ClientMessage;
	ev.xclient.window = scr->root;
	ev.xclient.message_type = _XA_KDE_SPLASH_PROGRESS;
	ev.xclient.format = 8;
	strcpy(ev.xclient.data.b, "wm started");
	XSendEvent(dpy, scr->root, False, SubstructureNotifyMask, &ev);
}

void
ewmh_update_startup_notification(void)
{
	XEvent ev = { 0, };
	const char *id;
	size_t len;
	char *buf, *p;
	int l;

	if (!(id = getenv("DESKTOP_STARTUP_ID")))
		return;

	XChangeProperty(dpy, scr->selwin, _XA_NET_STARTUP_ID, _XA_UTF8_STRING, 8,
			PropModeReplace, (unsigned char *) id, strlen(id) + 1);

	len = strlen(id) + 13;
	buf = calloc(len + 1, sizeof(*buf));
	snprintf(buf, len, "remove: ID=%s", id);

	ev.type = ClientMessage;
	ev.xclient.message_type = _XA_NET_STARTUP_INFO_BEGIN;
	ev.xclient.display = dpy;
	ev.xclient.window = scr->selwin;
	ev.xclient.format = 8;

	p = buf;
	l = strlen(buf) + 1;

	while (l > 0) {
		memset(ev.xclient.data.b, 0, 20);
		memcpy(ev.xclient.data.b, p, l > 20 ? 20 : l);
		p += 20;
		l -= 20;
		/* just PropertyChange mask in the spec doesn't work :( */
		if (!XSendEvent(dpy, scr->root, False, StructureNotifyMask |
				SubstructureNotifyMask | SubstructureRedirectMask |
				PropertyChangeMask, &ev)) ;
		ev.xclient.message_type = _XA_NET_STARTUP_INFO;
	}
	XSync(dpy, False);
	free(buf);
}

#define OB_CONTROL_RECONFIG	1
#define OB_CONTROL_RESTART	2
#define OB_CONTROL_QUIT		3

Bool
clientmessage(XEvent *e)
{
	XClientMessageEvent *ev = &e->xclient;
	Client *c;
	Atom message_type = ev->message_type;

	if ((c = getmanaged(ev->window, ClientWindow))) {
		if (0) {
		} else if (message_type == _XA_NET_CLOSE_WINDOW) {
			if (c->can.close)
				killclient(c);
		} else if (message_type == _XA_NET_ACTIVE_WINDOW ||
			   message_type == _XA_WIN_FOCUS) {
			if (c->can.select) {
				c->is.icon = False;
				c->is.hidden = False;
				if (c->can.focus)
					focus(c);
				arrange(clientview(c));
			}
		} else if (message_type == _XA_NET_WM_STATE) {
			ewmh_process_state_atom(c, (Atom) ev->data.l[1], ev->data.l[0]);
			if (ev->data.l[2])
				ewmh_process_state_atom(c,
							(Atom) ev->data.l[2],
							ev->data.l[0]);
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
				if (c->can.tag)
					tagonly(c, index);
		} else if (message_type == _XA_NET_WM_DESKTOP_MASK ||
			   message_type == _XA_WIN_WORKSPACES) {
			unsigned index = ev->data.l[0];
			unsigned mask = ev->data.l[1], oldmask = 0;
			unsigned num = (scr->ntags + 31) >> 5;
			unsigned i, j;

			if (!c->can.tag)
				return False;
			if (0 > index || index >= num)
				return False;
			for (i = 0, j = index << 5; j < scr->ntags; i++, j++) {
				if (c->tags & (1ULL << j))
					oldmask |= (1 << i);
				if (mask & (1 << i))
					c->tags |= (1ULL << j);
				else
					c->tags &= ~(1ULL << j);
			}
			if (islost(c))
				for (i = 0, j = index << 5; j < scr->ntags; i++, j++)
					if (oldmask & (1 << i))
						c->tags |= (1ULL << j);
					else
						c->tags &= ~(1ULL << j);
			else {
				/* what toggletag does */
				if (c->is.managed)
					ewmh_update_net_window_desktop(c);
				drawclient(c);
				ewmh_update_net_window_extents(c);
				arrange(NULL);
			}
			/* TODO */
		} else if (message_type == _XA_WM_PROTOCOLS) {
			/* TODO */
		} else if (message_type == _XA_NET_WM_FULLSCREEN_MONITORS) {
			if (!c->can.full)
				return False;
			if (!configuremonitors(e, c))
				return False;
			XChangeProperty(dpy, c->win, _XA_NET_WM_FULLSCREEN_MONITORS,
					XA_CARDINAL, 32, PropModeReplace,
					(unsigned char *) &ev->data.l[0], 4L);
		} else if (message_type == _XA_NET_MOVERESIZE_WINDOW) {
			unsigned flags = (unsigned) ev->data.l[0];
			unsigned source = ((flags >> 12) & 0x0f);
			unsigned gravity = flags & 0xff;
			XConfigureRequestEvent cev = { 0, };

			if (!c->can.move && !c->can.size)
				return False;
			if (source != 0 && source != 1 && source != 2)
				return False;
			cev.value_mask = 0;
			if (c->can.move && (flags & (1 << 8))) {
				cev.value_mask |= CWX;
				cev.x = ev->data.l[1];
			}
			if (c->can.move && (flags & (1 << 9))) {
				cev.value_mask |= CWY;
				cev.y = ev->data.l[2];
			}
			if (c->can.size && (flags & (1 << 10))) {
				cev.value_mask |= CWWidth;
				cev.width = ev->data.l[3];
			}
			if (c->can.size && (flags & (1 << 11))) {
				cev.value_mask |= CWHeight;
				cev.height = ev->data.l[4];
			}
			if (gravity == 0)
				gravity = c->sh.win_gravity;
			XPRINTF("calling configureclient for _NET_WM_MOVERESIZE_WINDOW message\n");
			configureclient(c, (XEvent *) &cev, gravity);
		} else if (message_type == _XA_NET_WM_MOVERESIZE) {
			int x_root = (int) ev->data.l[0];
			int y_root = (int) ev->data.l[1];
			int direct = (int) ev->data.l[2];
			int button = (int) ev->data.l[3];
			int source = (int) ev->data.l[4];
			XButtonEvent bev;

			if (!c->can.move && !c->can.size)
				return False;
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
							&bev.subwindow, &bev.x_root,
							&bev.y_root, &bev.x, &bev.y,
							&bev.state);
			switch (direct) {
			case 0:	/* _NET_WM_MOVERESIZE_SIZE_TOPLEFT */
			case 1:	/* _NET_WM_MOVERESIZE_SIZE_TOP */
			case 2:	/* _NET_WM_MOVERESIZE_SIZE_TOPRIGHT */
			case 3:	/* _NET_WM_MOVERESIZE_SIZE_RIGHT */
			case 4:	/* _NET_WM_MOVERESIZE_SIZE_BOTTOMRIGHT */
			case 5:	/* _NET_WM_MOVERESIZE_SIZE_BOTTOM */
			case 6:	/* _NET_WM_MOVERESIZE_SIZE_BOTTOMLEFT */
			case 7:	/* _NET_WM_MOVERESIZE_SIZE_LEFT */
				if (!c->can.size || (!c->can.sizeh && !c->can.sizev))
					return False;
				mouseresize_from(c, direct, (XEvent *) &bev, False);
				break;
			case 8:	/* _NET_WM_MOVERESIZE_MOVE */
				if (!c->can.move)
					return False;
				mousemove_from(c, CursorEvery, (XEvent *) &bev, False);
				break;
			case 9:	/* _NET_WM_MOVERESIZE_SIZE_KEYBOARD */
				if (!c->can.size)
					return False;
				/* TODO */
				break;
			case 10:	/* _NET_WM_MOVERESIZE_MOVE_KEYBOARD */
				/* TODO */
				if (!c->can.move)
					return False;
				break;
			case 11:	/* _NET_WM_MOVERESIZE_CANCEL */
				/* intercepted while moving or resizing */
				break;
			}
		} else if (message_type == _XA_NET_RESTACK_WINDOW) {
			unsigned source = ev->data.l[0];
			Window sibling = ev->data.l[1];
			unsigned detail = ev->data.l[2];
			XConfigureRequestEvent cev;

			if (source != 0 && source != 1 && source != 2)
				return False;
			cev.value_mask = 0;
			cev.value_mask |= CWSibling;
			cev.above = sibling;
			cev.value_mask |= CWStackMode;
			cev.detail = detail;
			XPRINTF("calling configureclient for _NET_RESTACK_WINDOW message\n");
			configureclient(c, (XEvent *) &cev, c->sh.win_gravity);
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
			/* Ignore desktop geometry changes but change the property in
			   response */
			ewmh_update_net_desktop_geometry();
		} else if (message_type == _XA_NET_DESKTOP_VIEWPORT ||
			   message_type == _XA_WIN_AREA) {
			/* Ignore viewport changes but change the property in response. */
			ewmh_update_net_desktop_viewport();
		} else if (message_type == _XA_NET_CURRENT_DESKTOP ||
			   message_type == _XA_WIN_WORKSPACE) {
			int tag = ev->data.l[0];

			if (0 <= tag && tag < scr->ntags)
				view(selview(), tag);
		} else if (message_type == _XA_NET_SHOWING_DESKTOP) {
			if (!ev->data.l[0] != !scr->options.showdesk)
				toggleshowing();
		} else if (message_type == _XA_NET_RELOAD) {
			reload();
		} else if (message_type == _XA_NET_RESTART) {
			restart(NULL);
		} else if (message_type == _XA_NET_SHUTDOWN) {
			quit(NULL);
		} else if (message_type == _XA_MANAGER) {
			/* TODO */
		} else if (message_type == _XA_WM_PROTOCOLS) {
			if (0) {
			} else if (ev->data.l[0] == _XA_NET_WM_PING) {
				if ((c = getmanaged(ev->data.l[2], ClientWindow))) {
					c->is.pinging = 0;
					c->is.killing = 0;
				}
			}
		} else if (message_type == _XA_WM_COLORMAP_NOTIFY) {
			if (ev->data.l[0])
				; /* need timestamp, really */
			if (ev->data.l[1] == 1)
				scr->colormapnotified = True;
			if (ev->data.l[1] == 0)
				scr->colormapnotified = False;
		} else if (message_type == _XA_OB_CONTROL) {
			if (ev->data.l[0] == OB_CONTROL_RECONFIG)
				reload();
			else
			if (ev->data.l[0] == OB_CONTROL_RESTART)
				restart(NULL);
			else
			if (ev->data.l[0] == OB_CONTROL_QUIT)
				quit(NULL);
		}
	} else {
		if (0) {
		} else if (message_type == _XA_NET_REQUEST_FRAME_EXTENTS) {
			Window win = ev->window;
			unsigned int wintype;
			Bool title = True;
			Bool grips = True;
			int th, gh, hw, border = scr->style.border;
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
			hw = scr->style.fullgrips ? gh : 0;
			data[0] = border + hw;
			data[1] = border + hw;
			data[2] = border + th + hw;
			data[3] = border + gh + hw;
			XChangeProperty(dpy, win, _XA_NET_FRAME_EXTENTS, XA_CARDINAL, 32,
					PropModeReplace, (unsigned char *) &data, 4L);
		} else if (message_type == _XA_NET_STARTUP_INFO_BEGIN) {
#ifdef STARTUP_NOTIFICATION
			sn_display_process_event(sn_dpy, e);
#endif
		} else if (message_type == _XA_NET_STARTUP_INFO) {
#ifdef STARTUP_NOTIFICATION
			sn_display_process_event(sn_dpy, e);
#endif
		} else
			return False;
	}
	return True;
}

void
setopacity(Client *c, unsigned int opacity)
{
	long data = opacity, *card;
	unsigned long n = 0;

	/* if the client wants to set opacity, we cannot */
	if ((card = getcard(c->win, _XA_NET_WM_WINDOW_OPACITY, &n))) {
		data = card[0];
		XFree(card);
	} else if (!data)	/* should not be zero! */
		return;
	XChangeProperty(dpy, c->frame, _XA_NET_WM_WINDOW_OPACITY, XA_CARDINAL, 32,
			PropModeReplace, (unsigned char *) &data, 1L);
}

Atom *
getatom(Window win, Atom atom, unsigned long *nitems)
{
	int format, status;
	Atom *ret = NULL;
	unsigned long extra, num = 64;
	Atom real;

      try_harder:
	status = XGetWindowProperty(dpy, win, atom, 0L, num, False, XA_ATOM,
				    &real, &format, nitems, &extra,
				    (unsigned char **) &ret);
	if (status != Success) {
		*nitems = 0;
		return NULL;
	}
	if (extra) {
		num += ((extra + 1) >> 2);
		if (ret) {
			XFree(ret);
			ret = NULL;
		}
		goto try_harder;
	}
	/* don't return empty properties */
	if (*nitems == 0) {
		if (ret) {
			XFree(ret);
			ret = NULL;
		}
	}
	return ret;
}

long *
getcard(Window win, Atom atom, unsigned long *nitems)
{
	int format, status;
	long *ret = NULL;
	unsigned long extra, num = 64;
	Atom real;

      try_harder:
	status = XGetWindowProperty(dpy, win, atom, 0L, num, False, XA_CARDINAL,
				    &real, &format, nitems, &extra,
				    (unsigned char **) &ret);
	if (status != Success) {
		*nitems = 0;
		return NULL;
	}
	if (extra) {
		num += ((extra + 1) >> 2);
		if (ret) {
			XFree(ret);
			ret = NULL;
		}
		goto try_harder;
	}
	/* don't return empty properties */
	if (*nitems == 0) {
		if (ret) {
			XFree(ret);
			ret = NULL;
		}
	}
	return ret;
}

Pixmap *
getpixmaps(Window win, Atom atom, unsigned long *nitems)
{
	int format, status;
	Pixmap *ret = NULL;
	unsigned long extra, num = 64;
	Atom real;

      try_harder:
	status = XGetWindowProperty(dpy, win, atom, 0L, num, False, XA_PIXMAP,
				    &real, &format, nitems, &extra,
				    (unsigned char **) &ret);
	if (status != Success) {
		*nitems = 0;
		return NULL;
	}
	if (extra) {
		num += ((extra + 1) >> 2);
		if (ret) {
			XFree(ret);
			ret = NULL;
		}
		goto try_harder;
	}
	/* don't return empty properties */
	if (*nitems == 0) {
		if (ret) {
			XFree(ret);
			ret = NULL;
		}
	}
	return ret;
}

Window *
getwind(Window win, Atom atom, unsigned long *nitems)
{
	int format, status;
	Window *ret = NULL;
	unsigned long extra, num = 64;
	Atom real;

      try_harder:
	status = XGetWindowProperty(dpy, win, atom, 0L, num, False, XA_WINDOW,
				    &real, &format, nitems, &extra,
				    (unsigned char **) &ret);
	if (status != Success) {
		*nitems = 0;
		return NULL;
	}
	if (extra) {
		num += ((extra + 1) >> 2);
		if (ret) {
			XFree(ret);
			ret = NULL;
		}
		goto try_harder;
	}
	/* don't return empty properties */
	if (*nitems == 0) {
		if (ret) {
			XFree(ret);
			ret = NULL;
		}
	}
	return ret;
}

long *
gethints(Window win, Atom atom, unsigned long *nitems)
{
	int format, status;
	long *ret = NULL;
	unsigned long extra;
	Atom real;

	status = XGetWindowProperty(dpy, win, atom, 0L, 64L, False, XA_WM_SIZE_HINTS,
				    &real, &format, nitems, &extra,
				    (unsigned char **) &ret);
	if (status != Success) {
		*nitems = 0;
		return NULL;
	}
	XPRINTF("Format of WM_SIZE_HINTS is %d\n", format);
	/* don't return empty properties */
	if (*nitems == 0) {
		if (ret) {
			XFree(ret);
			ret = NULL;
		}
	}
	return ret;
}

pid_t
getnetpid(Client *c)
{
	long *card = NULL;
	unsigned long n = 0;
	Window win;
	pid_t pid = 0;

	if (((win = c->win)      &&
	     (card = getcard(win, _XA_NET_WM_PID, &n))) ||
	    ((win = c->leader)   && (win != c->win) &&
	     (card = getcard(win, _XA_NET_WM_PID, &n))) ||
	    ((win = c->session)  && (win != c->win) &&
	     (card = getcard(win, _XA_NET_WM_PID, &n))) ||
	    ((win = c->transfor) && (win != c->win) && (win != scr->root) &&
	     (card = getcard(win, _XA_NET_WM_PID, &n)))
	    ) {
		pid = *card;
		XFree(card);
	}
	return (pid);
}

Bool
getgrpclasshint(Client *c, XClassHint *ch)
{
	Window win;

	/* ICCCM 2.0/4.1.9: Window managers will ignore any WM_CLASS hints they find on
	   icon windows. */
	if (((win = c->leader) && XGetClassHint(dpy, win, ch)) ||
	    ((win = c->win)    && XGetClassHint(dpy, win, ch)))
		return True;
	return False;
}

Bool
getclasshint(Client *c, XClassHint *ch)
{
	Window win;

	/* ICCCM 2.0/4.1.9: Window managers will ignore any WM_CLASS properties
	   they find on icon windows. */
	if (((win = c->win)    && XGetClassHint(dpy, win, ch)) ||
	    ((win = c->leader) && XGetClassHint(dpy, win, ch)))
		return True;
	return False;
}

Bool
getcommand(Client *c, char ***v, int *n)
{
	Window win;

	/* ICCCM 2.0/4.1.9: Window managers will ignore any WM_COMMAND properties
	   they find on icon windows. */
	/* ICCCM 2.0/C.1.1: A client with multiple top-level windows should ensure
	   that exactly one of them has a WM_COMMAND with nonzero length.  Zero-length
	   WM_COMMAND properties can be used to reply to WM_SAVE_YOURSELF messages on
	   other top-level windows but will otherwise be ignored. */
	if (((win = c->win)     && XGetCommand(dpy, win, v, n)) ||
	    ((win = c->session) && XGetCommand(dpy, win, v, n)) ||
	    ((win = c->leader)  && XGetCommand(dpy, win, v, n)))
		return True;

	return False;
}

char *
getstartupid(Client *c)
{
	Window win;
	char *id = NULL;

	if (((win = c->win) &&
	     gettextprop(win, _XA_NET_STARTUP_ID, &id)) ||
	    ((win = c->leader) && win != c->win &&
	     gettextprop(win, _XA_NET_STARTUP_ID, &id)) ||
	    ((win = c->session) && win != c->win &&
	     gettextprop(win, _XA_NET_STARTUP_ID, &id))
	    ) {
		return (id);
	}
	return (id);
}

void
setstartupid(Client *c, const char *id)
{
	Window win = None;

	if (!win && c->session && c->win != c->session)
		win = c->session;
	if (!win && c->leader && c->win != c->leader)
		win = c->leader;
	if (!win)
		win = c->win;
	XChangeProperty(dpy, win, _XA_NET_STARTUP_ID, _XA_UTF8_STRING, 8, PropModeReplace,
			(unsigned char *) id, strlen(id) + 1);
}

char *
getclientmachine(Client *c)
{
	Window win;
	char *host = NULL;

	/* ICCCM 2.0/4.1.9: Window managers will ignore any WM_CLIENT_MACHINE properties
	   they find * on icon windows. */
	if (((win = c->win)     && gettextprop(win, XA_WM_CLIENT_MACHINE, &host)) ||
	    ((win = c->session) && gettextprop(win, XA_WM_CLIENT_MACHINE, &host)) ||
	    ((win = c->leader)  && gettextprop(win, XA_WM_CLIENT_MACHINE, &host)))
		return (host);

	return (host);
}

Window
getrecwin(Client *c, Atom atom)
{
	Window *wins = NULL, win = None;
	unsigned long n = 0;

	if ((wins = getwind(c->win, atom, &n))) {
		Window other = wins[0];

		XFree(wins);
		/* check recursive */
		if ((wins = getwind(other, atom, &n))) {
			Window again = wins[0];

			XFree(wins);
			win = (other == again) ? again : None;
		}
	}
	return (win);
}

Bool
checkatom(Window win, Atom bigatom, Atom smallatom)
{
	Atom *state;
	unsigned long i, n;
	Bool ret = False;

	if ((state = getatom(win, bigatom, &n))) {
		for (i = 0; i < n; i++)
			if (state[i] == smallatom) {
				ret = True;
				break;
			}
		XFree(state);
	}
	return ret;
}

unsigned int
getwintype(Window win)
{
	Atom *state;
	unsigned long i, j, n = 0;
	unsigned int ret = 0;
	long *layer;

	if ((state = getatom(win, _XA_NET_WM_WINDOW_TYPE, &n))) {
		for (i = 0; i < n; i++)
			for (j = WindowTypeDesk; j <= WindowTypeNormal; j++)
				if (state[i] == atom[j])
					ret |= WTFLAG(j);
		XFree(state);
	} else if ((layer = getcard(win, _XA_WIN_LAYER, &n))) {
		switch ((unsigned int) layer[0]) {
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
		case WIN_LAYER_FULLSCREEN:
		case WIN_LAYER_ABOVE_ALL:
		default:
			ret |= WTFLAG(WindowTypePopup);
			break;
		}
		XFree(layer);
	}
	if (ret == 0)
		ret = WTFLAG(WindowTypeNormal);
	return ret;
}

// Normally override-redirect: top to bottom
//
// WindowTypeDnd:-	override: window being dragged
// WindowTypeNotify:-	override: notification
// WindowTypeTooltip:-	override: tooltip
// WindowTypeDrop:-	override: drop down menu
// WindowTypePopup: -	override: popup menu
// WindowTypeCombo:-	override: combo box popup

// Normally managed: top to bottom
//
// WindowTypeSplash:	managed: splash display
// WindowTypeToolbar:-	managed: pinned toobar
// WindowTypeMenu:	managed: pinned menu
// WindowTypeDock:-	managed: dock or panel (but depends) on other settings
// WindowTypeDialog:	managed: dialog window
// WindowTypeUtil:	managed: persistent utility window
// WindowTypeNormal:	managed: normal windows
// WindowTypeDesk:-	managed: desktop

void
ewmh_process_net_window_type(Client *c)
{
	c->wintype = getwintype(c->win);

	if (!c->wintype) {
		/* window type is meant to replace MOTIF_WM_HINTS */
		mwmh_process_motif_wm_hints(c);
		/* When _NET_WM_WINDOW_TYPE is not set: */
		if (c->transfor)
			/* Managed windows with WM_TRANSIENT_FOR set MUST be taken as
			   _NET_WM_WINDOW_TYPE_DIALOG. */
			c->wintype |= WTFLAG(WindowTypeDialog);
		else
			/* Managed windows without WM_TRANSIENT_FOR set MUST be taken as
			   _NET_WM_WINDOW_TYPE_NORMAL. */
			c->wintype |= WTFLAG(WindowTypeNormal);
	}
	if (WTCHECK(c, WindowTypeDesk) ||
	    WTCHECK(c, WindowTypeSplash) ||
	    WTCHECK(c, WindowTypeDrop) ||
	    WTCHECK(c, WindowTypePopup) ||
	    WTCHECK(c, WindowTypeTooltip) ||
	    WTCHECK(c, WindowTypeNotify) ||
	    WTCHECK(c, WindowTypeCombo) || WTCHECK(c, WindowTypeDnd)) {
		c->is.bastard = True;
		c->skip.skip = -1U;	/* skip everything */
		c->can.can = 0;		/* no user functionality */
		c->has.has = 0;		/* no decorations */
		c->needs.has = 0;	/* no decorations */
		c->is.floater = True;
	} else if (WTCHECK(c, WindowTypeDialog) || WTCHECK(c, WindowTypeMenu)) {
		c->skip.arrange = True;
		c->is.floater = True;
		c->can.arrange = False;
		c->can.size = False;
		c->has.grips = False;
		// c->can.move = False;
	} else if (WTCHECK(c, WindowTypeToolbar) || WTCHECK(c, WindowTypeUtil)) {
		c->skip.arrange = True;
	} else if (WTCHECK(c, WindowTypeDock)) {
		c->is.bastard = True;
		c->skip.skip = -1U;	/* skip everything */
		c->skip.focus = False;	/* except focus */
		c->skip.sloppy = False; /* and sloppy focus */
		c->can.can = 0;		/* no user functionality */
		c->can.select = True;	/* except maybe selecting it */
		c->has.has = 0;		/* no decorations */
		c->needs.has = 0;	/* no decorations */
		c->is.floater = True;
		c->tags = ((1ULL << scr->ntags) - 1);
	}
}

Bool
checkwintype(Window win, int wintype)
{
	Atom *state;
	unsigned long i, n = 0;
	Bool ret = False;

	if ((state = getatom(win, _XA_NET_WM_WINDOW_TYPE, &n))) {
		for (i = 0; i < n; i++)
			if (state[i] == atom[wintype]) {
				ret = True;
				break;
			}
		XFree(state);
	} else if (wintype == WindowTypeNormal)
		ret = True;
	return ret;
}

static Bool
strut_overlap(long min1, long max1, long min2, long max2)
{
	long tmp;

	if (min1 > max1) {
		tmp = min1;
		min1 = max1;
		max1 = tmp;
	}
	if (min2 > max2) {
		tmp = min2;
		min2 = max2;
		max2 = tmp;
	}
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
getstrut(Client *c, Atom atom)
{
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

	if (!(prop = getcard(c->win, atom, &n)))
		return 0;
	if (n > 12)
		n = 12;
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

int
getstruts(Client *c)
{
	return (getstrut(c, _XA_NET_WM_STRUT_PARTIAL) || getstrut(c, _XA_NET_WM_STRUT));
}
