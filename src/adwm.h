/* enums */

#include <X11/cursorfont.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <X11/Xft/Xft.h>
#ifdef SYNC
#include <X11/extensions/sync.h>
#endif
#ifdef STARTUP_NOTIFICATION
#define SN_API_NOT_YET_FROZEN
#include <libsn/sn.h>
#endif

#ifdef IMLIB2
#include <Imlib2.h>
#endif

enum {
	Manager, Utf8String, WMProto, WMDelete, WMSaveYourself, WMState, WMChangeState,
	WMTakeFocus,
	ELayout, ESelTags, WMRestart, WMShutdown, DeskLayout,
	/* MWM/DTWM properties follow */
	WMDesktop, MWMBindings, MWMDefaultBindings, MWMMessages, MWMOffset, MWMHints, MWMMenu,
	MWMInfo, DTWorkspaceHints, DTWorkspacePresence, DTWorkspaceList, DTWorkspaceCurrent,
	DTWorkspaceInfo, DTWMHints, DTWMRequest, DTWMEmbedded, DTWMSaveHint,
	/* _WIN_PROTOCOLS following */
	WinAppState, WinAreaCount, WinArea, WinClientList, WinClientMoving,
	WinButtonProxy, WinExpandedSize, WinFocus, WinHints, WinIcons, WinLayer, WinMaxGeom,
	WinProtocols, WinResize, WinState, WinCheck, WinWorkarea, WinWorkCount, WinWorkNames,
	WinWorkspace, WinWorkspaces, SwmVroot,
	/* _NET_SUPPORTED following */
	ClientList, ActiveWindow, WindowDesk, WindowDeskMask, NumberOfDesk, DeskNames,
	CurDesk, WorkArea, DeskViewport, ShowingDesktop, DeskGeometry, DesksVisible,
	MonitorGeometry,
	DeskModes, DeskModeFloating, DeskModeTiled, DeskModeBottomTiled, DeskModeMonocle,
	DeskModeTopTiled, DeskModeLeftTiled,
	ClientListStacking, WindowOpacity, MoveResizeWindow, RestackWindow, WindowMoveResize,
	WindowExtents, HandledIcons, RequestFrameExt, VirtualRoots,
	WindowType, WindowTypeDesk, WindowTypeDock, WindowTypeToolbar, WindowTypeMenu,
	WindowTypeUtil, WindowTypeSplash, WindowTypeDialog, WindowTypeDrop,
	WindowTypePopup, WindowTypeTooltip, WindowTypeNotify, WindowTypeCombo,
	WindowTypeDnd, WindowTypeNormal,
	StrutPartial, Strut, WindowPid, WindowName, WindowNameVisible, WindowIconName,
	WindowIconNameVisible, WindowUserTime, UserTimeWindow, NetStartupId,
	StartupInfo, StartupInfoBegin,
	WindowSync, WindowCounter, WindowFsMonitors,
	WindowState, WindowStateModal, WindowStateSticky, WindowStateMaxV,
	WindowStateMaxH, WindowStateShaded, WindowStateNoTaskbar, WindowStateNoPager,
	WindowStateHidden, WindowStateFs, WindowStateAbove, WindowStateBelow,
	WindowStateAttn, WindowStateFocused, WindowStateFixed, WindowStateFloating,
	WindowStateFilled,
	WindowActions, WindowActionAbove, WindowActionBelow, WindowActionChangeDesk,
	WindowActionClose, WindowActionFs, WindowActionMaxH, WindowActionMaxV,
	WindowActionMin, WindowActionMove, WindowActionResize, WindowActionShade,
	WindowActionStick, WindowActionFloat, WindowActionFill,
	WMCheck, CloseWindow, WindowPing, Supported,
	SystemTrayWindows, WindowFrameStrut, WindowForSysTray, WindowTypeOverride,
	KdeSplashProgress, WindowChangeState,
	NATOMS
}; /* keep in sync with atomnames[] in ewmh.c */

#define WTFLAG(_type) (1<<((_type)-WindowTypeDesk))
#define WTTEST(_wintype, _type) (((_wintype) & WTFLAG(_type)) ? True : False)
#define WTCHECK(_client, _type) WTTEST((_client)->wintype, (_type))

