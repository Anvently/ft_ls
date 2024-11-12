#include <stdio.h>
#include <libft.h>
#include <ft_ls.h>
#include <limits.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <string.h>

/* static char**	options_map_str[sizeof(enum OPTIONS)] = {\
		[OPT_FORMAT_ATIME] = (char*[]){"", ""},
		[OPT_COLOR] = (char*[]){"", ""}
}; */


// int	ft_ls(int nbr_args, char** args, char** env) {
	
// 	return (0);
// }

static int	free_all(int status, t_data* data) {
	ft_lstclear(&data->files, &ls_free_file_info);
	ft_lstclear(&data->targets, &ls_free_file_info);
	if (data->columns_width)
		free(data->columns_width);
	if (data->colors.ls_colors)
		ft_free_strs(data->colors.ls_colors);
	return (status);
}

static int	init_data(t_data* data, char** env) {
	struct 			winsize w;
	char*	env_width;

	data->options.statx_mask = LS_STATX_DFT_MASK;
	data->options.statx_flags = AT_STATX_SYNC_AS_STAT;
	data->options.deref_symlink_argument = true;
	if (isatty(STDOUT_FILENO)) {
		data->options.is_tty = true;
		if (ioctl(0, TIOCGWINSZ, &w) < 0)
			return (ERROR_FATAL);
		data->options.tty_width = w.ws_col;
	}
	else if (errno != ENOTTY)
		return (ERROR_FATAL);
	else {
		data->options.format_by = FORMAT_BY_LINE;
		env_width = ft_getenv("COLUMNS", env);
		if (env_width == NULL)
			return (ERROR_FATAL);
		if (env_width[0] == '\0')
			data->options.tty_width = 80;
		else
			data->options.tty_width = (unsigned int)ft_atoi(env_width);
		free(env_width);
	}
	return (0);
}

int	main(int argc, char **argv, char **env) {
	t_data	data = {0};
	int	ret = 0;

	if (init_data(&data, env))
		return (ERROR_FATAL);
	ret = ls_parse_args(argc - 1, argv + 1, &data, env);
	if ((ret < 0 || ret > 1) || (ret == 1 && !data.files && !data.targets))
		return (free_all(ret, &data));
	if (!data.files && !data.targets && (ret = ls_retrieve_arg_file(".", &data)))
		return (free_all(ret, &data));
	ret = ls_print(&data);
	// ls_print_options(&data.options);
	return (free_all(ret, &data));
}
