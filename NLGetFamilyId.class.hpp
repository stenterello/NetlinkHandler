/*
 * Created by m0nt4lb4n0 on 05/02/24.
 */

#pragma once
#include "utils.hpp"

class NLGetFamilyId {
	private:
		struct nlmsghdr*			_nlmsgptr{};
		struct genlmsghdr*			_genmsgptr{};
		struct nlattr*				_attr{};
		char 						_buffer[BUFFER_SIZE]{};
		std::string 				_familyName{};
		struct msghdr				_msg{};
		struct iovec				_iov{};
		int 						_sock{};
		struct sockaddr_nl			_kernel{};
		int 						_ret{-1};
	public:
		explicit		NLGetFamilyId(const char* familyName = "nl80211") : _familyName(familyName) {};

		struct msghdr&	forgeRequest(struct sockaddr_nl& dst) {
			_nlmsgptr = (struct nlmsghdr*)_buffer;
			_nlmsgptr->nlmsg_len = 32;
			_nlmsgptr->nlmsg_type = GENL_ID_CTRL;
			_nlmsgptr->nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK;
			_nlmsgptr->nlmsg_pid = getpid();
			_nlmsgptr->nlmsg_seq = 1;
			_genmsgptr = (struct genlmsghdr*)(_buffer + sizeof(*_nlmsgptr));
			_genmsgptr->cmd = CTRL_CMD_GETFAMILY;
			_genmsgptr->version = 1;
			_attr = (struct nlattr*)((char*)_genmsgptr + sizeof (*_genmsgptr));
			_attr->nla_type = CTRL_ATTR_FAMILY_NAME;
			_attr->nla_len = NLA_HDRLEN + _familyName.size() + 1;
			memcpy(NLA_DATA(_attr), _familyName.c_str(), std::strlen(_familyName.c_str()) + 1);

			_iov.iov_base = &_buffer;
			_iov.iov_len = _nlmsgptr->nlmsg_len;

			_msg.msg_name = &dst;
			_msg.msg_namelen = sizeof(dst);
			_msg.msg_iov = &_iov;
			_msg.msg_iovlen = 1;

			return _msg;
		}

		void			parseResponse(char* recvBuffer, ssize_t bytes) {
			for (auto ptr = (struct nlmsghdr*)recvBuffer; NLMSG_OK(ptr, bytes); ptr = NLMSG_NEXT(ptr, bytes)) {
				switch (ptr->nlmsg_type) {
					case NLMSG_ERROR:
						std::cerr << "Error packet received" << std::endl;
						break ;
					case NLMSG_DONE:
						return ;
					case GENL_ID_CTRL: {
						NLGetFamilyId::parseMessage(ptr);
						return ;
					}
					default: {
						break;
					}
				}
			}
		}

		int	receiveResponse(struct msghdr &msg) {
			memset(&_buffer, 0, sizeof(_buffer));
			msg.msg_iov->iov_base = _buffer;
			msg.msg_iov->iov_len = BUFFER_SIZE;
			msg.msg_iovlen = 1;

			ssize_t	bytes;

			do {
				bytes = recvmsg(_sock, &msg, 0);
				if (bytes < 0) {
					perror("recv");
					close(_sock);
					exit(EXIT_FAILURE);
				}

				auto	ptr = (struct nlmsghdr*)_buffer;
				if (ptr->nlmsg_type == NLMSG_ERROR) {
					std::cerr << "Received error packet" << std::endl;
				} else if (ptr->nlmsg_type == NLMSG_DONE)
					return _ret;

				parseResponse(_buffer, bytes);

				if (_ret != -1)
					return _ret;

				if ((ptr->nlmsg_flags & NLM_F_MULTI) == false) break ;

			} while (bytes > 0);

			return _ret;
		}

		void	parseMessage(struct nlmsghdr* ptr) {
			auto	start = (uint8_t*)(ptr + 1);
			start += 4;
			uint8_t*	end = start + ptr->nlmsg_len;
			do {
				int len = ((uint16_t*)start)[0];
				if (!len) break ;
				int bufflen = (len+3)&0xfffc;
				int type = start[2];
				if (type == 1) {
					_ret = (*((int *) (start + 4)));
				}
				start += bufflen;
			} while (start < end);
		}

		void						openSocket() {
			this->_sock = socket(AF_NETLINK, SOCK_RAW, NETLINK_GENERIC);
			if (this->_sock < 0) {
				perror("Open socket");
				exit(EXIT_FAILURE);
			}

			int size = BUFFER_SIZE;
			setsockopt(_sock, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
			size = BUFFER_SIZE;
			setsockopt(_sock, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

			_kernel.nl_family = AF_NETLINK;
			_kernel.nl_pad = 0;
			_kernel.nl_pid = 0;
			_kernel.nl_groups = 0;
		}

		int							exec() {
			openSocket();
			struct msghdr& msg = this->forgeRequest((sockaddr_nl &)_kernel);
			if (sendmsg(_sock, &msg, MSG_NOSIGNAL) < 0)
			{
				perror("send");
				close(_sock);
				exit(EXIT_FAILURE);
			}
			int ret = receiveResponse(msg);
			if (close(_sock) < 0) {
				perror("Close socket");
				exit(EXIT_FAILURE);
			}
			return ret;
		}
};
