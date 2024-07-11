#include <ft_ls.h>

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

int	option_set_color(t_opts* options, char* arg) {
	(void) options;
	(void) arg;
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

int	option_set_column_width(t_opts* options, char* width) {
	(void) options;
	(void) width;
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

int	option_set_alias_f(t_opts* options, char* arg) {
	(void) arg;
	option_set_color(options, "none");
	option_set_filter_all(options, 0);
	option_set_sort_none(options, 0);
	return (0);
}

int	option_argument_sort(t_opts* options, char* arg) {
	(void) options;
	(void) arg;
	return (0);
}

int	option_argument_time(t_opts* options, char* arg) {
	(void) options;
	(void) arg;
	return (0);
};

int	option_set_column(t_opts* options, char* arg) {
	(void) arg;
	options->format_by = FORMAT_BY_COLUMN;
	return (0);
};