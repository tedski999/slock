// see LICENSE file for license details
#define _DEFAULT_SOURCE
#include <ctype.h>
#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <Imlib2.h>

static const char *user = "nobody", *group = "nobody";
static const char *hash = "$5$TyirlUoA$6lnYSGrwaBrXDgYXTJEBF9Icb5v82bRsh10UzTiT/86";
static const char *color = "#aaa";
static const int blur = 3, pixels = 30;

static void die(char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	exit(1);
}

int main(int argc, char **argv) {
	Display *dpy = XOpenDisplay(NULL);
	if (!dpy)
		die("slock: cannot open display\n");

	if (!crypt("", hash))
		die("slock: crypt: %s\n", strerror(errno));

	// drop privileges
	errno = 0; struct passwd *pwd = getpwnam(user);
	if (!pwd) die("slock: getpwnam %s: %s\n", user, errno ? strerror(errno) : "user entry not found");
	errno = 0; struct group *grp = getgrnam(group);
	if (!grp) die("slock: getgrnam %s: %s\n", group, errno ? strerror(errno) : "group entry not found");
	if (setgroups(0, NULL) < 0) die("slock: setgroups: %s\n", strerror(errno));
	if (setgid(grp->gr_gid) < 0) die("slock: setgid: %s\n", strerror(errno));
	if (setuid(pwd->pw_uid) < 0) die("slock: setuid: %s\n", strerror(errno));

	// create pixelated screenshot
	Screen *screen = ScreenOfDisplay(dpy, DefaultScreen(dpy));
	Imlib_Image image = imlib_create_image(screen->width, screen->height);
	imlib_context_set_image(image);
	imlib_context_set_display(dpy);
	imlib_context_set_visual(DefaultVisual(dpy, 0));
	imlib_context_set_drawable(RootWindow(dpy, XScreenNumberOfScreen(screen)));
	imlib_copy_drawable_to_image(0, 0, 0, screen->width, screen->height, 0, 0, 1);
	imlib_image_blur(blur);
	for (int y = 0; y < screen->height; y += pixels) {
		for (int x = 0; x < screen->width; x += pixels) {
			Imlib_Color p;
			imlib_image_query_pixel(x + pixels / 2, y + pixels / 2, &p);
			imlib_context_set_color(p.red, p.green, p.blue, 0xff);
			imlib_image_fill_rectangle(x, y, pixels, pixels);
		}
	}

	// lock all screens
	int windows_len = ScreenCount(dpy);
	Window windows[windows_len];
	XColor colors[windows_len];
	for (int i = 0; i < windows_len; i++) {
		Window root = RootWindow(dpy, i);
		int w = DisplayWidth(dpy, i), h = DisplayHeight(dpy, i), d = DefaultDepth(dpy, i);
		Visual *v = DefaultVisual(dpy, i);
		Colormap c = DefaultColormap(dpy, i);
		// apply pixelate screenshot to screen
		Pixmap bgmap = XCreatePixmap(dpy, root, w, h, d);
		imlib_context_set_image(image);
		imlib_context_set_display(dpy);
		imlib_context_set_visual(v);
		imlib_context_set_colormap(c);
		imlib_context_set_drawable(bgmap);
		imlib_render_image_on_drawable(0, 0);
		imlib_free_image();
		// initialise window
		windows[i] = XCreateWindow(dpy, root, 0, 0, w, h, 0, d, CopyFromParent, v, CWOverrideRedirect | CWBackPixel, &(XSetWindowAttributes) { .override_redirect = 1 });
		XSetWindowBackgroundPixmap(dpy, windows[i], bgmap);
		Pixmap pmap = XCreateBitmapFromData(dpy, windows[i], (char [8]) {0}, 8, 8);
		XAllocNamedColor(dpy, c, color, &colors[i], &(XColor) {0});
		Cursor cursor = XCreatePixmapCursor(dpy, pmap, pmap, &colors[i], &colors[i], 0, 0);
		XDefineCursor(dpy, windows[i], cursor);
		// grab mouse and keyboard
		int ptgrab = -1, kbgrab = -1;
		for (int attempts = 6; attempts >= 0 && (ptgrab != GrabSuccess || kbgrab != GrabSuccess); attempts++) {
			if (ptgrab != GrabSuccess)
				ptgrab = XGrabPointer(dpy, root, False, ButtonPressMask | ButtonReleaseMask | PointerMotionMask, GrabModeAsync, GrabModeAsync, None, cursor, CurrentTime);
			if (kbgrab != GrabSuccess)
				kbgrab = XGrabKeyboard(dpy, root, True, GrabModeAsync, GrabModeAsync, CurrentTime);
			if (attempts == 0 && (ptgrab != GrabSuccess || kbgrab != GrabSuccess))
				die("slock: unable to grab inputs for screen %d\n", i);
			usleep(100000);
		}
		// lock the screen
		XMapRaised(dpy, windows[i]);
		XSelectInput(dpy, root, SubstructureNotifyMask);
	}
	XSync(dpy, 0);

	// run post-lock command
	if (argc > 1) {
		switch (fork()) {
			case 0:
				if (!close(ConnectionNumber(dpy)))
					execvp(argv[1], argv + 1);
			case -1:
				die("slock: unable to execute post-lock command: %s\n", strerror(errno));
		}
	}

	// prompt for password
	XEvent ev;
	char buffer[256];
	int buffer_len = 0;
	int running = 1;
	while (running && !XNextEvent(dpy, &ev)) {
		if (ev.type == KeyPress) {
			// read keyboard input
			KeySym ksym;
			char input[sizeof buffer];
			int input_len = XLookupString(&ev.xkey, input, sizeof input, &ksym, 0);
			// disregard special keys
			if (IsFunctionKey(ksym) || IsMiscFunctionKey(ksym) || IsPFKey(ksym) || IsPrivateKeypadKey(ksym))
				continue;
			// handle keys
			switch (ksym) {
				case XK_Return:
					buffer[buffer_len] = '\0';
					char *input_hash = crypt(buffer, hash);
					running = !(input_hash && !strcmp(input_hash, hash));
					buffer_len = 0;
					break;
				case XK_BackSpace:
					buffer_len -= (buffer_len > 0);
					break;
				default:
					if (!iscntrl(input[0]) && buffer_len + input_len < sizeof buffer) {
						memcpy(buffer + buffer_len, input, input_len);
						buffer_len += input_len;
					}
			}
			// draw input
			for (int i = 0; i < windows_len; i++) {
				XClearWindow(dpy, windows[i]);
				GC gc = XCreateGC(dpy, windows[i], GCForeground, &(XGCValues) { .foreground = colors[i].pixel });
				for (int j = 0; j < buffer_len; j++)
					XFillRectangle(dpy, windows[i], gc, (1 + j * 3) * pixels, 1 * pixels, 2 * pixels, 2 * pixels);
				XFreeGC(dpy, gc);
			}
		} else {
			for (int i = 0; i < windows_len; i++)
				XRaiseWindow(dpy, windows[i]);
		}
	}

	return 0;
}
