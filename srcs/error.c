#include <libft.h>
#include <ft_ls.h>
#include <string.h>

// static void	

int	ls_error_invalid_flag(const char flag) {
	ft_dprintf(2, "ft_ls: invalid option -- '%c'\n", flag);
	return (ERROR_INPUT);
}

int	ls_error_invalid_option(const char* arg) {
	ft_dprintf(2, "ft_ls: unrecognised option '--%s'\n", arg);
	return (ERROR_INPUT);
}

int	ls_error_ambiguous_option(const char* arg, t_list* matches) {
	ft_dprintf(2, "ft_ls: option '--%s' is ambiguous; possibilities: ", arg);
	while (matches) {
		ft_dprintf(2, " '--%s'", ((t_ls_flag*)matches->content)->long_id);
		matches = matches->next;
	}
	write(2, "\n", 1);
	return (ERROR_INPUT);
}

int	ls_error_flag_missing_argument(const char option) {
	ft_dprintf(2, "ft_ls: option requires an argument -- '%c'\n", option);
	return (ERROR_INPUT);
}

int	ls_error_option_missing_argument(const char* option) {
	ft_dprintf(2, "ft_ls: option '--%s' requires an argument\n", option);
	return (ERROR_INPUT);
}

int	ls_error_option_extra_argument(const char* option, int end) {
	ft_dprintf(2, "ft_ls: option '--%.*s' doesn't allow an argument\n", end, option);
	return (ERROR_INPUT);
}

int	ls_error_no_access(const char* path, int errno) {
	ft_dprintf(2, "ft_ls: cannot access '%s': %s\n", path, strerror(errno));
	return (ERROR_SYS);
}