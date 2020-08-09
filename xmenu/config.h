static struct Config config = {
	/* font, separate different fonts with comma */
	.font = "xos4 Terminus:size=9",

	/* colors */
	.background_color = "#000000",
	.foreground_color = "#bbbbbb",
	.selbackground_color = "#800060",
	.selforeground_color = "#eeeeee",
	.separator_color = "#bbbbbb",
	.border_color = "#000000",

	/* sizes in pixels */
	.width_pixels = 130,        /* minimum width of a menu */
	.height_pixels = 25,        /* height of a single menu item */
	.border_pixels = 0,         /* menu border */
	.separator_pixels = 6,      /* space around separator */
	.gap_pixels = 0,            /* gap between menus */

	/*
	 * The variables below cannot be set by X resources.
	 * Their values must be less than .height_pixels.
	 */

	/* geometry of the right-pointing isoceles triangle for submenus */
	.triangle_width = 3,
	.triangle_height = 7,

	/* the icon size is equal to .height_pixels - .iconpadding * 2 */
	.iconpadding = 2,

	/* area around the icon, the triangle and the separator */
	.horzpadding = 8,
};
