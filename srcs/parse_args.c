#include <libft.h>
#include <ft_ls.h>

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
		.has_param = true,
		.handler = option_set_color
	},
	[OPT_FORMAT_ATIME]		= (t_ls_flag) {
		.short_id = 'u',
		.handler = option_set_access_time
	},
	[OPT_TIME]			= (t_ls_flag) {
		.long_id = "time",
		.has_param = true,
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
		.has_param = true,
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
		.handler = option_set_sort_none
	},
	[OPT_SORT] 			= (t_ls_flag) {
		.long_id = "sort",
		.has_param = true,
		.handler = option_argument_sort
	},
	[OPT_RECURSIVE] 	= (t_ls_flag) {
		.short_id = 'R',
		.long_id = "recursive",
		.handler = option_set_recursive
	},
	[OPT_ALIAS_f]		= (t_ls_flag) {
		.short_id = 'f',
		.handler = option_set_alias_f
	}
};

/// @brief Parse a short flag in the form and enable it
/// @param arg 
/// @param options 
/// @return ```2``` f unknown flag
static int	ls_parse_arg_short(char* option, t_opts *options) {
	t_ls_flag*	flag_info = NULL;

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
	if (flag_info->has_param) {
		if (option[1] == '\0')
			return (ls_error_flag_missing_argument(*option));
		return ((*flag_info->handler)(options, option + 1));
	}
	return ((*flag_info->handler)(options, NULL));
}

static int	ls_parse_flag_list(char* arg, t_opts* options) {
	for (int i = 0; arg[i]; i++)
		if (ls_parse_arg_short(arg + i, options))
			return (ERROR_INPUT);
	return (0);
}

static int	ls_parse_option_long(char* arg, t_opts* options) {
	t_ls_flag*	flag_info = NULL;
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
				return (ERROR_INTERNAL);
			}
			ft_lstadd_back(&matches, node);
		}
	}
	if (matches == NULL)
		ret = ls_error_invalid_option(arg);
	else if (matches->next != NULL)
		ret = ls_error_ambiguous_option(arg, matches);
	else {
		flag_info = matches->content;
		if (flag_info->has_param) {
			if (arg[end] == '\0' || arg[end + 1] == '\0')
				ret = ls_error_option_missing_argument(flag_info->long_id);
			else
				ret = (*flag_info->handler)(options, arg + end + 1);
		} else {
			if (arg[end] != '\0')
				ret = ls_error_option_extra_argument(arg, end);
			else
				ret = (*flag_info->handler)(options, NULL);
		}
	}
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
	for (int i = 0; i < nbr; i++) {
		if (args[i][0] == '-') {
			if (args[i][1] == '-' && (ft_isspace(args[i][2]) || !args[i][2])) {
				args[i] = NULL;	
				break;
			}
			else if (args[i][1] == '-') {
				if (ls_parse_option_long(args[i] + 2, options))
					return (ERROR_INPUT);
			} else {
				if (ls_parse_flag_list(args[i] + 1, options))
					return (ERROR_INPUT);
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
int	ls_parse_args(int nbr, char** args, t_opts *options, t_list** files) {
	int		ret = 0;

	if (ls_parse_retrieve_options(nbr, args, options))
		return (ERROR_INPUT);
	for (int i = 0; i < nbr; i++) {
		if (args[i] == NULL)
			continue;	
		if (ls_push_file(args[i], options, files))
			ret = 1;
	}
	return (ret);
}
