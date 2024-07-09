#include <stdio.h>
#include <libft.h>
#include <ft_ls.h>

/* static char**	options_map_str[sizeof(enum OPTIONS)] = {\
		[OPT_FORMAT_ATIME] = (char*[]){"", ""},
		[OPT_COLOR] = (char*[]){"", ""}
}; */


// int	ft_ls(int nbr_args, char** args, char** env) {
	
// 	return (0);
// }


int	main(int argc, char **argv, char **env) {
	t_opts	options = {0};
	t_list*	files = NULL;
	int	ret = 0;

	(void) env;
	ret = ls_parse_args(argc - 1, argv + 1, &options, &files);
	ls_print_options(&options);
	ft_lstclear(&files, NULL);
	return (ret);
}