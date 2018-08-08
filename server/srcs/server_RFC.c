#include    "../includes/server.h"

zframe_t *identify(t_server_info *server_info)
{
	if ((search(server_info->game_info->list_players, server_info->parsed_param) == NULL) && server_info->nb_clients >= 4) {
		printf("'%s' Can't go in list_player, because list is full.\n", server_info->parsed_param);
		return zframe_from("KO|game full");
	} else if (search(server_info->game_info->list_players, server_info->parsed_param) == NULL) {
		printf("'%s' added to list_player.\n", server_info->parsed_param);
		server_info->game_info->list_players = prepend(server_info->game_info->list_players, server_info->parsed_param, server_info->player_info);
		server_info->nb_clients++;
		return zframe_from("OK|You are in !");
	} else {
		return zframe_from("KO|identity already exists");
	}
}

zframe_t *forward(t_server_info *server_info)
{
	(void)server_info;
	return zframe_from("OK|Forward");	
}

zframe_t *backward(t_server_info *server_info)
{
	(void)server_info;
	return zframe_from("OK|Backward");	
}

zframe_t *leftfwd(t_server_info *server_info)
{
	(void)server_info;
	return zframe_from("OK|Leftfwd");	
}

zframe_t *rightfwd(t_server_info *server_info)
{
	(void)server_info;
	return zframe_from("OK|Rightfwd");	
}

zframe_t *right(t_server_info *server_info)
{
	(void)server_info;
	return zframe_from("OK|Right");	
}

zframe_t *left(t_server_info *server_info)
{
	(void)server_info;
	return zframe_from("OK|Left");
}

zframe_t *looking(t_server_info *server_info)
{
	(void)server_info;
	return zframe_from("OK|Looking");
}

zframe_t *gather(t_server_info *server_info)
{
	(void)server_info;
	return zframe_from("OK|Gather");
}

zframe_t *watch(t_server_info *server_info)
{
	(void)server_info;
	return zframe_from("OK|Watch");
}

zframe_t *attack(t_server_info *server_info)
{
	(void)server_info;
	return zframe_from("OK|Attack");
}

zframe_t *selfid(t_server_info *server_info)
{
	(void)server_info;
	return zframe_from("OK|Selfid");
}

zframe_t *selfstats(t_server_info *server_info)
{
	(void)server_info;
	return zframe_from("OK|Selfstats");
}

zframe_t *inspect(t_server_info *server_info)
{
	(void)server_info;
	return zframe_from("OK|Inspect");
}

zframe_t *next(t_server_info *server_info)
{
	(void)server_info;
	return zframe_from("OK|Next");
}

zframe_t *jump(t_server_info *server_info)
{
	(void)server_info;
	return zframe_from("OK|Jump");
}
