/*
 * Created by m0nt4lb4n0 on 07/02/24.
 */

#pragma once
#include "strategies/mgmt/RTMgmtStrategy.class.hpp"

class	RTSetGateway : public RTMgmtStrategy {
	protected:
		const char*		_interface;
		const char*		_ip;
		int 			_ifaceIndex{};
	public:
		RTSetGateway(const char* interface, const char* ip): _interface(interface), _ip(ip) {};

		struct msghdr&				forgeRequest(struct sockaddr_nl& dst) override {
			char addr[INET_ADDRSTRLEN];

			_nlmsgptr->nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg));
			_nlmsgptr->nlmsg_type = RTM_NEWROUTE;
			_nlmsgptr->nlmsg_flags = NLM_F_CREATE | NLM_F_ACK | NLM_F_REQUEST | NLM_F_REPLACE;

			_rtm = (struct rtmsg*)NLMSG_DATA(_nlmsgptr);
			_rtm->rtm_family = AF_INET;
			_rtm->rtm_table = RT_TABLE_MAIN;
			_rtm->rtm_protocol = RTPROT_STATIC;
			_rtm->rtm_scope = RT_SCOPE_UNIVERSE;
			_rtm->rtm_type = RTN_UNICAST;

			_rta = (struct rtattr *)RTM_RTA(_rtm);
			_rta->rta_type = RTA_GATEWAY;
			inet_pton(AF_INET, _ip, &addr);
			_rta->rta_len = RTA_LENGTH(sizeof(addr));
			std::cout << "Gateway addr to set " << _ip << std::endl;
			memcpy(RTA_DATA(_rta), &addr, sizeof(addr));

			_nlmsgptr->nlmsg_len += RTA_ALIGN(_rta->rta_len);

			_rta = (struct rtattr *)((char *)_rta + NLMSG_ALIGN(_rta->rta_len));
			_rta->rta_type = RTA_OIF;
			_rta->rta_len = RTA_LENGTH(sizeof(int));

			std::cout << "interface on which gateway will be set " << _interface << std::endl;
			_ifaceIndex = (int)if_nametoindex(_interface);
			memcpy(RTA_DATA(_rta), &_ifaceIndex, sizeof(int));

			_nlmsgptr->nlmsg_len += RTA_ALIGN(_rta->rta_len);

			return RTStrategy::forgeRequest(dst);
		};
};