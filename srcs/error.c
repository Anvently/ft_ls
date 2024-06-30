#include <libft.h>
#include <ft_ls.h>

// static void	

int	ls_error_invalid_flag(char flag) {
	ft_dprintf(2, "ft_ls: invalid option -- '%c'\n", flag);
	return (2);
}

int	ls_error_invalid_otion(char* arg) {
	ft_dprintf(2, "ft_ls: unrecognised option '%s'\n", arg);
	return (2);
}

int	ls_error_ambiguous_option(char* arg, char** matches) {
	ft_dprintf(2, "ft_ls: option '%s' is ambiguous; possibilities: ", arg);
	while (matches && *matches)
		ft_dprintf(2, " '--%s'", *matches++);
	write(2, "\n", 1);
	return (2);
};