#define _XA_MANAGER				atom[Manager]
#define _XA_UTF8_STRING				atom[Utf8String]
#define _XA_WM_PROTOCOLS			atom[WMProto]
#define _XA_WM_DELETE_WINDOW			atom[WMDelete]
#define _XA_WM_SAVE_YOURSELF			atom[WMSaveYourself]
#define _XA_WM_STATE				atom[WMState]
#define _XA_WM_CHANGE_STATE			atom[WMChangeState]
#define _XA_WM_TAKE_FOCUS			atom[WMTakeFocus]
#define _XA_ECHINUS_LAYOUT			atom[ELayout]
#define _XA_ECHINUS_SELTAGS			atom[ESelTags]
#define _XA_NET_RESTART				atom[WMRestart]
#define _XA_NET_SHUTDOWN			atom[WMShutdown]
#define _XA_NET_DESKTOP_LAYOUT			atom[DeskLayout]
/* MWM/DTWM properties follow */
#define _XA_WM_DESKTOP				atom[WMDesktop]
#define _XA_MOTIF_BINDINGS			atom[MWMBindings]
#define _XA_MOTIF_DEFAULT_BINDINGS		atom[MWMDefaultBindings]
#define _XA_MOTIF_WM_MESSAGES			atom[MWMMessages]
#define _XA_MOTIF_WM_OFFSET			atom[MWMOffset]
#define _XA_MOTIF_WM_HINTS			atom[MWMHints]
#define _XA_MOTIF_WM_MENU			atom[MWMMenu]
#define _XA_MOTIF_WM_INFO			atom[MWMInfo]
#define _XA_DT_WORKSPACE_HINTS			atom[DTWorkspaceHints]
#define _XA_DT_WORKSPACE_PRESENCE		atom[DTWorkspacePresence]
#define _XA_DT_WORKSPACE_LIST			atom[DTWorkspaceList]
#define _XA_DT_WORKSPACE_CURRENT		atom[DTWorkspaceCurrent]
#define _XA_DT_WORKSPACE_INFO			atom[DTWorkspaceInfo]
#define _XA_DT_WM_HINTS				atom[DTWMHints]
#define _XA_DT_WM_REQUEST			atom[DTWMRequest]
#define _XA_DT_WORKSPACE_EMBEDDED_CLIENTS	atom[DTWMEmbedded]
#define _XA_DT_WMSAVE_HINT			atom[DTWMSaveHint]
/* _WIN_PROTOCOLS following */
#define _XA_WIN_APP_STATE			atom[WinAppState]
#define _XA_WIN_AREA_COUNT			atom[WinAreaCount]
#define _XA_WIN_AREA				atom[WinArea]
#define _XA_WIN_CLIENT_LIST			atom[WinClientList]
#define _XA_WIN_CLIENT_MOVING			atom[WinClientMoving]
#define _XA_WIN_DESKTOP_BUTTON_PROXY		atom[WinButtonProxy]
#define _XA_WIN_EXPANDED_SIZE			atom[WinExpandedSize]
#define _XA_WIN_FOCUS				atom[WinFocus]
#define _XA_WIN_HINTS				atom[WinHints]
#define _XA_WIN_ICONS				atom[WinIcons]
#define _XA_WIN_LAYER				atom[WinLayer]
#define _XA_WIN_MAXIMIZED_GEOMETRY		atom[WinMaxGeom]
#define _XA_WIN_PROTOCOLS			atom[WinProtocols]
#define _XA_WIN_RESIZE				atom[WinResize]
#define _XA_WIN_STATE				atom[WinState]
#define _XA_WIN_SUPPORTING_WM_CHECK		atom[WinCheck]
#define _XA_WIN_WORKAREA			atom[WinWorkarea]
#define _XA_WIN_WORKSPACE_COUNT			atom[WinWorkCount]
#define _XA_WIN_WORKSPACE_NAMES			atom[WinWorkNames]
#define _XA_WIN_WORKSPACE			atom[WinWorkspace]
#define _XA_WIN_WORKSPACES			atom[WinWorkspaces]
#define _XA_SWM_VROOT				atom[SwmVroot]
/* _NET_SUPPORTED following */
#define _XA_NET_CLIENT_LIST			atom[ClientList]
#define _XA_NET_ACTIVE_WINDOW			atom[ActiveWindow]
#define _XA_NET_WM_DESKTOP			atom[WindowDesk]
#define _XA_NET_WM_DESKTOP_MASK			atom[WindowDeskMask]
#define _XA_NET_NUMBER_OF_DESKTOPS		atom[NumberOfDesk]
#define _XA_NET_DESKTOP_NAMES			atom[DeskNames]
#define _XA_NET_CURRENT_DESKTOP			atom[CurDesk]
#define _XA_NET_WORKAREA			atom[WorkArea]
#define _XA_NET_DESKTOP_VIEWPORT		atom[DeskViewport]
#define _XA_NET_SHOWING_DESKTOP			atom[ShowingDesktop]
#define _XA_NET_DESKTOP_GEOMETRY		atom[DeskGeometry]
#define _XA_NET_VISIBLE_DESKTOPS		atom[DesksVisible]
#define _XA_NET_MONITOR_GEOMETRY		atom[MonitorGeometry]

#define _XA_NET_DESKTOP_MODES			atom[DeskModes]
#define _XA_NET_DESKTOP_MODE_FLOATING		atom[DeskModeFloating]
#define _XA_NET_DESKTOP_MODE_TILED		atom[DeskModeTiled]
#define _XA_NET_DESKTOP_MODE_BOTTOM_TILED	atom[DeskModeBottomTiled]
#define _XA_NET_DESKTOP_MODE_MONOCLE		atom[DeskModeMonocle]
#define _XA_NET_DESKTOP_MODE_TOP_TILED		atom[DeskModeTopTiled]
#define _XA_NET_DESKTOP_MODE_LEFT_TILED		atom[DeskModeLeftTiled]

#define _XA_NET_CLIENT_LIST_STACKING		atom[ClientListStacking]
#define _XA_NET_WM_WINDOW_OPACITY		atom[WindowOpacity]
#define _XA_NET_MOVERESIZE_WINDOW		atom[MoveResizeWindow]
#define _XA_NET_RESTACK_WINDOW			atom[RestackWindow]
#define _XA_NET_WM_MOVERESIZE			atom[WindowMoveResize]
#define _XA_NET_FRAME_EXTENTS			atom[WindowExtents]
#define _XA_NET_WM_HANDLED_ICONS		atom[HandledIcons]
#define _XA_NET_REQUEST_FRAME_EXTENTS		atom[RequestFrameExt]
#define _XA_NET_VIRTUAL_ROOTS			atom[VirtualRoots]

#define _XA_NET_WM_WINDOW_TYPE			atom[WindowType]
#define _XA_NET_WM_WINDOW_TYPE_DESKTOP		atom[WindowTypeDesk]
#define _XA_NET_WM_WINDOW_TYPE_DOCK		atom[WindowTypeDock]
#define _XA_NET_WM_WINDOW_TYPE_TOOLBAR		atom[WindowTypeToolbar]
#define _XA_NET_WM_WINDOW_TYPE_MENU		atom[WindowTypeMenu]
#define _XA_NET_WM_WINDOW_TYPE_UTILITY		atom[WindowTypeUtil]
#define _XA_NET_WM_WINDOW_TYPE_SPLASH		atom[WindowTypeSplash]
#define _XA_NET_WM_WINDOW_TYPE_DIALOG		atom[WindowTypeDialog]
#define _XA_NET_WM_WINDOW_TYPE_DROPDOWN_MENU	atom[WindowTypeDrop]
#define _XA_NET_WM_WINDOW_TYPE_POPUP_MENU	atom[WindowTypePopup]
#define _XA_NET_WM_WINDOW_TYPE_TOOLTIP		atom[WindowTypeTooltip]
#define _XA_NET_WM_WINDOW_TYPE_NOTIFICATION	atom[WindowTypeNotify]
#define _XA_NET_WM_WINDOW_TYPE_COMBO		atom[WindowTypeCombo]
#define _XA_NET_WM_WINDOW_TYPE_DND		atom[WindowTypeDnd]
#define _XA_NET_WM_WINDOW_TYPE_NORMAL		atom[WindowTypeNormal]

