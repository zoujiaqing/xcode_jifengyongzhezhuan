#ifndef MANAGER_HPP_
#define MANAGER_HPP_
#include <typeinfo>
#include "stdint.h"
#include <map>
#include <assert.h>
#include "sigslot.hpp"
#include "Singleton.hpp"

namespace base {

template <typename KeyType=uint32_t>
class Object {
public:
typedef KeyType Key;
typedef Object<KeyType> BaseType;

	Object(Key id = Key()): id_(id), m_del(false) {
	}

	Key get_id() const {
		return id_;
	}

	const std::type_info & key_type() const {
		return typeid(KeyType);		
	}

	sigslot::signal1<BaseType*> sig_obj_del;

	void safe_release() {
		sig_obj_del(this);
		m_del = true;
		delete this;
	};
	
protected:
	virtual ~Object() {assert(m_del);};

	Key id_;
	
	bool m_del;
};

/*
template<typename KeyType>
Object<KeyType>::~Object(){
	sig_obj_del(this);
}*/

template<typename ObjectClass>
class Linker {
public:
	Linker(ObjectClass* pobject):
		linker_(pobject)
	{
		pobject->sig_obj_del.connect(this, &Linker<ObjectClass>::on_linker_del);
	}

	ObjectClass* operator()() {
		return linker_;
	}

	void on_linker_del(Object<typename ObjectClass::Key>* object) {
		delete this;
	}

private:
	ObjectClass* linker_;
};

template <typename ObjectClass>
class Manager: public sigslot::has_slots<> {
public: 
typedef typename ObjectClass::Key Key; 
typedef typename ObjectClass::BaseType ObjectType; 
	Manager() {
    }
	~Manager() {
		sig_manager_del(this);
    }
    Manager& operator=(const Manager&);

	ObjectClass* get_ref_by_key(Key key);
    
	void add_obj(ObjectClass* obj);
	void add_obj(ObjectClass& obj);

	void add_copy_obj(ObjectClass* obj);
	void add_copy_obj(ObjectClass& obj);

	void reduce_obj(ObjectClass* obj);
	void reduce_obj(ObjectClass& obj);
	void reduce_obj(Key key);

	virtual void do_empty(){};

	bool empty() const;
	
	uint32_t size() const;
	
	bool exist_key(Key key);

	bool exist_obj(ObjectClass* obj);
	
	void clear();

	sigslot::signal2< Manager<ObjectClass>*,ObjectClass* > sig_obj_pre_increase;
	sigslot::signal2< Manager<ObjectClass>*,ObjectClass*> sig_obj_post_increase;
	sigslot::signal2< Manager<ObjectClass>*,ObjectClass*> sig_obj_pre_decrease;
	sigslot::signal2< Manager<ObjectClass>*,ObjectClass*> sig_obj_post_decrease;
	
	sigslot::signal1< Manager<ObjectClass>* > sig_manager_del;
    /*slots*/

    void on_obj_del(ObjectType*);

	ObjectClass* next_ref(ObjectClass* pObj=NULL);

