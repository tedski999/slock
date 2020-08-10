/* user and group to drop privileges to */
static const char *user  = "nobody";
static const char *group = "nobody";

static const char *colorname[NUMCOLS] = {
	[INIT] =   "black",     /* after initialization */
	[INPUT] =  "#999999",   /* during input */
	[FAILED] = "#800000",   /* wrong password */
};

/* treat a cleared input like a wrong password (color) */
static const int failonclear = 0;

/* display config */
static const int blurRadius = 2;
static const int pixelSize = 30;
static const int inputXPos = 1;
static const int inputYPos = 33;