#define _XA_NET_WM_STRUT_PARTIAL		atom[StrutPartial]
#define _XA_NET_WM_STRUT			atom[Strut]
#define _XA_NET_WM_PID				atom[WindowPid]
#define _XA_NET_WM_NAME				atom[WindowName]
#define _XA_NET_WM_VISIBLE_NAME			atom[WindowNameVisible]
#define _XA_NET_WM_ICON_NAME			atom[WindowIconName]
#define _XA_NET_WM_VISIBLE_ICON_NAME		atom[WindowIconNameVisible]
#define _XA_NET_WM_USER_TIME			atom[WindowUserTime]
#define _XA_NET_WM_USER_TIME_WINDOW		atom[UserTimeWindow]
#define _XA_NET_STARTUP_ID			atom[NetStartupId]
#define _XA_NET_STARTUP_INFO			atom[StartupInfo]
#define _XA_NET_STARTUP_INFO_BEGIN		atom[StartupInfoBegin]
#define _XA_NET_WM_SYNC_REQUEST			atom[WindowSync]
#define _XA_NET_WM_SYNC_REQUEST_COUNTER		atom[WindowCounter]
#define _XA_NET_WM_FULLSCREEN_MONITORS		atom[WindowFsMonitors]

#define _XA_NET_WM_STATE			atom[WindowState]
#define _XA_NET_WM_STATE_MODAL			atom[WindowStateModal]
#define _XA_NET_WM_STATE_STICKY			atom[WindowStateSticky]
#define _XA_NET_WM_STATE_MAXIMIZED_VERT		atom[WindowStateMaxV]
#define _XA_NET_WM_STATE_MAXIMIZED_HORZ		atom[WindowStateMaxH]
#define _XA_NET_WM_STATE_SHADED			atom[WindowStateShaded]
#define _XA_NET_WM_STATE_SKIP_TASKBAR		atom[WindowStateNoTaskbar]
#define _XA_NET_WM_STATE_SKIP_PAGER		atom[WindowStateNoPager]
#define _XA_NET_WM_STATE_HIDDEN			atom[WindowStateHidden]
#define _XA_NET_WM_STATE_FULLSCREEN		atom[WindowStateFs]
#define _XA_NET_WM_STATE_ABOVE			atom[WindowStateAbove]
#define _XA_NET_WM_STATE_BELOW			atom[WindowStateBelow]
#define _XA_NET_WM_STATE_DEMANDS_ATTENTION	atom[WindowStateAttn]
#define _XA_NET_WM_STATE_FOCUSED		atom[WindowStateFocused]
#define _XA_NET_WM_STATE_FIXED			atom[WindowStateFixed]
#define _XA_NET_WM_STATE_FLOATING		atom[WindowStateFloating]
#define _XA_NET_WM_STATE_FILLED			atom[WindowStateFilled]

#define _XA_NET_WM_ALLOWED_ACTIONS		atom[WindowActions]
#define _XA_NET_WM_ACTION_ABOVE			atom[WindowActionAbove]
#define _XA_NET_WM_ACTION_BELOW			atom[WindowActionBelow]
#define _XA_NET_WM_ACTION_CHANGE_DESKTOP	atom[WindowActionChangeDesk]
#define _XA_NET_WM_ACTION_CLOSE			atom[WindowActionClose]
#define _XA_NET_WM_ACTION_FULLSCREEN		atom[WindowActionFs]
#define _XA_NET_WM_ACTION_MAXIMIZE_HORZ		atom[WindowActionMaxH]
#define _XA_NET_WM_ACTION_MAXIMIZE_VERT		atom[WindowActionMaxV]
#define _XA_NET_WM_ACTION_MINIMIZE		atom[WindowActionMin]
#define _XA_NET_WM_ACTION_MOVE			atom[WindowActionMove]
#define _XA_NET_WM_ACTION_RESIZE		atom[WindowActionResize]
#define _XA_NET_WM_ACTION_SHADE			atom[WindowActionShade]
#define _XA_NET_WM_ACTION_STICK			atom[WindowActionStick]
#define _XA_NET_WM_ACTION_FLOAT			atom[WindowActionFloat]
#define _XA_NET_WM_ACTION_FILL			atom[WindowActionFill]

#define _XA_NET_SUPPORTING_WM_CHECK		atom[WMCheck]
#define _XA_NET_CLOSE_WINDOW			atom[CloseWindow]
#define _XA_NET_WM_PING				atom[WindowPing]
#define _XA_NET_SUPPORTED			atom[Supported]

#define _XA_KDE_NET_SYSTEM_TRAY_WINDOWS		atom[SystemTrayWindows]
#define _XA_KDE_NET_WM_FRAME_STRUT		atom[WindowFrameStrut]
#define _XA_KDE_NET_WM_SYSTEM_TRAY_WINDOW_FOR	atom[WindowForSysTray]
#define _XA_KDE_NET_WM_WINDOW_TYPE_OVERRIDE	atom[WindowTypeOverride]
#define _XA_KDE_SPLASH_PROGRESS			atom[KdeSplashProgress]
#define _XA_KDE_WM_CHANGE_STATE			atom[WindowChangeState]

enum { XrandrBase, XineramaBase, XsyncBase, BaseLast };  /* X11 extensions */
enum { LeftStrut, RightStrut, TopStrut, BotStrut, LastStrut }; /* ewmh struts */
enum { ColFG, ColBG, ColBorder, ColButton, ColLast };	/* colors */
enum { ClientWindow, ClientIcon, ClientTitle, ClientGrips, ClientFrame, ClientTimeWindow, ClientGroup,
       ClientTransFor, ClientTransForGroup, ClientLeader, ClientAny, SysTrayWindows,
       ClientPing, ClientDead, ClientSync, ScreenContext, PartLast }; /* client parts */
typedef enum { IconifyBtn, MaximizeBtn, CloseBtn, ShadeBtn, StickBtn, LHalfBtn, RHalfBtn,
	FillBtn, FloatBtn, SizeBtn, TitleTags, TitleName, TitleSep, LastElement,
	LastBtn = TitleTags } ElementType;
