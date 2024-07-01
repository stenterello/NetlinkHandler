/*
 * Created by m0nt4lb4n0 on 10/02/24.
 */

#pragma once
#include "strategies/NetlinkStrategy.class.hpp"

struct	batch_msg {
	struct nlmsghdr	nl{};
	struct nfgenmsg	nf{};
};

class	NFStrategy : public NetlinkStrategy {
	protected:
		struct nfgenmsg*	_nfgenptr{};
		struct nfattr*		_nfa{};
		struct nlattr*		_nla{};
		const char*			_table_name;
		batch_msg			_batches[2]{};
		struct iovec		_multi_iov[3]{};
	public:
		explicit NFStrategy(const char* table_name = nullptr) : _table_name(table_name), NetlinkStrategy(NETLINK_NETFILTER) {
			craftBatch(_batches[0], true);
			craftBatch(_batches[1], false);
		};

		static void	craftBatch(struct batch_msg& b, bool begin) {
			b.nl.nlmsg_len = NLMSG_SPACE(sizeof(struct nfgenmsg));
			if (begin) {
				b.nl.nlmsg_type = NFNL_MSG_BATCH_BEGIN;
				b.nf.res_id = NFNL_SUBSYS_NFTABLES;
			}
			else {
				b.nl.nlmsg_type = NFNL_MSG_BATCH_END;
			}
			b.nl.nlmsg_flags = NLM_F_REQUEST;
			b.nl.nlmsg_pid = getpid();
			b.nl.nlmsg_seq = 0;
		}

		struct msghdr&				forgeRequest(struct sockaddr_nl& dst) override {
			_nlmsgptr->nlmsg_seq = 0;
			_nlmsgptr->nlmsg_pid = getpid();

			memset(&_multi_iov, 0, sizeof(_multi_iov));
//			for (int i = 0; i < _batches[0].nl.nlmsg_len; i++) {
//				printf("%02x", ((unsigned char*)&(_batches[0]))[i]);
//			}
//			printf("\n");
//			for (int i = 0; i < _nlmsgptr->nlmsg_len; i++) {
//				printf("%02x", ((unsigned char*)&(_nlmsgptr))[i]);
//			}
//			printf("\n");
//			for (int i = 0; i < _batches[1].nl.nlmsg_len; i++) {
//				printf("%02x", ((unsigned char*)&(_batches[1]))[i]);
//			}
//			printf("\n");
			_multi_iov[0].iov_base = &_batches[0];
			_multi_iov[0].iov_len = _batches[0].nl.nlmsg_len;
			_multi_iov[1].iov_base = _buffer;
			_multi_iov[1].iov_len = _nlmsgptr->nlmsg_len;
			_multi_iov[2].iov_base = &_batches[1];
			_multi_iov[2].iov_len = _batches[1].nl.nlmsg_len;

			memset(&_msg, 0, sizeof(_msg));
			_msg.msg_name = (void *) &dst;
			_msg.msg_namelen = sizeof(dst);
			_msg.msg_iov = _multi_iov;
			_msg.msg_iovlen = 3;

			return _msg;
		};
};