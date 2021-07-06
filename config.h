/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx = 1;
static const unsigned int gappx = 4;
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
	[SchemeSel]			= { col_bg, col_fg, "#777777" },
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
	/* class instance title tags mask isfloating monitor */
	{ NULL,  NULL,    NULL, 0,        0,         -1 },
};

/* layout(s) */
static const float mfact = 0.5125; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1; /* number of clients in master area */
static const int resizehints = 0; /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol		arrange function */
	{ "🮉 🮁",		tile },	 /* first entry is default */
	{ "|M|",		centeredmaster },
	{ "###",		gaplessgrid },
	{ "🮥🮮 ",		NULL },	 /* no layout function means floating behavior */
	{ "[M]",		monocle },
	{ NULL,			NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,						KEY,	view,		{.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,			KEY,	toggleview,	{.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,				KEY,	tag,		{.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,	toggletag,	{.ui = 1 << TAG} }, \
	{ MODKEY|Mod1Mask,				KEY,	viewtag,	{.ui = 1 << TAG} },

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, NULL };
static const char *termcmd[] = { "st", NULL };

#define CMD(...) {.v = (const char*[]){__VA_ARGS__, NULL}}

static Key keys[] = {
	/* modifier				key			function		   argument */
	{ MODKEY,				XK_p,		spawn,			{.v = dmenucmd } },
	{ MODKEY,				XK_Return,	spawn,			{.v = termcmd } },
	{ MODKEY,				XK_i,		spawn,			CMD("brave-bin", "--new-window") },
	{ MODKEY,				XK_y,		spawn,			CMD("brave-bin", "--new-window", "youtube.com") },
	{ MODKEY|ControlMask,	XK_y,		spawn,			CMD("st", "-e", "python") },
	{ Mod1Mask|ControlMask,	XK_l,		spawn,			CMD("slock") },
	{ MODKEY|Mod1Mask,		XK_s,		spawn,			CMD("brightnessctl", "set", "1%+") },
	{ MODKEY|Mod1Mask,		XK_a,		spawn,			CMD("brightnessctl", "set", "1%-") },
	{ MODKEY|ControlMask,	XK_e,		spawn,			CMD("/home/johanmi/.local/bin/dmenuunicode") },
	{ MODKEY|ControlMask,	XK_r,		spawn,			CMD("/home/johanmi/.local/bin/subreddits") },
	{ MODKEY|ControlMask,	XK_p,		spawn,			CMD("scrot", "%Y-%m-%d-%H-%M-%S_$wx$h.png", "-e", "mv $f ~/Pictures/") },
	{ ControlMask,	XF86XK_AudioMute,	spawn,			CMD("playerctl", "play-pause") },
	{ ControlMask,	XF86XK_AudioLowerVolume, spawn,		CMD("playerctl", "previous") },
	{ ControlMask,	XF86XK_AudioRaiseVolume, spawn,		CMD("playerctl", "next") },
	{ MODKEY|Mod1Mask,		XK_w,		spawn,			CMD("/home/johanmi/.local/bin/change-volume", "1%+") },
	{ MODKEY|Mod1Mask,		XK_q,		spawn,			CMD("/home/johanmi/.local/bin/change-volume", "1%-") },
	{ MODKEY,				XK_x,		spawn,			CMD("xdotool", "mousemove", "675", "757") },
	{ MODKEY|Mod1Mask,		XK_h,		spawn,			CMD("xdotool", "mousemove_relative", "--", "-10", "0") },
	{ MODKEY|Mod1Mask,		XK_l,		spawn,			CMD("xdotool", "mousemove_relative", "10", "0") },
	{ MODKEY|Mod1Mask,		XK_k,		spawn,			CMD("xdotool", "mousemove_relative", "--", "0", "-10") },
	{ MODKEY|Mod1Mask,		XK_j,		spawn,			CMD("xdotool", "mousemove_relative", "0", "10") },
	{ MODKEY|Mod1Mask,		XK_u,		spawn,			CMD("xdotool", "click", "--clearmodifiers", "1") },
	{ MODKEY|Mod1Mask,		XK_i,		spawn,			CMD("xdotool", "click", "--clearmodifiers", "3") },
	{ MODKEY|Mod1Mask,		XK_o,		spawn,			CMD("xdotool", "click", "--clearmodifiers", "2") },
	{ MODKEY|Mod1Mask,		XK_y,		spawn,			CMD("xdotool", "click", "--clearmodifiers", "5") },
	{ MODKEY|Mod1Mask,		XK_p,		spawn,			CMD("xdotool", "click", "--clearmodifiers", "4") },
	{ ControlMask|Mod1Mask,	XK_Delete,	spawn,			CMD("mpv", "--fs", "/usr/local/etc/rickroll.mp4") },
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

#undef CMD

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