typedef enum { OnClientTitle, OnClientGrips, OnClientFrame, OnClientDock, OnClientWindow,
	OnClientIcon, OnRoot, LastOn } OnWhere;
typedef enum { ButtonImageDefault,
	ButtonImagePressed, ButtonImageToggledPressed,
	ButtonImageHover, ButtonImageFocus, ButtonImageUnfocus,
	ButtonImageToggledHover, ButtonImageToggledFocus, ButtonImageToggledUnfocus,
	ButtonImageDisabledHover, ButtonImageDisabledFocus, ButtonImageDisabledUnfocus,
	ButtonImageToggledDisabledHover, ButtonImageToggledDisabledFocus, ButtonImageToggledDisabledUnfocus,
	LastButtonImageType } ButtonImageType;
typedef enum { CauseDestroyed, CauseUnmapped, CauseReparented,
	       CauseQuitting, CauseSwitching, CauseRestarting } WithdrawCause;
typedef enum { ColSmartPlacement, RowSmartPlacement, MinOverlapPlacement,
	       UnderMousePlacement, CascadePlacement, RandomPlacement } WindowPlacement;
typedef enum { ModalModeless, ModalPrimary, ModalSystem, ModalGroup } Modality;
typedef enum { NoInputModel, PassiveInputModel, GloballyActiveModel, LocallyActiveModel } InputModel;
typedef enum { OrientLeft, OrientTop, OrientRight, OrientBottom, OrientLast } LayoutOrientation;
typedef enum { StrutsOn, StrutsOff, StrutsHide } StrutsPosition;
typedef enum { DockNone, DockEast, DockNorthEast, DockNorth, DockNorthWest, DockWest,
	DockSouthWest, DockSouth, DockSouthEast } DockPosition;
typedef enum { DockSideEast, DockSideNorth, DockSideWest, DockSideSouth } DockSide;
typedef enum { DockHorz, DockVert } DockOrient;
typedef enum { RelativeNone, RelativeNorthWest, RelativeNorth, RelativeNorthEast,
	RelativeWest, RelativeCenter, RelativeEast, RelativeSouthWest, RelativeSouth,
	RelativeSouthEast, RelativeStatic, RelativeNext, RelativePrev, RelativeLast }
	RelativeDirection;
typedef enum { SetFlagSetting, UnsetFlagSetting, ToggleFlagSetting } FlagSetting;
typedef enum { IncCount, DecCount, SetCount } ActionCount;
typedef enum { FocusClient, ActiveClient, PointerClient, AnyClient, AllClients, EveryClient } WhichClient;
typedef enum { IncludeIcons, ExcludeIcons, OnlyIcons } IconsIncluded;
enum { _NET_WM_ORIENTATION_HORZ, _NET_WM_ORIENTATION_VERT };
enum { _NET_WM_TOPLEFT, _NET_WM_TOPRIGHT, _NET_WM_BOTTOMRIGHT, _NET_WM_BOTTOMLEFT };
typedef enum { PatternSolid, PatternParent, PatternGradient, PatternPixmap } Pattern;
typedef enum { ResizeTiled, ResizeScaled, ResizeStretched } Resizing;
typedef enum { GradientDiagonal, GradientCrossDiagonal, GradientRectangle, GradientPyramid,
	GradientPipeCross, GradientElliptic, GradientMirrorHorizontal, GradientHorizontal,
	GradientSplitVertical, GradientVertical } Gradient;
typedef enum { ReliefRaised, ReliefFlat, ReliefSunken } Relief;
typedef enum { Bevel1, Bevel2 } Bevel;
typedef enum { JustifyLeft, JustifyCenter, JustifyRight } Justify;
enum { CurResizeTopLeft, CurResizeTop, CurResizeTopRight, CurResizeRight,
       CurResizeBottomRight, CurResizeBottom, CurResizeBottomLeft, CurResizeLeft,
       CurMove, CurNormal, CurLast };	    /* cursor */
enum { Clk2Focus, SloppyFloat, AllSloppy, SloppyRaise };    /* focus model */

typedef struct {
	Pattern pattern;		/* default solid */
	Gradient gradient;		/* default none */
	Relief relief;			/* default raised */
	Resizing resize;		/* default tiled */
	Bevel bevel;			/* default bevel1 */
	Bool interlaced;		/* default False */
	Bool border;			/* default False */
} Appearance;

typedef struct {
	const char *file;
	Pixmap pixmap, mask;
	int x, y;
	unsigned depth, width, height;
} AdwmPixmap;

typedef struct {
	Appearance appearance;		/* default 'flat solid' */
	XColor color;			/* color1 => color => default color */
	XColor colorTo;			/* color2 => colorTo => default color */
	XColor picColor;		/* picColor => color => default color */
	AdwmPixmap pixmap;		/* None */
	XftColor textColor;		/* textColor => black */
	unsigned textOpacity;		/* 0 */
	XftColor textShadowColor;	/* textShadowColor => default color */
	unsigned textShadowOpacity;	/* 0 */
	unsigned textShadowXOffset;	/* 0 */
	unsigned textShadowYOffset;	/* 0 */
	XColor borderColor;		/* borderColor => black */
	unsigned borderWidth;		/* borderWidth => 1 */
	XColor backgroundColor;		/* backgroundColor => color => default color */
	XColor foregroundColor;		/* foregroundColor => picColor => opposite color */
	unsigned opacity;		/* 0 */
	unsigned borders[4];		/* left, right, top, bottom pixmap borders */
} Texture;

typedef struct {
	XftFont *font;
	XGlyphInfo extents;
	unsigned ascent, descent, width, height;
} AdwmFont;

typedef struct {
	/* ordered for imlib DATA32 */
	unsigned blue:8;
	unsigned green:8;
	unsigned red:8;
	unsigned alpha:8;
} ARGB;

typedef struct {
	Bool shadow;
	XftColor color;
	unsigned transparency;
	int offset;
} TextShadow;

typedef struct {
	Pixmap pixmap;
	unsigned width, height;
} AdwmBitmap;

