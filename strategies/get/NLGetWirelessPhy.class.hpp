/*
 * Created by m0nt4lb4n0 on 31/01/24.
 */

#pragma once
#include "NLGetStrategy.class.hpp"

class	NLGetWirelessPhy : public NLGetStrategy {
	private:
	public:
		struct msghdr&	forgeRequest(struct sockaddr_nl& dst) override {
			_genmsgptr = (struct genlmsghdr*)(_buffer + sizeof(*_nlmsgptr));
			_genmsgptr->cmd = NL80211_CMD_GET_WIPHY;
			_genmsgptr->version = 1;
			return NLGetStrategy::forgeRequest(dst);
		}

		std::vector<std::string>	parseMessage(struct nlmsghdr* ptr, int bytes) override {
			std::vector<std::string>	ret{};
			struct nlattr*				tb[NL80211_ATTR_MAX + 1];

			memset(tb, 0, NL80211_ATTR_MAX + 1);
			auto	start = (uint8_t*)(ptr + 1);
			uint8_t*	end = start + ptr->nlmsg_len;
			start += 4;
			NLStrategy::parseAttr(tb, NL80211_ATTR_MAX, start, end);
			if (tb[NL80211_ATTR_WIPHY_NAME])
				ret.emplace_back((char*)NLA_DATA(tb[NL80211_ATTR_WIPHY_NAME]));

			return ret;
		}
};