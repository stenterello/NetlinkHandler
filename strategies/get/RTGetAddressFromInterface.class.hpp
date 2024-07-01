/*
 * Created by m0nt4lb4n0 on 08/02/24.
 */

#pragma once
#include "strategies/RTStrategy.class.hpp"

class	RTGetAddressFromInterface : public RTStrategy {
	private:
		const char*	_interface;
	public:
		explicit					RTGetAddressFromInterface(const char* iface) : RTStrategy(), _interface(iface) {};

		struct msghdr&				forgeRequest(struct sockaddr_nl& dst) override {
			_nlmsgptr->nlmsg_len = NLMSG_LENGTH(sizeof(struct ifinfomsg));
			_nlmsgptr->nlmsg_type = RTM_GETADDR;
			_nlmsgptr->nlmsg_flags = NLM_F_REQUEST | NLM_F_ROOT;
			_rtgenmsg = (struct rtgenmsg*)_buffer + sizeof(*_nlmsgptr);
			_rtgenmsg->rtgen_family = AF_PACKET;

			_ifi = (struct ifinfomsg*) NLMSG_DATA(&_nlmsgptr);
			memset(_ifi, 0, sizeof(struct ifaddrmsg));
			_ifi->ifi_family = AF_UNSPEC;
			_ifi->ifi_index = static_cast<int>(if_nametoindex(_interface));

			return RTStrategy::forgeRequest(dst);
		};

		std::vector<std::string>	parseMessage(struct nlmsghdr* ptr, int bytes) override {
			std::vector<std::string>	ret{};
			struct rtattr*				tb[IFLA_MAX + 1];
			struct rtattr*				rta;

			for (; NLMSG_OK(ptr, bytes); ptr = NLMSG_NEXT(ptr, bytes)) {
				auto ifa_entry = (struct ifinfomsg*) NLMSG_DATA(ptr);
				rta = (struct rtattr*)IFLA_RTA(ifa_entry);
				int len = IFLA_PAYLOAD(ptr);

				parseAttr(tb, RTA_MAX, rta, len);


				// this control is here because of this, I think
				// https://lists.infradead.org/pipermail/libnl/2013-June/001014.html

				if (tb[IFA_LOCAL] && ifa_entry->ifi_index == _ifi->ifi_index) {
					char interface[INET_ADDRSTRLEN];
					inet_ntop(AF_INET, RTA_DATA(tb[IFA_LOCAL]), interface, sizeof(interface));
					ret.emplace_back(interface);
					memset(interface, 0, INET_ADDRSTRLEN);
				}
			}
			return ret;
		}
};