#include "utils.hpp"

#define INTERFACE1	"wlo1"
#define INTERFACE2	"wlx38d54706b71a"
#define INTERFACE3	"eno2"
#define INTERFACE4	"enp8s0"

int	main() {
	NetlinkHandler	nh;

//	std::cout << std::endl << "All interfaces: " << std::endl;
//	nh.setStrategy(new RTGetInterfaces());
//	std::vector<std::string>	ifaces = nh.execStrategy();
//	std::for_each(ifaces.begin(), ifaces.end(), [](const std::string& iface){ std::cout << iface << std::endl; });
//
//	std::cout << std::endl << "Wi-Fi interfaces: " << std::endl;
//	nh.setStrategy(new NLGetWirelessInterfaces());
//	std::vector<std::string>	wifiIfaces = nh.execStrategy();
//	std::for_each(wifiIfaces.begin(), wifiIfaces.end(), [](const std::string& iface){ std::cout << iface << std::endl; });
//
//	std::cout << std::endl << "Wi-Fi Physic devices: " << std::endl;
//	nh.setStrategy(new NLGetWirelessPhy());
//	std::vector<std::string>	wifiPhy = nh.execStrategy();
//	std::for_each(wifiPhy.begin(), wifiPhy.end(), [](const std::string& iface){ std::cout << iface << std::endl; });
//
//	std::cout << std::endl << "P2P-Client and P2P-Go devices: " << std::endl;
//	nh.setStrategy(new NLGetP2PWithoutDevicePhy());
//	std::vector<std::string>	p2pIfaces = nh.execStrategy();
//	std::for_each(p2pIfaces.begin(), p2pIfaces.end(), [](const std::string& iface){ std::cout << iface << std::endl; });
//
//	std::cout << std::endl << "P2P-Device devices: " << std::endl;
//	nh.setStrategy(new NLGetP2PDevicePhy());
//	std::vector<std::string>	p2pDevPhy = nh.execStrategy();
//	std::for_each(p2pDevPhy.begin(), p2pDevPhy.end(), [](const std::string& iface){ std::cout << iface << std::endl; });
//
//	std::cout << std::endl << "Getting P2P-Device indexes... ";
//	nh.setStrategy(new NLGetP2PDevicePhy(true));
//	std::vector<std::string>	p2pDevPhyIdx = nh.execStrategy();
//	std::for_each(p2pDevPhyIdx.begin(), p2pDevPhyIdx.end(), [](const std::string& iface){ std::cout << iface << " "; });
//	std::cout << std::endl;
//
//	std::cout << std::endl << "Getting P2P-Client and P2P-GO indexes... ";
//	nh.setStrategy(new NLGetP2PWithoutDevicePhy(true));
//	std::vector<std::string>	p2pNoDevPhyIdx = nh.execStrategy();
//	std::for_each(p2pNoDevPhyIdx.begin(), p2pNoDevPhyIdx.end(), [](const std::string& iface){ std::cout << iface << " "; });
//	std::cout << std::endl;
//
//	std::cout << std::endl << "P2P-Device interfaces: " << std::endl;
//	nh.setStrategy(new NLGetP2PDeviceInterfaces(std::move(p2pDevPhyIdx)));
//	std::vector<std::string>	p2pDevIfaces = nh.execStrategy();
//	std::for_each(p2pDevIfaces.begin(), p2pDevIfaces.end(), [](const std::string& iface){ std::cout << iface << std::endl; });
//
//	std::cout << std::endl << "P2P-Client and P2P-GO interfaces: " << std::endl;
//	nh.setStrategy(new NLGetP2PWithoutDeviceInterfaces(std::move(p2pNoDevPhyIdx)));
//	std::vector<std::string>	p2pNoDevIfaces = nh.execStrategy();
//	std::for_each(p2pNoDevIfaces.begin(), p2pNoDevIfaces.end(), [](const std::string& iface){ std::cout << iface << std::endl; });
//
//	std::cout << std::endl << "Getting available interfaces: (no single method)" << std::endl;
//	std::for_each(p2pDevIfaces.begin(), p2pDevIfaces.end(), [](const std::string& iface){ std::cout << iface << std::endl; });
//	std::for_each(p2pNoDevIfaces.begin(), p2pNoDevIfaces.end(), [](const std::string& iface){ std::cout << iface << std::endl; });
//
//	std::cout << std::endl << "Interfaces with default Gateway: " << std::endl;
//	nh.setStrategy(new RTGetInterfacesWithGW());
//	std::vector<std::string>	activeInterfaces = nh.execStrategy();
//	std::for_each(activeInterfaces.begin(), activeInterfaces.end(), [](const std::string& iface){ std::cout << iface << std::endl; });
//
//	std::cout << std::endl << "Getting GW from interface: " << INTERFACE2 << std::endl;
//	nh.setStrategy(new RTGetGWFromInterface(INTERFACE2));
//	std::vector<std::string>	GWFromInterface = nh.execStrategy();
//	std::for_each(GWFromInterface.begin(), GWFromInterface.end(), [](const std::string& iface){ std::cout << iface << std::endl; });
//
//	std::cout << std::endl << "Getting address from interface: " << INTERFACE2 << std::endl;
//	nh.setStrategy(new RTGetAddressFromInterface(INTERFACE2));
//	std::vector<std::string>	addressFromInterface = nh.execStrategy();
//	std::for_each(addressFromInterface.begin(), addressFromInterface.end(), [](const std::string& iface){ std::cout << iface << std::endl; });
//
//	std::cout << std::endl << "Getting subnet from interface: " << INTERFACE2 << std::endl;
//	nh.setStrategy(new RTGetSubnetFromInterface(INTERFACE2));
//	std::vector<std::string>	subnetFromInterface = nh.execStrategy();
//	std::for_each(subnetFromInterface.begin(), subnetFromInterface.end(), [](const std::string& iface){ std::cout << iface << std::endl; });
//
//	std::cout << std::endl << "Adding address" << "10.10.10.10/25" << "on interface: " << INTERFACE2 << std::endl;
//	nh.setStrategy(new RTSetAddressAndSubnet(INTERFACE2, "10.10.10.10", 25));
//	nh.execStrategy();
//
//	std::cout << std::endl << "Set Gateway on interface: " << INTERFACE2 << std::endl;
//	nh.setStrategy(new RTSetGateway(INTERFACE2, "10.10.10.1"));
//	nh.execStrategy();
//
//	std::cout << std::endl << "Delete Gateway on interface: " << INTERFACE2 << std::endl;
//	nh.setStrategy(new RTDelGateway(INTERFACE2, "10.10.10.1"));
//	nh.execStrategy();
//
//	std::cout << std::endl << "Deleting address on interface: " << INTERFACE2 << std::endl;
//	nh.setStrategy(new RTDelAddress(INTERFACE2, "10.10.10.10"));
//	nh.execStrategy();
//
	std::cout << "Getting test table with NETLINK_NETFILTER" << std::endl;
	nh.setStrategy(new NFGetTable("test"));
	nh.execStrategy();

//	std::cout << "Adding test table with NETLINK_NETFILTER" << std::endl;
//	nh.setStrategy(new NFCreateTable("test\0"));
//	nh.execStrategy();

//	std::cout << "Deleting test table with NETLINK_NETFILTER" << std::endl;
//	nh.setStrategy(new NFDeleteTable("test\0"));
//	nh.execStrategy();

//	std::cout << "Adding chain to test table with NETLINK_NETFILTER" << std::endl;
//	nh.setStrategy(new NFCreateNATChain("test\0", "test_chain\0", NF_INET_POST_ROUTING, 100));
//	nh.execStrategy();

	std::cout << "Deleting chain to test table with NETLINK_NETFILTER" << std::endl;
	nh.setStrategy(new NFDeleteNATChain("test\0", "test_chain\0"));
	nh.execStrategy();

	return 0;
}