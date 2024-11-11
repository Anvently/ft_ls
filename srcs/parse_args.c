#include <libft.h>
#include <ft_ls.h>

int	option_set_long_listing(t_opts* options, char*);
int	option_set_human_readable(t_opts* options, char*);
int	option_argument_color(t_opts* options, char* arg);
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
int	option_set_column(t_opts* options, char*);
int	option_deref_symlink(t_opts* options, char*);
int	option_deref_symlink_argument(t_opts* options, char* arg);
int	option_argument_sort(t_opts* options, char* arg);
int	option_argument_time(t_opts* options, char* arg);
int	option_only_group(t_opts* options, char* arg);
int	option_hide_group(t_opts* options, char* arg);

static t_ls_flag options_map[] = {
	[OPT_LONG]			= (t_ls_flag) {
		.short_id = 'l',
		.handler = option_set_long_listing
	},
	[OPT_HUMAN_READABLE]= (t_ls_flag) {
		.short_id = 'h',
		.long_id = "human-readable",
		.handler = option_set_human_readable
	},
	[OPT_COLOR]			= (t_ls_flag) {
		.long_id = "color",
		.arg = ARG_OPTIONNAL,
		.handler = option_argument_color
	},
	[OPT_FORMAT_ATIME]		= (t_ls_flag) {
		.short_id = 'u',
		.handler = option_set_access_time
	},
	[OPT_TIME]			= (t_ls_flag) {
		.long_id = "time",
		.arg = ARG_REQUIRED,
		.handler = option_argument_time
	},
	[OPT_INODE]			= (t_ls_flag) {
		.short_id = 'i',
		.long_id = "inode",
		.handler = option_set_inode
	},
	[OPT_ROW]			= (t_ls_flag) {
		.short_id = '1',
		.handler = option_set_row_format
	},
	[OPT_WIDTH] 		= (t_ls_flag) {
		.short_id = 'w',
		.long_id = "width",
		.arg = ARG_REQUIRED,
		.handler = option_set_column_width
	},
	[OPT_FLT_ALL] 		= (t_ls_flag) {
		.short_id = 'a',
		.long_id = "all",
		.handler = option_set_filter_all
	},
	[OPT_FLT_ALMOST_ALL]= (t_ls_flag) {
		.short_id = 'A',
		.long_id = "almost-all",
		.handler = option_set_filter_aall
	},
	[OPT_FLT_FOLDER]	= (t_ls_flag) {
		.short_id = 'd',
		.long_id = "directory",
		.handler = option_set_filter_folder
	},
	[OPT_SORT_REV] 		= (t_ls_flag) {
		.short_id = 'r',
		.long_id = "reverse",
		.handler = option_set_sort_reverse
	},
	[OPT_SORT_TIME] 	= (t_ls_flag) {
		.short_id = 't',
		.handler = option_set_sort_time
	},
	[OPT_SORT_NONE] 	= (t_ls_flag) {
		.short_id = 'U',
		.handler = option_set_sort_none
	},
	[OPT_SORT_SIZE] 	= (t_ls_flag) {
		.short_id = 'S',
		.handler = option_set_sort_size
	},
	[OPT_SORT] 			= (t_ls_flag) {
		.long_id = "sort",
		.arg = ARG_REQUIRED,
		.handler = option_argument_sort
	},
	[OPT_RECURSIVE] 	= (t_ls_flag) {
		.short_id = 'R',
		.long_id = "recursive",
		.handler = option_set_recursive
	},
	[OPT_ALIAS_f]		= (t_ls_flag) {
		.short_id = 'f',
		.long_id = "fast",
		.handler = option_set_alias_f
	},
	[OPT_FORCE_COLUMN]	= (t_ls_flag) {
		.short_id = 'C',
		.handler = option_set_column
	},
	[OPT_DEREF_LINK]	= (t_ls_flag) {
		.short_id = 'L',
		.long_id = "dereference",
		.handler = option_deref_symlink
	},
	[OPT_DEREF_LINK_ARGUMENT]	= (t_ls_flag) {
		.short_id = 'H',
		.long_id = "dereference-command-line",
		.handler = option_deref_symlink_argument
	},
	[OPT_ONLY_GROUP]	= (t_ls_flag) {
		.short_id = 'g',
		.handler = option_only_group
	},
	[OPT_HIDE_GROUP]	= (t_ls_flag) {
		.short_id = 'G',
		.handler = option_hide_group
	}
};

