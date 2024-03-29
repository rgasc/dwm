/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx = 2;         /* border pixel of windows */
static const unsigned int snap = 32;            /* snap pixel */
static const unsigned int gappih = 10;          /* horiz inner gap between windows */
static const unsigned int gappiv = 10;          /* vert inner gap between windows */
static const unsigned int gappoh = 10;          /* horiz outer gap between windows and screen edge */
static const unsigned int gappov = 10;          /* vert outer gap between windows and screen edge */
static int smartgaps = 1;                       /* 1 means no outer gap when there is only one window */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0; 	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int swallowfloating = 1;           /* 1 means swallow floating windows by default */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray = 1;               /* 0 means no systray */
static const int showbar = 1;                   /* 0 means no bar */
static const int topbar = 1;                    /* 0 means bottom bar */
static const char *fonts[] = { "Iosevka Extended:size=10" };
static const char colour1[] = "#181a1b";
static const char colour2[] = "#0c0d0e";
static const char colour3[] = "#c5c8c6";
static const char *colors[][3] = {
    /*                  fg          bg          border   */
    [SchemeNorm]    = { colour3,    colour1,    colour2 },
    [SchemeSel]     = { colour1,    colour3,    colour3 },
    [SchemeTitle]   = { colour3,    colour1,    colour1 },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class                    instance    title           tags mask   isfloating  isterminal  noswallow   monitor */
    { "St",                     NULL,       NULL,           0,          0,          1,          0,          -1 },
    { NULL,                     NULL,       "Event Tester", 0,          0,          0,          1,          -1 },
    { "Blueman-manager",        NULL,       NULL,           0,          1,          0,          0,          -1 },
    { "Nm-connection-editor",   NULL,       NULL,           0,          1,          0,          0,          -1 },
    { "SpeedCrunch",            NULL,       NULL,           0,          1,          0,          0,          -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",      tile },    /* first entry is default */
    { "[M]",      monocle },
    { "H[]",      deck },
    { "|M|",      centeredmaster },
    { ">M>",      centeredfloatingmaster },
    { "[@]",      spiral },
    { "[\\]",     dwindle },
    { ":::",      gaplessgrid },
    { "TTT",      bstack },
    { "===",      bstackhoriz },
    { "><>",      NULL },    /* no layout function means floating behavior */
    { NULL,       NULL },
    /* { "HHH",      grid }, */
    /* { "###",      nrowgrid }, */
    /* { "---",      horizgrid }, */
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} }, \


