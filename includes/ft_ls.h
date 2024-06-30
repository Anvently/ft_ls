
#include <stdbool.h>

enum	SORT_BY {SORT_BY_NONE = -1,
				SORT_BY_ASCII,
				SORT_BY_ALPHA,
				SORT_BY_TIME,
				SORT_BY_SIZE};

enum	TIME_BY {TIME_BY_MTIME = 0, //Modification time
				TIME_BY_CTIME, //Change time
				TIME_BY_ATIME, //Acces time
				TIME_BY_BTIME}; //Creation time

enum	COLORIZE {COLOR_NONE = -1,
				COLOR_AUTO, //Colorize if stdout is a tty
				COLOR_ALWAYS}; //Always colorize

enum	FORMAT_BY {
			FORMAT_BY_COLUMN = 0, //Compute column width
			FORMAT_BY_LINE //Limit column width
};

enum	FILTER_FILE {
			FILTER_DEFAULT = 0,
			FILTER_SPECIAL,
			FILTER_ALL
};

typedef struct s_options
{
	bool				long_listing;
	bool				human_readable;
	bool				inode;
	enum FORMAT_BY		format_by;
	int					column_max_width;
	enum SORT_BY		sort_by;
	bool				sort_rev;
	bool				filter_dir;
	enum TIME_BY		time_by;
	bool				recursive;
	bool				colorize;
	enum FILTER_FILE	filter;
} t_opts;

enum	OPTIONS {
			OPT_LONG, //long listing -l
			OPT_HUMAN_READABLE, // -h
			OPT_COLOR, // --color
			OPT_FORMAT_ATIME, // -u
			OPT_TIME, // --time
			OPT_INODE, // -i
			OPT_ROW, // -1
			OPT_WIDTH, // -w / --width
			OPT_FLT_ALL, // -a
			OPT_FLT_ALMOST_ALL, // -A
			OPT_FLT_FOLDER, // -d
			OPT_SORT_REV, // -r / --reverse
			OPT_SORT_TIME, // -t
			OPT_SORT_NONE, // -U
			OPT_SORT_SIZE, // -S
			OPT_SORT, // --sort
			OPT_RECURSIVE, // -R
			OPT_ALIAS_f // => --color=none + -a + -U
};


typedef struct s_ls_flag {
	char	short_id;
	char*	long_id;
	bool	has_param;
	int		(*handler)(t_opts*, char*);
} t_ls_flag;


int	option_set_long_listing(t_opts* options, char*);
int	option_set_human_readable(t_opts* options, char*);
int	option_set_color(t_opts* options, char* arg);
int	option_set_access_time(t_opts* options, char*);
int	option_set_inode(t_opts* options, char*);
int	option_set_row_format(t_opts* options, char*);
int	option_set_column_width(t_opts* options, char* width);
int	option_set_filter_all(t_opts* options, char*);
int	option_set_filter_aall(t_opts* options, char*);
int	option_set_filter_folder(t_opts* options, char*);
int	option_set_sort_reverse(t_opts* options, char*);
int	option_set_sort_time(t_opts* options, char*);
int	option_set_sort_none(t_opts* options, char*);
int	option_set_sort_size(t_opts* options, char*);
int	option_set_recursive(t_opts* options, char*);
int	option_set_alias_f(t_opts* options, char*);

int	option_argument_sort(t_opts* options, char* arg);
int	option_argument_time(t_opts* options, char* arg);


int	ls_parse_flags(int nbr, char** args, t_opts* options);

int	ls_parse_args(int nbr, char** args, t_opts* options);