#define GIVE_FOCUS (1<<0)
#define TAKE_FOCUS (1<<1)

/* typedefs */
typedef struct {
	int x, y, w, h, b, g, n, s, th;
} LayoutArgs;

typedef struct {
	int x, y, w, h, b;
} Geometry;

typedef struct {
	int x, y, w, h, b, t, g;
} ClientGeometry;

typedef struct {
	int x, y, w, h;
} Workarea;

typedef struct Monitor Monitor;
struct Monitor {
	Workarea sc, wa;
	unsigned long struts[LastStrut];
	unsigned long long seltags;
	unsigned long long prevtags;
	Monitor *next;
	int mx, my;
	unsigned curtag;
	unsigned num, index;
	Window veil;
	struct {
		Workarea wa;
		DockPosition position;
		DockOrient orient;
	} dock;
	int row, col;			/* row and column in monitor layout */
};

typedef struct {
	void (*arrange) (Monitor *m);
	char symbol;
	int features;
	LayoutOrientation major;	/* overall orientation */
	LayoutOrientation minor;	/* master area orientation */
	WindowPlacement placement;	/* float placement policy */
} Layout;

typedef struct {
	Bool present, hovered;
	unsigned pressed;
	struct {
		int x, y, w, h, b;
	} g;
} ElementClient;

typedef struct Client Client;
struct Client {
	char *name;
	char *icon_name;
	char *startup_id;
	int monitor;			/* initial monitor */
	Geometry c, r, s;		/* current, restore, static */
	int th, gh;			/* title/grip height and width */
	int basew, baseh, incw, inch, maxw, maxh, minw, minh;
	int minax, maxax, minay, maxay, gravity;
	int ignoreunmap;
	long flags;
	int wintype;
	int winstate;
	Bool wasfloating;
	unsigned long long tags;
	int nonmodal;
	union {
		struct {
			unsigned taskbar:1;
			unsigned pager:1;
			unsigned winlist:1;
			unsigned cycle:1;
			unsigned focus:1;
			unsigned arrange:1;
			unsigned sloppy:1;
		};
		unsigned skip;
	} skip;
	union {
		struct {
			unsigned transient:1;
			unsigned grptrans:1;
			unsigned banned:1;
			unsigned max:1;
			unsigned floater:1;
			unsigned maxv:1;
			unsigned maxh:1;
			unsigned shaded:1;
			unsigned icon:1;
			unsigned fill:1;
			unsigned modal:2;
			unsigned above:1;
			unsigned below:1;
			unsigned attn:1;
			unsigned sticky:1;
			unsigned hidden:1;
			unsigned bastard:1;
			unsigned full:1;
			unsigned focused:1;
                        unsigned dockapp:1;
			unsigned moveresize:1;
			unsigned managed:1;
		};
		unsigned is;
	} is, was;
	union {
		struct {
			unsigned border:1;
			unsigned grips:1;
			unsigned title:1;
			struct {
				unsigned menu:1;
				unsigned min:1;
				unsigned max:1;
				unsigned close:1;
				unsigned size:1;
				unsigned shade:1;
				unsigned stick:1;
				unsigned fill:1;
				unsigned floats:1;
			} but __attribute__ ((packed));
		};
		unsigned has;
	} has;
	union {
		struct {
			unsigned struts:1;
			unsigned time:1;
		};
		unsigned with;
	} with;
	union {
		struct {
			unsigned move:1;
			unsigned size:1;
			unsigned sizev:1;
			unsigned sizeh:1;
			unsigned min:1;
			unsigned max:1;
			unsigned maxv:1;
			unsigned maxh:1;
			unsigned close:1;
			unsigned shade:1;
			unsigned stick:1;
			unsigned full:1;
			unsigned above:1;
			unsigned below:1;
			unsigned fill:1;
			unsigned fillh:1;
			unsigned fillv:1;
			unsigned floats:1;
			unsigned hide:1;
			unsigned tag:1;
			unsigned arrange:1;
			unsigned focus:2;
		};
		unsigned can;
	} can;
	Monitor *curmon;
	Client *next;
	Client *prev;
	Client *snext;
	Client *cnext;
	Client *fnext;
	Window win;
        Window icon;
	Window title;
	Window grips;
	Window frame;
	Window time_window;
	Window leader;
	Window transfor;
	ElementClient *element;
	Time user_time;
#ifdef SYNC
	struct {
		XID counter;
		Bool waiting;
		XSyncValue val;
		XSyncAlarm alarm;
		int w;
		int h;
	} sync;
#endif
#ifdef STARTUP_NOTIFICATION
	SnStartupSequence *seq;
#endif
};

typedef struct _CycleList CycleList;
struct _CycleList {
	Client *c;
	CycleList *next;
	CycleList *prev;
};

typedef struct Group Group;
struct Group {
	Window *members;
	unsigned count;
	int modal_transients;
};

typedef struct View {
	StrutsPosition barpos;		/* show struts? */
	Bool dectiled;			/* decorate tiled? */
	int nmaster;
	int ncolumns;
	double mwfact;			/* master width factor */
	double mhfact;			/* master height factor */
	LayoutOrientation major;	/* overall orientation */
	LayoutOrientation minor;	/* master area orientation */
	WindowPlacement placement;	/* float placement policy */
	Layout *layout;
	int index;
	int row, col;			/* row and column in desktop layout */
} View; /* per-tag settings */

typedef struct {
	unsigned x, y, w, h;
	struct {
		XGlyphInfo *extents;
		int ascent;
		int descent;
		int height;
		int width;
	} font[2];
	GC gc;
	struct {
		Pixmap pixmap;
		XftDraw *xft;
		int w;
		int h;
	} draw;
} DC;				/* draw context */

typedef struct {
#if defined IMLIB2 || defined XPM
	Pixmap pixmap, mask;
#endif
	Pixmap bitmap;
	Bool present;
	int x, y;
	unsigned w, h, b;

} ButtonImage;

typedef struct {
	ButtonImage *image;
	void (**action) (Client *, XEvent *);
} Element;

