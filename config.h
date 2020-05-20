/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx	= 1;		/* border pixel of windows */
static const unsigned int gappx		= 0;		/* gaps between windows */
static const unsigned int snap		= 32;		/* snap pixel */
static const unsigned int systraypinning = 0;	/* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;	/* systray spacing */
static const int systraypinningfailfirst = 1;	/* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray		= 1;	 /* 0 means no systray */
static const int showbar			= 0;		/* 0 means no bar */
static const int topbar				= 0;		/* 0 means bottom bar */
static const char *fonts[]			= { "noto sans:size=11", "JoyPixels:size=10" };

static const char col_bg[]				= "#0c0c0c";
static const char col_fg[]				= "#eeeeee";
static const char col_gray_mid[]		= "#2a2a2a";
static const char *colors[][3]			= {
	/*						fg			bg		   border	*/
	[SchemeNorm]		= { col_fg,	col_bg, col_bg },
	[SchemeSel]			= { col_bg,	col_fg,	col_fg	},
	[SchemeStatus]		= { col_fg,	col_bg, "#000000" }, // Statusbar right
	[SchemeTagsSel]		= { col_fg,	col_bg, "#000000" }, // Tagbar left selected
	[SchemeTagsNorm]	= { "#aaaaaa",	col_bg, "#000000" }, // Tagbar left unselected
	[SchemeInfoSel]		= { col_fg,	col_bg, "#000000" }, // infobar middle	selected
	[SchemeInfoNorm]	= { col_fg,	col_bg, "#000000" }, // infobar middle	unselected
	[SchemeLayout]		= { col_fg,	col_bg, "#000000" },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class	  instance	  title		  tags mask		isfloating	 monitor */
	{ "Firefox",  NULL,		  NULL,		  1 << 8,		0,			 -1 },
};

