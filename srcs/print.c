#include <ft_ls.h>
#include <libft.h>

/// @brief 
/// @param data 
/// @return 
static bool	check_nbr_column(t_data* data) {
	unsigned int total_width = 0;
	t_list*	file_node;

	data->column_len = (data->nbr_files / data->nbr_column);
	if ((((float)data->nbr_files / (float)data->nbr_column) - (float)data->column_len) > 0.f) 
		data->column_len += 1;
	// size_last_column = data->nbr_files - (data->nbr_column - 1) * data->column_len;
	file_node = data->files;
	for (unsigned int col_index = 0; col_index < data->nbr_column; col_index++) {
		unsigned int	max_path_w = 0, max_inode_w = 0;
		for (unsigned int file_index = data->column_len * col_index;
			file_index < (data->column_len * (col_index + 1)) && file_index < data->nbr_files;
			file_index++) {
			if (((t_file_info*)(file_node->content))->path_w > max_path_w)
				max_path_w = ((t_file_info*)(file_node->content))->path_w;
			if (((t_file_info*)(file_node->content))->inode_w > max_inode_w)
				max_inode_w = ((t_file_info*)(file_node->content))->inode_w;
			file_node = file_node->next;
		}
		data->columns_width[col_index * 2] = max_path_w;
		data->columns_width[col_index * 2 + 1] = max_inode_w;
		total_width += max_path_w + (max_inode_w ? max_inode_w + 1 : 0) ;
		if (col_index + 1 != data->nbr_column)
			total_width += 2;
		if (total_width > data->options.tty_width)
			return (false);
	}
	return (true);
}

/// @brief Calculate the number and size of each column.
/// @param data 
/// @return ```-1``` if allocation error
/// ```1``` if no column solution fitting the tty width was found
static int	compute_columns(t_data* data) {
	unsigned int	min_column_width = 0;
	min_column_width = (data->options.inode ? data->size_limits.min_inode_w + 1 : 0) + data->size_limits.min_path_w;
	int	max_nbr_column = data->options.tty_width / min_column_width;
	// ft_printf("tty width = %u, min_column_width = %u ,max = %u\n", data->tty_width, min_column_width, data->size_limits.max_path_w + data->size_limits.max_inode_w);
	for (data->nbr_column = max_nbr_column; data->nbr_column > 1; data->nbr_column--) {
		// ft_printf("nbr col = %u\n", data->nbr_column);
		data->columns_width = malloc(2 * sizeof(unsigned int) * data->nbr_column);
		if (data->columns_width == NULL)
			return (ERROR_FATAL);
		if (check_nbr_column(data) == true)
			break;
		free(data->columns_width);
		data->columns_width = NULL;
	}
	if (data->nbr_column == 1)
		return (1);
	return (0);
}

static int	print_file_short(t_file_info* file_info, t_opts* options, unsigned int path_w, unsigned int inode_w) {
	if (options->inode) {
		if (ft_printf("%-*u %-*s", inode_w, (unsigned int) file_info->stat.st_ino, path_w, file_info->path) < 0)
			return (ERROR_FATAL);
		return (0);
	}
	if (ft_printf("%-*s", path_w, file_info->path) < 0)
		return (ERROR_FATAL);
	return (0);
}

static int	print_single_row(t_data* data) {
	t_list*			file_node = data->files;
	t_file_info*	file_info;
	for (unsigned int i = 0; i < data->nbr_files && file_node; i++) {
		file_info = (t_file_info*)file_node->content;
		if (print_file_short(file_info, &data->options, file_info->path_w, file_info->inode_w))
			return (ERROR_FATAL);
		if (i + 1 == data->nbr_files)
			write(1, "\n", 1);
		else
			write(1, "  ", 2);
		file_node = file_node->next;
	}
	return (0);
}

static int	print_column(t_data* data) {
	t_list**			file_nodes;

	if (data->options.tty_width == 0)
		return (print_single_row(data));
	file_nodes = malloc(sizeof(t_list*) * data->nbr_column);
	if (file_nodes == NULL)
		return (ERROR_FATAL);
	for (unsigned int col = 0; col < data->nbr_column; col++)
		file_nodes[col] = ft_lstat(data->files, col * data->column_len);
	for (unsigned int line = 0; line < data->column_len; line++) {
		for (unsigned int col = 0; col < data->nbr_column; col++) {
			if (file_nodes[col] == NULL)
				continue;
			if (print_file_short((t_file_info*)file_nodes[col]->content, &data->options, \
					data->columns_width[col * 2], data->columns_width[col * 2 + 1]) < 0) {
				free (file_nodes);
				return (ERROR_FATAL);
			}
			write(1, "  ", 2);
			file_nodes[col] = file_nodes[col]->next;
		}
		write(1, "\n", 1);
	}
	free(file_nodes);
	return (0);
}

static int	print_files(t_data* data, t_file_info* dir) {
	int ret = 0;

	if (dir)
		ft_printf("%s:\n", dir->path);
	if (data->nbr_files == 0)
		return (0);
	if (data->options.format_by == FORMAT_BY_LINE 
		|| data->options.long_listing
		|| (ret = compute_columns(data)) > 0) {
		return (0);
	} else if (ret < 0)
		return (ERROR_FATAL);
	if (print_column(data))
		return (ERROR_FATAL);
	return (0);
}

void	ls_reset_limits(t_data* data) {
	ft_memset(&data->size_limits, 0, sizeof(t_size_limits));
	data->size_limits.min_path_w = UINT_MAX;
	if (data->options.inode)
		data->size_limits.min_inode_w = UINT_MAX;
	if (data->options.long_listing) {
		data->size_limits.min_size_w = UINT_MAX;
		data->size_limits.min_date_w = UINT_MAX;
		data->size_limits.min_group_w = UINT_MAX;
		data->size_limits.min_user_w = UINT_MAX;
	}
	data->nbr_column = 0;
}

static void	clear_files(t_data* data) {
	ft_lstclear(&data->files, &ls_free_file_info);
	if (data->columns_width) {
		free(data->columns_width);
		data->columns_width = NULL;
	}
	data->nbr_files = 0;
	ls_reset_limits(data);
}

int	ls_print(t_data* data) {
	int ret = 0, res, nbr_iter = 0;

	if (data->nbr_files && print_files(data, NULL))
		return (ERROR_FATAL);
	while (data->targets) {
		clear_files(data);
		res = ls_retrieve_dir_files(data->targets, data);
		if (res < 0)
			return (ERROR_FATAL);
		else if (res > 0)
			ret = 1;
		print_files(data, (nbr_iter || data->targets->next) ? (t_file_info*)data->targets->content : NULL);
		ft_lstpop_front(&data->targets, &ls_free_file_info);
		nbr_iter++;
	}
	return (ret);
}

