#ifndef FT_LS_H
# define FT_LS_H

# include <stdbool.h>

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

enum	LS_ERRORS {
		ERROR_INTERNAL = 1,
		ERROR_INPUT = 2
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

typedef struct s_list t_list;


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
			OPT_ALIAS_f, // => --color=none + -a + -U
			NBR_OPTIONS
};


typedef struct s_ls_flag {
	char	short_id;
	char*	long_id;
	bool	has_param;
	int		(*handler)(t_opts*, char*);
} t_ls_flag;


int	ls_error_invalid_flag(char flag);
int	ls_error_invalid_option(char* arg);
int	ls_error_ambiguous_option(char* arg, t_list* matches);
int	ls_error_flag_missing_argument(char option);
int	ls_error_option_missing_argument(char* option);
int	ls_error_option_extra_argument(char* option, int end);

int	ls_parse_args(int nbr, char** args, t_opts* options, t_list** files);
int	ls_push_file(char* path, t_opts* options, t_list** files);

void	ls_print_options(t_opts* options);

#endif