/* layout(s) */
static const float mfact	 = 0.5635; /* factor of master area size [0.05..0.95] */
static const int nmaster	 = 1;	 /* number of clients in master area */
static const int resizehints = 0;	 /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol	  arrange function */
	{ "🮉🮁",	  tile },	 /* first entry is default */
	{ "🮥🮮",	  NULL },	 /* no layout function means floating behavior */
	{ "[M]",	  monocle },
	{ NULL,		  NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,						KEY,	  view,			  {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,			KEY,	  toggleview,	  {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,				KEY,	  tag,			  {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,	  toggletag,	  {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, NULL };
static const char *termcmd[]  = { "st", NULL };

static Key keys[] = {
	/* modifier				key			function		   argument */
	{ MODKEY,				XK_p,		spawn,			{.v = dmenucmd } },
	{ MODKEY,				XK_Return,	spawn,			{.v = termcmd } },
	{ MODKEY,				XK_i,		spawn,			{.v = (const char*[]){"/usr/lib/brave-bin/brave", "--new-window", NULL} } },
	{ MODKEY,				XK_y,		spawn,			{.v = (const char*[]){"/usr/lib/brave-bin/brave", "--new-window", "youtube.com", NULL} } },
	{ MODKEY,				XK_m,		spawn,			{.v = (const char*[]){"thunderbird", NULL} } },
	{ Mod1Mask|ControlMask,	XK_l,		spawn,			{.v = (const char*[]){"slock", NULL} } },
	{ 0,	XF86XK_MonBrightnessUp,		spawn,			{.v = (const char*[]){"brightnessctl", "set", "2%+", NULL} } },
	{ 0,	XF86XK_MonBrightnessDown,	spawn,			{.v = (const char*[]){"brightnessctl", "set", "2%-", NULL} } },
	{ MODKEY|ControlMask,	XK_e,		spawn,			SHCMD("dmenuunicode") },
	{ MODKEY|ControlMask,	XK_r,		spawn,			SHCMD("subreddits") },
	{ MODKEY|ControlMask,	XK_p,		spawn,			SHCMD("scrot '%Y-%m-%d-%H-%M_$wx$h.png' -e 'mv $f ~/Pictures/'") },
	{ ControlMask,	XF86XK_AudioMute,	spawn,			SHCMD("playerctl play-pause") },
	{ ControlMask,	XF86XK_AudioLowerVolume, spawn,		SHCMD("playerctl previous") },
	{ ControlMask,	XF86XK_AudioRaiseVolume, spawn,		SHCMD("playerctl next") },
	{ 0,	XF86XK_AudioRaiseVolume,	spawn,			SHCMD("change-volume 2%+") },
	{ 0,	XF86XK_AudioLowerVolume,	spawn,			SHCMD("change-volume 2%-") },
	{ MODKEY,				XK_b,		togglebar,		{0} },
	{ MODKEY|ControlMask,	XK_j,		rotatestack,	{.i = +1 } },
	{ MODKEY|ControlMask,	XK_k,		rotatestack,	{.i = -1 } },
	{ MODKEY,				XK_j,		focusstack,		{.i = +1 } },
	{ MODKEY,				XK_k,		focusstack,		{.i = -1 } },
	{ MODKEY|ShiftMask,		XK_j,		pushdown,		{0} },
	{ MODKEY|ShiftMask,		XK_k,		pushup,			{0} },
	{ MODKEY|ShiftMask,		XK_d,		incnmaster,		{.i = +1 } },
	{ MODKEY,				XK_d,		incnmaster,		{.i = -1 } },
	{ MODKEY,				XK_h,		setmfact,		{.f = -0.0125} },
	{ MODKEY,				XK_l,		setmfact,		{.f = +0.0125} },
	{ MODKEY|ShiftMask,		XK_Return,	zoom,			{0} },
	{ MODKEY,				XK_Tab,		view,			{0} },
	{ MODKEY|ShiftMask,		XK_c,		killclient,		{0} },
	{ MODKEY,				XK_f,		fullscreen,		{0} },
	{ MODKEY|ControlMask,	XK_comma,	cyclelayout,	{.i = -1 } },
	{ MODKEY|ControlMask,	XK_period,	cyclelayout,	{.i = +1 } },
	{ MODKEY,				XK_space,	togglefloating,	{0} },
	{ MODKEY,				XK_0,		view,			{.ui = ~0 } },
	{ MODKEY|ShiftMask,		XK_0,		tag,			{.ui = ~0 } },
	{ MODKEY,				XK_comma,	focusmon,		{.i = -1 } },
	{ MODKEY,				XK_period,	focusmon,		{.i = +1 } },
	{ MODKEY|ShiftMask,		XK_comma,	tagmon,			{.i = -1 } },
	{ MODKEY|ShiftMask,		XK_period,	tagmon,			{.i = +1 } },
	TAGKEYS(				XK_1,						0)
	TAGKEYS(				XK_2,						1)
	TAGKEYS(				XK_3,						2)
	TAGKEYS(				XK_4,						3)
	TAGKEYS(				XK_5,						4)
	TAGKEYS(				XK_6,						5)
	TAGKEYS(				XK_7,						6)
	TAGKEYS(				XK_8,						7)
	TAGKEYS(				XK_9,						8)
	{ MODKEY|ShiftMask,		XK_q,	   quit,			{0} },
	{ MODKEY|ControlMask|ShiftMask,	XK_q,	quit,		{1} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click			event mask	button		function		argument */
	{ ClkLtSymbol,		0,			Button1,	setlayout,		{0} },
	{ ClkLtSymbol,		0,			Button3,	setlayout,		{.v = &layouts[2]} },
	{ ClkWinTitle,		0,			Button2,	zoom,			{0} },
	{ ClkStatusText,	0,			Button2,	spawn,			{.v = termcmd } },
	{ ClkClientWin,		MODKEY,		Button1,	movemouse,		{0} },
	{ ClkClientWin,		MODKEY,		Button2,	togglefloating, {0} },
	{ ClkClientWin,		MODKEY,		Button3,	resizemouse,	{0} },
	{ ClkTagBar,		0,			Button1,	view,			{0} },
	{ ClkTagBar,		0,			Button3,	toggleview,		{0} },
	{ ClkTagBar,		MODKEY,		Button1,	tag,			{0} },
	{ ClkTagBar,		MODKEY,		Button3,	toggletag,		{0} },
};