	Key next_key(Key key=Key());

protected:
	typedef std::map<Key, ObjectClass*> ObjectMap;
    ObjectMap m_mapObj;
};
	
template <typename ObjectClass>
ObjectClass* Manager<ObjectClass>::get_ref_by_key(Key key) {
	typename ObjectMap::iterator itr = m_mapObj.find(key);
	if(itr != m_mapObj.end()) {
		return m_mapObj[key];	
	}
	return NULL;
}
    
template <typename ObjectClass>
void Manager<ObjectClass>::on_obj_del(ObjectType* pObj) {
	reduce_obj( static_cast<ObjectClass*>(pObj) );	
}
	
template <typename ObjectClass>
void Manager<ObjectClass>::add_obj(ObjectClass* pObj) {
	typename ObjectMap::iterator itr = m_mapObj.find(pObj->get_id());
	if(itr == m_mapObj.end()) {
		Key key =pObj->get_id();
		sig_obj_pre_increase(this, pObj);
		m_mapObj[key] = pObj;
		sig_obj_post_increase(this, pObj);
		pObj->sig_obj_del.connect(this, &Manager<ObjectClass>::on_obj_del);
	}
}

template <typename ObjectClass>
void Manager<ObjectClass>::add_obj(ObjectClass& pObj) {
	add_obj(&pObj);
}

template <typename ObjectClass>	
void Manager<ObjectClass>::add_copy_obj(ObjectClass* obj) {
	add_copy_obj(*obj);
}

template <typename ObjectClass>	
void Manager<ObjectClass>::add_copy_obj(ObjectClass& obj) {
	ObjectClass* obj_copy = new ObjectClass(obj);
	add_obj(obj_copy);
}

template <typename ObjectClass>
void Manager<ObjectClass>::reduce_obj(ObjectClass* pObj) {
	if(!pObj) return ;
	typename ObjectMap::iterator itr = m_mapObj.find(pObj->get_id());
	if(itr != m_mapObj.end()) {
		ObjectClass* pDelObj = itr->second;
		sig_obj_pre_decrease(this, pDelObj);
		m_mapObj.erase(itr);
		sig_obj_post_decrease(this, pDelObj);	
	}
	if(empty())	 {
		do_empty();
	}
}

template <typename ObjectClass>
void Manager<ObjectClass>::reduce_obj(Key key) {
	typename ObjectMap::iterator itr = m_mapObj.find(key);
	if(itr != m_mapObj.end()) {
		ObjectClass* pDelObj = itr->second;
		sig_obj_pre_decrease(this, pDelObj);
		m_mapObj.erase(itr);
		sig_obj_post_decrease(this, pDelObj);	
	}	
	if(empty())	 {
		do_empty();
	}
}

template <typename ObjectClass>
void Manager<ObjectClass>::reduce_obj(ObjectClass& pObj) {
	reduce_obj(&pObj);
}	

template <typename ObjectClass> 	
bool Manager<ObjectClass>::empty() const {
	return m_mapObj.empty();
}
	
template <typename ObjectClass>
uint32_t Manager<ObjectClass>::size() const {
	return m_mapObj.size();
}
	
template <typename ObjectClass>
Manager<ObjectClass>& Manager<ObjectClass>::operator=(const Manager<ObjectClass>&) {
	return *this;
}

template <typename ObjectClass>
bool Manager<ObjectClass>::exist_key(Key key) {
	return m_mapObj.find(key) != m_mapObj.end();
}
	
template <typename ObjectClass>
bool Manager<ObjectClass>::exist_obj(ObjectClass* obj) {
	typename ObjectMap::iterator itr = m_mapObj.begin();
	for(; itr!=m_mapObj.end(); itr++) {
		if(itr->second == obj) return true;
	}
	return false;
}
	
template <typename ObjectClass>
void Manager<ObjectClass>::clear() {
	typename ObjectMap::iterator itr = m_mapObj.begin();
	for(; itr!=m_mapObj.end();) {
		typename ObjectMap::iterator itr_tmp = itr;
		ObjectClass* pDelObj = itr->second;
		itr++ ;
		sig_obj_pre_decrease(this, pDelObj);
		m_mapObj.erase(itr_tmp);
		sig_obj_post_decrease(this, pDelObj);	
	}
}

template <typename ObjectClass>
ObjectClass* Manager<ObjectClass>::next_ref(ObjectClass* pObj) {
	if(!pObj) {
		if(!m_mapObj.empty()) {
			return m_mapObj.begin()->second;
		} else {
			return NULL;
		}
	}
	typename ObjectMap::iterator itr = m_mapObj.upper_bound(pObj->get_id());
	if(itr!=m_mapObj.end()) {
		return itr->second;
	} else {
		return NULL;
	}
}

template <typename ObjectClass>
typename Manager<ObjectClass>::Key Manager<ObjectClass>::next_key(Key key) {
	bool hit = false;
	if(key == Key()) hit= true;
	typename ObjectMap::iterator itr = m_mapObj.begin();
	for(; itr!=m_mapObj.end(); itr++) {
		if( hit && !(itr->first == key) ) {
			return itr->first ;
		} else if ( key < itr->first ) {
			return itr->first;
		} else if ( itr->first == key ) {
			hit = true;
		}
	}
	return Key();
}

template <typename ObjectClass>
class GManager: public sigslot::has_slots<> {
public: 
typedef typename ObjectClass::Key Key; 
typedef typename ObjectClass::BaseType ObjectType; 
typedef Manager<ObjectClass> ManagerType; 
	GManager() {
    
	}

    GManager& operator=(const GManager&);

	ManagerType* get_manager_by_key(Key key);
    
