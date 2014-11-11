#ifndef RU_MESSAGE_HANDLER_MANAGER_H_ // RU: Reverse Universe
#define RU_MESSAGE_HANDLER_MANAGER_H_

// standard C++ headers go here
#include <stdexcept>
#include <string>
#ifndef FRONT
#include <unordered_map>
#else 
#include <map>
#endif
#include <utility>

// 3rd-party lib headers go here
#include <google/protobuf/message.h>

// project-specific headers go here
#include "../common/fwd_decl.h"

/**
 * @brief Message Handler Manager
 */
class MsgHandlerMgr {
public:
	/**
	 * @brief Handler Type
	 */
	typedef int (*HandlerType)(Player* player, const google::protobuf::Message* msg);

public:
	/**
	 * @brief register a handler
	 * @param msg message to handle
	 * @param hdlr handler to handle 'msg'
	 * @throw throw logic_error if register more than one handler with the same 'msg'
	 */
	void register_handler(const std::string& msg, HandlerType hdlr);
	/**
	 * @brief find a handler
	 * @param msg find a handler to handle 'msg'
	 * @return pointer to the handler or 0 if handler not found
	 */
	HandlerType find_handler(const std::string& msg);

private:
	// types
#ifndef FRONT
	typedef std::unordered_map<std::string, HandlerType> HandlerHolderType;
#else 
	typedef std::map<std::string, HandlerType> HandlerHolderType;
#endif

private:
	HandlerHolderType m_hdlrs;
};

// method definitions
inline void MsgHandlerMgr::register_handler(const std::string& msg, HandlerType hdlr)
{
	std::pair<HandlerHolderType::iterator, bool> r = m_hdlrs.insert(make_pair(msg, hdlr));
	if (r.second != true) {
		throw std::logic_error(std::string("'") + msg + "' had been registered!");
	}
}

inline MsgHandlerMgr::HandlerType MsgHandlerMgr::find_handler(const std::string& msg)
{
	HandlerHolderType::iterator it = m_hdlrs.find(msg);
	if (it != m_hdlrs.end()) {
		return it->second;
	}

	return 0;
}

#endif // RU_MESSAGE_HANDLER_MANAGER_H_
