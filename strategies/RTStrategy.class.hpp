/*
 * Created by m0nt4lb4n0 on 06/02/24.
 */

#pragma once
#include "NetlinkStrategy.class.hpp"

class	RTStrategy : public NetlinkStrategy {
	protected:
		struct rtgenmsg*			_rtgenmsg{};
		struct ifaddrmsg*			_ifa{};
		struct ifinfomsg*			_ifi{};
		struct rtmsg*				_rtm{};
		struct in_addr				_addr{};
		struct rtattr*				_rta{};
		std::vector<std::string>	_interfaces{};
	public:
											RTStrategy() : NetlinkStrategy(NETLINK_ROUTE) {};

		std::vector<std::string>			parseResponse(char* recvBuffer, ssize_t bytes) override {
			std::vector<std::string>	ret{};

			for (auto ptr = (struct nlmsghdr*)recvBuffer; NLMSG_OK(ptr, ptr->nlmsg_len); ptr = NLMSG_NEXT(ptr, ptr->nlmsg_len)) {
				switch (ptr->nlmsg_type) {
					case NLMSG_ERROR:{
						auto err = (struct nlmsgerr *) NLMSG_DATA(ptr);
						errno = -err->error;
						printf("errno = %s \n", strerror(errno));
						return ret;
					}
					case NLMSG_DONE: return ret;
					default: {
						std::vector<std::string>	newValues = this->parseMessage(ptr, (int)bytes);
						for (const auto& elem : newValues) {
							if (std::find(_interfaces.begin(), _interfaces.end(), elem) == _interfaces.end()) {
								_interfaces.emplace_back(elem);
								ret.emplace_back(elem);
							}
						}
						break ;
					}
				}
			}

			return ret;
		}

		static void							parseAttr(struct rtattr* tb[], int max, struct rtattr* rta, int len) {
			memset(tb, 0, sizeof(struct rtattr*) * (max + 1));

			for (; RTA_OK(rta, len); rta = RTA_NEXT(rta, len)) {
				if (rta->rta_type <= max) {
					tb[rta->rta_type] = rta;
				}
			}
		}
};