typedef struct {
	unsigned border;
	unsigned margin;
	unsigned outline;
	unsigned spacing;
	unsigned titleheight;
	unsigned gripsheight;
	unsigned gripswidth;
	unsigned opacity;
	char titlelayout[32];
	XftFont *font[2];
	unsigned drop[2];
	struct {
		unsigned long norm[ColLast];
		unsigned long sel[ColLast];
		XftColor *font[2];
		XftColor *shadow[2];
	} color;
} Style;

typedef struct _Key Key;
struct _Key {
	unsigned long mod;
	KeySym keysym;
	void (*func) (XEvent *, Key *);
	void (*stop) (XEvent *, Key *);
	Key *chain;
	Key *cnext;
	char *arg;
	RelativeDirection dir;
	Bool wrap;
	ActionCount act;
	FlagSetting set;
	WhichClient any;
	IconsIncluded ico;
	unsigned tag;
	CycleList *cycle, *where;
	unsigned num;
	Bool cyc;
}; /* keyboard shortcuts */

typedef struct AScreen AScreen;
struct AScreen {
	Bool managed;
	Window root;
	Window selwin;
	Client *clients;
	Monitor *monitors;
	int last;
	unsigned nmons;
	Client *stack;
	Client *clist;
	Client *flist;
	Bool showing_desktop;
	int screen;
	char **tags;
	unsigned ntags;
	Atom *dt_tags;
	View *views;
	Key **keys;
	unsigned nkeys;
	struct {
		int orient;		/* orientation */
		int rows, cols;		/* rows and cols (one can be zero) */
		int start;		/* starting corner */
	} layout;
	struct {
		int rows, cols;		/* rows and cols in desk/monitor layout */
	} d, m;
	int sh, sw;
	DC dc;
	Element element[LastElement];
	Style style;
#ifdef IMLIB2
	Imlib_Context context;
#else
	Visual *visual;
	Colormap colormap;
	unsigned int depth;
	Bool dither;
	unsigned int bpp;
	unsigned char *rctab; /* red color table */
	unsigned char *gctab; /* green color table */
	unsigned char *bctab; /* blue color table */
	XColor *colors;	/* colormap */
	int ncolors; /* number of colors in colormap */
	int cpc;
#endif
};

typedef struct {
	char *prop;
	char *tags;
	Bool isfloating;
	Bool hastitle;
	regex_t *propregex;
	regex_t *tagregex;
} Rule; /* window matching rules */

#ifdef STARTUP_NOTIFICATION
typedef struct Notify Notify;
struct Notify {
	Notify *next;
	SnStartupSequence *seq;
	Bool assigned;
};
#endif

typedef struct {
	void *handle;
	const char *name;
	const char *clas;
	void (*initrcfile)(void);
	void (*initconfig)(void);
	void (*initkeys)(void);
	void (*initstyle)(void);
	void (*deinitstyle)(void);
	void (*drawclient)(Client *);
} AdwmOperations;

typedef struct {
	void *handle;
	const char *name;
	void (*initlayout)(Monitor *m, View *v, char code);
	void (*addclient)(Client *c, Bool front);
	void (*delclient)(Client *c);
	void (*raise)(Client *c);
	void (*lower)(Client *c);
	void (*raiselower)(Client *c);
} LayoutOperations;

typedef struct {
	Bool attachaside;
	Bool dectiled;
	Bool decmax;
	Bool hidebastards;
	Bool autoroll;
	int focus;
	int snap;
	char command[255];
	DockPosition dockpos;
	DockOrient dockori;
	unsigned dragdist;
} Options;

/* ewmh.c */
Bool checkatom(Window win, Atom bigatom, Atom smallatom);
unsigned getwintype(Window win);
Bool checkwintype(Window win, int wintype);
Bool clientmessage(XEvent * e);
void ewmh_release_user_time_window(Client *c);
Atom *getatom(Window win, Atom atom, unsigned long *nitems);
long *getcard(Window win, Atom atom, unsigned long *nitems);
void initewmh(Window w);
void exitewmh(WithdrawCause cause);
void ewmh_add_client(Client *c);
void ewmh_del_client(Client *c, WithdrawCause cause);
void setopacity(Client * c, unsigned opacity);
int getstruts(Client * c);

void ewmh_process_net_desktop_names(void);
void ewmh_process_net_number_of_desktops(void);
void ewmh_process_net_showing_desktop(void);
void ewmh_process_net_desktop_layout(void);
void ewmh_update_kde_splash_progress(void);
void ewmh_update_net_active_window(void);
void ewmh_update_net_client_list(void);
void ewmh_update_net_current_desktop(void);
void ewmh_update_net_desktop_geometry(void);
void ewmh_update_net_desktop_modes(void);
void ewmh_update_net_number_of_desktops(void);
void ewmh_update_net_showing_desktop(void);
void ewmh_update_net_virtual_roots(void);
void ewmh_update_net_work_area(void);
void ewmh_update_net_desktop_layout(void);

void mwmh_process_motif_wm_hints(Client *);

Bool ewmh_process_net_window_desktop(Client *);
Bool ewmh_process_net_window_desktop_mask(Client *);

void ewmh_process_net_window_type(Client *);
void ewmh_process_kde_net_window_type_override(Client *);
void ewmh_process_net_startup_id(Client *);
void ewmh_process_net_window_state(Client *c);
void ewmh_process_net_window_sync_request_counter(Client *);
void ewmh_process_net_window_user_time(Client *);
void ewmh_process_net_window_user_time_window(Client *);
void ewmh_update_net_window_desktop(Client *);
void ewmh_update_net_window_extents(Client *);
void ewmh_update_net_window_fs_monitors(Client *);
void ewmh_update_net_window_state(Client *);
void ewmh_update_net_window_visible_icon_name(Client *);
void ewmh_update_net_window_visible_name(Client *);
void wmh_process_win_window_hints(Client *);
void wmh_process_win_layer(Client *);

