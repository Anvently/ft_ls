#include <libft.h>
#include <ft_ls.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <dirent.h>
#include <sys/sysmacros.h>
#include <string.h>

// /// @brief 
// /// @param file_stat 
// /// @param options 
// /// @return ```false``` if set filter requires to refuse the file
// bool	ls_check_filters(const char* path, t_stat* file_stat, t_opts* options) {
	
// }

/// @brief Return the filename part of a path, including the extension, ignore 
/// leading ```.``` at the beginning of the filename.
/// @param path 
/// @return 
static const char*	extract_filename(const char* path) {
	const char*	start = ft_strrchr(path, '/');
	if (start++ == NULL)
		start = path;
	while (*start == '.')
		start++;
	if (*start == '\0')
		return (path);
	// ft_printf("start = %s\n", start);
	return (start);
}

/// @brief Return path extension
/// @param path 
/// @return ```NULL``` if no extension, empty string if trailing dot
static const char*	extract_extension(const char* path) {
	const char* start = ft_strrchr(path, '.');
	if (start == NULL)
		return (NULL);
	return (start + 1);
}

static int	comp_ascii(void* lhd, void* rhd) {
	return (ft_strcmp(((t_file_info*)lhd)->path, ((t_file_info*)rhd)->path));
}

static int	comp_alpha(void* lhd, void* rhd) {
	int	diff = ft_stricmp(extract_filename(((t_file_info*)lhd)->path), extract_filename(((t_file_info*)rhd)->path));
	if (diff)
		return (diff);
	return (comp_ascii(lhd, rhd));
}

static int	comp_extension(void* lhd, void* rhd) {
	const char	*lext = extract_extension(((t_file_info*)lhd)->path);
	const char	*rext = extract_extension(((t_file_info*)rhd)->path);
	// ft_printf("for %s and %s => comparing %s and %s\n", ((t_file_info*)lhd)->path, ((t_file_info*)rhd)->path, lext, rext);
	if (lext == NULL) {
		if (rext == NULL)
			return (comp_alpha(lhd, rhd));
		return (-1);
	}
	if (rext == NULL)
		return (1);
	int	diff = ft_stricmp(lext, rext);
	if (diff == 0) {
		diff = ft_strcmp(lext, rext);
		if (diff == 0)
			return (comp_alpha(lhd, rhd));
	}
	return (diff);
}

static int	comp_size(void* lhd, void* rhd) {
	if (((t_file_info*)lhd)->stat.stx_size < ((t_file_info*)rhd)->stat.stx_size)
		return (-1);
	else if (((t_file_info*)lhd)->stat.stx_size > ((t_file_info*)rhd)->stat.stx_size)
		return (1);
	return (comp_alpha(lhd, rhd));
}

static int	comp_btime(void* lhd, void* rhd) {
/* 	// printf("comparing : \n\
	// 	- %s : sec = %llu and nsec = %u \n\
	// 	- %s : sec = %llu and nsec = %u \n", ((t_file_info*)lhd)->path, ((t_file_info*)lhd)->stat.stx_btime.tv_sec,
	// 					((t_file_info*) lhd)->stat.stx_btime.tv_nsec, 
	// 				((t_file_info*) rhd)->path, ((t_file_info*) rhd)->stat.stx_btime.tv_sec,
	// 					((t_file_info*) rhd)->stat.stx_btime.tv_nsec); */
	if (((t_file_info*)lhd)->stat.stx_btime.tv_sec > ((t_file_info*)rhd)->stat.stx_btime.tv_sec)
		return (-1);
	else if (((t_file_info*)lhd)->stat.stx_btime.tv_sec == ((t_file_info*)rhd)->stat.stx_btime.tv_sec) {
		if (((t_file_info*)lhd)->stat.stx_btime.tv_nsec > ((t_file_info*)rhd)->stat.stx_btime.tv_nsec)
			return (-1);
		else if (((t_file_info*)lhd)->stat.stx_btime.tv_nsec == ((t_file_info*)rhd)->stat.stx_btime.tv_nsec)
			return (comp_alpha(lhd, rhd));
		return (1);
	} 
	return (1);
}

static int	comp_ctime(void* lhd, void* rhd) {
	if (((t_file_info*)lhd)->stat.stx_ctime.tv_sec > ((t_file_info*)rhd)->stat.stx_ctime.tv_sec)
		return (-1);
	else if (((t_file_info*)lhd)->stat.stx_ctime.tv_sec == ((t_file_info*)rhd)->stat.stx_ctime.tv_sec) {
		if (((t_file_info*)lhd)->stat.stx_ctime.tv_nsec > ((t_file_info*)rhd)->stat.stx_ctime.tv_nsec)
			return (-1);
		else if (((t_file_info*)lhd)->stat.stx_ctime.tv_nsec == ((t_file_info*)rhd)->stat.stx_ctime.tv_nsec)
			return (comp_alpha(lhd, rhd));
		return (1);
	} 
	return (1);
}

