#include <libft.h>
#include <ft_ls.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/sysmacros.h>

// /// @brief 
// /// @param file_stat 
// /// @param options 
// /// @return ```false``` if set filter requires to refuse the file
// bool	ls_check_filters(const char* path, t_stat* file_stat, t_opts* options) {
	
// }

/// @brief Return an allocated ```file_info``` struct
/// @param path 
/// @param file_info 
/// @return ```1``` if can't access to path
/// ```-1``` if allocation error
static int	get_file_info(const char* path, t_file_info** file_info) {
	*file_info = ft_calloc(1, sizeof(t_file_info));
	if (*file_info == NULL)
		return (ERROR_FATAL);
	if (stat(path, &(*file_info)->stat) < 0) {
		free(*file_info);
		return (ls_error_no_access(path, errno));
	}
	if (((*file_info)->path = ft_strdup(path)) == NULL) {
		free(*file_info);
		return (ERROR_FATAL);
	}
	return (0);
}

static int	comp_ctime(void* lhd, void* rhd) {
	return (((t_file_info*)lhd)->stat.st_ctime < ((t_file_info*)rhd)->stat.st_ctime);
}

static int	comp_mtime(void* lhd, void* rhd) {
	return (((t_file_info*)lhd)->stat.st_mtime < ((t_file_info*)rhd)->stat.st_mtime);
}

static int	comp_atime(void* lhd, void* rhd) {
	return (((t_file_info*)lhd)->stat.st_atime < ((t_file_info*)rhd)->stat.st_atime);	
}

static int	comp_ascii(void* lhd, void* rhd) {
	return (ft_strcmp(((t_file_info*)lhd)->path, ((t_file_info*)rhd)->path));
}

static int	comp_alpha(void* lhd, void* rhd) {
	return (ft_stricmp(((t_file_info*)lhd)->path, ((t_file_info*)rhd)->path));
}

static int	comp_size(void* lhd, void* rhd) {
	return (((t_file_info*)lhd)->stat.st_size < ((t_file_info*)rhd)->stat.st_size);
}

/// @brief Append ```file_info``` to ```dest``` according to
// the provided sort option in ```data```
/// @param file_info 
/// @param dest linked-list of ```t_file_info```
/// @param data 
/// @return ```-1``` if allocation error
static int	push_file_info(t_file_info* file_info, t_list** dest, t_opts* options) {
	int	(*comp_time)(void*, void*);
	t_list*	node;

	node = ft_lstnew(file_info);
	if (node == NULL)
		return (ERROR_FATAL);
	switch (options->sort_by)
	{
		case SORT_BY_NONE:
			if (options->sort_rev == false)
				ft_lstadd_back(dest, node);
			else
				ft_lstadd_front(dest, node);
			break;

		case SORT_BY_TIME:
			switch (options->time_by)
			{
				case TIME_BY_ATIME:
					comp_time = &comp_atime;
					break;
				
				case TIME_BY_BTIME:
					comp_time = &comp_ctime;
					break;

				case TIME_BY_CTIME:
					comp_time = &comp_ctime;
					break;

				case TIME_BY_MTIME:
					comp_time = &comp_mtime;
					break;
			}
			ft_lstinsert_comp(dest, node, comp_time, options->sort_rev);
			break;

		case SORT_BY_ASCII:
			ft_lstinsert_comp(dest, node, &comp_ascii, options->sort_rev);
			break;

		case SORT_BY_ALPHA:
			ft_lstinsert_comp(dest, node, &comp_alpha, options->sort_rev);
			break;

		case SORT_BY_SIZE:
			ft_lstinsert_comp(dest, node, &comp_size, options->sort_rev);
			break;
	}
	return (0);
}

static unsigned int	len_nb(size_t len,  unsigned int nb)
{
	if (nb > 0)
		len = len_nb(len + 1, nb / 10);
	if (len > 0)
		return (len);
	else
		return (1);
}

/// @brief Calculate the nbr of characters required to print the given
/// entry in the column format, update ```file_info``` and check the current
/// minimum and maximum widths
/// @param file_info 
/// @param options 
/// @return 
static void	ls_compute_file_width(t_file_info* file_info, t_data* data) {
	file_info->column_width = (unsigned int)ft_strlen(file_info->path);
	if (data->options.inode)
		file_info->column_width += len_nb(0, (unsigned int)file_info->stat.st_ino) + 2;
	if (file_info->column_width > data->max_column_width)
		data->max_column_width = file_info->column_width;
	if (file_info->column_width < data->min_column_width)
		data->min_column_width = file_info->column_width;
}

void	ls_free_file_info(void* ptr) {
	t_file_info*	file_info = (t_file_info*) ptr;

	if (!file_info || file_info == NULL)
		return ;
	if (file_info->path)
		free(file_info->path);
	free(file_info);
}

/// @brief Given an arg, either add it to :
/// - files if it isn't a directory or if flag -d is enabled
/// - targets if it's a directory and flag -d is disabled
/// @param path 
/// @param data
/// @return ```1``` if file_stat could not be retrieved
/// ```-1``` if allocation error.
int	ls_retrieve_arg_file(const char* path, t_data* data) {
	t_file_info* file_info;
	t_list**	destination;
	int			ret = 0;
	
	if ((ret = get_file_info(path, &file_info)))
		return (ret);
	if (S_ISDIR(file_info->stat.st_mode) && data->options.filter_dir == false)
		destination = &data->targets;
	else
		destination = &data->files;
	if (push_file_info(file_info, destination, &data->options)) {
		ls_free_file_info(&file_info);
		return (ERROR_FATAL);
	}
	data->nbr_files++;
	if (data->options.format_by == FORMAT_BY_COLUMN && destination == &data->files)
		ls_compute_file_width(file_info, &data);
	
	return (0);
}
