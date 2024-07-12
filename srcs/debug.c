#include <ft_ls.h>
#include <libft.h>

void	ls_print_options(t_opts* options) {
	ft_printf("long listing: %d\n", options->long_listing);
	ft_printf("human readable: %d\n", options->human_readable);
	ft_printf("inode: %d\n", options->inode);
	ft_printf("format by: ");
	switch (options->format_by)
	{
		case FORMAT_BY_COLUMN:
			ft_printf("colum\n");
			break;
		
		case FORMAT_BY_LINE:
			ft_printf("line\n");
			break;
	}
	ft_printf("column max width: %d\n", options->tty_width);
	ft_printf("sort by: ");
	switch (options->sort_by)
	{
		case SORT_BY_ALPHA:
			ft_printf("alphanumeric\n");
			break;
		
		case SORT_BY_ASCII:
			ft_printf("ascii\n");
			break;

		case SORT_BY_TIME:
			ft_printf("time\n");
			break;

		case SORT_BY_NONE:
			ft_printf("none\n");
			break;

		case SORT_BY_SIZE:
			ft_printf("size\n");
			break;

		default:
			break;
	}
	ft_printf("reverse sort: %d\n", options->sort_rev);
	ft_printf("only dir: %d\n", options->filter_dir);
	ft_printf("time by: ");
	switch (options->time_by)
	{
		case TIME_BY_ATIME:
			ft_printf("access time\n");
			break;
		
		case TIME_BY_BTIME:
			ft_printf("birth time\n");
			break;

		case TIME_BY_CTIME:
			ft_printf("change time\n");
			break;

		case TIME_BY_MTIME:
			ft_printf("modification time\n");
			break;
	}
	ft_printf("recursive: %d\n", options->recursive);
	ft_printf("colorize: %d\n", options->colorize);
	ft_printf("filter by: ");
	switch (options->filter)
	{
		case FILTER_DEFAULT:
			ft_printf("default\n");
			break;
		
		case FILTER_SPECIAL:
			ft_printf("special\n");
			break;

		case FILTER_ALL:
			ft_printf("all\n");
			break;
	}
}