static int	comp_mtime(void* lhd, void* rhd) {
	if (((t_file_info*)lhd)->stat.stx_mtime.tv_sec > ((t_file_info*)rhd)->stat.stx_mtime.tv_sec)
		return (-1);
	else if (((t_file_info*)lhd)->stat.stx_mtime.tv_sec == ((t_file_info*)rhd)->stat.stx_mtime.tv_sec) {
		if (((t_file_info*)lhd)->stat.stx_mtime.tv_nsec > ((t_file_info*)rhd)->stat.stx_mtime.tv_nsec)
			return (-1);
		else if (((t_file_info*)lhd)->stat.stx_mtime.tv_nsec == ((t_file_info*)rhd)->stat.stx_mtime.tv_nsec)
			return (comp_alpha(lhd, rhd));
		return (1);
	} 
	return (1);
}

static int	comp_atime(void* lhd, void* rhd) {
	if (((t_file_info*)lhd)->stat.stx_atime.tv_sec > ((t_file_info*)rhd)->stat.stx_atime.tv_sec)
		return (-1);
	else if (((t_file_info*)lhd)->stat.stx_atime.tv_sec == ((t_file_info*)rhd)->stat.stx_atime.tv_sec) {
		if (((t_file_info*)lhd)->stat.stx_atime.tv_nsec > ((t_file_info*)rhd)->stat.stx_atime.tv_nsec)
			return (-1);
		else if (((t_file_info*)lhd)->stat.stx_atime.tv_nsec == ((t_file_info*)rhd)->stat.stx_atime.tv_nsec)
			return (comp_alpha(lhd, rhd));
		return (1);
	} 
	return (1);
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
					comp_time = &comp_btime;
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
		
		case SORT_BY_EXTENSION:
			ft_lstinsert_comp(dest, node, &comp_extension, options->sort_rev);
			break;

		default:
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

static void	assign_min_max(unsigned int* dest_max, unsigned int* dest_min, unsigned int value) {
	if (value > *dest_max)
		*dest_max = value;
	if (value < *dest_min)
		*dest_min = value;
}

/// @brief Calculate the nbr of characters required to print the given
/// entry, update ```file_info``` and check the current
/// minimum and maximum widths
/// @param file_info 
/// @param options 
/// @return 
static void	ls_compute_file_width(t_file_info* file_info, t_data* data) {
	file_info->path_w = (unsigned int)ft_strlen(file_info->path);
	assign_min_max(&data->size_limits.max_path_w, &data->size_limits.min_path_w, file_info->path_w);
	if (data->options.inode) {
		file_info->inode_w = len_nb(0, (unsigned int)file_info->stat.stx_ino);
		assign_min_max(&data->size_limits.max_inode_w, &data->size_limits.min_inode_w, file_info->inode_w);
	}
}

void	ls_free_file_info(void* ptr) {
	t_file_info*	file_info = (t_file_info*) ptr;

	if (!file_info || file_info == NULL)
		return ;
	if (file_info->path && file_info->path != &file_info->filename[0])
		free(file_info->path);
	free(file_info);
}

static bool	check_name_filter(struct dirent* dir_entry, t_opts* options) {
	switch (options->filter)
	{
		case FILTER_ALL:
			return (true);
		
		case FILTER_DEFAULT:
			if (dir_entry->d_name[0] == '.')
				return (false);
			return (true);

		case FILTER_SPECIAL:
			if (ft_strcmp(dir_entry->d_name, ".") == 0 || ft_strcmp(dir_entry->d_name, "..") == 0)
				return (false);
			return (true);

		default:
			return (true);
	}
}

/// @brief Return an allocated ```file_info``` struct
/// @param path 
/// @param file_info 
/// @return ```1``` if can't access to path
/// ```-1``` if allocation error
static int	get_file_info(const char* path, t_file_info** file_info, t_data* data) {
	*file_info = ft_calloc(1, sizeof(t_file_info));
	if (*file_info == NULL)
		return (ERROR_FATAL);
	if (statx(AT_FDCWD, path, 0, data->options.statx_mask, &(*file_info)->stat) < 0) {
		free(*file_info);
		return (ls_error_no_access(path, errno));
	}
	if (((*file_info)->path = ft_strdup(path)) == NULL) {
		free(*file_info);
		return (ERROR_FATAL);
	}
	return (0);
}

/// @brief Return allocated ```file_info``` struct of a file from it's directory
/// entry. If filter set in option, check name. Only call stat() if required by the options.
/// @param dir_path used to build the full path to the file
/// @param dir_entry use to retrieve dir_type and inode
/// @param file_info destination pointer.
/// @return ```1``` if can't access to path
/// ```-1``` if allocation error
static int	get_file_info_from_dir(int dir_fd, struct dirent* dir_entry, t_file_info** file_info, t_data* data) {
	*file_info = ft_calloc(1, sizeof(t_file_info));
	if (*file_info == NULL)
		return (ERROR_FATAL);
	ft_strlcpy(&(*file_info)->filename[0], &dir_entry->d_name[0], 256);
	(*file_info)->path = &(*file_info)->filename[0];
	if (data->options.statx_mask != LS_STATX_DFT_MASK) {
		if (statx(dir_fd, &dir_entry->d_name[0], 0, data->options.statx_mask, &(*file_info)->stat) < 0) {
			free(*file_info);
			return (ERROR_SYS);	
		}
	} else {
		(*file_info)->stat.stx_ino = dir_entry->d_ino;
		(*file_info)->stat.stx_mode = DTTOIF(dir_entry->d_type);
	}
	return (0);
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
	
	if ((ret = get_file_info(path, &file_info, data)))
		return (ret);
	if (S_ISDIR(file_info->stat.stx_mode) && data->options.filter_dir == false)
		destination = &data->targets;
	else
		destination = &data->files;
	if (push_file_info(file_info, destination, &data->options)) {
		ls_free_file_info(&file_info);
		return (ERROR_FATAL);
	}
	if (destination == &data->files)
		data->nbr_files++;
	if (data->options.format_by == FORMAT_BY_COLUMN && destination == &data->files)
		ls_compute_file_width(file_info, data);
	return (0);
}

static int	append_recursive_subfolder(t_list* current_node, t_list** dest, t_file_info* dir_info, t_opts* options) {
	t_file_info*	target_info;
	size_t			parent_path_len, dir_name_len;

	target_info = malloc(sizeof(t_file_info));
	if (target_info == NULL)
		return (ERROR_FATAL);
	ft_memcpy(target_info, dir_info, sizeof(t_file_info));
	parent_path_len = ft_strlen(((t_file_info*)current_node->content)->path);
	dir_name_len = ft_strlen(&dir_info->filename[0]);
	target_info->path = malloc(parent_path_len + dir_name_len + 2);
	if (target_info->path == NULL) {
		free(target_info);
		return (ERROR_FATAL);
	}
	ft_strlcpy(target_info->path, ((t_file_info*)current_node->content)->path, parent_path_len + 1);
	if (target_info->path[parent_path_len - 1] != '/') {
		ft_strlcpy(target_info->path + parent_path_len, "/", 2);
		ft_strlcpy(target_info->path + parent_path_len + 1, &dir_info->filename[0], parent_path_len + dir_name_len + 2);
	}
	else
		ft_strlcpy(target_info->path + parent_path_len, &dir_info->filename[0], parent_path_len + dir_name_len + 2);
	if (push_file_info(target_info, dest, options)) {
		ls_free_file_info(target_info);
		return (ERROR_FATAL);
	}
	return (0);
}

static inline bool	check_recursive_subfolder(t_file_info* dir_info) {
	if (S_ISDIR(dir_info->stat.stx_mode) == false || ft_strcmp(dir_info->filename, ".") == 0
		|| ft_strcmp(dir_info->filename, "..") == 0)
		return (false);
	return (true);
}

/// @brief Read every files in dir and add them to files list.
/// If recursive flag is enabled, also adds folder as the next target
/// @param dir_info 
/// @param data 
/// @return ```1``` if a directory could not be opened
/// ```-1``` if allocation error.
int	ls_retrieve_dir_files(t_list* current_node, t_data* data) {
	t_file_info*	dir_info = (t_file_info*)current_node->content;
	t_file_info		*file_info = NULL;
	t_list*			add_targets = NULL;
	DIR*			dir = NULL;
	struct dirent*	dir_entry = NULL;
	int				ret = 0, res, dir_fd = -1;

	errno = 0;
	dir = opendir(dir_info->path);
	if (dir == NULL)
		return (ls_error_open(dir_info->path, errno));
	dir_fd = dirfd(dir);
	while (dir_fd > 0 && (dir_entry = readdir(dir))) {
		if (data->options.filter != FILTER_ALL && check_name_filter(dir_entry, &data->options) == false)
			continue;
		if ((res = get_file_info_from_dir(dir_fd, dir_entry, &file_info, data))) {
			ret = res;
			if (res < 0)
				break;
		}
		if (data->options.format_by == FORMAT_BY_COLUMN)
			ls_compute_file_width(file_info, data);
		if ((ret = push_file_info(file_info, &data->files, &data->options)))
			break;
		data->nbr_files++;
		if (data->options.recursive && check_recursive_subfolder(file_info)
			&& (ret = append_recursive_subfolder(current_node, &add_targets, file_info, &data->options)))
			break;
	}
	if (ret < 0 && file_info)
		ls_free_file_info(file_info);
	else if (errno && ret >= 0) {
		ft_dprintf(2, "Unknown exception: %d: %s", errno, strerror(errno));
		ret = ERROR_FATAL;
	}
	if (add_targets != NULL)
		ft_lstmerge(current_node, &add_targets);
	closedir(dir);
	return (0);
}
