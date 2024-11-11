#include <ft_ls.h>
#include <libft.h>

/*
bd=40;33;01:
ca=30;41:
cd=40;33;01:
di=01;34:
do=01;35:
ex=01;32:
ln=01;36:
mh=00:
mi=00:
or=40;31;01:
ow=34;42:
pi=40;33:
rs=0:
sg=30;43:
so=01;35:
su=37;41:
st=37;44:
tw=30;42:
*.tar=01;31:
*.tgz=01;31:
*.arc=01;31:
*.arj=01;31:
*.taz=01;31:
*.lha=01;31:
*.lz4=01;31:
*.lzh=01;31:
*.lzma=01;31:
*.tlz=01;31:*.txz=01;31:*.tzo=01;31:*.t7z=01;31:*.zip=01;31:*.z=01;31:*.dz=01;31:*.gz=01;31:*.lrz=01;31:*.lz=01;31:*.lzo=01;31:*.xz=01;31:*.zst=01;31:*.tzst=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.alz=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.cab=01;31:*.wim=01;31:*.swm=01;31:*.dwm=01;31:*.esd=01;31:*.jpg=01;35:*.jpeg=01;35:*.mjpg=01;35:*.mjpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.webm=01;35:*.webp=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=00;36:*.au=00;36:*.flac=00;36:*.m4a=00;36:*.mid=00;36:*.midi=00;36:*.mka=00;36:*.mp3=00;36:*.mpc=00;36:*.ogg=00;36:*.ra=00;36:*.wav=00;36:*.oga=00;36:*.opus=00;36:*.spx=00;36:*.xspf=00;36:
*/

static int ls_retrieve_colors(t_data* data, char** color_strs) {
	char*	color;

	for (color = *color_strs; *color_strs; color = *color_strs++) {
		if (color[0] == '*')
			continue;
		if (ft_strchr(color, '=') - color != 2 || color[3] == '\0') {
			ls_error_unparsable_color(color);
			continue;
		}
		switch (color[0])
		{
			case 'b':
				data->colors.block_device = color + 3;
				break;
			
			case 'c':
				switch (color[1])
				{
					case 'a':
						data->colors.capability = color + 3;
						data->options.retrieve_xattr = true;
						break;
					
					case 'd':
						data->colors.character_device = color + 3;
						break;

					default:
						ls_error_unparsable_color(color);
						break;
				}
				break;

			case 'd':
				switch (color[1])
				{
					case 'i':
						data->colors.dir = color + 3;
						break;
					
					case 'o':
						data->colors.door = color + 3;
						break;

					default:
						ls_error_unparsable_color(color);
						break;
				}
				break;

			case 'e':
				data->colors.exec = color + 3;
				break;

			case 'l':
				data->colors.link = color + 3;
				break;

			case 'm':
				data->colors.multi_hard_link = color + 3; 
				data->options.statx_mask |= STATX_NLINK;
				break;

			case 'o':
				switch (color[1])
				{
					case 'r':
						data->colors.orphan = color + 3;
						data->options.check_symlink = true;
						break;
					
					case 'w':
						data->colors.other_writable = color + 3;
						break;

					default:
						ls_error_unparsable_color(color);
						break;
				}
				break;

			case 'p':
				data->colors.fifo = color + 3;
				break;

			case 'r':
				data->colors.reset = color + 3;
				break;

			case 's':
				switch (color[1])
				{
					case 'g':
						data->colors.setgid = color + 3;
						break;
					
					case 'o':
						data->colors.socket = color + 3;
						break;

					case 'u':
						data->colors.setuid = color + 3;
						break;
					
					case 't':
						data->colors.sticky = color + 3;
						break;

					default:
						ls_error_unparsable_color(color);
						break;
				}
				break;

			case 't':
				data->colors.sticky_other_writable = color + 3;
				break;

			default:
				ls_error_unparsable_color(color);
				break;
		}
	}
	return (0);
}

/// @brief Retrieve ```LS_COLORS``` env variable and assign them 
/// to s_colors structure
/// @param data 
/// @param env 
/// @return ```-1``` if allocation error 
int	ls_parse_colors(t_data* data, char** env) {
	char*			colors;
	static char*	empty_color = "0";
	int				ret = 0;

	for (char** dest = &data->colors.reset; dest <= (char**)&data->colors.exec; dest++) {
		*dest = &empty_color[0];
	}
	colors = ft_getenv("LS_COLORS", env);
	if (colors == NULL)
		return (ERROR_SYS);
	else if (colors[0] == '\0') {
		free (colors);
		data->options.colorize = COLOR_NONE;
		return (0);
	}
	data->colors.ls_colors = ft_split(colors, ':');
	if (data->colors.ls_colors == NULL) {
		free(colors);
		return (ERROR_SYS);
	}
	ret = ls_retrieve_colors(data, data->colors.ls_colors);
	free(colors);
	return (ret);
}

static char*	ls_color_get_by_extension(char* filename, t_data* data) {
	char*	color;
	char*	value;
	size_t	extension_len;
	char**	color_strs = data->colors.ls_colors;

	for (color = *color_strs; *color_strs; color = *color_strs++) {
		if (color[0] != '*')
			continue;
		value = ft_strchr(color + 1, '=');
		if (!value++)
			continue;
		*(value - 1) = '\0';
		extension_len = (value - 1) - (color + 1);
		if (ft_strncmp_rev(filename, color + 1, extension_len) == 0) {
			*(value - 1) = '=';
			return (value);
		}
		*(value - 1) = '=';
	}
	return (data->colors.reset);
}

char*	ls_color_get(char* filename, unsigned short mode, unsigned int nlink, bool orphan, t_data* data) {
	char*	color = data->colors.reset;

	if (mode == 0 && orphan)
		return (data->colors.orphan);
	if (nlink > 1)
		color = data->colors.multi_hard_link;
	if (mode & S_IEXEC)
		color = data->colors.exec;
	if (mode & S_ISGID)
		color = data->colors.setgid;
	if (mode & S_ISUID)
		color = data->colors.setuid;
	switch (mode & __S_IFMT)
	{
		case __S_IFDIR:
			if ((mode & S_IWOTH) && (mode & S_ISVTX))
				color = data->colors.sticky_other_writable;
			else if (mode & S_IWOTH)
				color = data->colors.other_writable;
			else if (mode & S_ISVTX)
				color = data->colors.sticky;
			else
				color = data->colors.dir;
			break;
		
		case __S_IFBLK:
			color = data->colors.block_device;
			break;

		case __S_IFCHR:
			color = data->colors.character_device;
			break;

		case __S_IFIFO:
			color = data->colors.fifo;
			break;

		case __S_IFLNK:
			if (orphan)
				color = data->colors.orphan;
			else
				color = data->colors.link;
			break;

		case __S_IFSOCK:
			color = data->colors.socket;
			break;

		default:
			if (color == data->colors.reset)
				color = ls_color_get_by_extension(filename, data);
			break;
	}
	return (color);
}

