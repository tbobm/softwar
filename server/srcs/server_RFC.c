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
	t_player 	*player;

	if ((player = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	printf("Moving '%s' forward.\n", server_info->identity);
	return go_forward(server_info, player, 1);
}

zframe_t 		*backward(t_server_info *server_info)
{
	t_player 	*player;

	if ((player = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	printf("Moving '%s' backward.\n", server_info->identity);
    return go_backward(server_info, player);
}

zframe_t 		*leftfwd(t_server_info *server_info)
{
	t_player 	*player;
	zframe_t 	*res;

	if ((player = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	printf("Rotating left, then moving '%s' forward.\n", server_info->identity);
    rotate_left(player);
    res = go_forward(server_info, player, 1);
    if (zframe_strdup(res)[0] == 'K') {
    	rotate_right(player);
    }
    return res;
}

zframe_t 		*rightfwd(t_server_info *server_info)
{
	t_player 	*player;
	zframe_t 	*res;

	if ((player = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	printf("Rotating right, then moving '%s' forward.\n", server_info->identity);
    rotate_right(player);
    res = go_forward(server_info, player, 1);
    if (zframe_strdup(res)[0] == 'K') {
    	rotate_left(player);
    }
    return res;
}

zframe_t 		*right(t_server_info *server_info)
{
	t_player 	*player;

	if ((player = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	rotate_right(player);
	return zframe_from("OK|Rotated right");
}

zframe_t 		*left(t_server_info *server_info)
{
	t_player 	*player;

	if ((player = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	rotate_left(player);
	return zframe_from("OK|Rotated left");
}

zframe_t 		*looking(t_server_info *server_info)
{
	t_player	*player;
	char		buff[5];

	if ((player = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	sprintf(buff, "OK|%d", player->looking);
	return zframe_from(buff);
}

zframe_t 		*gather(t_server_info *server_info)
{
	(void)server_info;
	return zframe_from("OK|Gather");
}

zframe_t 		*watch(t_server_info *server_info)
{
	t_player	*player;

	if ((player = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	return watch_vision(server_info, player);
}

zframe_t 		*attack(t_server_info *server_info)
{
	(void)server_info;
	return zframe_from("OK|Attack");
}

zframe_t 		*selfid(t_server_info *server_info)
{
	t_player 	*player;

	if ((player = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}

	char 		buff[strlen(player->name + 3)];
	
	sprintf(buff, "OK|%s", player->name);
	return zframe_from(buff);
}

zframe_t 		*selfstats(t_server_info *server_info)
{
	t_player 	*player;
	char 		buff[7];

	if ((player = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	sprintf(buff, "OK|%d", player->energy);
	return zframe_from(buff);
}

zframe_t 		*inspect(t_server_info *server_info)
{
	t_player 	*player;
	t_player 	*inspected_player;
	char 		buff[7];

	if ((player = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	if ((inspected_player = search_by_name(server_info->game_info.list_players, server_info->parsed_param)) == NULL) {
		return zframe_from("KO|process does not exist");
	}
	sprintf(buff, "OK|%d", inspected_player->energy);
	return zframe_from(buff);
}

zframe_t 		*next(t_server_info *server_info)
{
	(void)server_info;
	return zframe_from("OK|Next");
}

zframe_t 		*jump(t_server_info *server_info)
{
	t_player 	*player;
	zframe_t 	*res;

	if ((player = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	res = go_forward(server_info, player, 2);
    if (zframe_strdup(res)[0] == 'O') {
    	player->energy -= 2;
    }
	printf("Jumping '%s' 2 cells forward.\n", server_info->identity);
	return res;
}
