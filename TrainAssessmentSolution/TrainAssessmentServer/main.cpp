#include "Server.h"

int main(int argc,char* argv[])
{
	ServerBaseConfig config;
	config._commconfig.enable_acceptor();
	config._commconfig.set_listen_port(5050);
	config._framesendconfig.set_print_interval(1000);
	config._engineconfig.enable_slow_log();
	config._engineconfig.set_slow_log_filename("test_server_slow_log.log");
	Server s(config);
	s.start();
	while(true)
	{
		string cmd;
		cin >> cmd;
		if (cmd == "quit")
		{
			break;
		}
		Sleep(1000);
	}
	s.stop();

	return 0;
}