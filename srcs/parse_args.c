#include <libft.h>
#include <ft_ls.h>

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

// /// @brief Loop through args and retrieve options
// /// @param nbr 
// /// @param args each flag entry is substituted with ```NULL``` 
// /// @param options 
// /// @return ```2``` if unknown flag, error is printed
// int	ls_parse_flags(int nbr, char** args, t_opts *options) {

// }

// /// @brief Loop args, retrieve flags and add every file
// /// @param nbr 
// /// @param args 
// /// @param options 
// /// @param files 
// /// @return 
// int	ls_parse_args(int nbr, char** args, t_opts *options, t_list** files) {
	
// }