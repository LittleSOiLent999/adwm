/* See COPYING file for copyright and license details. */

#include "adwm.h"
#include "ewmh.h"
#include "layout.h"
#include "tags.h"
#include "resource.h"
#include "config.h"
#include "save.h" /* verification */


void
save(void)
{
	AScreen *save_screen = scr;
	char resource[256] = { 0, }, value[256] = { 0, }, line[BUFSIZ] = { 0, };

	/* session GENERAL and VIEW DIRECTIVES */

	if (options.debug) {
		snprintf(line, sizeof(line), "Adwm*debug:\t\t%d\n", options.debug);
		XrmPutLineResource(&srdb, line);
	}
	if (options.useveil) {
		snprintf(line, sizeof(line), "Adwm*useveil:\t\t%d\n", options.useveil);
		XrmPutLineResource(&srdb, line);
	}
	if (!options.attachaside) {
		snprintf(line, sizeof(line), "Adwm*attachaside:\t\t%d\n", options.attachaside);
		XrmPutLineResource(&srdb, line);
	}
	if (options.command && COMMAND && strcmp(options.command, COMMAND)) {
		snprintf(line, sizeof(line), "Adwm*command:\t\t%s\n", options.command);
		XrmPutLineResource(&srdb, line);
	}
	if (options.command2) {
		snprintf(line, sizeof(line), "Adwm*command2:\t\t%s\n", options.command2);
		XrmPutLineResource(&srdb, line);
	}
	if (options.command3) {
		snprintf(line, sizeof(line), "Adwm*command3:\t\t%s\n", options.command3);
		XrmPutLineResource(&srdb, line);
	}
	if (options.menucommand) {
		snprintf(line, sizeof(line), "Adwm*menucommand:\t\t%s\n", options.menucommand);
		XrmPutLineResource(&srdb, line);
	}
	if (options.dectiled != DECORATETILED) {
		snprintf(line, sizeof(line), "Adwm*decoratetiled:\t\t%d\n", options.dectiled);
		XrmPutLineResource(&srdb, line);
	}
	if (options.decmax != DECORATEMAX) {
		snprintf(line, sizeof(line), "Adwm*decoratemax:\t\t%d\n", options.decmax);
		XrmPutLineResource(&srdb, line);
	}
	if (options.hidebastards) {
		snprintf(line, sizeof(line), "Adwm*hidebastards:\t\t%d\n", options.hidebastards);
		XrmPutLineResource(&srdb, line);
	}
	if (options.strutsactive) {
		snprintf(line, sizeof(line), "Adwm*strutsactive:\t\t%d\n", options.strutsactive);
		XrmPutLineResource(&srdb, line);
	}
	


	snprintf(value, sizeof(value), "%u", options.ntags);
	putsessionres("tags.number", value);

	for (scr = screens; scr < screens + nscr; scr++) {
		unsigned i;

		if (!scr->managed)
			continue;
		putscreenres("attachaside", scr->options.attachaside ? "1" : "0");
		if (scr->options.command)
			putscreenres("command", scr->options.command);
		if (scr->options.command2)
			putscreenres("command2", scr->options.command2);
		if (scr->options.command3)
			putscreenres("command3", scr->options.command3);
		if (scr->options.menucommand)
			putscreenres("menucommand", scr->options.menucommand);
		putscreenres("decoratetiled", scr->options.dectiled ? "1" : "0");
		snprintf(value, sizeof(value), "%d", scr->options.hidebastards);
		putscreenres("hidebastards", value);
		putscreenres("strutsactive", scr->options.strutsactive ? "1" : "0");
		putscreenres("autoroll", scr->options.autoroll ? "1" : "0");
		snprintf(value, sizeof(value), "%lu", scr->options.strutsdelay);
		putscreenres("strutsdelay", value);
		snprintf(value, sizeof(value), "%d", scr->options.focus);
		putscreenres("sloppy", value);
		snprintf(value, sizeof(value), "%d", scr->options.snap);
		putscreenres("snap", value);
		snprintf(value, sizeof(value), "%d", scr->options.dockpos);
		putscreenres("dock.position", value);
		snprintf(value, sizeof(value), "%d", scr->options.dockori);
		putscreenres("dock.orient", value);
		snprintf(value, sizeof(value), "%d", scr->options.dockmon);
		putscreenres("dock.monitor", value);
		snprintf(value, sizeof(value), "%d", scr->options.dragdist);
		putscreenres("dragdistance", value);
		snprintf(value, sizeof(value), "%f", scr->options.mwfact);
		putscreenres("mwfact", value);
		snprintf(value, sizeof(value), "%f", scr->options.mhfact);
		putscreenres("mhfact", value);
		snprintf(value, sizeof(value), "%d", scr->options.nmaster);
		putscreenres("nmaster", value);
		snprintf(value, sizeof(value), "%d", scr->options.ncolumns);
		putscreenres("ncolumns", value);
		snprintf(value, sizeof(value), "%c", scr->options.deflayout);
		putscreenres("ncolumns", value);

		for (i = 0; i < MAXTAGS; i++) {
			Tag *t = scr->tags + i;

			if (t->name) {
				snprintf(resource, sizeof(resource), "tags.name%u", i);
				putscreenres(resource, t->name);
			}
		}
		for (i = 0; i < MAXTAGS; i++) {
			// View *v = scr->views + i;
			// Layout *l;
		}

	}
	scr = save_screen;
}

