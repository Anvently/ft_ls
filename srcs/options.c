#include <ft_ls.h>
#include <libft.h>

/// @brief 
/// @param option 
/// @param arg 
/// @param valids List of strings array, each array corresponding to the different alias
/// associated with one value. The array itself and the array it contains MUST be
/// NULL terminated.
/// @return ```>= 0``` selected option (index in ```valids```)
///  ```-1``` invalid argument
///  ```-2``` ambiguous argument
static int	option_argument(const char* arg, const char*** valids) {
	int		index = -1, nbr_match = 0;
	size_t	len_arg = ft_strlen(arg);

	for (int i = 0; valids[i] != NULL; i++) {
		for (int j = 0; valids[i][j]; j++) {
			if (ft_strncmp(arg, valids[i][j], len_arg) == 0) {
				index = i;
				nbr_match++;
				if (nbr_match > 1)
					return (-2);
				break;
			}
		}
	}
	return (index);
}

int	option_set_long_listing(t_opts* options, char* arg) {
	(void) arg;
	options->long_listing = true;
	options->format_by = FORMAT_BY_LINE;
	return (0);
}


int	option_set_human_readable(t_opts* options, char* arg) {
	(void) arg;
	options->human_readable = true;
	return (0);
}

int	option_set_access_time(t_opts* options, char* arg) {
	(void) arg;
	options->time_by = TIME_BY_ATIME;
	return (0);
}

int	option_set_inode(t_opts* options, char* arg) {
	(void) arg;
	options->inode = true;
	return (0);
}

int	option_set_row_format(t_opts* options, char* arg) {
	(void) arg;
	options->format_by = FORMAT_BY_LINE;
	return (0);
}

int	option_set_column_width(t_opts* options, char* arg) {
	int	width = 0;

	if (ft_strtoi(arg, &width))
		return (ls_error_invalid_width(arg));
	for (int i = 0; arg[i]; i++)
		if (ft_isdigit(arg[i]) == false)
			return (ls_error_invalid_width(arg));
	options->tty_width = (unsigned short) width;
	return (0);
}

int	option_set_filter_all(t_opts* options, char* arg) {
	(void) arg;
	options->filter = FILTER_ALL;
	return (0);
}

int	option_set_filter_aall(t_opts* options, char* arg) {
	(void) arg;
	options->filter =  FILTER_SPECIAL;
	return (0);
}

int	option_set_filter_folder(t_opts* options, char* arg) {
	(void) arg;
	options->filter_dir = true;
	return (0);
}

int	option_set_sort_reverse(t_opts* options, char* arg) {
	(void) arg;
	options->sort_rev = true;
	return (0);
}

int	option_set_sort_time(t_opts* options, char* arg) {
	(void) arg;
	options->sort_by = SORT_BY_TIME;
	return (0);
}

int	option_set_sort_none(t_opts* options, char* arg) {
	(void) arg;
	options->sort_by = SORT_BY_NONE;
	return (0);
}

int	option_set_sort_size(t_opts* options, char* arg) {
	(void) arg;
	options->sort_by = SORT_BY_SIZE;
	return (0);
}

int	option_set_recursive(t_opts* options, char* arg) {
	(void) arg;
	options->recursive = true;
	return (0);
}

int	option_argument_color(t_opts* options, char* arg);

int	option_set_alias_f(t_opts* options, char* arg) {
	(void) arg;
	option_argument_color(options, "none");
	option_set_filter_all(options, 0);
	option_set_sort_none(options, 0);
	return (0);
}

int	option_argument_sort(t_opts* options, char* arg) {
	int	ret = 0;
	static const char*	arg_none[] = {"none", NULL};
	static const char*	arg_ascii[] = {"ascii", NULL};
	static const char*	arg_alpha[] = {"alpha", NULL};
	static const char*	arg_time[] = {"time", NULL};
	static const char*	arg_size[] = {"size", NULL};
	static const char**	valids[] = {arg_none, arg_ascii, arg_alpha, arg_time, arg_size, NULL};
	switch ((ret = option_argument(arg, valids)))
	{	
		case -1:
			return (ls_error_invalid_argument("sort", arg, valids));
		
		case -2:
			return (ls_error_ambiguous_argument("sort", arg, valids));

		default:
			options->sort_by = ret - 1;
			return (0);
	}
}

int	option_argument_time(t_opts* options, char* arg) {
	int	ret = 0;
	static const char*	arg_mtime[] = {"mtime", "modif", NULL};
	static const char*	arg_ctime[] = {"ctime", "status", NULL};
	static const char*	arg_atime[] = {"atime", "access", "use", NULL};
	static const char*	arg_btime[] = {"birth", "creation", NULL};
	static const char**	valids[] = {arg_mtime, arg_ctime, arg_atime, arg_btime, NULL};
	switch ((ret = option_argument(arg, valids)))
	{
		case -1:
			return (ls_error_invalid_argument("time", arg, valids));
		
		case -2:
			return (ls_error_ambiguous_argument("time", arg, valids));

		default:
			options->time_by = ret;
			return (0);
	}
};

int	option_argument_color(t_opts* options, char* arg) {
	int	ret = 0;
	static const char*	arg_never[] = {"never", "no", "none", NULL};
	static const char*	arg_auto[] = {"auto", "tty", "if-tty", NULL};
	static const char*	arg_always[] = {"always", "yes", "force", NULL};
	static const char**	valids[] = {arg_never, arg_auto, arg_always, NULL};

	if (arg == NULL)
		arg = "always";
	switch ((ret = option_argument(arg, valids)))
	{
		case -1:
			return (ls_error_invalid_argument("color", arg, valids));
		
		case -2:
			return (ls_error_ambiguous_argument("color", arg, valids));

		default:
			options->colorize = ret;
			if (options->colorize == COLOR_AUTO && options->is_tty == false)
				options->colorize = COLOR_NONE;
			return (0);
	}
}

int	option_set_column(t_opts* options, char* arg) {
	(void) arg;
	options->format_by = FORMAT_BY_COLUMN;
	return (0);
};

/*
	return (ls_error_invalid_argument("time", arg, (const char***) {
				(const char**){"atime", "access", "use", NULL},
				(const char**){"ctime", "access", "use", NULL},
				(const char**){"atime", "access", "use", NULL}
			}));
*/