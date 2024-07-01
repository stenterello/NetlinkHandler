/*
 * Created by m0nt4lb4n0 on 07/02/24.
 */

#pragma once
#include "strategies/NLStrategy.class.hpp"

class	NLGetStrategy : public NLStrategy {
	public:
		struct msghdr&	forgeRequest(struct sockaddr_nl& dst) override {
			_nlmsgptr->nlmsg_len = NLMSG_LENGTH(sizeof(struct genlmsghdr));
			_nlmsgptr->nlmsg_type = _nl80211_id;
			_nlmsgptr->nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;

			return NetlinkStrategy::forgeRequest(dst);
		};

		static std::vector<unsigned int>	parseNested(struct nlattr* nla) {
			int							len = nla->nla_len - NLA_HDRLEN;
			auto						nested_attr = (struct nlattr *)NLA_DATA(nla);
			std::vector<unsigned int>	ret{};

			while (len > 0) {
				if (nested_attr->nla_len < NLA_HDRLEN) break;
				int nested_len = NLA_LENGTH(nested_attr);
				if (nested_len > len) break;

				ret.emplace_back(nested_attr->nla_type);

				len -= NLA_ALIGN(nested_attr->nla_len);
				nested_attr = (struct nlattr *) ((char *) nested_attr + NLA_ALIGN(nested_attr->nla_len));
			}
			return ret;
		}
};