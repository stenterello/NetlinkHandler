/*
 * Created by m0nt4lb4n0 on 08/02/24.
 */

#pragma once
#include "strategies/RTStrategy.class.hpp"

class	RTGetGWFromInterface : public RTStrategy {
	private:
		const char*					_interface;
		char 						_gatewayStr[INET_ADDRSTRLEN]{};
		char						_if_name[IFNAMSIZ]{};
	public:
		explicit					RTGetGWFromInterface(const char* iface) : RTStrategy(), _interface(iface) {};

		struct msghdr&				forgeRequest(struct sockaddr_nl& dst) override {
			_nlmsgptr->nlmsg_len = NLMSG_LENGTH(sizeof(struct rtgenmsg));
			_nlmsgptr->nlmsg_type = RTM_GETROUTE;
			_nlmsgptr->nlmsg_flags = NLM_F_REQUEST | NLM_F_ROOT;
			_rtgenmsg = (struct rtgenmsg*)(_buffer + sizeof(*_nlmsgptr));
			_rtgenmsg->rtgen_family = AF_PACKET;

			_rtm = (struct rtmsg*)NLMSG_DATA(&_nlmsgptr);
			memset(_rtm, 0, sizeof(*_rtm));
			_rtm->rtm_family = AF_INET;
			_rtm->rtm_table = RT_TABLE_MAIN;
			_rtm->rtm_protocol = RTPROT_UNSPEC;
			_rtm->rtm_type = RTN_UNICAST;
			_rtm->rtm_scope = RT_SCOPE_NOWHERE;

			return RTStrategy::forgeRequest(dst);
		};

		std::vector<std::string>	parseMessage(struct nlmsghdr* ptr, int bytes) override {
			std::vector<std::string>	ret{};
			struct rtattr*				tb[RTM_MAX + 1];
			struct rtattr*				rta;

			for (; NLMSG_OK(ptr, bytes); ptr = NLMSG_NEXT(ptr, bytes)) {
				auto route_entry = (struct rtmsg*) NLMSG_DATA(ptr);
				rta = (struct rtattr*)RTM_RTA(route_entry);
				int len = RTM_PAYLOAD(ptr);

				parseAttr(tb, RTA_MAX, rta, len);
				if (tb[RTA_GATEWAY])
					inet_ntop(AF_INET, RTA_DATA(tb[RTA_GATEWAY]), _gatewayStr, INET_ADDRSTRLEN);
				if (tb[RTA_OIF])
					if_indextoname(*(int*)RTA_DATA(tb[RTA_OIF]), _if_name);
				if (*_gatewayStr && *_if_name) {
					if (std::strlen(_if_name) == std::strlen(_interface)) {
						int 	diff = std::strncmp(_interface, _if_name, std::strlen(_interface));
						if (!diff) {
							ret.emplace_back(_gatewayStr);
							memset(_gatewayStr, 0, sizeof(_gatewayStr));
							memset(_if_name, 0, sizeof(_if_name));
							break ;
						}
					}
				}
			}
			return ret;
		}
};