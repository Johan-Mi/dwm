/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx = 1;
static const unsigned int gappx = 0;
static const unsigned int snap = 16; /* snap pixel */
static const int swallowfloating = 0; /* 1 means swallow floating */
static const unsigned int systraypinning = 0; /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;
static const int systraypinningfailfirst = 1; /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray = 1;
static const int showbar = 0;
static const int topbar = 1;
static const char *fonts[] = { "noto sans:size=11", "JoyPixels:size=10" };

static const char col_bg[] = "#020202";
static const char col_fg[] = "#bbc2cf";
static const char *colors[][3] = {
	/*						fg		bg		border */
	[SchemeNorm]		= { col_fg, col_bg, col_bg },
	[SchemeSel]			= { col_bg, col_fg, col_fg },
	[SchemeStatus]		= { col_fg, col_bg, col_fg },
	[SchemeTagsSel]		= { col_fg, col_bg, col_fg },
	[SchemeTagsNorm]	= { col_fg, col_bg, col_fg },
	[SchemeInfoSel]		= { col_fg, col_bg, col_fg },
	[SchemeInfoNorm]	= { col_fg, col_bg, col_fg },
	[SchemeLayout]		= { col_fg, col_bg, col_fg },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ NULL, "Thunderbird", NULL,           1 << 7,    0,          0,           0,        -1 },
	{ NULL,     "Discord", NULL,           1 << 8,    0,          0,           0,        -1 },
	{ "st",      NULL,     NULL,           0,         0,          1,          -1,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         1,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact = 0.5125; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1; /* number of clients in master area */
static const int resizehints = 0; /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol		arrange function */
	{ "🮉 🮁",		tile },	 /* first entry is default */
	{ "###",		gaplessgrid },
	{ "🮥🮮 ",		NULL },	 /* no layout function means floating behavior */
	{ "[M]",		monocle },
	{ NULL,			NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,						KEY,  view,			  {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,			KEY,  toggleview,	  {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,				KEY,  tag,			  {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,  toggletag,	  {.ui = 1 << TAG} }, \
	{ MODKEY|Mod1Mask,				KEY,  viewtag,		  {.ui = 1 << TAG} },

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, NULL };
static const char *termcmd[] = { "st", NULL };

static Key keys[] = {
	/* modifier				key			function		   argument */
	{ MODKEY,				XK_p,		spawn,			{.v = dmenucmd } },
	{ MODKEY,				XK_Return,	spawn,			{.v = termcmd } },
	{ MODKEY,				XK_i,		spawn,			{.v = (const char*[]){"/usr/lib/brave-bin/brave", "--new-window", NULL} } },
	{ MODKEY,				XK_y,		spawn,			{.v = (const char*[]){"/usr/lib/brave-bin/brave", "--new-window", "youtube.com", NULL} } },
	{ MODKEY|ControlMask,	XK_y,		spawn,			{.v = (const char*[]){"st", "-e", "python", NULL} } },
	{ MODKEY,				XK_m,		spawn,			{.v = (const char*[]){"thunderbird", NULL} } },
	{ MODKEY|ControlMask,	XK_d,		spawn,			{.v = (const char*[]){"discord", NULL} } },
	{ Mod1Mask|ControlMask,	XK_l,		spawn,			{.v = (const char*[]){"slock", NULL} } },
	{ MODKEY,				XK_e,		spawn,			{.v = (const char*[]){"emacs", NULL} } },
	{ 0,	XF86XK_MonBrightnessUp,		spawn,			{.v = (const char*[]){"brightnessctl", "set", "2%+", NULL} } },
	{ 0,	XF86XK_MonBrightnessDown,	spawn,			{.v = (const char*[]){"brightnessctl", "set", "2%-", NULL} } },
	{ MODKEY|Mod1Mask,		XK_s,		spawn,			{.v = (const char*[]){"brightnessctl", "set", "1%+", NULL} } },
	{ MODKEY|Mod1Mask,		XK_a,		spawn,			{.v = (const char*[]){"brightnessctl", "set", "1%-", NULL} } },
	{ MODKEY|ControlMask,	XK_e,		spawn,			{.v = (const char*[]){"/home/johanmi/.local/bin/dmenuunicode", NULL} } },
	{ MODKEY|ControlMask,	XK_r,		spawn,			{.v = (const char*[]){"/home/johanmi/.local/bin/subreddits", NULL} } },
	{ MODKEY|ControlMask,	XK_p,		spawn,			{.v = (const char*[]){"scrot", "%Y-%m-%d-%H-%M_$wx$h.png", "-e", "mv $f ~/Pictures/", NULL} } },
	{ ControlMask,	XF86XK_AudioMute,	spawn,			{.v = (const char*[]){"playerctl", "play-pause", NULL} } },
	{ ControlMask,	XF86XK_AudioLowerVolume, spawn,		{.v = (const char*[]){"playerctl", "previous", NULL} } },
	{ ControlMask,	XF86XK_AudioRaiseVolume, spawn,		{.v = (const char*[]){"playerctl", "next", NULL} } },
	{ 0,	XF86XK_AudioRaiseVolume,	spawn,			{.v = (const char*[]){"/home/johanmi/.local/bin/change-volume", "2%+", NULL} } },
	{ 0,	XF86XK_AudioLowerVolume,	spawn,			{.v = (const char*[]){"/home/johanmi/.local/bin/change-volume", "2%-", NULL} } },
	{ MODKEY|Mod1Mask,		XK_w,		spawn,			{.v = (const char*[]){"/home/johanmi/.local/bin/change-volume", "1%+", NULL} } },
	{ MODKEY|Mod1Mask,		XK_q,		spawn,			{.v = (const char*[]){"/home/johanmi/.local/bin/change-volume", "1%-", NULL} } },
	{ MODKEY,				XK_x,		spawn,			{.v = (const char*[]){"xdotool", "mousemove", "675", "760", NULL} } },
	{ MODKEY|Mod1Mask,		XK_h,		spawn,			{.v = (const char*[]){"xdotool", "mousemove_relative", "--", "-10", "0", NULL} } },
	{ MODKEY|Mod1Mask,		XK_l,		spawn,			{.v = (const char*[]){"xdotool", "mousemove_relative", "10", "0", NULL} } },
	{ MODKEY|Mod1Mask,		XK_k,		spawn,			{.v = (const char*[]){"xdotool", "mousemove_relative", "--", "0", "-10", NULL} } },
	{ MODKEY|Mod1Mask,		XK_j,		spawn,			{.v = (const char*[]){"xdotool", "mousemove_relative", "0", "10", NULL} } },
	{ MODKEY|Mod1Mask,		XK_u,		spawn,			{.v = (const char*[]){"xdotool", "click", "--clearmodifiers", "1", NULL} } },
	{ MODKEY|Mod1Mask,		XK_i,		spawn,			{.v = (const char*[]){"xdotool", "click", "--clearmodifiers", "3", NULL} } },
	{ MODKEY|Mod1Mask,		XK_o,		spawn,			{.v = (const char*[]){"xdotool", "click", "--clearmodifiers", "2", NULL} } },
	{ MODKEY|Mod1Mask,		XK_y,		spawn,			{.v = (const char*[]){"xdotool", "click", "--clearmodifiers", "5", NULL} } },
	{ MODKEY|Mod1Mask,		XK_p,		spawn,			{.v = (const char*[]){"xdotool", "click", "--clearmodifiers", "4", NULL} } },
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
	{ MODKEY|ShiftMask,		XK_q,		quit,			{0} },
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

