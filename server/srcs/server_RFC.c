#include    	"../includes/server.h"

zframe_t 		*identify(t_server_info *server_info)
{
	if ((search_by_name(server_info->game_info.list_players, server_info->parsed_param) == NULL) && server_info->nb_clients >= 4) {
		return zframe_from("KO|game full");
	}
	if (search_by_name(server_info->game_info.list_players, server_info->parsed_param) == NULL) {
		printf("Adding '%s' to list_player.\n", server_info->parsed_param);
		position_to_fill(server_info);
		server_info->game_info.list_players = prepend(server_info->game_info.list_players, server_info->parsed_param, server_info->player_info);
		printf("Added '%s' to list_player.\n", server_info->parsed_param);
		server_info->nb_clients++;
		return zframe_from("OK|You are in !");
	}
	return zframe_from("KO|identity already exists");
}

zframe_t 		*forward(t_server_info *server_info)
{
	t_player 	*tmp;

	if ((tmp = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	printf("Moving '%s' forward.\n", server_info->identity);
	return go_forward(server_info, tmp);
}

zframe_t 		*backward(t_server_info *server_info)
{
	t_player 	*tmp;

	if ((tmp = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	printf("Moving '%s' backward.\n", server_info->identity);
    return go_backward(server_info, tmp);
}

zframe_t 		*leftfwd(t_server_info *server_info)
{
	t_player 	*tmp;
	zframe_t 	*res;

	if ((tmp = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	printf("Rotating left, then moving '%s' forward.\n", server_info->identity);
    rotate_left(tmp);
    res = go_forward(server_info, tmp);
    if (zframe_strdup(res)[0] == 'K') {
    	rotate_right(tmp);
    }
    return res;
}

zframe_t 		*rightfwd(t_server_info *server_info)
{
	t_player 	*tmp;
	zframe_t 	*res;

	if ((tmp = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	printf("Rotating right, then moving '%s' forward.\n", server_info->identity);
    rotate_right(tmp);
    res = go_forward(server_info, tmp);
    if (zframe_strdup(res)[0] == 'K') {
    	rotate_left(tmp);
    }
    return res;
}

zframe_t 		*right(t_server_info *server_info)
{
	t_player 	*tmp;

	if ((tmp = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	rotate_right(tmp);
	return zframe_from("OK|Rotated right");
}

zframe_t 		*left(t_server_info *server_info)
{
	t_player 	*tmp;

	if ((tmp = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	rotate_left(tmp);
	return zframe_from("OK|Rotated left");
}

zframe_t 		*looking(t_server_info *server_info)
{
	t_player	*tmp;
	char		buff[5];

	if ((tmp = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	sprintf(buff, "OK|%d", tmp->looking);
	return zframe_from(buff);
}

zframe_t 		*gather(t_server_info *server_info)
{
	(void)server_info;
	return zframe_from("OK|Gather");
}

zframe_t 		*watch(t_server_info *server_info)
{
	(void)server_info;
	return zframe_from("OK|Watch");
}

zframe_t 		*attack(t_server_info *server_info)
{
	(void)server_info;
	return zframe_from("OK|Attack");
}

zframe_t 		*selfid(t_server_info *server_info)
{
	t_player 	*tmp;

	if ((tmp = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}

	char 		buff[strlen(tmp->name + 3)];
	
	sprintf(buff, "OK|%s", tmp->name);
	return zframe_from(buff);
}

zframe_t 		*selfstats(t_server_info *server_info)
{
	t_player 	*tmp;
	char 		buff[7];

	if ((tmp = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	sprintf(buff, "OK|%d", tmp->energy);
	return zframe_from(buff);
}

zframe_t 		*inspect(t_server_info *server_info)
{
	(void)server_info;
	return zframe_from("OK|Inspect");
}

zframe_t 		*next(t_server_info *server_info)
{
	(void)server_info;
	return zframe_from("OK|Next");
}

zframe_t 		*jump(t_server_info *server_info)
{
	(void)server_info;
	return zframe_from("OK|Jump");
}
