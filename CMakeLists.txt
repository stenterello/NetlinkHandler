cmake_minimum_required(VERSION 3.22)
project(NetlinkHandler)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(NetlinkHandler
		main.cpp
		NetlinkHandler.class.cpp
		strategies/get/NLGetP2PDeviceInterfaces.class.hpp
		strategies/get/NLGetP2PWithoutDeviceInterfaces.class.hpp
		strategies/get/RTGetInterfacesWithGW.class.hpp
		strategies/mgmt/RTSetGateway.class.hpp
		strategies/get/RTGetGWFromInterface.class.hpp
		strategies/get/RTGetAddressFromInterface.class.hpp
		strategies/get/RTGetSubnetFromInterface.class.hpp
		strategies/mgmt/RTSetAddressAndSubnet.class.hpp
		strategies/mgmt/RTDelAddress.class.hpp
		strategies/mgmt/RTDelGateway.class.hpp
		strategies/mgmt/RTMgmtStrategy.class.hpp
		strategies/NFStrategy.class.hpp
		strategies/mgmt/NFCreateTable.class.hpp
		strategies/get/NFGetTable.class.hpp
		strategies/mgmt/NFDeleteTable.class.hpp
		strategies/mgmt/NFCreateNATChain.class.hpp
		strategies/mgmt/NFDeleteNATChain.class.hpp)
