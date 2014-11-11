#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#ifndef WIN32
#  include <unistd.h>
#  include <sys/errno.h>
#endif
//#define  DEBUG_SGIP

#ifndef DEF_HP
#ifndef WIN32
#	include  <sys/select.h>
#	include  <sys/socket.h>
#else
#	include "Win32Platform/socket_win32.h"
#endif
#endif
#include "cocos2d.h"
#include "CSocketUtil.h"

#include "ccMacros.h"

#include "boost/bind.hpp"

CSocketUtil::CSocketUtil()
	: m_pSocket(NULL)
	, m_pIOServer(NULL)
	, m_pConnectTimer(NULL)
    , m_pWriteTimer(NULL)
	, m_bReadCompleted(false)
	, m_bWriteCompleted(true)
	, m_pDelegate(NULL)
    , m_pThread(NULL)
	, poll_status(false)
{

}

CSocketUtil::~CSocketUtil()
{
	Close();

	if(m_pIOServer)
	{
		delete m_pIOServer;
		m_pIOServer = NULL;
	}

	m_pDelegate = NULL;
    
	
}

/******************************************************************************************************************
 *       函数名称:     CreateSocket
 *       函数描述:     创建一个socket
 *       输入参数:     无
 *       输出参数:     无
 *       返 回 值:     socket句柄
 ******************************************************************************************************************/
int CSocketUtil::Create(void)
{
#ifdef _WIN32
   // CCAssert( m_pIOServer == NULL, "remove boost::asio::io_service !");
    CCAssert( m_pConnectTimer == NULL, "remove boost::asio::deadline_timer !");
    CCAssert( m_pSocket == NULL, "remove boost::asio::ip::tcp::socket !");
#endif
	if(m_pIOServer == NULL)
		m_pIOServer = new boost::asio::io_service();
    
    m_pConnectTimer = new boost::asio::deadline_timer(*m_pIOServer);
	
    m_pWriteTimer = new boost::asio::deadline_timer(*m_pIOServer);
    
    // socket对象
	m_pSocket = new boost::asio::ip::tcp::socket(*m_pIOServer);
    
 
    
 
	return 1;
}
	
/******************************************************************************************************************
 *       函数名称:     CancelSocket
 *       函数描述:     关闭socket连接
 *       输入参数:     无
 *       输出参数:     无
 *       返 回 值:     无
 ******************************************************************************************************************/

void CSocketUtil::Close(void)
{
	
 
	if(m_pSocket)
	{
		
        m_pSocket->cancel();
		m_pSocket->close();
		
		delete m_pSocket;
		m_pSocket = NULL;
	}

	if(m_pConnectTimer)
	{
        m_pConnectTimer->cancel();
		delete m_pConnectTimer;
		m_pConnectTimer = NULL;
	}

    if(m_pWriteTimer)
	{
        m_pWriteTimer->cancel();
		delete m_pWriteTimer;
		m_pWriteTimer = NULL;
	}
    
    if(m_pIOServer)
	{
		
			
			m_pIOServer->stop();
	
			m_pIOServer->reset();
			

        // can't delete m_pIOServer ?
			if(!poll_status)
			{
				delete m_pIOServer;
				m_pIOServer = NULL;

			}
	}
	m_bReadCompleted = false;
}

/******************************************************************************************************************
 *       函数名称:     ReadByNoTime
 *       函数描述:     读socket数据
 *       输入参数:     sReadData
 *       输出参数:     无
 *       返 回 值:     读取的长度
 ******************************************************************************************************************/
