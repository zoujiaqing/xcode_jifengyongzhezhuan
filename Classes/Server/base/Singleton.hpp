/**
 * =====================================================================================
 *       @file  Singleton.hpp
 *      @brief  
 *
 *  Detailed description starts here.
 *
 *   @internal
 *     Created  10/14/2009 01:46:21 PM 
 *    Revision  1.0.0.0
 *    Compiler  gcc/g++
 *     Company  TaoMee.Inc, ShangHai.
 *   Copyright  Copyright (c) 2009, TaoMee.Inc, ShangHai.
 *
 *     @author  sylar (), sylar@taomee.com
 * This source code is wrote for TaoMee,Inc. ShangHai CN.
 * =====================================================================================
 */
#ifndef FAIRY_SINGLETON_HPP
#define FAIRY_SINGLETON_HPP

#include <assert.h>

template < class type > class Singleton //保证只有一个实例化对象
{
    public:
        virtual ~Singleton( ) {
            m_singleton = 0;
        }

        //static type & get_singleton( ) { ASSERT( m_singleton ); return *m_singleton; }
        static type & get_singleton( ) { if(m_singleton == NULL) new type(); return *m_singleton; }
        static type * get_singleton_ptr( ) { if(m_singleton == NULL) new type();return m_singleton; }

    protected:
        Singleton( ) {
            /// If you hit this ASSERT, this singleton already exists -- you can't create another one!
            assert( m_singleton == 0 );
            m_singleton = static_cast<type *>(this);//this在使用时是指向子类的
        }

        /// Singleton pointer, must be set to 0 prior to creating the object
        static type * m_singleton;
};
template <class type> type * Singleton < type > :: m_singleton = 0;
#endif