/// @brief Parse a short flag in the form and enable it
/// @param arg 
/// @param options 
/// @return ```2``` if unknown flag. ```-1``` if no error but what's remaining
/// in arg should be considered as option argument and thus ignored.
static int	ls_parse_arg_short(char** next_arg, char* option, t_opts *options) {
	t_ls_flag*	flag_info = NULL;
	int			ret = 0;

	if (*option == '\0')
		return (0);
	for (int i = 0; i < NBR_OPTIONS; i++) {
		if (options_map[i].short_id == *option) {
			flag_info = &options_map[i];
			break;
		}
	}
	if (flag_info == NULL)
		return (ls_error_invalid_flag(*option));
	if (flag_info->arg) {
		if (option[1] == '\0') {
			if (flag_info->arg == ARG_REQUIRED) {
				if (next_arg == NULL)
					return (ls_error_flag_missing_argument(*option));
				ret = (*flag_info->handler)(options, *next_arg);
				*next_arg = NULL;
				return (ret);
			}
			ret = (*flag_info->handler)(options, NULL);
		}
		ret = (*flag_info->handler)(options, option + 1);
		if (ret)
			return (ret);
		return (-1);
	}
	return ((*flag_info->handler)(options, NULL));
}

static int	ls_parse_flag_list(char** next_arg, char* arg, t_opts* options) {
	int	ret = 0;

	for (int i = 0; arg[i]; i++) {
		ret = ls_parse_arg_short(next_arg, arg + i, options);
		if (ret > 0)
			return (ERROR_INPUT);
		if (ret < 0)
			break;
	}
	return (0);
}

static int	ls_option_long_handle_arg(char** next_arg, t_ls_flag* flag_info, char* option, size_t end, t_opts* options) {
	int	ret = 0;

	if (option[end] == '\0') {
		if (flag_info->arg == ARG_REQUIRED) {
			if (next_arg == NULL)
				return (ls_error_option_missing_argument(flag_info->long_id));
			ret = (*flag_info->handler)(options, *next_arg);
			*next_arg = NULL;
			return (ret);
		}
		return ((*flag_info->handler)(options, NULL));
	}
	if (flag_info->arg == ARG_NONE)
		return (ls_error_option_extra_argument(option, end));
	return ((*flag_info->handler)(options, option + end + 1));
}

/// @brief 
/// @param arg 
/// @param options 
/// @return ```-1``` if allocation error.
/// ```2``` of input error
static int	ls_parse_option_long(char** next_arg, char* arg, t_opts* options) {
	t_list*		matches = NULL, *node;
	size_t		end;
	int			ret = 0;

	for (end = 0; arg[end] && arg[end] != '='; end++);
	for (int i = 0; i < NBR_OPTIONS; i++) {
		if (options_map[i].long_id == NULL)
			continue;
		if (ft_strncmp(arg, options_map[i].long_id, end) == 0) {
			node = ft_lstnew(&options_map[i]);
			if (node == NULL) {
				ft_lstclear(&matches, NULL);
				return (ERROR_FATAL);
			}
			ft_lstadd_back(&matches, node);
		}
	}
	if (matches == NULL)
		ret = ls_error_invalid_option(arg);
	else if (matches->next != NULL)
		ret = ls_error_ambiguous_option(arg, matches);
	else
		ret = ls_option_long_handle_arg(next_arg, (t_ls_flag*)matches->content, arg, end, options);
	ft_lstclear(&matches, NULL);
	return (ret);
}

/// @brief Extract options in ```args``` and replaced them with ```NULL```.
/// Ignore file entries
/// @param nbr 
/// @param args 
/// @param options 
/// @return ```2``` if input errors
static int	ls_parse_retrieve_options(int nbr, char** args, t_opts* options) {
	int		ret = 0;

	for (int i = 0; i < nbr; i++) {
		if (args[i] == NULL)
			continue;
		if (args[i][0] == '-') {
			if (args[i][1] == '-' && (ft_isspace(args[i][2]) || !args[i][2])) {
				args[i] = NULL;	
				break;
			}
			else if (args[i][1] == '-') {
				if ((ret = ls_parse_option_long((i + 1 == nbr ? NULL : &args[i + 1]), args[i] + 2, options)))
					return (ret);
			} else {
				if ((ret = ls_parse_flag_list((i + 1 == nbr ? NULL : &args[i + 1]), args[i] + 1, options)))
					return (ret);
			}
			args[i] = NULL;
		}
	}
	return (0);
}

/// @brief Loop args, retrieve flags and add every file
/// @param nbr 
/// @param args 
/// @param options 
/// @param files 
/// @return ```2``` if flag error cause by user.
/// ```1``` if at least one file was inaccessibe
/// ```-1``` if fatal error
int	ls_parse_args(int nbr, char** args, t_data* data, char** env) {
	int		ret = 0;

	if ((ret = ls_parse_retrieve_options(nbr, args, &data->options)))
		return (ret);
	ls_reset_limits(data);
	if (data->options.colorize && (ret = ls_parse_colors(data, env)))
		return (-1);
	for (int i = 0; i < nbr; i++) {
		if (args[i] == NULL)
			continue;	
		switch (ls_retrieve_arg_file(args[i], data)) {
			case 1:
				ret = 1;
				break;
			case -1:
				return (-1);
		}
	}
	return (ret);
}
