// TAOMEE GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Taomee Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 2012-2015 Taomee Game Technologies. 
//      All Rights Reserved.
//
// Taomee Game Technologies, Shanghai, China
// http://www.taomee.com
//

#ifndef iSeer_UIControlBase_h
#define iSeer_UIControlBase_h

#include "TXGUI.h"
#include "NiRTTI.h"
#include "NiStreamMacros.h"
#include "NiObject.h"


#include <list>


class ASprite;

namespace TXGUI {
    USING_NS_CC;
    class IconButton;

    class UIControlBase : public NiObject
    {      
        NiDeclareRTTI;
        NiDeclareAbstractStream;
        
    public:
        UIControlBase(cocos2d::CCNode* pParentNode);
		UIControlBase(std::string name, int tag, float z, UIControlBase *parent, CCSize size);
		UIControlBase(std::string name, int tag, float z, UIControlBase *parent, CCSize size, CCNode* controlNode);
		virtual ~UIControlBase();
        
        // local position 
        virtual CCPoint getPosition() {
            return m_ptLocalPosition;
        }
        
        virtual void setPosition(CCPoint pt) {
            m_ptLocalPosition = pt; 
        }
        
        // world position 
        virtual CCPoint getWorldPosition() {
            return m_ptWorldPosition;
        }
        
        virtual void setWorldPosition(CCPoint pt) {
            m_ptWorldPosition = pt; 
        }

		// Z order
		virtual float getZOrder() {
			return m_fPosZ;
		}

		virtual void setZOrder(float posZ) {
			m_fPosZ = posZ;

			if(m_pControlNode->getParent())
			{
				m_pParentNode = m_pControlNode->getParent();
				m_pControlNode->removeFromParentAndCleanup(false);
				m_pParentNode->addChild(m_pControlNode, m_fPosZ);
			}
		}

		// scale
		virtual void setScale(float scale)
		{
			if(!m_bScaleSetted)
			{
				m_bScaleSetted = true;
				setPosition(ccp(m_ptLocalPosition.x * scale, m_ptLocalPosition.y * scale));
				CalWorldPos();
				m_uiContentSize.width *= scale;
				m_uiContentSize.height *= scale;
			}

			for(list<UIControlBase*>::iterator it = m_ChildrenControls.begin();
				it != m_ChildrenControls.end(); it++)
			{
				(*it)->setScale(scale);
			}
		}

		virtual void setScale(float xScale, float yScale)
		{
			if(!m_bScaleSetted)
			{
				m_bScaleSetted = true;
				setPosition(ccp(m_ptLocalPosition.x * xScale, m_ptLocalPosition.y * yScale));
				CalWorldPos();
				m_uiContentSize.width *= xScale;
				m_uiContentSize.height *= yScale;
			}

			for(list<UIControlBase*>::iterator it = m_ChildrenControls.begin();
				it != m_ChildrenControls.end(); it++)
			{
				(*it)->setScale(xScale, yScale);
			}
		}

		virtual void CalWorldPos()
		{
			CCPoint localPos = getPosition();
			CCPoint newWorldPos = LocalPt2World(localPos);
			setWorldPosition(newWorldPos);
		}

		virtual void UpdateWorldPosition() {

			CalWorldPos();

			for(list<UIControlBase*>::iterator it = m_ChildrenControls.begin();
				it != m_ChildrenControls.end(); it++)
			{
				(*it)->UpdateWorldPosition();
			}
		}
        
        virtual CCPoint LocalPt2World(CCPoint pt);
        
		virtual CCAffineTransform getLocal2WorldTransform()
        {            
            CCAffineTransform trans = CCAffineTransformTranslate(CCAffineTransformIdentity,                                                                  
                                       m_ptLocalPosition.x,
                                       m_ptLocalPosition.y);
            if (m_pParent)
            {
				CCPoint parentWorldPosition = m_pParent->getWorldPosition();
				CCAffineTransform parentTrans = CCAffineTransformTranslate(CCAffineTransformIdentity,
										parentWorldPosition.x,
										parentWorldPosition.y);
                trans = CCAffineTransformConcat(parentTrans,trans);
            }
            
            return trans;
        }
        
        // audio effect
        virtual void setAudioEffect(int iAudio){
            m_iAudioEffect = iAudio;
        }
        
        virtual void PlayAudio();

        virtual void StopAudio();
        
        virtual void PauseAudio();
        
        // tag of Control
        virtual int  getTag() {
            return m_iTag;
        };
        virtual void setTag(int iTag) {
            m_iTag = iTag;
        };
        
        // name of control
        virtual void setName(std::string name) {
            m_strName = name;
        }
        
        virtual std::string getName() {
            return m_strName;
        }
        
        virtual cocos2d::CCNode* getCurrentNode() {return m_pControlNode;};
        
        void setParent(UIControlBase* pParent);
        
        UIControlBase* getParent() {return m_pParent;};

		std::list<UIControlBase*> &getChildren(){ return m_ChildrenControls;}
        
        /**
         * @brief 增加一个子结点
         * @param pChild, 子结点
         * @return void, 
         * @
         */
        void AddChild(UIControlBase* pChild);
        
        /**
         * @brief 删除一个子结点
         * @param pChild, 子结点
         * @return void, 
         * @
         */
        void RemoveChild(UIControlBase* pChild, bool clean = false);
        
