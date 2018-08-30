#include    	"../includes/server.h"

static zframe_t *why_cant_act(int reason)
{
	switch (reason) {
		case 1 :
			return zframe_from("KO|game not started");
		case 2 :
			return zframe_from("KO|process dead");
		case 3 :
			return zframe_from("KO|process stunned");
		case 4 :
			return zframe_from("KO|not enough action points");
	}
	return zframe_from("KO|error while trying to make an action");
}

static int 		can_act(uint game_status, t_player *player, float action_point)
{
	if (game_status == 0) {
		return 1;
	}
	if (player->energy == 0) {
		return 2;
	}
	if (player->stun_duration > 0) {
		return 3;
	}
	if (player->action - action_point < 0.0f) {
		return 4;
	}
	return 0;
}

static int 		has_enough_energy(t_player *player, uint needed_energy)
{
	if (player->energy >= needed_energy) {
		return 1;
	}
	return 0;
}

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
		if (server_info->nb_clients == 4) {
			server_info->game_info.game_status = 1;
			server_info->game_info.map_size = (uint)server_info->args->size;
		}
		return zframe_from("OK|You are in !");
	}
	return zframe_from("KO|identity already exists");
}

zframe_t 		*forward(t_server_info *server_info)
{
	t_player 	*player;
	int 		reason;

	reason = 0;
	if ((player = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	if ((reason = can_act(server_info->game_info.game_status, player, 0.5f)) != 0) {
		return why_cant_act(reason);
	}
	printf("Moving '%s' forward.\n", server_info->identity);
	player->action -= 0.5f;
	return go_forward(server_info, player, 1);
}

zframe_t 		*backward(t_server_info *server_info)
{
	t_player 	*player;
	int 		reason;

	reason = 0;

	if ((player = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	if ((reason = can_act(server_info->game_info.game_status, player, 1.0f)) != 0) {
		return why_cant_act(reason);
	}
	printf("Moving '%s' backward.\n", server_info->identity);
	player->action -= 1.0f;
    return go_backward(server_info, player);
}

zframe_t 		*leftfwd(t_server_info *server_info)
{
	t_player 	*player;
	zframe_t 	*res;
	int 		reason;

	reason = 0;
	if ((player = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	if ((reason = can_act(server_info->game_info.game_status, player, 1.0f)) != 0) {
		return why_cant_act(reason);
	}
	printf("Rotating left, then moving '%s' forward.\n", server_info->identity);
    rotate_left(player);
    res = go_forward(server_info, player, 1);
    if (zframe_strdup(res)[0] == 'K') {
    	rotate_right(player);
    } else {
		player->action -= 1.0f;
    }
    return res;
}

zframe_t 		*rightfwd(t_server_info *server_info)
{
	t_player 	*player;
	zframe_t 	*res;
	int 		reason;

	reason = 0;
	if ((player = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	if ((reason = can_act(server_info->game_info.game_status, player, 1.0f)) != 0) {
		return why_cant_act(reason);
	}
	printf("Rotating right, then moving '%s' forward.\n", server_info->identity);
    rotate_right(player);
    res = go_forward(server_info, player, 1);
    if (zframe_strdup(res)[0] == 'K') {
    	rotate_left(player);
    } else {
		player->action -= 1.0f;
    }
    return res;
}

zframe_t 		*right(t_server_info *server_info)
{
	t_player 	*player;
	int 		reason;

	reason = 0;
	if ((player = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	if ((reason = can_act(server_info->game_info.game_status, player, 0.5f)) != 0) {
		return why_cant_act(reason);
	}
	player->action -= 0.5f;
	rotate_right(player);
	return zframe_from("OK|Rotated right");
}

zframe_t 		*left(t_server_info *server_info)
{
	t_player 	*player;
	int 		reason;

	reason = 0;
	if ((player = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	if ((reason = can_act(server_info->game_info.game_status, player, 0.5f)) != 0) {
		return why_cant_act(reason);
	}
	player->action -= 0.5f;
	rotate_left(player);
	return zframe_from("OK|Rotated left");
}

zframe_t 		*looking(t_server_info *server_info)
{
	t_player	*player;
	char		buff[5];
	int 		reason;

	reason = 0;
	if ((player = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	if ((reason = can_act(server_info->game_info.game_status, player, 0.0f)) != 0) {
		return why_cant_act(reason);
	}
	sprintf(buff, "OK|%d", player->looking);
	return zframe_from(buff);
}

zframe_t 		*gather(t_server_info *server_info)
{
	t_player	*player;
	int 		reason;

	reason = 0;
	if ((player = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	if ((reason = can_act(server_info->game_info.game_status, player, 1.0f)) != 0) {
		return why_cant_act(reason);
	}
	player->action -= 1.0f;
	return gather_energy(server_info, player);
}

zframe_t 		*watch(t_server_info *server_info)
{
	t_player	*player;
	int 		reason;

	reason = 0;
	if ((player = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	if ((reason = can_act(server_info->game_info.game_status, player, 0.0f)) != 0) {
		return why_cant_act(reason);
	}
	return watch_vision(server_info, player);
}

zframe_t 		*attack(t_server_info *server_info)
{
	t_player	*player;
	int 		reason;

	reason = 0;
	if ((player = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	if ((reason = can_act(server_info->game_info.game_status, player, 0.5f)) != 0) {
		return why_cant_act(reason);
	}
	if (has_enough_energy(player, 1) == 0) {
		return zframe_from("KO|process lack of energy");
	}
	player->action -= 0.5f;
	player->energy -= 1;
	return cone_attack(server_info, player);
}

zframe_t 		*selfid(t_server_info *server_info)
{
	t_player 	*player;
	int 		reason;

	reason = 0;
	if ((player = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	if ((reason = can_act(server_info->game_info.game_status, player, 0.0f)) != 0) {
		return why_cant_act(reason);
	}
	char 		buff[strlen(player->name + 3)];
	
	sprintf(buff, "OK|%s", player->name);
	return zframe_from(buff);
}

zframe_t 		*selfstats(t_server_info *server_info)
{
	t_player 	*player;
	char 		buff[7];
	int 		reason;

	reason = 0;
	if ((player = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	if ((reason = can_act(server_info->game_info.game_status, player, 0.0f)) != 0) {
		return why_cant_act(reason);
	}
	sprintf(buff, "OK|%d", player->energy);
	return zframe_from(buff);
}

zframe_t 		*inspect(t_server_info *server_info)
{
	t_player 	*player;
	t_player 	*inspected_player;
	char 		buff[7];
	int 		reason;

	reason = 0;
	if ((player = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	if ((reason = can_act(server_info->game_info.game_status, player, 0.5f)) != 0) {
		return why_cant_act(reason);
	}
	if ((inspected_player = search_by_name(server_info->game_info.list_players, server_info->parsed_param)) == NULL) {
		return zframe_from("KO|process does not exist");
	}
	player->action -= 0.5f;
	sprintf(buff, "OK|%d", inspected_player->energy);
	return zframe_from(buff);
}

zframe_t 		*next(t_server_info *server_info)
{
	t_player 	*player;

	if ((player = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	if (server_info->game_info.game_status == 0) {
		return zframe_from("KO|game not started");
	}
	player->action = 0.0f;
	return zframe_from("OK|process waiting next cycle");
}

zframe_t 		*jump(t_server_info *server_info)
{
	t_player 	*player;
	zframe_t 	*res;
	int 		reason;

	reason = 0;
	if ((player = search_by_name(server_info->game_info.list_players, server_info->identity)) == NULL) {
		return zframe_from("KO|identity unknown");
	}
	if ((reason = can_act(server_info->game_info.game_status, player, 0.0f)) != 0) {
		return why_cant_act(reason);
	}
	if (has_enough_energy(player, 2) == 0) {
		return zframe_from("KO|process lack of energy");
	}
	res = go_forward(server_info, player, 2);
    if (zframe_strdup(res)[0] == 'O') {
    	player->energy -= 2;
    }
	printf("Jumping '%s' 2 cells forward.\n", server_info->identity);
	return res;
}