/* main */
void inittag(unsigned i);
Monitor *clientmonitor(Client * c);
Monitor *curmonitor();
Monitor *selmonitor();
Monitor *nearmonitor();
void *ecalloc(size_t nmemb, size_t size);
void *emallocz(size_t size);
void *erealloc(void *ptr, size_t size);
void eprint(const char *errstr, ...);
const char *getresource(const char *resource, const char *defval);
Client *getclient(Window w, int part);
Monitor *getmonitor(int x, int y);
Bool gettextprop(Window w, Atom atom, char **text);
void getpointer(int *x, int *y);
void hide(Client *c);
void hideall(Monitor *m);
void iconify(Client *c);
void iconifyall(Monitor *m);
void setborder(int px);
void incborder(int px);
void decborder(int px);
void setmargin(int px);
void incmargin(int px);
void decmargin(int px);
Bool isvisible(Client *c, Monitor *m);
void focus(Client * c);
void focusicon(void);
void focusnext(Client *c);
void focusprev(Client *c);
void focusview(Monitor *m, int index);
AScreen *getscreen(Window win);
AScreen *geteventscr(XEvent *ev);
void killclient(Client *c);
void applygravity(Client *c, ClientGeometry *g, int gravity);
Bool configurerequest(XEvent * e);
void m_move(Client *c, XEvent *ev);
void m_resize(Client *c, XEvent *ev);
void pushtime(Time time);
void quit(const char *arg);
void restart(const char *arg);
void spawn(const char *arg);
void tag(Client *c, int index);
void taketo(Client *c, int index);
void togglestruts(Monitor *m, View *v);
void toggledectiled(Monitor *m, View *v);
void togglesticky(Client *c);
void togglemin(Client *c);
void togglepager(Client *c);
void toggletaskbar(Client *c);
void togglemodal(Client *c);
void togglemonitor(void);
void toggletag(Client *c, int index);
void toggleview(Monitor *cm, int index);
void toggleshowing(void);
void togglehidden(Client *c);
void view(int index);
void viewlefttag(void);
void viewprevtag(void);
void viewrighttag(void);
Bool selectionclear(XEvent *e);
void appendtag(void);
void rmlasttag(void);
void settags(unsigned numtags);

/* needed by layout.c */
void getworkarea(Monitor * m, Workarea *w);
void updategeom(Monitor *m);
extern Cursor cursor[CurLast];
extern int ebase[BaseLast];
Bool handle_event(XEvent *ev);
void reparentclient(Client *c, AScreen *new_scr, int x, int y);
Monitor *closestmonitor(int x, int y);
Bool newsize(Client *c, int w, int h, Time time);
void send_configurenotify(Client *c, Window above);
void ban(Client *c);
void unban(Client *c, Monitor *m);
extern Group window_stack;

/* layout.c */
void addclient(Client *c, Bool focusme, Bool raiseme);
void delclient(Client *c);
void tookfocus(Client *c);
Bool isfloating(Client *c, Monitor *m);
Bool enterclient(XEvent *e, Client *c);
Bool configureclient(XEvent *e, Client *c, int gravity);
Bool configuremonitors(XEvent *e, Client *c);
Client *nextdockapp(Client *c, Monitor *m);
Client *prevdockapp(Client *c, Monitor *m);
Client *nexttiled(Client *c, Monitor *m);
Client *prevtiled(Client *c, Monitor *m);
void restack_client(Client *c, int stack_mode, Client *sibling);
void toggleabove(Client *c);
void togglebelow(Client *c);
void arrange(Monitor * m);
void setlayout(const char *arg);
void raisetiled(Client *c);
void lowertiled(Client *c);
void raiseclient(Client *c);
void lowerclient(Client *c);
void raiselower(Client *c);
void setmwfact(Monitor *m, View *v, double factor);
void setnmaster(Monitor *m, View *v, int n);
void decnmaster(Monitor *m, View *v, int n);
void incnmaster(Monitor *m, View *v, int n);
Bool mousemove(Client *c, XEvent *e, Bool toggle);
Bool mouseresize_from(Client *c, int from, XEvent *e, Bool toggle);
Bool mouseresize(Client * c, XEvent *e, Bool toggle);
void moveresizekb(Client *c, int dx, int dy, int dw, int dh, int gravity);
void moveto(Client *c, RelativeDirection position);
void moveby(Client *c, RelativeDirection direction, int amount);
void snapto(Client *c, RelativeDirection direction);
void edgeto(Client *c, int direction);
void rotateview(Client *c);
void unrotateview(Client *c);
void rotatezone(Client *c);
void unrotatezone(Client *c);
void rotatewins(Client *c);
void unrotatewins(Client *c);
void togglefloating(Client *c);
void togglefill(Client *c);
void togglefull(Client *c);
void togglemax(Client *c);
void togglemaxv(Client *c);
void togglemaxh(Client *c);
void toggleshade(Client *c);
void zoom(Client *c);
void zoomfloat(Client *c);

/* parse.c */
void initrules(void);
void initkeys(void);
void parsekeys(const char *s, Key *spec);
void addchain(Key *chain);
void freechain(Key *chain);

