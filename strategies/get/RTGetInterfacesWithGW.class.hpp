/*
 * Created by m0nt4lb4n0 on 07/02/24.
 */

#pragma once
#include "strategies/RTStrategy.class.hpp"

class	RTGetInterfacesWithGW : public RTStrategy {
	private:
	public:
		struct msghdr&				forgeRequest(struct sockaddr_nl& dst) override {
			_nlmsgptr->nlmsg_len = NLMSG_LENGTH(sizeof(struct nlmsghdr));
			_nlmsgptr->nlmsg_type = RTM_GETROUTE;
			_nlmsgptr->nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
			return RTStrategy::forgeRequest(dst);
		};

		std::vector<std::string>	parseMessage(struct nlmsghdr* ptr, int bytes) override {
			std::vector<std::string>	ret{};
			struct rtattr*				tb[IFLA_MAX + 1];
			struct rtattr*				rta;
			char 						gateway[IFNAMSIZ];
			char 						interface[IFNAMSIZ];
			memset(gateway, 0, IFNAMSIZ);
			memset(interface, 0, IFNAMSIZ);

			for (; NLMSG_OK(ptr, bytes); ptr = NLMSG_NEXT(ptr, bytes)) {
				auto route_entry = (struct rtmsg*) NLMSG_DATA(ptr);
				rta = (struct rtattr*)RTM_RTA(route_entry);
				int len = RTM_PAYLOAD(ptr);

				parseAttr(tb, RTA_MAX, rta, len);
				if (tb[RTA_GATEWAY] && tb[RTA_OIF]) {
					inet_ntop(AF_INET, RTA_DATA(tb[RTA_GATEWAY]),
							  gateway, sizeof(gateway));
					if_indextoname(*(int*)RTA_DATA(tb[RTA_OIF]), interface);
					if (*gateway && *interface) {
						ret.emplace_back(interface);
					}
					memset(gateway, 0, IFNAMSIZ);
					memset(interface, 0, IFNAMSIZ);
				}
			}
			return ret;
		}
};