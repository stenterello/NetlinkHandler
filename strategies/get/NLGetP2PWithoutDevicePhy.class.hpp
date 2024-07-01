/*
 * Created by m0nt4lb4n0 on 01/02/24.
 */

#pragma once
#include "NLGetStrategy.class.hpp"

class	NLGetP2PWithoutDevicePhy : public NLGetStrategy {
	private:
		bool	_indexes;
	public:
		explicit		NLGetP2PWithoutDevicePhy(bool indexes = false): _indexes(indexes) {};

		struct msghdr&	forgeRequest(struct sockaddr_nl& dst) override {
			_genmsgptr = (struct genlmsghdr*)(_buffer + sizeof(*_nlmsgptr));
			_genmsgptr->cmd = NL80211_CMD_GET_WIPHY;
			_genmsgptr->version = 1;
			return NLGetStrategy::forgeRequest(dst);
		}

		std::vector<std::string>	parseMessage(struct nlmsghdr* ptr, int bytes) override {
			std::vector<unsigned int>	types{};
			std::vector<std::string>	ret{};
			static struct nlattr*		tb[NL80211_ATTR_MAX + 1] = {};

			auto	start = (uint8_t*)(ptr + 1);
			uint8_t*	end = start + ptr->nlmsg_len;
			start += 4;
			NLStrategy::parseAttr(tb, NL80211_ATTR_MAX, start, end);
			if (tb[NL80211_ATTR_SUPPORTED_IFTYPES])
				types = parseNested(tb[NL80211_ATTR_SUPPORTED_IFTYPES]);
			if (tb[NL80211_ATTR_WIPHY_NAME]
				&& tb[NL80211_ATTR_WIPHY]
				&& std::find(types.begin(), types.end(), NL80211_IFTYPE_P2P_CLIENT) != types.end()
				&& std::find(types.begin(), types.end(), NL80211_IFTYPE_P2P_GO) != types.end()
				&& std::find(types.begin(), types.end(), NL80211_IFTYPE_P2P_DEVICE) == types.end()) {
				if (_indexes)
					ret.emplace_back(std::to_string(*(unsigned int*)NLA_DATA(tb[NL80211_ATTR_WIPHY])));
				else {
					ret.emplace_back((char*)NLA_DATA(tb[NL80211_ATTR_WIPHY_NAME]));
				}
				memset(tb, 0, NL80211_ATTR_MAX + 1);
				types.clear();
			}
			return ret;
		}
};
