/*
 * Created by m0nt4lb4n0 on 11/02/24.
 */

#pragma once
#include "strategies/NFStrategy.class.hpp"

class	NFDeleteNATChain : public NFStrategy {
	protected:
		const char*		_chainName;
	public:
		explicit	NFDeleteNATChain(const char* table_name, const char* chainName) : NFStrategy(table_name), _chainName(chainName) {};

		struct msghdr&				forgeRequest(struct sockaddr_nl& dst) override {
			_nlmsgptr->nlmsg_len = NLMSG_LENGTH(sizeof(struct nfgenmsg));
			_nlmsgptr->nlmsg_type = (NFNL_SUBSYS_NFTABLES << 8) | NFT_MSG_DELCHAIN;
			_nlmsgptr->nlmsg_flags = NLM_F_REQUEST | NLM_F_ACK;

			_nfgenptr = (struct nfgenmsg*)(_buffer + sizeof(*_nlmsgptr));
			_nfgenptr->nfgen_family = NFPROTO_IPV4;
			_nfgenptr->version = NFNETLINK_V0;
			_nfgenptr->res_id = 0;

			// Table name
			_nla = (struct nlattr*)((char*)_nfgenptr + sizeof(nfgenmsg));
			_nla->nla_type = NFTA_CHAIN_TABLE;
			memcpy(NLA_DATA(_nla), _table_name, std::strlen(_table_name));
			_nla->nla_len = NFA_LENGTH(std::strlen(_table_name) + 1);
			_nlmsgptr->nlmsg_len += NFA_ALIGN(_nla->nla_len);

			// Chain name
			_nla = (struct nlattr*)((char*)_nla + NFA_ALIGN(_nla->nla_len));
			_nla->nla_type = NFTA_CHAIN_NAME;
			memcpy(NLA_DATA(_nla), _chainName, std::strlen(_chainName));
			_nla->nla_len = NFA_LENGTH(std::strlen(_chainName) + 1);
			_nlmsgptr->nlmsg_len += NFA_ALIGN(_nla->nla_len);

			return NFStrategy::forgeRequest(dst);
		};

		std::vector<std::string>	parseMessage(struct nlmsghdr*, int) override {
			return {};
		}

		std::vector<std::string>			parseResponse(char* recvBuffer, ssize_t bytes) override {
			return {};
		}
};