/*
 * Created by m0nt4lb4n0 on 31/01/24.
 */

#pragma once
#include "utils.hpp"

class	NetlinkStrategy {
	protected:
		struct msghdr			_msg{};
		struct iovec			_iov{};
		int 					_protocol{};
		char 					_buffer[BUFFER_SIZE]{};
		struct nlmsghdr*		_nlmsgptr = (struct nlmsghdr*)_buffer;
	public:
		explicit							NetlinkStrategy(int protocol) : _protocol(protocol) {};
		virtual 							~NetlinkStrategy() = default;
		virtual struct msghdr&				forgeRequest(struct sockaddr_nl& dst) {
			_nlmsgptr->nlmsg_seq = 1;
			_nlmsgptr->nlmsg_pid = getpid();

			memset(&_iov, 0, sizeof(_iov));
			_iov.iov_base = _buffer;
			_iov.iov_len = _nlmsgptr->nlmsg_len;

			memset(&_msg, 0, sizeof(_msg));
			_msg.msg_name = (void *) &dst;
			_msg.msg_namelen = sizeof(dst);
			_msg.msg_iov = &_iov;
			_msg.msg_iovlen = 1;

			return _msg;
		};
		virtual std::vector<std::string>	parseMessage(struct nlmsghdr*, int bytes) = 0;
		virtual std::vector<std::string>	parseResponse(char* recvBuffer, ssize_t bytes) = 0;
		[[nodiscard]] int 					getProtocol() const { return this->_protocol; };
};
