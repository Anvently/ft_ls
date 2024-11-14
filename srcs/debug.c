#include <ft_ls.h>
#include <libft.h>

void	ls_print_options(t_opts* options) {
	printf("long listing: %d\n", options->long_listing);
	printf("human readable: %d\n", options->human_readable);
	printf("inode: %d\n", options->inode);
	printf("format by: ");
	switch (options->format_by)
	{
		case FORMAT_BY_COLUMN:
			printf("colum\n");
			break;
		
		case FORMAT_BY_LINE:
			printf("line\n");
			break;
	}
	printf("column max width: %d\n", options->tty_width);
	printf("sort by: ");
	switch (options->sort_by)
	{
		case SORT_BY_ALPHA:
			printf("alphanumeric\n");
			break;
		
		case SORT_BY_ASCII:
			printf("ascii\n");
			break;

		case SORT_BY_TIME:
			printf("time\n");
			break;

		case SORT_BY_NONE:
			printf("none\n");
			break;

		case SORT_BY_SIZE:
			printf("size\n");
			break;

		default:
			break;
	}
	printf("reverse sort: %d\n", options->sort_rev);
	printf("only dir: %d\n", options->filter_dir);
	printf("time by: ");
	switch (options->time_by)
	{
		case TIME_BY_ATIME:
			printf("access time\n");
			break;
		
		case TIME_BY_BTIME:
			printf("birth time\n");
			break;

		case TIME_BY_CTIME:
			printf("change time\n");
			break;

		case TIME_BY_MTIME:
			printf("modification time\n");
			break;
	}
	printf("recursive: %d\n", options->recursive);
	printf("colorize: %d\n", options->colorize);
	printf("filter by: ");
	switch (options->filter)
	{
		case FILTER_DEFAULT:
			printf("default\n");
			break;
		
		case FILTER_SPECIAL:
			printf("special\n");
			break;

		case FILTER_ALL:
			printf("all\n");
			break;
	}
}

void	ls_print_statx_mask(unsigned int mask) {
	printf("%u=> %s%s%s%s%s%s%s%s%s%s%s\n", mask, 
		((mask & STATX_TYPE) ? "TYPE | " : ""),
		((mask & STATX_MODE) ? "MODE | " : ""),
		((mask & STATX_NLINK) ? "NLINK | " : ""),
		((mask & STATX_UID) ? "UID | " : ""),
		((mask & STATX_GID) ? "GID | " : ""),
		((mask & STATX_ATIME) ? "ATIME | " : ""),
		((mask & STATX_MTIME) ? "MTIME | " : ""),
		((mask & STATX_CTIME) ? "CTIME | " : ""),
		((mask & STATX_INO) ? "INO | " : ""),
		((mask & STATX_SIZE) ? "SIZE | " : ""),
		((mask & STATX_BTIME) ? "BTIME | " : "")
	);
}
