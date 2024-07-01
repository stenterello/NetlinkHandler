/*
 * Created by m0nt4lb4n0 on 31/01/24.
 */

#include "NetlinkHandler.class.hpp"

NetlinkHandler::NetlinkHandler(): _strategy(nullptr) {}

/**
 * @brief
 *
 * \bug applicazione da completare
 * \todo manca lo share
 * \warning attenzione non funziona
 * @return int
 */
NetlinkHandler::~NetlinkHandler() {
	delete _strategy;
}

void						NetlinkHandler::openSocket() {
	this->_sock = socket(AF_NETLINK, SOCK_RAW, this->_strategy->getProtocol());
	if (this->_sock < 0) {
		perror("Open socket");
		exit(EXIT_FAILURE);
	}

	int size = BUFFER_SIZE;
	setsockopt(_sock, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
	size = BUFFER_SIZE;
	setsockopt(_sock, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

	this->_local.nl_family = AF_NETLINK;
	this->_local.nl_pad = 0;
	this->_local.nl_pid = getpid();
	this->_local.nl_groups = 0;

	_kernel.nl_family = AF_NETLINK;
	_kernel.nl_pad = 0;
	_kernel.nl_pid = 0;
	_kernel.nl_groups = 0;
}

void						NetlinkHandler::closeSocket() const {
	if (close(_sock) < 0) {
		perror("Close socket");
		exit(EXIT_FAILURE);
	}
}

void						NetlinkHandler::sendRequest(const struct msghdr& msg) const {
	if (sendmsg(_sock, &msg, MSG_NOSIGNAL) < 0)
	{
		perror("send");
		close(_sock);
		exit(EXIT_FAILURE);
	}
}

std::vector<std::string>	NetlinkHandler::receiveResponse(struct msghdr &msg) {
	std::vector<std::string>		ret{};

	memset(&_recvBuffer, 0, sizeof(_recvBuffer));
	memset(&msg.msg_iov->iov_base, 0, sizeof(struct iovec));
	msg.msg_iov->iov_base = _recvBuffer;
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

		auto	ptr = (struct nlmsghdr*)_recvBuffer;
		if (ptr->nlmsg_type == NLMSG_ERROR) {
			auto err = (struct nlmsgerr *) NLMSG_DATA(ptr);
			errno = -err->error;
			printf("errno = %s \n", strerror(errno));
		} else if (ptr->nlmsg_type == NLMSG_DONE)
			return ret;

		std::vector<std::string>	newValues =	this->_strategy->parseResponse(_recvBuffer, bytes);
		if (!newValues.empty())
			ret.insert(ret.end(), newValues.begin(), newValues.end());

		if (_protocol == NETLINK_GENERIC && _nl80211_id == -1)
			return ret;

		if ((ptr->nlmsg_flags & NLM_F_MULTI) == false) break ;

	} while (bytes > 0);

	return ret;
}

void						NetlinkHandler::setStrategy(NetlinkStrategy* strategy) {
	delete _strategy;
	this->_strategy = strategy;
}

std::vector<std::string>	NetlinkHandler::execStrategy() {
	if (!this->_strategy) {
		std::cerr << "No strategy is set" << std::endl;
		return {};
	}
	this->openSocket();
	struct msghdr& msg = this->_strategy->forgeRequest(this->_kernel);
	this->sendRequest(msg);
	std::vector<std::string> vec =  this->receiveResponse(msg);
	this->closeSocket();
	return vec;
}