int CSocketUtil::Read(char *pBuff, int iLen,  int iTimeOut)
{
	// TODO : TimeOut

	int iRet = -1;

	if(m_pSocket)
	{
		if(m_bReadCompleted)
		{
			m_bReadCompleted = false;

			m_pSocket->async_read_some(boost::asio::buffer(pBuff, iLen),
			boost::bind(&CSocketUtil::OnReadCompleted, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
			);


			// 这个是必须督导iLen个字节才会返回。
			/*boost::asio::async_read(
			*m_pSocket,
			boost::asio::buffer( pBuff, iLen ),
			boost::asio::transfer_at_least(iLen),
			boost::bind( 
			&CSocketUtil::OnReadCompleted, 
			this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred         
			)
			);*/
			//boost::asio::error::
		}
	}
	
	
	return iRet;
}

void CSocketUtil::OnReadCompleted(const boost::system::error_code& err, size_t bytes_transferred)
{
	m_bReadCompleted = true;
         
	// TODO : err
	if(err)
	{
		CCLOG("%s error: %d, %s ", __FUNCTION__, err.value(), err.message().c_str() );
		if(err != boost::asio::error::operation_aborted && err != boost::asio::error::timed_out)
		{
			CCLOG("%s error: %d, %s", __FUNCTION__, err.value(), err.message().c_str());
			//if(err == boost::asio::error::eof)
			//{
			//	// it means the connection has been closed.
			//}
			//CCLOG("%s error: %s", __FUNCTION__, err.value());

			if(m_pDelegate)
			{
				// TODO : error
				m_pDelegate->DidReadCompleted(bytes_transferred, CSocketUtilDelegate::E_ERROR);
			}
		}
	}
	else
	{
		if(m_pDelegate)
		{
			// TODO : error
			m_pDelegate->DidReadCompleted(bytes_transferred, CSocketUtilDelegate::E_SUCESS);
		}
	}
}

/******************************************************************************************************************
 *       函数名称:     WriteByNoTime
 *       函数描述:     发送数据到socket
 *       输入参数:     sWriteData 数据缓冲区地址
 *                     iCount   长度
 *       输出参数:     无
 *       返 回 值:     >0(发送长度)--成功, <0--失败
 ******************************************************************************************************************/
int CSocketUtil::Write(char *pBuff, int iCount,  int iTimeOut)
{
	int ret = -1;
	if(m_pSocket)
	{
		// TODO : TimeOut
		// pBuff 在真的发送成功之前，一定要确保pBuff是存在的，且不能被修改。
		// 调用完成的回调函数是，一定是全部发送出去了。
		// m_socket.async_write_some,只是发送了数据，有可能只发送了一部分，要自己来判断是不是真的发送成功。
		boost::asio::async_write(
		*m_pSocket,
		boost::asio::buffer( pBuff, iCount ),
		boost::asio::transfer_at_least(iCount),
		boost::bind( 
		&CSocketUtil::OnWriteCompleted, 
		this,
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred         
		)
		);

		//ret = m_pSocket->send(boost::asio::buffer( pBuff, iCount));
        
       if(m_pWriteTimer)
        {
            m_pWriteTimer->expires_from_now(boost::posix_time::seconds(iTimeOut));
            m_pWriteTimer->async_wait(boost::bind(&CSocketUtil::OnWriteTimeout, this, boost::asio::placeholders::error));
        }
	}
	

	return ret;
}

void CSocketUtil::OnWriteCompleted(const boost::system::error_code& err, size_t bytes_transferred)
{
    if(m_pWriteTimer)
    {
        m_pWriteTimer->cancel();
    }

	// TODO : err
	if(err)
	{
		CCLOG("%s error: %d, %s ", __FUNCTION__, err.value(), err.message().c_str() );
        if(err != boost::asio::error::operation_aborted && err != boost::asio::error::timed_out)
        {
            CCLOG("%s error: %d, %s", __FUNCTION__, err.value(), err.message().c_str());
        
            if(m_pDelegate)
            {
                // TODO : error
                m_pDelegate->DidWriteCompleted(bytes_transferred, CSocketUtilDelegate::E_ERROR);
            }
        }
	}
	else
	{
		if(m_pDelegate)
		{
			// TODO : error
			m_pDelegate->DidWriteCompleted(bytes_transferred, CSocketUtilDelegate::E_SUCESS);
		}

	}
	
	

}


void CSocketUtil::OnWriteTimeout(const boost::system::error_code& error)
{

            
	
    if(error)
    {
      
        if(error != boost::asio::error::operation_aborted)
        {
            CCLOG("%s error: %d, %s", __FUNCTION__, error.value(), error.message().c_str());
            if(m_pDelegate)
            {
                // TODO : error
                m_pDelegate->DidWriteCompleted(0, CSocketUtilDelegate::E_TIMEOUT);
            }
        }
    }
    else
    {
        if(m_pDelegate)
        {
            // TODO : error
            m_pDelegate->DidWriteCompleted(0, CSocketUtilDelegate::E_TIMEOUT);
        }
    }

}
/******************************************************************************************************************
 *       函数名称:     ConnectTo
 *       函数描述:     连接到服务器监听socket
 *       输入参数:     sConnAddr   连接地址
 *                     iPort       端口
 *       输出参数:     无
 *       返 回 值:     0--成功, 非0--失败
 ******************************************************************************************************************/
/*
int CSocketUtil::ConnectTo(char *sConnAddr, int iPort)
{
	int ret = 0;
	// 连接端点，这里使用了本机连接，可以修改IP地址测试远程连接
	boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::from_string(sConnAddr), iPort);
	// 连接服务器
	boost::system::error_code ec;
	if(m_pSocket)
	{
		m_pSocket->connect(ep, ec);
		
	}
	else
	{
		//ASSERT("Please Create Socket !!!");
        CCLOG("%s error: %d, %s", __FUNCTION__, ec.value(), ec.message().c_str());
		ret = 1;
	}

	// 如果出错，打印出错信息
	if(ec)
	{
		// TODO : 
		CCLOG("connect error ");
		ret = -1;
	}
    
	m_bReadCompleted = true;
    m_bWriteCompleted = true;
	return ret;
}

*/

/******************************************************************************************************************
 *       函数名称:     ConnectTo
 *       函数描述:     连接到服务器监听socket(在指定超时时间内)
 *       输入参数:     sConnAddr   连接地址
 *                     iPort       端口
 *                     iOutTime    超时时间(单位:秒)
 *       输出参数:     无
 *       返 回 值:     0:成功, -1:连接出错,-2:连接超时,-3:系统错
 ******************************************************************************************************************/
int CSocketUtil::ConnectTo(char *sConnAddr, int iPort, int iOutTime)
{
	int iRet = 0;

	if(m_pSocket)
	{
		boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::from_string(sConnAddr), iPort);

		m_pSocket->async_connect(ep,
			boost::bind(&CSocketUtil::OnConnecting, this, boost::asio::placeholders::error)
			);

		if(m_pConnectTimer == NULL)
		{
			m_pConnectTimer = new boost::asio::deadline_timer(*m_pIOServer);
		}
		m_pConnectTimer->expires_from_now(boost::posix_time::seconds(iOutTime));  
		m_pConnectTimer->async_wait(boost::bind(&CSocketUtil::OnConnectedTimeout, this, boost::asio::placeholders::error));
	}
	else
	{
#ifdef _WIN32
		CCAssert(false, "Please Create Socket !!!");
#endif
        
		if(m_pDelegate)
		{
			m_pDelegate->DidConnected(CSocketUtilDelegate::E_NO_SOCKET);
		}

		iRet = -1;
	}

	return iRet;
}