	void add_manager(ManagerType* manager);
	void add_manager(ManagerType& manager);

	void reduce_manager(ManagerType* manager);
	void reduce_manager(ManagerType& manager);
	
	void clear();
	
	sigslot::signal1<ManagerType*> sig_manager_pre_increase;
	sigslot::signal1<ManagerType*> sig_manager_post_increase;
	sigslot::signal1<ManagerType*> sig_manager_pre_decrease;
	sigslot::signal1<ManagerType*> sig_manager_post_decrease;
    /*slots*/

	 void on_manager_increase_obj(ManagerType* manager, ObjectClass* obj);   	
	 void on_manager_decrease_obj(ManagerType* manager, ObjectClass* obj);   
	
	 void on_manager_del(ManagerType*);

	 bool exist_obj(const ObjectClass* obj);
	
	 bool exist_obj(ObjectClass& obj);

protected:
	typedef std::map<Key, ManagerType*> ManagerMap;
    ManagerMap m_mapManagers;
};

template<typename ObjectClass>
typename GManager<ObjectClass>::ManagerType* GManager<ObjectClass>::get_manager_by_key(Key key) {
	if(m_mapManagers.find(key) != m_mapManagers.end() ) {
		return m_mapManagers[key];
	}
	else return NULL;
}
	
template<typename ObjectClass>
void GManager<ObjectClass>::add_manager(ManagerType* manager) {
	Key key = manager->next_key(Key());
	for(; key!=Key(); key = manager->next_key(key)) {
		if(m_mapManagers.find(key) != m_mapManagers.end() && m_mapManagers[key] != manager ) {
			m_mapManagers[key]->reduce_obj(key);
		}
		m_mapManagers[key] = manager;
	}
	manager->sig_obj_post_increase.connect(this, &GManager<ObjectClass>::on_manager_increase_obj);
	manager->sig_obj_pre_decrease.connect(this, &GManager<ObjectClass>::on_manager_decrease_obj);
	manager->sig_manager_del.connect(this, &GManager<ObjectClass>::on_manager_del);
}

template<typename ObjectClass>
void GManager<ObjectClass>::add_manager(ManagerType& manager) {
	add_manager(&manager);
}

template<typename ObjectClass>	
void GManager<ObjectClass>::reduce_manager(ManagerType* manager) {
	Key key = manager->next_key(Key());
	for(; key!=Key(); key = manager->next_key(Key())) {
		m_mapManagers.erase(key);
	}
	manager->sig_obj_pre_increase.disconnect(this);
	manager->sig_obj_post_decrease.disconnect(this);
}

template<typename ObjectClass>
void GManager<ObjectClass>::reduce_manager(ManagerType& manager) {
	reduce_manager(&manager);
}
	
template<typename ObjectClass>
void GManager<ObjectClass>::clear() {
	typename ManagerMap::iterator itr = m_mapManagers.begin();
	for(; itr!=m_mapManagers.end(); itr++) {
		(*itr)->sig_obj_pre_increase.disconnect(this);
		(*itr)->sig_obj_post_decrease.disconnect(this);
	}	
}
	 
template<typename ObjectClass>
void GManager<ObjectClass>::on_manager_increase_obj(ManagerType* manager, ObjectClass* obj) {
	Key key = obj->get_id();
	if(m_mapManagers.find(key) != m_mapManagers.end() && m_mapManagers[key] != manager ) {
		m_mapManagers[key]->reduce_obj(key);
	}
	m_mapManagers[key] = manager;
}
	 
template<typename ObjectClass>
void GManager<ObjectClass>::on_manager_decrease_obj(ManagerType* manager, ObjectClass* obj) {
	m_mapManagers.erase(obj->get_id());
} 
	 
template<typename ObjectClass>
void GManager<ObjectClass>::on_manager_del(ManagerType* manager) {
	Key key = manager->next_key(Key());
	for(; key!=Key(); key = manager->next_key(key)) {
		m_mapManagers.erase(key);
	}
}
	 
template<typename ObjectClass>
bool GManager<ObjectClass>::exist_obj(const ObjectClass* obj) {
	return m_mapManagers.find(obj->get_id()) != m_mapManagers.end();
}

template<typename ObjectClass>
bool GManager<ObjectClass>::exist_obj(ObjectClass& obj) {
	return exist_obj(&obj);
}

}
#endif
