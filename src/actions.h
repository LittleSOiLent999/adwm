/* actions.c */

#ifndef __LOCAL_ACTIONS_H__
#define __LOCAL_ACTIONS_H__

void m_move(Client *c, XEvent *ev);
void m_nexttag(Client *c, XEvent *ev);
void m_prevtag(Client *c, XEvent *ev);
void m_resize(Client *c, XEvent *ev);
void m_shade(Client *c, XEvent *ev);
void m_spawn(Client *c, XEvent *ev);
void m_zoom(Client *c, XEvent *ev);

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

#endif				/* __LOCAL_ACTIONS_H__ */

