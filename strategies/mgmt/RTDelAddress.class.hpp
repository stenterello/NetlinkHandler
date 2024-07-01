/*
 * Created by m0nt4lb4n0 on 07/02/24.
 */

#pragma once
#include "strategies/mgmt/RTMgmtStrategy.class.hpp"

class	RTDelAddress : public RTMgmtStrategy {
	private:
		const char*		_interface;
		const char*		_ip;
	public:
		RTDelAddress(const char* iface, const char* ip): _interface(iface),
																		_ip(ip) {};

		struct msghdr&				forgeRequest(struct sockaddr_nl& dst) override {
			_nlmsgptr->nlmsg_len = NLMSG_LENGTH(sizeof(struct ifaddrmsg));
			_nlmsgptr->nlmsg_type = RTM_DELADDR;
			_nlmsgptr->nlmsg_flags = NLM_F_CREATE | NLM_F_ACK | NLM_F_REQUEST;
			_rtgenmsg = (struct rtgenmsg*)(_buffer + sizeof(*_nlmsgptr));
			_rtgenmsg->rtgen_family = AF_PACKET;

			_ifa = (struct ifaddrmsg*)NLMSG_DATA(_nlmsgptr);
			memset(_ifa, 0, sizeof(ifaddrmsg));
			_ifa->ifa_family = AF_INET;
			_ifa->ifa_index = if_nametoindex(_interface);
			if (_ifa->ifa_index == 0) {
				std::cerr << "if_nametoindex" << std::endl;
			}
			_ifa->ifa_scope = 0;

			inet_pton(AF_INET, _ip, &_addr);
			_rta = (struct rtattr*)((char*)_ifa + NLMSG_ALIGN(sizeof(struct ifaddrmsg)));
			memset(_rta, 0, sizeof(struct rtattr));
			_rta->rta_type = IFA_LOCAL;
			_rta->rta_len = RTA_LENGTH(sizeof(struct in_addr));
			memcpy(RTA_DATA(_rta), &_addr, sizeof(struct in_addr));
			_nlmsgptr->nlmsg_len = NLMSG_ALIGN(((struct nlmsghdr*)_buffer)->nlmsg_len) + RTA_ALIGN(_rta->rta_len);

			return RTStrategy::forgeRequest(dst);
		};
};