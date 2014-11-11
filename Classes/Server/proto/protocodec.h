#ifndef RU_PROTOCODEC_H_
#define RU_PROTOCODEC_H_

// standard C++ headers go here
#include <stdint.h>
#include <string>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>

// project-specific message definition headers go here
#include "cs_basic.pb.h"
#include "db.pb.h"
#include "head.pb.h"
#include "../base/byteswap.hpp"
#include "../common/log.h"

/*
1. 和客户端通讯的包格式：
	total_len(u32) + header_len(u32) + CSMessageHeader + msgpayload(例如CSLoginReq)
	| 整个包的长度 | 包头长度(包含本身) |     包头     |          包体            |
2. 客户端协议命名规则：
	CSXXXXReq/Rsp：以大写CS开头，客户端发来的请求以Req结尾，服务端的相应包以Rsp结尾，
					中间的XXXX代表对应的功能，比如CSLoginReq/Rsp。

3. 和DB通讯的包格式:
     total_len(u32) + header_len(u32) + msghead + msgbody
	|   整包长度   |  包头长度（包括本身） | 包头 | 包体 |
4. DB协议命名规则：
	DBXXXXReq/Rsp：以大写DB开头，Online发来的请求以Req结尾，DB应答包以Rsp结尾，
					中间的XXXX代表对应的功能，比如DBLoginReq/Rsp。

5. 使用规则：
    (1) 包头格式绝对不能改；
	(2) 字段只能加，不能减；
	(3) 增加的字段只能是optional类型，且必须给出default值，最好还有相关说明；
	(4) 不能改变原来字段的序号。
*/

/**
 * @brief 打包完毕的数据包
 */
struct FullPacket {
    int len;            // 数据包长度
    const void* packet; // 数据包
};


struct fdsession_t
{
	int fd;
};


/**
 * @brief 把打包/解包的错误日志输出到日志文件中
 */
void set_codec_log_to_file();

/**
 * @brief 打包一个标准错误码返回包。本函数不可重入，使用时必须注意。
 * @param errcode 错误码
 * @return 成功返回标准数据包，失败返回0。注意，返回的是静态变量，无需调用delete释放内存。
 */
const FullPacket* encode(uint32_t errcode);
/**
 * @brief 把包头和包体打包成标准数据包。本函数不可重入，使用时必须注意。
 * @param msgpayload 包体
 * @param errcode 错误码
 * @return 成功返回标准数据包，失败返回0。注意，返回的是静态变量，无需调用delete释放内存。
 */
const FullPacket* encode(const google::protobuf::Message& msgpayload);
/**
 * @brief 把包头和包体打包成标准数据包。本函数不可重入，使用时必须注意。
 * @param msgpayload 包体
 * @param target_uid 被操作的米米号
 * @param target_regtime 被操作的角色创建时间
 * @param src_uid 发起操作的米米号
 * @param login_id 发起操作的角色的登录ID，如果填0，则表示DB返回不需要返回给用户
 * @param trans_id 操作ID，用于记录一一对应的流水帐，如果填0，则代表不记录流水帐
 * @param ret_needed 是否需要处理DB返回，默认是不需要（false）
 * @return 成功返回标准数据包，失败返回0。注意，返回的是静态变量，无需调用delete释放内存。
 */
const FullPacket* encode(const google::protobuf::Message& msgpayload, uint32_t target_uid, uint32_t target_regtime,
                         uint32_t src_uid, uint32_t login_id, uint32_t trans_id = 0, bool ret_needed = false);

/**
 * @brief 解包data，调用者必须保证data至少大于10字节。
 * @param msghdr 函数调用者必须先为msghdr分配好空间，包头会被解包至此。
 * @param msgpayload 函数调用者无需为msgpayload分配空间，包体会被解包至此。
 *                   不再需要msgpayload时，函数调用者必须调用delete释放msgpayload的内存。
 * @return 成功返回true，失败返回false
 */
template <typename MsgHdrT>
bool decode(const void* data, MsgHdrT* msghdr, google::protobuf::Message** msgpayload)
{
    using namespace google::protobuf;

    uint32_t total_len = taomee::bswap(*static_cast<const uint32_t*>(data));
    uint32_t hdr_len   = taomee::bswap(*(static_cast<const uint32_t*>(data) + 1));
    // hdr_len包括其本身所占空间
    if (hdr_len > (total_len - sizeof(total_len))) {
        return false;
    }
    // 解析包头
    const uint8_t* pkg = static_cast<const uint8_t*>(data) + sizeof(total_len) + sizeof(hdr_len);
    if (msghdr->ParseFromArray(pkg, hdr_len - sizeof(hdr_len)) == false) {
        return false;
    }
    // 解析包体
    uint32_t datalen = total_len - sizeof(total_len) - hdr_len;
    if (datalen == 0) { // 如果包体长度为0，直接返回成功。
        *msgpayload = 0;
        return true;
    }
    pkg += (hdr_len - sizeof(hdr_len));

    // TODO: 确定这么这会不会有内存泄漏，思考更加高效的办法
	const Descriptor* descriptor = DescriptorPool::generated_pool()->FindMessageTypeByName(head_msg::CSMsgName_Name(msghdr->msg_id()));
	if (descriptor) {
		const Message* prototype = MessageFactory::generated_factory()->GetPrototype(descriptor);
		if (prototype) {
		    Message* payload = prototype->New();
			if (payload) {
			    if (payload->ParseFromArray(pkg, datalen)) {
			        *msgpayload = payload;
			        return true;
			    }
			    delete payload;
			}
		}
	}

	return false;
}

int send_pkg_to_client(const fdsession_t *socket, const void* buffer, int len);


/**
 * @brief 打包一个标准CS错误码数据包，并发送给客户端
 * @return 成功返回0，失败返回-1
 */
inline int send_stderr(const fdsession_t* fdsess, uint32_t errcode)
{
    const FullPacket* fp = encode(errcode);
    if (fp) {
        return send_pkg_to_client(fdsess, fp->packet, fp->len);
    }

    ERROR_LOG("encode error: msg=ErrorCode fd=%d errcode=%u", fdsess->fd, errcode);
    return -1;
}

#endif // RU_PROTOCODEC_H_