void CSocketUtil::OnConnecting(const boost::system::error_code& err)  
{
    if(m_pConnectTimer)
    {
        m_pConnectTimer->cancel();
    }
            
	if (err)
	{  
		//std::cout << "Connect error: " << err.message() << "\n";
		CCLOG("%s error: %d, %s", __FUNCTION__, err.value(), err.message().c_str());
        m_bReadCompleted = false;

		if(m_pDelegate)
		{
            if(err == boost::asio::error::operation_aborted)
            {
                m_pDelegate->DidConnected(CSocketUtilDelegate::E_CANCEL);
            }
            else
            {
                m_pDelegate->DidConnected(CSocketUtilDelegate::E_ERROR);
            }
		}
	}  
	else  
	{  
		//std::cout << "Successful connection\n";  
		
		
		//delete m_pTimer;
		//m_pTimer = NULL;

        m_bReadCompleted = true;
        m_bWriteCompleted = true;
        
		if(m_pDelegate)
		{
			m_pDelegate->DidConnected(CSocketUtilDelegate::E_SUCESS);
		}
	}  
}  

void CSocketUtil::OnConnectedTimeout(const boost::system::error_code& error)
{
	if(m_pConnectTimer)
	{
        if(error)
        {
            if(error != boost::asio::error::operation_aborted)
            {
				 CCLOG("%s error: %d, %s", __FUNCTION__, error.value(), error.message().c_str());

                if(m_pDelegate)
                {
                    m_pDelegate->DidConnected(CSocketUtilDelegate::E_TIMEOUT);
                }
                
                Close();
            }
        }
        else
        {
            if(m_pDelegate)
            {
                m_pDelegate->DidConnected(CSocketUtilDelegate::E_TIMEOUT);
            }
            
            Close();
        }
	}
	
}
/******************************************************************************************************************
 *       函数名称:     DataArrival
 *       函数描述:     是否有数据到达
 *       输入参数:     无
 *       输出参数:     无
 *       返 回 值:     1--到达, 0--还没到达
 ******************************************************************************************************************/

int CSocketUtil::CanRead( void )
{
// TODO : 
	if(m_bReadCompleted)
		return 1;
	return 0;
}


//@return 61 connected refuse
//        0  success
int  CSocketUtil::CanWrite(void)
{
	if(m_bWriteCompleted)
		return 1;
   return 0;
}


void CSocketUtil::CancelAllTimer( void )
{
	if(m_pWriteTimer)
	{
		m_pWriteTimer->cancel();
	}

	/*if(m_pConnectTimer)
	{
	m_pConnectTimer->cancel();
	}*/
}

void CSocketUtil::Update( void )
{
   
    if(m_pIOServer && m_pSocket)
    {
		
		if(!m_pIOServer->stopped())
		{
			poll_status = true ;
			m_pIOServer->poll();
			// TODO : ???
			m_pIOServer->reset();
			poll_status = false ;

			
			
		}
    }
    
}