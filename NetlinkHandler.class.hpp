/*
 * Created by m0nt4lb4n0 on 31/01/24.
 */

#pragma once
#include "utils.hpp"

class	NetlinkHandler {
	private:
		NetlinkStrategy*			_strategy;
		struct sockaddr_nl			_local{};
		struct sockaddr_nl			_kernel{};
		int							_sock{};
		int							_protocol;
		int 						_nl80211_id;
		char 						_recvBuffer[BUFFER_SIZE]{};

		void						openSocket();
		void						closeSocket() const;
		void						sendRequest(const struct msghdr& msg) const;
		std::vector<std::string>	receiveResponse(struct msghdr& msg);
	public:
									NetlinkHandler();
									~NetlinkHandler();

		void						setStrategy(NetlinkStrategy* strategy);
		std::vector<std::string>	execStrategy();
};