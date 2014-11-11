// standard C++ headers go here
// project-specific headers go here
#include "protocodec.h"

// 3rd-party lib headers go here
#include "../base/byteswap.hpp"



using namespace google::protobuf;
using namespace taomee;
using namespace rudb;
using namespace std;

namespace {

/**
 * @brief  把包头h和包体msgpayload打包到buf中，并把最终结果赋予fullpacket。
 * @return 成功返回指向标准数据包fullpacket的指针，失败返回0。
 */
const FullPacket* encode(const Message& h, const Message& msgpayload, string& buf, FullPacket& fullpacket)
{
    // 计算整包长度和包头长度
    uint32_t hlen = h.ByteSize() + sizeof(hlen);
    uint32_t tlen = sizeof(tlen) + hlen + msgpayload.ByteSize();
    // 打包长度信息
    hlen = bswap(hlen);
    tlen = bswap(tlen);
    buf.append(reinterpret_cast<char*>(&tlen), sizeof(tlen));
    buf.append(reinterpret_cast<char*>(&hlen), sizeof(hlen));
    //打包包头和包体
    if (h.AppendToString(&buf) && msgpayload.AppendToString(&buf)) {
        fullpacket.len    = static_cast<int>(buf.size());
        fullpacket.packet = buf.c_str();
        return &fullpacket;
    }

	return 0;
}

}

const FullPacket* encode(uint32_t errcode)
{
    // 使用静态变量避免频繁分配释放内存，缺点是本函数不可重入，使用时必须注意
    static string buf;
    static FullPacket fullpacket;

    buf.clear();
    // 构造包头
    CSMessageHeader h;
    h.set_msg_id(head_msg::ErrorCode);
    h.set_errcode(errcode);
    // 计算整包长度和包头长度
    uint32_t hlen = h.ByteSize() + sizeof(hlen);
    uint32_t tlen = sizeof(tlen) + hlen;
    // 打包长度信息
    hlen = bswap(hlen);
    tlen = bswap(tlen);
    buf.append(reinterpret_cast<char*>(&tlen), sizeof(tlen));
    buf.append(reinterpret_cast<char*>(&hlen), sizeof(hlen));
    //打包包头
    if (h.AppendToString(&buf)) {
        fullpacket.len    = static_cast<int>(buf.size());
        fullpacket.packet = buf.c_str();
        return &fullpacket;
    }

	return 0;
}

const FullPacket* encode(const Message& msgpayload)
{
    // 使用静态变量避免频繁分配释放内存，缺点是本函数不可重入，使用时必须注意
    static string buf;
    static FullPacket fullpacket;

    // 构造包头
    CSMessageHeader h;
	head_msg::CSMsgName EMsgName ;
	string SMsgName =msgpayload.GetTypeName() ;
	head_msg::CSMsgName_Parse(SMsgName ,&EMsgName);
    h.set_msg_id(EMsgName);
    h.set_errcode(0);

    buf.clear();
    return encode(h, msgpayload, buf, fullpacket);
}

const FullPacket* encode(const Message& msgpayload, uint32_t target_uid, uint32_t target_regtime,
                         uint32_t src_uid, uint32_t login_id, uint32_t trans_id, bool ret_needed)
{
    // 使用静态变量避免频繁分配释放内存，缺点是本函数不可重入，使用时必须注意
    static string buf;
    static FullPacket fullpacket;

    // 构造包头
    db_msgheader_t h;
	
	head_msg::CSMsgName EMsgName ;
	string SMsgName =msgpayload.GetTypeName() ;
	head_msg::CSMsgName_Parse(SMsgName ,&EMsgName);

   // h.set_msg_id(EMsgName);

    h.set_msg_name(msgpayload.GetTypeName() );
    h.set_target_uid(target_uid);
    h.set_reg_time(target_regtime);
    h.set_src_uid(src_uid);
    h.set_login_id(login_id);
    h.set_trans_id(trans_id);
    if (ret_needed) {
        h.set_aux("1");
    }
    h.set_errcode(0);

    buf.clear();
    return encode(h, msgpayload, buf, fullpacket);
}

namespace {

void pb_log_handler(LogLevel level, const char* filename, int line, const string& message)
{
	static const char* level_names[] = { "INFO", "WARNING", "ERROR", "FATAL" };

	ERROR_LOG("[%s %s:%d] %s",	level_names[level], filename, line, message.c_str());
}

};

void set_codec_log_to_file()
{
    SetLogHandler(pb_log_handler);
}




//////////////////////////////////////////////////////////////////////////
// send package to client
int send_pkg_to_client(const fdsession_t* socket, const void* buffer, int len)
{

	return len;
}