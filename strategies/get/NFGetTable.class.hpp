/*
 * Created by m0nt4lb4n0 on 10/02/24.
 */

#pragma once
#include "strategies/NFStrategy.class.hpp"

class	NFGetTable : public NFStrategy {
	protected:
	public:
		explicit					NFGetTable(const char* name) : NFStrategy(name) {};

		struct msghdr&				forgeRequest(struct sockaddr_nl& dst) override {
			_nlmsgptr->nlmsg_len = NLMSG_LENGTH(sizeof(struct nfgenmsg));
			_nlmsgptr->nlmsg_type = (NFNL_SUBSYS_NFTABLES << 8) | NFT_MSG_GETTABLE;
			_nlmsgptr->nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK;

			_nfgenptr = (struct nfgenmsg*)(_buffer + sizeof(*_nlmsgptr));
			_nfgenptr->nfgen_family = NFPROTO_IPV4;
			_nfgenptr->version = NFNETLINK_V0;
			_nfgenptr->res_id = 0;

			_nfa = (struct nfattr*)((char*)_nfgenptr + sizeof(nfgenmsg));
			_nfa->nfa_type = 1;
			_nfa->nfa_len = NFA_LENGTH(sizeof(struct nfattr) + NFA_ALIGN(std::strlen(_table_name)));
			memcpy(NFA_DATA(_nfa), _table_name, std::strlen(_table_name));

			_nlmsgptr->nlmsg_len += NFA_ALIGN(_nfa->nfa_len);

			return NetlinkStrategy::forgeRequest(dst);
		};

		std::vector<std::string>	parseMessage(struct nlmsghdr*, int) override {
			return {};
		}

		std::vector<std::string>			parseResponse(char* recvBuffer, ssize_t bytes) override {
			std::vector<std::string>	ret{};
			struct nlattr*				tb[NFTA_TABLE_MAX + 1]{};

			auto	start = (uint8_t*)((struct nlmsghdr*)recvBuffer + 1);
			uint8_t*	end = start + ((struct nlmsghdr*)recvBuffer)->nlmsg_len;
			start += 4;
			NLGetStrategy::parseAttr(tb, NFTA_TABLE_MAX, start, end);

			if (tb[NFTA_TABLE_NAME])
				std::cout << "Table name: " << (char*) NFA_DATA(tb[NFTA_TABLE_NAME]) << std::endl;
			if (tb[NFTA_TABLE_FLAGS])
				std::cout << "Table flags: " << *(unsigned int*) NFA_DATA(tb[NFTA_TABLE_FLAGS]) << std::endl;
			if (tb[NFTA_TABLE_USE])
				std::cout << "Table use: " << ntohl(*(unsigned int*) NFA_DATA(tb[NFTA_TABLE_USE])) << std::endl;

			return ret;
		}
};
