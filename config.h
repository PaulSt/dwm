/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 10;       /* gap pixel between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#282828";
static const char col_gray2[]       = "#282828";
static const char col_gray3[]       = "#928374";
static const char col_gray4[]       = "#fbf1c7";
static const char col_cyan[]        = "#458588";
static const char col_gb_bg[]       = "#282828";
static const char col_gb_aqua[]     = "#8ec07c";
static const char col_gb_fg[]       = "#ebdbb2";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gb_fg, col_gb_bg, col_gb_bg},
	[SchemeSel]  = { col_gb_aqua, col_gb_bg,  col_gb_aqua},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0<<0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1<<0,            0,           -1 },
	{ "orgmode",  NULL,       NULL,       1<<4,            0,           -1 },
	{ "mutt",     NULL,       NULL,       1<<4,            0,           -1 },
	{ "calcurse", NULL,       NULL,       1<<4,            0,           -1 },
	{ "newsboat", NULL,       NULL,       1<<4,            0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *lockcmd[]  = { "slock", NULL };
static const char *volup[]  = { "amixer", "-c", "0", "set", "Master", "5%+", NULL };
static const char *voldo[]  = { "amixer", "-c", "0", "set", "Master", "5%-", NULL };
static const char *volmu[]  = { "amixer", "-c", "0", "set", "Master", "1+" , "toggle",  NULL };
static const char *brightinc[]  = { "xbacklight", "-inc", "5", NULL };
static const char *brightdec[]  = { "xbacklight", "-dec", "5", NULL };
/* runorraise commands, name on entry 6 */
static const char *firefoxcmd[] = { "firefox", NULL,NULL,NULL,NULL,NULL,NULL,"Firefox" };
static const char *calcmd[]  = { "st", "-c", "calcurse", "-e", "calcurse-sync",NULL, NULL, "calcurse" };
static const char *mailcmd[]  = { "st", "-c" , "mutt", "-e", "mutt", NULL,NULL, "mutt" };
static const char *newscmd[]  = { "st", "-c" , "newsboat", "-e", "newsboat",NULL, NULL, "newsboat" };
static const char *orgcmd[]  = { "st", "-c" , "orgmode", "-e", "vim", "/home/paul/Dropbox/orgmode/bist.org",NULL, "orgmode" };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
    { MODKEY,                       XK_v,      runorraise,     {.v = newscmd} },
    { MODKEY,                       XK_c,      runorraise,     {.v = calcmd} },
    { MODKEY,                       XK_x,      runorraise,     {.v = mailcmd} },
    { MODKEY,                       XK_y,      runorraise,     {.v = firefoxcmd} },
    { MODKEY,                       XK_a,      runorraise,     {.v = orgcmd} },
    { MODKEY,                       XK_Escape, spawn,          {.v = lockcmd} },
    { 0,            XF86XK_AudioRaiseVolume,   spawn,          {.v = volup} },
    { 0,            XF86XK_AudioLowerVolume,   spawn,          {.v = voldo} },
    { 0,            XF86XK_AudioMute,          spawn,          {.v = volmu} },
    { 0,            XF86XK_MonBrightnessUp,    spawn,          {.v = brightinc} },
    { 0,            XF86XK_MonBrightnessDown,  spawn,          {.v = brightdec} },
    { MODKEY,                       XK_b,      togglebar,      {0} },
    { MODKEY|ShiftMask,             XK_j,      rotatestack,    {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      rotatestack,    {.i = -1 } },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