/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-i", NULL };
static const char *termcmd[]  = { "st", NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", "-e", "nvim", NULL };

#include <X11/XF86keysym.h>
#include "movestack.c"

static Key keys[] = {
    /* SPAWN */
    { MODKEY,               XK_d,       spawn,  {.v = dmenucmd } },
    { MODKEY,               XK_Return,  spawn,  {.v = termcmd } },
    { MODKEY,               XK_p,       spawn,  SHCMD("st -e htop") },
    { MODKEY,               XK_v,       spawn,  SHCMD("st -e pulsemixer") },
    { MODKEY|ShiftMask,     XK_v,       spawn,  SHCMD("noisetorch") },
    { MODKEY,               XK_w,       spawn,  SHCMD("$BROWSER") },
    { MODKEY,               XK_y,       spawn,  SHCMD("signal-desktop --use-tray-icon") },
    { MODKEY|ShiftMask,     XK_y,       spawn,  SHCMD("resize_signal") },
    { 0,                    XK_Print,   spawn,  SHCMD("maim -s -u | xclip -selection clipboard -t image/png") },
    { MODKEY|ShiftMask,     XK_x,       spawn,  SHCMD("slock") },
    { MODKEY,               XK_n,       spawn,  SHCMD("st -e zsh -c n") },
    { MODKEY|ShiftMask,     XK_n,       spawn,  SHCMD("st -e newsboat") },
    { MODKEY,               XK_z,       spawn,  SHCMD("hide_signal") },
    { MODKEY|ShiftMask,     XK_z,       spawn,  SHCMD("xclip -selection primary -i /dev/null && xclip -selection clipboard -i /dev/null") },
    { MODKEY|ShiftMask,     XK_e,       spawn,  SHCMD("exit_dmenu") },
    { MODKEY|ShiftMask,     XK_e,       spawn,  SHCMD("exit_dmenu") },

    /* FUNCTION KEYS */
    { 0,            XF86XK_Tools,               spawn,  SHCMD("xset dpms force off") },
    { 0,            XF86XK_MonBrightnessUp,     spawn,  SHCMD("xbacklight -inc 10 && kill -10 $(pidof slstatus)") },
    { 0,            XF86XK_MonBrightnessDown,   spawn,  SHCMD("xbacklight -dec 10 && kill -10 $(pidof slstatus)") },
    { ShiftMask,    XF86XK_MonBrightnessUp,     spawn,  SHCMD("xbacklight -set 100 && kill -10 $(pidof slstatus)") },
    { ShiftMask,    XF86XK_MonBrightnessDown,   spawn,  SHCMD("xbacklight -set 1 && kill -10 $(pidof slstatus)") },
    { 0,            XF86XK_AudioRaiseVolume,    spawn,  SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +5% && kill -10 $(pidof slstatus)") },
    { 0,            XF86XK_AudioLowerVolume,    spawn,  SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -5% && kill -10 $(pidof slstatus)") },
    { 0,            XF86XK_AudioMute,           spawn,  SHCMD("pactl set-sink-mute @DEFAULT_SINK@ toggle && kill -10 $(pidof slstatus)") },
    { 0,            XF86XK_AudioMicMute,        spawn,  SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },

    /* MANAGE WINDOWS */
    { MODKEY,                       XK_j,       focusstack,             {.i = +1 } },   /* move to next window */
    { MODKEY,                       XK_k,       focusstack,             {.i = -1 } },   /* move to previous window */
    { MODKEY,                       XK_Left,    focusdir,               {.i = 0 } },    /* focus left */
    { MODKEY,                       XK_Right,   focusdir,               {.i = 1 } },    /* focus right */
    { MODKEY,                       XK_Up,      focusdir,               {.i = 2 } },    /* focus up */
    { MODKEY,                       XK_Down,    focusdir,               {.i = 3 } },    /* focus down */
    { MODKEY|ShiftMask,             XK_equal,   incnmaster,             {.i = +1 } },   /* increase # of master windows */
    { MODKEY|ShiftMask,             XK_minus,   incnmaster,             {.i = -1 } },   /* decrease # of master windows */
    { MODKEY,                       XK_l,       setmfact,               {.f = +0.05} }, /* increase window width */
    { MODKEY,                       XK_h,       setmfact,               {.f = -0.05} }, /* decrease window width */
    { MODKEY|ShiftMask,             XK_h,       setcfact,               {.f = +0.25} }, /* increase cfact */
    { MODKEY|ShiftMask,             XK_l,       setcfact,               {.f = -0.25} }, /* decrease cfact */
    { MODKEY|ShiftMask,             XK_o,       setcfact,               {.f =  0.00} }, /* reset cfact */
    { MODKEY,                       XK_space,   togglecanfocusfloating, {0} },          /* toggle ability to focus floating */
    { MODKEY|ShiftMask,             XK_space,   togglefloating,         {0} },          /* make window float */
    { MODKEY,                       XK_f,       togglefullscr,          {0} },          /* actual fullscreen */
    { MODKEY,                       XK_s,       togglesticky,           {0} },          /* make window visible on all tags */
    { MODKEY|ShiftMask,             XK_Return,  zoom,                   {0} },          /* make window master */
    { MODKEY,                       XK_Tab,     view,                   {0} },          /* switch between tags */
    { MODKEY|ShiftMask,             XK_q,       killclient,             {0} },          /* close window */
    { MODKEY,                       XK_0,       view,                   {.ui = ~0 } },  /* show all windows */
    { MODKEY|ShiftMask,             XK_0,       tag,                    {.ui = ~0 } },  /* show window on all tags */
    { MODKEY,                       XK_period,  focusmon,               {.i = +1 } },   /* next monitor */
    { MODKEY,                       XK_comma,   focusmon,               {.i = -1 } },   /* previous monitor */
    { MODKEY|ShiftMask,             XK_period,  tagmon,                 {.i = +1 } },   /* move window to next monitor */
    { MODKEY|ShiftMask,             XK_comma,   tagmon,                 {.i = -1 } },   /* move window to previous monitor */
    { MODKEY|ControlMask|ShiftMask, XK_period,  tagallmon,              {.i = +1 } },   /* send all windows to next monitor */
    { MODKEY|ControlMask|ShiftMask, XK_comma,   tagallmon,              {.i = -1 } },   /* send all windows to previous monitor */
    { MODKEY|ControlMask,           XK_space,   focusmaster,            {0} },          /* focus on the master window */
    { MODKEY|ShiftMask,             XK_j,       movestack,              {.i = +1 } },   /* move window down the stack */
    { MODKEY|ShiftMask,             XK_k,       movestack,              {.i = -1 } },   /* move window up the stack */

    /* LAYOUTS */
    { MODKEY,           XK_t,   setlayout,  {.v = &layouts[0]} }, /* tile layout */
    { MODKEY,           XK_m,   setlayout,  {.v = &layouts[1]} }, /* monocle layout */
    { MODKEY,           XK_x,   setlayout,  {.v = &layouts[2]} }, /* deck layout */
    { MODKEY,           XK_c,   setlayout,  {.v = &layouts[3]} }, /* centered master layout */
    { MODKEY|ShiftMask, XK_c,   setlayout,  {.v = &layouts[4]} }, /* centered floating master layout */
    { MODKEY,           XK_u,   setlayout,  {.v = &layouts[5]} }, /* spiral layout */
    { MODKEY|ShiftMask, XK_u,   setlayout,  {.v = &layouts[6]} }, /* dwindle layout */
    { MODKEY,           XK_g,   setlayout,  {.v = &layouts[7]} }, /* gapless grid layout */
    { MODKEY,           XK_b,   setlayout,  {.v = &layouts[8]} }, /* bstack layout */
    { MODKEY|ShiftMask, XK_b,   setlayout,  {.v = &layouts[9]} }, /* bstackhoriz layout */
    /* { MODKEY,           XK_f,   setlayout,  {.v = &layouts[10]} }, /1* float layout *1/ */

    /* GAPS */
    { ALTKEY,              XK_g,      incrgaps,       {.i = +1 } },
    { ALTKEY|ShiftMask,    XK_g,      incrgaps,       {.i = -1 } },
    { ALTKEY,              XK_0,      defaultgaps,    {0} },
    { ALTKEY|ShiftMask,    XK_0,      togglegaps,     {0} },

    /* OTHER */
    { MODKEY|ControlMask|ShiftMask, XK_b,       togglebar,      {0} }, /* show/hide bar */
    { MODKEY|ShiftMask,             XK_r,       quit,           {1} }, /* restart dwm */
	{ MODKEY,                       XK_grave,   togglescratch,  {.v = scratchpadcmd } }, /* toggle scratchpad */

    /* TAGS */
    TAGKEYS(XK_1, 0)
    TAGKEYS(XK_2, 1)
    TAGKEYS(XK_3, 2)
    TAGKEYS(XK_4, 3)
    TAGKEYS(XK_5, 4)
    TAGKEYS(XK_6, 5)
    TAGKEYS(XK_7, 6)
    TAGKEYS(XK_8, 7)
    TAGKEYS(XK_9, 8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
    /* { ClkLtSymbol,          0,              Button1,        setlayout,      {0} }, */
    /* { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} }, */
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
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

/* signal definitions */
/* signum must be greater than 0 */
/* trigger signals using `xsetroot -name "fsignal:<signum>"` */
static Signal signals[] = {
    /* signum       function        argument  */
    { 1,            viewnext,       {0} },
    { 2,            viewprev,       {0} },
};
