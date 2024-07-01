/*
 * Created by m0nt4lb4n0 on 07/02/24.
 */

#pragma once
#include "NLGetStrategy.class.hpp"

class	NLGetP2PDeviceInterfaces : public NLGetStrategy {
	private:
		std::vector<unsigned int>	_indexes;
	public:
		explicit		NLGetP2PDeviceInterfaces(std::vector<std::string>&& strIndexes) : NLGetStrategy() {
			_indexes.reserve(strIndexes.size());
			std::transform(strIndexes.begin(), strIndexes.end(), std::inserter(_indexes, _indexes.begin()),
						   [](const std::string& index) { return std::stoi(index); });
		};

		struct msghdr&	forgeRequest(struct sockaddr_nl& dst) override {
			_genmsgptr = (struct genlmsghdr*)(_buffer + sizeof(*_nlmsgptr));
			_genmsgptr->cmd = NL80211_CMD_GET_INTERFACE;
			_genmsgptr->version = 1;
			return NLGetStrategy::forgeRequest(dst);
		}

		std::vector<std::string>	parseMessage(struct nlmsghdr* ptr, int bytes) override {
			std::vector<std::string>	ret{};
			static struct nlattr*		tb[NL80211_ATTR_MAX + 1] = {};

			auto	start = (uint8_t*)(ptr + 1);
			uint8_t*	end = start + ptr->nlmsg_len;
			start += 4;
			NLStrategy::parseAttr(tb, NL80211_ATTR_MAX, start, end);
			if (tb[NL80211_ATTR_IFNAME]
				&& tb[NL80211_ATTR_WIPHY]
				&& std::find(_indexes.begin(), _indexes.end(), *(unsigned int*)NLA_DATA(tb[NL80211_ATTR_WIPHY])) != _indexes.end()
				&& (*(unsigned int*)NLA_DATA(tb[NL80211_ATTR_IFTYPE]) < 8 || *(unsigned int*)NLA_DATA(tb[NL80211_ATTR_IFTYPE]) > 10)) {
				ret.emplace_back((char*)NLA_DATA(tb[NL80211_ATTR_IFNAME]));
				memset(tb, 0, NL80211_ATTR_MAX + 1);
			}

			return ret;
		}
};