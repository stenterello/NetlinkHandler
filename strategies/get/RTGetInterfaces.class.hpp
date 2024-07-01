/*
 * Created by m0nt4lb4n0 on 01/02/24.
 */

#pragma once
#include "strategies/RTStrategy.class.hpp"

class	RTGetInterfaces : public RTStrategy {
	private:
	public:
									~RTGetInterfaces() override = default;

		struct msghdr&				forgeRequest(struct sockaddr_nl& dst) override {
			_nlmsgptr->nlmsg_len = NLMSG_LENGTH(sizeof(struct rtgenmsg));
			_nlmsgptr->nlmsg_type = RTM_GETLINK;
			_nlmsgptr->nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
			_rtgenmsg = (struct rtgenmsg*)_buffer + sizeof(*_nlmsgptr);
			_rtgenmsg->rtgen_family = AF_PACKET;
			return RTStrategy::forgeRequest(dst);
		};

		std::vector<std::string>	parseMessage(struct nlmsghdr* ptr, int bytes) override {
			std::vector<std::string>	ret{};
			struct rtattr*				tb[IFLA_MAX + 1];
			struct ifinfomsg*			ifi;
			int 						len;

			for (; NLMSG_OK(ptr, bytes); ptr = NLMSG_NEXT(ptr, bytes)) {
				ifi = (struct ifinfomsg*)NLMSG_DATA(ptr);
				len = IFLA_PAYLOAD(ptr);

				if (ifi->ifi_family != AF_UNSPEC) {
					std::cout << "Ignoring non-RTM_NEWLINK" << std::endl;
					return {};
				}
				memset(tb, 0, IFLA_MAX + 1);
				parseAttr(tb, IFLA_MAX, IFLA_RTA(ifi), len);
				if (tb[IFLA_IFNAME]) {
					ret.emplace_back((char *) RTA_DATA(tb[IFLA_IFNAME]));
				}
			}
			return ret;
		}
};