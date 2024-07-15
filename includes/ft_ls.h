#ifndef FT_LS_H
# define FT_LS_H

// # define _GNU_SOURCE
# include <stdbool.h>
# include <sys/stat.h>
# include <statx.h>
// # include <statx-generic.h>
# include <stdlib.h>

# define LS_STATX_DFT_MASK (STATX_TYPE | STATX_MODE)


enum	SORT_BY {SORT_BY_NONE = -1,
				SORT_BY_ALPHA,
				SORT_BY_ASCII,
				SORT_BY_TIME,
				SORT_BY_SIZE,
				SORT_BY_EXTENSION,
				SIZE_SORT_BY};

enum	TIME_BY {TIME_BY_MTIME = 0, //Modification time
				TIME_BY_CTIME, //Change time
				TIME_BY_ATIME, //Acces time
				TIME_BY_BTIME}; //Creation time

enum	COLORIZE {COLOR_NONE = 0,
				COLOR_AUTO, //Colorize if stdout is a tty
				COLOR_ALWAYS}; //Always colorize

enum	FORMAT_BY {
			FORMAT_BY_COLUMN = 0, //Compute column width
			FORMAT_BY_LINE //Limit column width
};

enum	FILTER_FILE {
			FILTER_DEFAULT = 0,
			FILTER_SPECIAL, //print hidden but hide . and ..
			FILTER_ALL //print all
};

enum	LS_ERRORS {
		ERROR_FATAL = -1,
		ERROR_SYS = 1,
		ERROR_INPUT = 2
};

/// @brief Priority order
/// Seems to check for extension at the end if no other rule is found
typedef struct s_colors {
	char**	ls_colors; //array of strings containing all LS_COLORS entries
	char*	reset;
	char*	dir;
	char*	link; //if it is a symbolic link
	char*	multi_hard_link;
	char*	fifo;
	char*	socket;
	char*	door; //????
	char*	block_device;
	char*	character_device;
	char*	orphan; //symlink to deleted file or non stat-able file
	char*	setuid; //sticky uid ? (u+s)
	char*	setgid; //sticky gid ? (g+s)
	char*	capability; //???
	char*	sticky_other_writable; //dir which is sticky and other writable (+t, o+w)
	char*	other_writable; //otehr_writable file but not sticky
	char*	sticky; //sticky bit but not other-writable
	char*	exec; //file with execute permission
}	t_colors;

typedef struct s_options 
{
	bool				long_listing;
	bool				human_readable;
	bool				inode;
	enum FORMAT_BY		format_by;
	unsigned short		tty_width;
	enum SORT_BY		sort_by;
	bool				sort_rev;
	bool				filter_dir;
	enum TIME_BY		time_by;
	bool				recursive;
	bool				colorize;
	enum FILTER_FILE	filter;
	bool				is_tty;
	unsigned int		statx_mask;
	bool				retrieve_xattr;
	bool				reach_slink;
} t_opts;

typedef struct s_list t_list;
typedef struct stat t_stat;

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
			OPT_FORCE_COLUMN,
			NBR_OPTIONS
};

enum	ARG_TYPE {
			ARG_NONE,
			ARG_OPTIONNAL,
			ARG_REQUIRED
};

typedef struct s_ls_flag {
	char			short_id;
	char*			long_id;
	enum ARG_TYPE	arg;
	int				(*handler)(t_opts*, char*);
} t_ls_flag;

typedef struct s_ls_file_info {
	char			filename[256];
	char*			path;
	struct statx	stat;
	unsigned int	path_w;
	unsigned int	inode_w;
} t_file_info;

// typedef struct s_format_data {
// 	unsigned	int	
// } t_format;

typedef struct s_size_limits {
	unsigned int	min_path_w;
	unsigned int	max_path_w;
	unsigned int	min_inode_w;
	unsigned int	max_inode_w;
	unsigned int	min_date_w;
	unsigned int	max_date_w;
	unsigned int	min_user_w;
	unsigned int	max_user_w;
	unsigned int	min_group_w;
	unsigned int	max_group_w;
	unsigned int	min_size_w;
	unsigned int	max_size_w;
} t_size_limits;

typedef struct s_ls_data {
	t_list*			files;
	t_list*			targets;
	t_opts			options;
	t_size_limits	size_limits;
	size_t			nbr_files;
	unsigned int	nbr_column;
	unsigned int*	columns_width;
	// unsigned int*	columns_inode_width;
	unsigned int	column_len;
	t_colors		colors;
} t_data;

int	ls_error_invalid_flag(const char flag);
int	ls_error_invalid_option(const char* arg);
int	ls_error_ambiguous_option(const char* arg, t_list* matches);
int	ls_error_flag_missing_argument(const char option);
int	ls_error_option_missing_argument(const char* option);
int	ls_error_option_extra_argument(const char* option, int end);
int	ls_error_no_access(const char* path, int errno);
int	ls_error_open(const char* path, int errno);
int	ls_error_invalid_width(const char* width);
int	ls_error_invalid_argument(const char* option, const char* arg, const char*** valids);
int	ls_error_ambiguous_argument(const char* option, const char* arg, const char*** valids);
int	ls_error_unparsable_color(char* color);

int	ls_parse_args(int nbr, char** args, t_data* data);
int	ls_retrieve_arg_file(const char* path, t_data* data);
int	ls_retrieve_dir_files(t_list* current_node, t_data* data);

int	ls_parse_colors(t_data* data, char** env);
char*	ls_color_get(t_file_info* file_info, t_data* data);

void	ls_reset_limits(t_data* data);
void	ls_free_file_info(void* ptr);

void	ls_print_options(t_opts* options);
void	ls_print_statx_mask(unsigned int mask);

int	ls_print(t_data* data);

#endif