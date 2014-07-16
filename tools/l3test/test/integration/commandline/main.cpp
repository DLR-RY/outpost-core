
// enable hexdezimal arguments
//#define TCLAP_SETBASE_ZERO 1
//#include <tclap/CmdLine.h>

#include "lua/interpreter.h"

//namespace tclap = ::TCLAP ;

int
main(int argc, char* argv[])
{
/*	try {
		tclap::CmdLine cmd(
				"Command description message",
				' ',		// delimiter
				"0",		// version string (not used)
				false);		// don't include --version and --help automatically

		tclap::ValueArg<std::string> nameArg(
				"n", "name",
				"Name to print",
				true,		// required?
				"homer",	// default value
				"string");	// type
		cmd.add(nameArg);

		tclap::SwitchArg reverseSwitch(
				"r", "reverse",
				"Print name backwards",
				cmd,
				true);

		cmd.parse(argc, argv);

		std::string name = nameArg.getValue();
		bool reverseName = reverseSwitch.getValue();

		if (reverseName)
		{
			std::reverse(name.begin(),name.end());
			std::cout << "My name (spelled backwards) is: " << name << std::endl;
		}
		else {
			std::cout << "My name is: " << name << std::endl;
		}
	}
	catch (tclap::ArgException &e) {
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
	}*/

	lua::Interpreter lua;

	lua.openLibraries();

	const std::string filename = "test.lua";
	lua.doFile(filename.c_str());

	//lua::Table table = lua.get<lua::Table>("packet");
	//std::cout << "packet.size = " << table.get<int>("size") << std::endl;

	return 0;
}

