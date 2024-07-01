/*
 * Created by m0nt4lb4n0 on 10/02/24.
 */

#pragma once
#include "strategies/RTStrategy.class.hpp"

class	RTMgmtStrategy : public RTStrategy {
	protected:
	public:
		std::vector<std::string>	parseMessage(struct nlmsghdr* ptr, int bytes) override {
			return {};
		}

		std::vector<std::string>			parseResponse(char* recvBuffer, ssize_t bytes) override {
			for (auto ptr = (struct nlmsghdr*)recvBuffer; NLMSG_OK(ptr, ptr->nlmsg_len); ptr = NLMSG_NEXT(ptr, ptr->nlmsg_len)) {
				switch (ptr->nlmsg_type) {
					case NLMSG_ERROR:{
						auto err = (struct nlmsgerr *) NLMSG_DATA(ptr);
						errno = -err->error;
						printf("errno = %s \n", strerror(errno));
						return {std::to_string(errno)};
					}
					case NLMSG_DONE: return {"-1"};
					default: {
						break ;
					}
				}
			}
			return {"-1"};
		}
};