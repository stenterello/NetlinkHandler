/*
 * Created by m0nt4lb4n0 on 11/02/24.
 */

#pragma once
#include "strategies/NFStrategy.class.hpp"

class	NFCreateNATChain : public NFStrategy {
	protected:
		const char*		_chainName;
		nf_inet_hooks	_hook;
		int 			_priority;
	public:
		explicit	NFCreateNATChain(const char* table_name, const char* chainName, nf_inet_hooks hook = NF_INET_POST_ROUTING, int priority = 0) : NFStrategy(table_name), _hook(hook), _priority(priority), _chainName(chainName) {};

		struct msghdr&				forgeRequest(struct sockaddr_nl& dst) override {
			_nlmsgptr->nlmsg_len = NLMSG_LENGTH(sizeof(struct nfgenmsg));
			_nlmsgptr->nlmsg_type = (NFNL_SUBSYS_NFTABLES << 8) | NFT_MSG_NEWCHAIN;
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

			// Start nested
			_nla = (struct nlattr*)((char*)_nla + NFA_ALIGN(_nla->nla_len));
			_nla->nla_type = NLA_F_NESTED | NFTA_CHAIN_HOOK;

			_nlmsgptr->nlmsg_len += NFA_ALIGN(sizeof(struct nlattr));

			// Chain hook
			_nla = (struct nlattr*)((char*)_nla + sizeof(struct nlattr));
			_nla->nla_type = NFTA_HOOK_HOOKNUM;
			uint32_t converted = htonl(_hook);
			memcpy(NLA_DATA(_nla), &converted, sizeof(converted));
			_nla->nla_len = NFA_LENGTH(sizeof(converted));
			_nlmsgptr->nlmsg_len += NFA_ALIGN(_nla->nla_len);

			// Chain priority
			_nla = (struct nlattr*)((char*)_nla + NFA_ALIGN(_nla->nla_len));
			_nla->nla_type = NFTA_HOOK_PRIORITY;
			converted = htonl(_priority);
			_nla->nla_len = NFA_LENGTH(sizeof(converted));
			memcpy(NLA_DATA(_nla), &converted, sizeof(converted));
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