void k_chain(XEvent *e, Key *k);
void k_zoom(XEvent *e, Key *k);
void k_killclient(XEvent *e, Key *k);
void k_moveresizekb(XEvent *e, Key *k);
void k_rotateview(XEvent *e, Key *k);
void k_unrotateview(XEvent *e, Key *k);
void k_rotatezone(XEvent *e, Key *k);
void k_unrotatezone(XEvent *e, Key *k);
void k_rotatewins(XEvent *e, Key *k);
void k_unrotatewins(XEvent *e, Key *k);
void k_viewprevtag(XEvent *e, Key *k);
void k_togglemonitor(XEvent *e, Key *k);
void k_appendtag(XEvent *e, Key *k);
void k_rmlasttag(XEvent *e, Key *k);
void k_raise(XEvent *e, Key *k);
void k_lower(XEvent *e, Key *k);
void k_raiselower(XEvent *e, Key *k);
void k_quit(XEvent *e, Key *k);
void k_restart(XEvent *e, Key *k);
void k_setmwfactor(XEvent *e, Key *k);
void k_setnmaster(XEvent *e, Key *k);
void k_setmargin(XEvent *e, Key *k);
void k_setborder(XEvent *e, Key *k);
void k_setfloating(XEvent *e, Key *k);
void k_setfill(XEvent *e, Key *k);
void k_setfull(XEvent *e, Key *k);
void k_setmax(XEvent *e, Key *k);
void k_setmaxv(XEvent *e, Key *k);
void k_setmaxh(XEvent *e, Key *k);
void k_setshade(XEvent *e, Key *k);
void k_sethidden(XEvent *e, Key *k);
void k_setmin(XEvent *e, Key *k);
void k_setabove(XEvent *e, Key *k);
void k_setbelow(XEvent *e, Key *k);
void k_setpager(XEvent *e, Key *k);
void k_settaskbar(XEvent *e, Key *k);
void k_setshowing(XEvent *e, Key *k);
void k_setstruts(XEvent *e, Key *k);
void k_setdectiled(XEvent *e, Key *k);
void k_moveto(XEvent *e, Key *k);
void k_snapto(XEvent *e, Key *k);
void k_edgeto(XEvent *e, Key *k);
void k_moveby(XEvent *e, Key *k);
void k_stop(XEvent *e, Key *k);
void k_focus(XEvent *e, Key *k);
void k_client(XEvent *e, Key *k);
void k_stack(XEvent *e, Key *k);
void k_group(XEvent *e, Key *k);
void k_tab(XEvent *e, Key *k);
void k_panel(XEvent *e, Key *k);
void k_dock(XEvent *e, Key *k);
void k_swap(XEvent *e, Key *k);
void k_toggletag(XEvent *e, Key *k);
void k_tag(XEvent *e, Key *k);
void k_focusview(XEvent *e, Key *k);
void k_toggleview(XEvent *e, Key *k);
void k_view(XEvent *e, Key *k);
void k_taketo(XEvent *e, Key *k);
void k_setlayout(XEvent *e, Key *k);
void k_spawn(XEvent *e, Key *k);

/* draw.c */
void drawclient(Client * c);
void deinitstyle();
void initstyle();

/* texture.c */

/* resource.c */
const char *readres(const char *name, const char *clas, const char *defval);
void getbitmap(const unsigned char *bits, int width, int height, AdwmBitmap * bitmap);
void getappearance(const char *descrip, Appearance *appear);
void getxcolor(const char *color, const char *defcol, XColor *xcol);
void getxftcolor(const char *color, const char *defcol, XftColor *xftcol);
void freexftcolor(XftColor *xftcol);
void getbool(const char *name, const char *clas, const char *true_val, Bool defval,
	     Bool *result);
void getfont(const char *font, const char *deffont, AdwmFont *afont);
void freefont(AdwmFont *afont);
void readtexture(const char *name, const char *clas, Texture *t, const char *defcol,
		 const char *oppcol);
void freetexture(Texture *t);
void getpixmap(const char *file, AdwmPixmap *p);
void getshadow(const char *descrip, TextShadow *shadow);


#define LENGTH(x)		(sizeof(x)/sizeof(*x))
#ifdef DEBUG
#define DPRINT			do { fprintf(stderr, "%s %s() %d\n",__FILE__,__func__, __LINE__); fflush(stderr); } while(0)
#define DPRINTF(args...)	do { fprintf(stderr, "%s %s():%d ", __FILE__,__func__, __LINE__); \
				     fprintf(stderr, args); fflush(stderr); } while(0)
#define CPRINTF(c,args...)	do { fprintf(stderr, "%s %s():%d [0x%08lx 0x%08lx 0x%08lx %-20s] ", __FILE__,__func__,__LINE__,(c)->frame,(c)->win,(c)->icon,(c)->name); \
				     fprintf(stderr, args); fflush(stderr); } while(0)
#define XPRINTF(args...)	do { } while(0)
#else
#define DPRINT			do { } while(0)
#define DPRINTF(args...)	do { } while(0)
#define CPRINTF(c,args...)	do { } while(0)
#define XPRINTF(args...)	do { } while(0)
#endif
#define DPRINTCLIENT(c) DPRINTF("%s: x: %d y: %d w: %d h: %d th: %d gh: %d f: %d b: %d m: %d\n", \
				    c->name, c->x, c->y, c->w, c->h, c->th, c->gh, c->skip.arrange, c->is.bastard, c->is.max)

#define OPAQUE			0xffffffff
#define RESNAME		       "adwm"
#define RESCLASS	       "Adwm"
#define STR(_s)			TOSTR(_s)
#define TOSTR(_s)		#_s
#define min(_a, _b)		((_a) < (_b) ? (_a) : (_b))
#define max(_a, _b)		((_a) > (_b) ? (_a) : (_b))

/* macros */
#define WINDOWMASK		(EnterWindowMask | LeaveWindowMask)
#define BUTTONMASK		(ButtonPressMask | ButtonReleaseMask)
#define CLEANMASK(mask)		(mask & ~(numlockmask | LockMask))
#define MOUSEMASK		(BUTTONMASK | PointerMotionMask)
#define CLIENTMASK	        (PropertyChangeMask | StructureNotifyMask)
#define CLIENTNOPROPAGATEMASK 	(BUTTONMASK | ButtonMotionMask)
#define FRAMEMASK               (MOUSEMASK | WINDOWMASK | SubstructureRedirectMask | SubstructureNotifyMask | FocusChangeMask)
#define MAPPINGMASK		(StructureNotifyMask | SubstructureRedirectMask | SubstructureNotifyMask | EnterWindowMask)
#define ROOTMASK		(BUTTONMASK | WINDOWMASK | MAPPINGMASK | FocusChangeMask)

/* globals */
extern Options options;
extern Atom atom[NATOMS];
extern Display *dpy;
extern AScreen *scr;
extern AScreen *screens;
extern AScreen *event_scr;
extern void (*actions[LastOn][5][2])(Client *, XEvent *);
extern Client *sel;
extern Client *gave;	/* gave focus last */
extern Client *took;	/* took focus last */
extern unsigned nscr;
extern unsigned nrules;
extern Rule **rules;
extern Layout layouts[];
extern unsigned modkey;
extern unsigned numlockmask;
extern XContext context[];
extern Time user_time;
extern Bool haveext[];
#ifdef STARTUP_NOTIFICATION
extern SnDisplay *sn_dpy;
extern SnMonitorContext *sn_ctx;
extern Notify *notifies;
#endif
extern XrmDatabase xresdb;
extern int cargc;
extern char **cargv;
