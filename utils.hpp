#pragma once
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/genetlink.h>
#include <linux/nl80211.h>
#include <arpa/inet.h>
#include <linux/netfilter.h>
#include <linux/netfilter/nfnetlink.h>
#include <linux/netfilter/nf_tables.h>
#include <linux/if_addr.h>
#include <sys/socket.h>
#include <net/if.h>
#include <unistd.h>
#include <memory.h>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <bitset>
#include <iostream>


#define BUFFER_SIZE					32768
#define NLA_DATA(nla)				((void *)((char *)(nla) + NLA_HDRLEN))
#define NLA_LENGTH(nla)				(nla->nla_len - NLA_HDRLEN)

#include "NLGetFamilyId.class.hpp"
#include "strategies/get/RTGetInterfaces.class.hpp"
#include "strategies/get/NLGetWirelessInterfaces.class.hpp"
#include "strategies/get/NLGetWirelessPhy.class.hpp"
#include "strategies/get/NLGetP2PWithoutDevicePhy.class.hpp"
#include "strategies/get/NLGetP2PDevicePhy.class.hpp"
#include "strategies/get/NLGetP2PDeviceInterfaces.class.hpp"
#include "strategies/get/NLGetP2PWithoutDeviceInterfaces.class.hpp"
#include "strategies/get/RTGetInterfacesWithGW.class.hpp"
#include "strategies/get/RTGetGWFromInterface.class.hpp"
#include "strategies/get/RTGetAddressFromInterface.class.hpp"
#include "strategies/get/RTGetSubnetFromInterface.class.hpp"
#include "strategies/get/NFGetTable.class.hpp"
#include "strategies/mgmt/RTSetAddressAndSubnet.class.hpp"
#include "strategies/mgmt/RTSetGateway.class.hpp"
#include "strategies/mgmt/RTDelAddress.class.hpp"
#include "strategies/mgmt/RTDelGateway.class.hpp"
#include "strategies/mgmt/NFCreateTable.class.hpp"
#include "strategies/mgmt/NFDeleteTable.class.hpp"
#include "strategies/mgmt/NFCreateNATChain.class.hpp"
#include "strategies/mgmt/NFDeleteNATChain.class.hpp"
#include "NetlinkHandler.class.hpp"