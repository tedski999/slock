/* time interval in seconds to sleep before looking for updates in the main loop */
#define SLEEPINTERVAL 1
#define PATH(name)    "/home/tedski999/.local/bin/blocks/"name

/* If interval of a block is set to 0, the block will only be updated once at startup.
 * If interval is set to a negative value, the block will never be updated in the main loop.
 * Set signal to 0 if signalling is not required for the block.
 * Keep signal for clickable blocks less than 10.
 * If multiple realtime signals are pending, then the lowest numbered signal is delivered first. */

/* pathu - path of the program whose output is to be used for status text
 * pathc - path of the program to be executed on clicks */
static Block blocks[] = {
/*    pathu                 pathc                            interval  signal */
	{ PATH("pacman.sh"),    PATH("pacman_button.sh"),        30,       7      },
	{ PATH("septs.sh"),     PATH("septs_button.sh"),         30,       6      },
	{ PATH("usage.sh"),     PATH("usage_button.sh"),         5,        5      },
	{ PATH("network.sh"),   PATH("network_button.sh"),       10,       4      },
	{ PATH("volume.sh"),    PATH("volume_button.sh"),        0,        3      },
	{ PATH("battery.sh"),   PATH("battery_button.sh"),       15,       2      },
	{ PATH("timedate.sh"),  PATH("timedate_button.sh"),      30,       1      },
	{ NULL }
};

static const char *delim =  " ";

