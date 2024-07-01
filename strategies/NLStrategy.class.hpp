/*
 * Created by m0nt4lb4n0 on 06/02/24.
 */

#pragma once
#include "NetlinkStrategy.class.hpp"
#include "NLGetFamilyId.class.hpp"

class	NLStrategy : public NetlinkStrategy {
	protected:
		struct genlmsghdr*	_genmsgptr{};
		int 				_nl80211_id{};
		NLGetFamilyId*		_getter;
	public:
											NLStrategy() : NetlinkStrategy(NETLINK_GENERIC) {
												_getter = new NLGetFamilyId();
												_nl80211_id = _getter->exec();
												delete _getter;
											};

		std::vector<std::string>			parseResponse(char* recvBuffer, ssize_t bytes) override {
			std::vector<std::string>		ret{};

			for (auto ptr = (struct nlmsghdr*)recvBuffer; NLMSG_OK(ptr, bytes); ptr = NLMSG_NEXT(ptr, bytes)) {
				switch (ptr->nlmsg_type) {
					case NLMSG_ERROR:
						std::cerr << "Error packet received" << std::endl;
						break ;
					case NLMSG_DONE:
						return ret;
					case GENL_ID_CTRL:
						return this->parseMessage(ptr, (int) bytes);
					default: {
						std::vector<std::string>	newValues = this->parseMessage(ptr, (int) bytes);
						std::for_each(newValues.begin(), newValues.end(), [&ret](const std::string& iface) {
							if (std::find(ret.begin(), ret.end(), iface) == ret.end())
								ret.push_back(iface);
						});
						break;
					}
				}
			}
			return ret;
		}

		static void							parseAttr(struct nlattr* tb[], int max, uint8_t* start, const uint8_t* end) {
			while (start < end) {
				auto	nla = (struct nlattr*)start;
				if (nla->nla_len == 0) break ;
				if (nla->nla_type <= max) {
					tb[nla->nla_type] = nla;
				}
				start += NFA_ALIGN(nla->nla_len);
			}
		}
};