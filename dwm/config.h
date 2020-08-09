/* See LICENSE file for copyright and license details. */

/* appearance */
#define FONT "xos4 Terminus"
static const unsigned int borderpx        = 0;   /* border pixel of windows */
static const unsigned int barpadding      = 6;   /* extra padding under bar */
static const unsigned int gappx           = 6;   /* gaps between windows */
static const unsigned int snap            = 16;  /* snap pixel */
static const unsigned int swallowfloating = 0;   /* 1 means swallow floating windows by default */
static const unsigned int tagwidth        = 30;  /* the size of each tag */
static const unsigned int tabwidth        = 300; /* the size of each clients tab */
static const unsigned int underlineWeight = 2;   /* size of line under text */
static const int showbar                  = 1;   /* 0 means no bar */
static const int topbar                   = 1;   /* 0 means bottom bar */
static const char *fonts[]                = { FONT };
static const char dmenu_lines[]           = "12";
static const char dmenu_border[]          = "0";
static char dmenumon[]                    = "0";

/* colors */
static const char col_black[]        = "#000000";
static const char col_selected[]     = "#111111";
static const char col_underline[]    = "#800060";
static const char col_lighttext[]    = "#eeeeee";
static const char col_darktext[]     = "#bbbbbb";
static const char col_dimtext[]      = "#888888";
static const unsigned int alp_clear  = 0x00U;
static const unsigned int alp_trans  = 0xA0U;
static const unsigned int alp_opaque = 0xFFU;
static const char *colors[][3]      = {
	/*                   fg             bg            border       */
	[SchemeNorm]     = { col_darktext,  col_black,    col_black    },
	[SchemeSel]      = { col_lighttext, col_selected, col_underline},
	[SchemeHid]      = { col_dimtext,   col_black,    col_black    },
	[SchemeStatNorm] = { col_lighttext, col_selected, col_underline},
	[SchemeStatDim]  = { col_dimtext,   col_selected, col_underline},
	[SchemeStatGood] = { "#00ff00",     col_selected, col_underline},
	[SchemeStatWarn] = { "#ff0000",     col_selected, col_underline},
};
static const unsigned int alphas[][3] = {
	/*                   fg          bg          border     */
	[SchemeNorm]     = { alp_opaque, alp_trans, alp_trans  },
	[SchemeSel]      = { alp_opaque, alp_trans, alp_opaque },
	[SchemeHid]      = { alp_opaque, alp_trans, alp_trans  },
	[SchemeStatNorm] = { alp_opaque, alp_trans, alp_opaque },
	[SchemeStatDim]  = { alp_opaque, alp_trans, alp_opaque },
	[SchemeStatGood] = { alp_opaque, alp_trans, alp_opaque },
	[SchemeStatWarn] = { alp_opaque, alp_trans, alp_opaque },
};

/* tagging */
static const char *tags[] = { "◯", "‹›", "🔍", "🗪", "🎧" };

static const Rule rules[] = {
	/* class            instance  title                  tags mask   isfloating  isterminal  noswallow  iscentered  monitor */
	{ "centered-term",  NULL,     NULL,                  0,          1,          1,          1,         1,         -1 },
	{ "popup-term",     NULL,     NULL,                  0,          1,          1,          1,         0,         -1 },
	{ "st-256color",    NULL,     NULL,                  0,          0,          1,          0,         0,         -1 },
	{ NULL,             NULL,     "Picture-in-Picture",  0,          1,          0,          1,         0,         -1 },
	{ NULL,             NULL,     "Event Tester",        0,          0,          0,          1,         0,         -1 },
	{ NULL,             NULL,     "XBindKey: Hit a key", 0,          0,          0,          1,         0,         -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile        }, /* first entry is default */
	{ "><>",      NULL        }, /* no layout function means floating behavior */
	{ "[M]",      monocle     },
	{ "TTT",      bstack      },
	{ "###",      gaplessgrid },
};

/* key definitions */
#define ALTKEY Mod1Mask
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
static const char *dmenucmd[]           = { "dmenu_run", "-c", "-bw", dmenu_border, "-fn", FONT, "-nf", col_darktext, "-sf", col_lighttext, "-nb", col_black, "-sb", col_underline, "-l", dmenu_lines, "-m", dmenumon, NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */

	/* layouts */
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY|ShiftMask,             XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_l,      setmfact,       {.f = +0.05} },

	/* navigation */
	{ ALTKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_h,      shiftview,      {.i = -1 } },
	{ MODKEY,                       XK_l,      shiftview,      {.i = +1 } },
	{ MODKEY,                       XK_equal,  incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_minus,  incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

	/* client manipulation */
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_q,      killunsel,      {0} },

	/* tagging */
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_s,      togglesticky,   {0} },

	/* system control */
	{ ALTKEY,                       XK_space,  spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask|ControlMask, XK_Escape, quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click         event mask  button   function        argument */
	{ ClkLtSymbol,   0,          Button1, setlayout,      {0} },
	{ ClkWinTitle,   0,          Button1, togglewin,      {0} },
	{ ClkWinTitle,   0,          Button3, killwin,        {0} },
	{ ClkStatusText, 0,          Button1, sigdwmblocks,   {.i = 1} },
	{ ClkStatusText, 0,          Button2, sigdwmblocks,   {.i = 2} },
	{ ClkStatusText, 0,          Button3, sigdwmblocks,   {.i = 3} },
	{ ClkClientWin,  MODKEY,     Button1, movemouse,      {0} },
	{ ClkClientWin,  MODKEY,     Button2, togglefloating, {0} },
	{ ClkClientWin,  MODKEY,     Button3, resizemouse,    {0} },
	{ ClkTagBar,     0,          Button1, view,           {0} },
	{ ClkTagBar,     ShiftMask,  Button2, shiftview,      {.i = -1 } },
	{ ClkTagBar,     0,          Button2, shiftview,      {.i = +1 } },
	{ ClkTagBar,     0,          Button3, toggleview,     {0} },
};