        /**
         * @brief 删除所有子结点
         * @return void, 
         * @
         */
        void RemoveAllChildrenWithCleanup();

        /**
         * @brief 删除一个子结点以及所有其儿子节点
         * @param name, 子结点的名字
         * @return void, 
         * @
         */
        void RemoveChildTreeByName(string name);
        
        /**
         * @brief 根据名字，查找一个子节点对象
         * @param name, 子结点名字
         * @return UIControlBase*, 节点对象
         * @
         */   
        UIControlBase* FindChildObjectByName(string name);
        
        template<class T>
        T* FindChildObjectByName(string name)
        {   
            // compare children name according to the given parameter
            for(std::list<UIControlBase*>::iterator it = m_ChildrenControls.begin();
                it != m_ChildrenControls.end(); 
                it++)
            {
                UIControlBase* pChild = *it;
                string val = pChild->getName();
                if (val.compare(name) == 0 && val.length() == name.length())
                {
                    if (dynamic_cast<T*>(pChild) != NULL)
                    {
                        return static_cast<T*>(pChild);
                    }
                }
            }   
            
            // search children node
            for(std::list<UIControlBase*>::iterator it = m_ChildrenControls.begin();
                it != m_ChildrenControls.end(); 
                it++)
            {
                T* pChild = (*it)->FindChildObjectByName<T>(name);
                if (NULL != pChild)
                {
                    return pChild;
                }
            } 
            
            return NULL;
        }

		//by benyang
		/**
		 * @brief get all children ending with the name including children's children
		 * @param name specified name
		 * @return container children pointer list
		 * @
		 */

        list<UIControlBase*> getAllChildEndsWithName(string name)
        {   
			list<UIControlBase*> res;

            // compare children name according to the given parameter
            for(std::list<UIControlBase*>::iterator it = m_ChildrenControls.begin();
                it != m_ChildrenControls.end(); 
                it++)
            {
                UIControlBase* pChild = *it;
                string val = pChild->getName();
                if (val.length() >= name.length() && val.substr(val.length() - name.length()) == name)
                {
                    if (pChild != NULL)
                    {
						res.push_back(pChild);
                    }
                }
            }

			// search children node
            for(std::list<UIControlBase*>::iterator it = m_ChildrenControls.begin();
                it != m_ChildrenControls.end(); 
                it++)
            {
                list<UIControlBase*> childRes = (*it)->getAllChildEndsWithName(name);
                if (!childRes.empty())
                {
                    for(std::list<UIControlBase*>::iterator it2 = childRes.begin();
						it2 != childRes.end(); 
						it2++)
					{
						res.push_back(*it2);
					}
                }
            } 

			return res;
        }
        
        /**
         * @brief 绑定结点
         * @param pChild, 子结点
         * @return void, 
         * @
         */
        virtual void BindNode(CCNode* pParentNode);
        
        virtual CCNode* getBindNode();

		virtual void setOpacity(GLubyte var);

		//virtual void setRoot(UIControlBase* pRootNode);

		//virtual UIControlBase* getRoot();

		virtual void setLuaScriptFile(std::string strFile);

		virtual std::string getLuaScriptFile();

		virtual void executeLuaScript();
        
		virtual void setTouchEnabled(bool value);

		/// show /hide function
		virtual void setVisible(bool visible);

		virtual bool isVisible(){return m_bIsVisible;}

		virtual CCSize getUIContentSize(){return m_uiContentSize;}

		//// for merge texture by hand
		CCTexture2D* RecursiveFindTexture(CCSprite* sprite);

		TXGUI::IconButton* FindIconButtonChildByName(string name);

    protected:
        UIControlBase();       

		/// color attribute
		void	GetColor(mutableDic* dic, NiStream &kStream, uint32_t& r, uint32_t& g, uint32_t & b);

		/// content attribute
		string  GetContent(mutableDic* dic, NiStream &kStream);

		//// merge small sprite to a big one use sprite batchnode
        CCSprite*	MergeSmallASprite(CCSprite* sprite);

		ASprite*	GetASpriteCfg(NiStream &kStream, mutableDic *bgDic);
		
		CCSprite*   GetASprite(NiStream &kStream, mutableDic *bgDic, ASprite* as, string key);
		
		CCSprite*   GetASprite(NiStream &kStream, mutableDic *bgDic, ASprite* as, string key, CCPoint& pt);

		TXGUI::IconButton* FindIconButtonInLayer(CCNode* root,string name);
    protected:
        int     m_iAudioEffect;     
        CCPoint m_ptLocalPosition;
        CCPoint m_ptWorldPosition;
		float	m_fPosZ;
		bool	m_bScaleSetted;
		bool	m_bIsVisible;
        
        int     m_iTag;
        std::string      m_strName;
        cocos2d::CCNode* m_pParentNode;
        cocos2d::CCNode* m_pControlNode;
        UIControlBase*   m_pParent;
		//UIControlBase*	 m_pRoot;
        std::list<UIControlBase*>m_ChildrenControls;

		std::string		 m_strLuaScriptFile;
        
        CCNode* m_pBindNode;
		CCSize	m_uiContentSize;
    };

	typedef  list<UIControlBase*> ControlList;
	typedef  ControlList::iterator ControlListInterator;
}


#endif
