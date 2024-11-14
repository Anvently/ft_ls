#include <libft.h>
#include <ft_ls.h>
#include <string.h>
#include <stdio.h>

// static void	

int	ls_error_invalid_flag(const char flag) {
	dprintf(2, "ft_ls: invalid option -- '%c'\n", flag);
	return (ERROR_INPUT);
}

int	ls_error_invalid_option(const char* arg) {
	dprintf(2, "ft_ls: unrecognised option '--%s'\n", arg);
	return (ERROR_INPUT);
}

int	ls_error_ambiguous_option(const char* arg, t_list* matches) {
	dprintf(2, "ft_ls: option '--%s' is ambiguous; possibilities: ", arg);
	while (matches) {
		dprintf(2, " '--%s'", ((t_ls_flag*)matches->content)->long_id);
		matches = matches->next;
	}
	printf("\n");
	return (ERROR_INPUT);
}

int	ls_error_flag_missing_argument(const char option) {
	dprintf(2, "ft_ls: option requires an argument -- '%c'\n", option);
	return (ERROR_INPUT);
}

int	ls_error_option_missing_argument(const char* option) {
	dprintf(2, "ft_ls: option '--%s' requires an argument\n", option);
	return (ERROR_INPUT);
}

int	ls_error_option_extra_argument(const char* option, int end) {
	dprintf(2, "ft_ls: option '--%.*s' doesn't allow an argument\n", end, option);
	return (ERROR_INPUT);
}

int	ls_error_no_access(const char* path, int errno) {
	dprintf(2, "ft_ls: cannot access '%s': %s\n", path, strerror(errno));
	return (ERROR_SYS);
}

int	ls_error_open(const char* path, int errno) {
	dprintf(2, "ft_ls: cannot open directory '%s': %s\n", path, strerror(errno));
	return (ERROR_SYS);
}

int	ls_error_read(const char* path, int errno) {
	dprintf(2, "ft_ls: reading directory '%s': %s\n", path, strerror(errno));
	return (ERROR_SYS);
}

int	ls_error_invalid_width(const char* width) {
	dprintf(2, "ft_ls: invalid line width: ‘%s’\n", width);
	return (ERROR_INPUT);
}

static int	print_valid_arguments(const char*** valids) {
	if (!valids || !*valids)
		return (0);
	dprintf(2, "Valid arguments are:\n");
	for (int i = 0; valids[i]; i++) {
		printf("  - ");
		for (int j = 0; valids[i][j]; j++) {
			dprintf(2, "‘%s’%s", valids[i][j], (valids[i][j + 1] ? ", " : ""));
		}
		printf("\n");
	}
	return (0);
}

int	ls_error_invalid_argument(const char* option, const char* arg, const char*** valids) {
	dprintf(2, "ft_ls: invalid argument ‘%s’ for ‘--%s’\n", arg, option);
	print_valid_arguments(valids);
	return (ERROR_INPUT);
}

int	ls_error_ambiguous_argument(const char* option, const char* arg, const char*** valids) {
	dprintf(2, "ft_ls: ambiguous argument ‘%s’ for ‘--%s’\n", arg, option);
	print_valid_arguments(valids);
	return (ERROR_INPUT);
}

int	ls_error_unparsable_color(char* color) {
	dprintf(2, "ft_ls: unparsable value for LS_COLORS environment variable: %s\n", color);
	return (0);
}
