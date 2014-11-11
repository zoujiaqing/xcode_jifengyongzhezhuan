// ASprite.cpp 


#include "ASprite.h"
#include "string.h"
#include "GameResourceManager.h"
#include <memory.h>
#include <fstream>
#include <iostream>

#include "cocos2d.h"
#include "GameResourceManager.h"

#define ANIM_SCALE 1.0f

USING_NS_CC;

using namespace std;

void ASprite::initSpriteFrame()
{
    
}

void ASprite::loadFile(const char *path, const char* pngname)
{ 
    m_strImageName[0] = pngname;
    loadFile(path);
}

std::string& ASprite::getImageName(int index)
{

	if (m_strImageName.find(index) != m_strImageName.end())
	{
		m_imagePath = m_strImageName[index];
	}else
	{
		// return the default image
		CCAssert(false, "can't find the image, please use setImageName() to set up the images of Asprite.");
		m_imagePath = m_strImageName[0];
	}

	m_imagePath = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(m_imagePath.c_str());
	return m_imagePath;

}

void ASprite::setImageName(int index, std::string ImageName)
{
	m_strImageName[index] = ImageName;
}

// read file to buffer
void ASprite::loadFile(const char *fileName)
{
	CCLOG("ASprite::loadFile(%s)", fileName);

    //by Stanley
	const char * filePath = GameResourceManager::sharedManager()->storedFullPathFromRelativePath(fileName);
	CCFileData data(filePath, "rb");
    _temp_buff = gll_new BBYTE[(data.getSize() + 1)];
    memset(_temp_buff, 0, data.getSize()+1);
    memcpy(_temp_buff, data.getBuffer(), data.getSize());
    
    Load((BBYTE *)_temp_buff, 0);
}
// ASprite_Load.hxx
////////////////////////////////////////////////////////////////////////////////////////////////////
// analysis file for each 
void ASprite::loadRGBImage(BBYTE* file, int offset)
{    
	_nModules = (file[offset]&0xFF) + ((file[offset+1]&0xFF)<<8);offset += 2;
    
    // 读取所有module的信息
	if (_nModules > 0)
	{
		//printf("_nModules=%d\n",_nModules);
		_modules_w = gll_new T_MODULE_WH[_nModules];
		_modules_h = gll_new T_MODULE_WH[_nModules];
        
		_modules_x = gll_new T_MODULE_WH[_nModules];
		_modules_y = gll_new T_MODULE_WH[_nModules];

		_modules_image = gll_new T_MODULE_WH[_nModules];
        
        TEST_BSPRITE_FLAGS(BS_MODULE_IMAGES)
        {
            _module_textures_w = gll_new int[_nModules];
            _module_textures_h = gll_new int[_nModules];
        }
        
#ifdef USE_MULTIPLE_MODULE_TYPES			
		_module_types = gll_new BBYTE[_nModules];
		_module_colors = gll_new int[_nModules];
#endif			
        
		for (int i = 0; i < _nModules; i++)
		{
			
            
			//offset+=4;//4 bytes -> color
			if ((file[offset]&0xFF) == 0xFF)
			{
				offset++;
#ifdef USE_MULTIPLE_MODULE_TYPES
				_module_types[i] = MD_RECT;
				_module_colors[i] = (file[offset]&0xFF) +
                ((file[offset+1]&0xFF)<<8) + 
                ((file[offset+2]&0xFF)<<16) + 
                ((file[offset+3]&0xFF)<<24);
#endif
				offset += 4;
			}
			else if ((file[offset]&0xFF) == 0xFE)
			{
				offset++;
#ifdef USE_MULTIPLE_MODULE_TYPES                
				_module_types[i] = MD_FILL_RECT;
				_module_colors[i] = (file[offset]&0xFF) +
                ((file[offset+1]&0xFF)<<8) + 
                ((file[offset+2]&0xFF)<<16) + 
                ((file[offset+3]&0xFF)<<24);
#endif
				offset += 4;
			}
			else
			{
				offset++;//1 byte -> 0xFF
#ifdef USE_MULTIPLE_MODULE_TYPES                
				_module_types[i] = MD_IMAGE;
#endif
			}

			// first field is img index!
			TEST_BSPRITE_FLAGS((BS_MODULES_IMG))
			{
				_modules_image[i] = (short)(file[offset] & 0xFF);				
				offset += 1;
			}
            
			TEST_BSPRITE_FLAGS(BS_MODULES_XY)
			{
				_modules_x[i] = (short)((file[offset]&0xFF) + ((file[offset+1]&0xFF)<<8));
				_modules_y[i] = (short)((file[offset+2]&0xFF) + ((file[offset+3]&0xFF)<<8));
				offset += 4;
			}
            
			TEST_BSPRITE_FLAGS(BS_MODULES_XY_SHORT)
			{
				_modules_x[i] = (short)((file[offset]&0xFF) + ((file[offset+1]&0xFF)<<8));
				_modules_y[i] = (short)((file[offset+2]&0xFF) + ((file[offset+3]&0xFF)<<8));
				offset += 4;
			}
            
			TEST_BSPRITE_FLAGS(BS_MODULES_WH_SHORT)
			{
				_modules_w[i] = (short)((file[offset]&0xFF) + ((file[offset+1]&0xFF)<<8));
				_modules_h[i] = (short)((file[offset+2]&0xFF) + ((file[offset+3]&0xFF)<<8));
				offset += 4;
			}

			TEST_BSPRITE_FLAGS(BS_MODULE_IMAGES)
			{
                _module_textures_w[i] = 1;
                _module_textures_h[i] = 1;
                
				while(_module_textures_w[i] < (_modules_w[i]&0xFFFF))
					_module_textures_w[i] <<= 1;
                
				while(_module_textures_h[i] < (_modules_h[i]&0xFFFF))
					_module_textures_h[i] <<= 1;
			}
            
		}
	}
    
	//////////////////////////////////////////////////////////////////////////
    // 读取所有 fmodule的信息
	int nFModules = (file[offset]&0xFF) + ((file[offset+1]&0xFF)<<8);offset+=2;
	if (nFModules > 0)
	{
		//printf("nFModules=%d\n",nFModules);
		_fmodules_id = gll_new BBYTE[nFModules];
		_fmodules_ox = gll_new T_FMODULE_OFF[nFModules];
		_fmodules_oy = gll_new T_FMODULE_OFF[nFModules];
		_fmodules_flags = gll_new BBYTE[nFModules];
        
		for(int i = 0; i < nFModules; i++)
		{
			_fmodules_id[i] = file[offset++];
            
#ifdef USE_FM_OFF_SHORT
			TEST_BSPRITE_FLAGS(BS_FM_OFF_SHORT)
			{
				_fmodules_ox[i] = (short)((file[offset]&0xFF) + ((file[offset+1]&0xFF)<<8));
				_fmodules_oy[i] = (short)((file[offset+2]&0xFF) + ((file[offset+3]&0xFF)<<8));
				offset += 4;
			}
#ifdef USE_BSPRITE_FLAGS
            else
            {
                _fmodules_ox[i] = (short)(file[offset++]);
                _fmodules_oy[i] = (short)(file[offset++]);
            }
#endif //USE_BSPRITE_FLAGS
#else //USE_FM_OFF_SHORT
			_fmodules_ox[i] = file[offset++];
			_fmodules_oy[i] = file[offset++];
#endif //USE_FM_OFF_SHORT
            
			_fmodules_flags[i] = file[offset++];
		}
	}
    
	/////////////////////////////////////////////////////////////
    //读取所有frame 信息
	m_nFrames = (file[offset]&0xFF) + ((file[offset+1]&0xFF)<<8);offset += 2;
    
	if (m_nFrames > 0)
	{
		_frames_nfm      = gll_new  BBYTE[m_nFrames];
		_frames_fm_start = gll_new short[m_nFrames];
#ifdef ALWAYS_BS_NO_FM_START
		short frame_start = 0;
#endif //ALWAYS_BS_NO_FM_START
		for (int i = 0; i < m_nFrames; i++)
		{
			_frames_nfm[i] = file[offset++]; 
#ifndef ALWAYS_BS_NFM_1_BYTE
			offset++;
#endif //ALWAYS_BS_NFM_1_BYTE
			//	_frames_nfm[i] = (short)((file[offset++]&0xFF) + ((file[offset++]&0xFF)<<8));
            
#ifdef ALWAYS_BS_NO_FM_START
			_frames_fm_start[i] = frame_start;
			frame_start += _frames_nfm[i] & 0xFF;
#else //ALWAYS_BS_NO_FM_START
			_frames_fm_start[i] = (short)((file[offset]&0xFF) + ((file[offset+1]&0xFF)<<8));
			offset+=2;
#endif //ALWAYS_BS_NO_FM_START
		}
        
#ifndef ALWAYS_BS_SKIP_FRAME_RC
		{
            offset += (m_nFrames<<3);
#ifdef USE_PRECOMPUTED_FRAME_RECT
			TEST_BSPRITE_FLAGS(BS_FRAME_COLL_RC)
			{
				// Bound rect for each frame...
				int nFrames4 = m_nFrames<<2;
				_frames_rc = gll_new short[nFrames4];
				TEST_BSPRITE_FLAGS(BS_FM_OFF_SHORT)
				{
					for (int i = 0; i < nFrames4; i++)
					{
						_frames_rc[i] = (short)((file[offset]&0xFF) + ((file[offset+1]&0xFF)<<8));
						offset+=2;
					}
				}else
                {						
					for (int i = 0; i < nFrames4; i++)
						_frames_rc[i] = file[offset++];
				}
			}
#else //USE_PRECOMPUTED_FRAME_RECT
			offset += (m_nFrames<<2);
#endif //USE_PRECOMPUTED_FRAME_RECT            
			
		}
#else //ALWAYS_BS_SKIP_FRAME_RC
		{
#ifdef USE_PRECOMPUTED_FRAME_RECT
			{
				// TODO: precompute frame rc
			}
#endif //USE_PRECOMPUTED_FRAME_RECT
		}
#endif //ALWAYS_BS_SKIP_FRAME_RC
        
#ifdef USE_FRAME_COLL_RC
		TEST_BSPRITE_FLAGS(BS_FRAME_COLL_RC)
		{
			// Collision rect for each frame...
			int nFrames4 = m_nFrames<<2;
			_frames_col = gll_new BBYTE[nFrames4];
			for (int i = 0; i < nFrames4; i++)
            {
				_frames_col[i] = file[offset++];
            }
		}
#endif //USE_FRAME_COLL_RC
	}
    
	//////////////////////////////
	// AFrames...
    
	m_nAFrames = (file[offset]&0xFF) + ((file[offset+1]&0xFF)<<8);offset+=2;		
    
	if (m_nAFrames > 0)
	{
		_aframes_frame = gll_new BBYTE[m_nAFrames];
		_aframes_time = gll_new BBYTE[m_nAFrames];
		_aframes_ox = gll_new T_AFRAME_OFF[m_nAFrames];
		_aframes_oy = gll_new T_AFRAME_OFF[m_nAFrames];
		_aframes_flags = gll_new BBYTE[m_nAFrames];
        
		for(int i = 0; i < m_nAFrames; i++)
		{
			_aframes_frame[i] = file[offset++];
			_aframes_time[i] = file[offset++];
            
#ifdef USE_AF_OFF_SHORT
			TEST_BSPRITE_FLAGS(BS_AF_OFF_SHORT)
			{
				_aframes_ox[i] = (short)((file[offset]&0xFF) + ((file[offset+1]&0xFF)<<8));
				_aframes_oy[i] = (short)((file[offset+2]&0xFF) + ((file[offset+3]&0xFF)<<8));
				offset += 4;
			}
#ifdef USE_BSPRITE_FLAGS
            else
            {
                _aframes_ox[i] = (short)(file[offset++]);
                _aframes_oy[i] = (short)(file[offset++]);
            }
#endif //USE_BSPRITE_FLAGS
#else //USE_AF_OFF_SHORT
			_aframes_ox[i] = file[offset++];
			_aframes_oy[i] = file[offset++];
#endif //USE_AF_OFF_SHORT
            
			_aframes_flags[i] = file[offset++];
		}
	}
    
	//////////////////////////////
	// Anims...
    
	m_nAnims = (file[offset] &0xFF) + ((file[offset+1]&0xFF)<<8);offset+=2;
	if (m_nAnims > 0)
	{
		_anims_naf = gll_new BBYTE[m_nAnims];
		// --- BREW ver
		_anims_af_start = gll_new short[m_nAnims];
#ifdef ALWAYS_BS_NO_AF_START
		short af_start = 0;
#endif //ALWAYS_BS_NO_AF_START
		for (int i = 0; i < m_nAnims; i++)
		{
			_anims_naf[i] = file[offset++];
#ifndef ALWAYS_BS_NFM_1_BYTE
			offset++;
#endif //ALWAYS_BS_NFM_1_BYTE
            
            
#ifdef ALWAYS_BS_NO_AF_START
			_anims_af_start[i] = af_start;
			af_start += _anims_naf[i];
#else //ALWAYS_BS_NO_AF_START
			_anims_af_start[i] = (short)((file[offset]&0xFF) + ((file[offset+1]&0xFF)<<8));offset+=2;
#endif //ALWAYS_BS_NO_AF_START
		}
	}
    
	//////////////////////////////
    // image

    
    // release buffer
	SAFE_DEL_ARRAY(_temp_buff);
    
    //initSpriteFrame();
}



void ASprite::Load(BBYTE* file, int offset)
{
	// BSprite version...
    
	_bs_flags = ((file[offset+2]&0xFF)    ) +
    ((file[offset+3]&0xFF)<< 8) +
    ((file[offset+4]&0xFF)<<16) +
    ((file[offset+5]&0xFF)<<24);	
	
    offset += 6;
    //TEST_BSPRITE_FLAGS(BS_RGB_8888_IMAGES)
	{
		//printf("image type: RGB_8888\n");
		loadRGBImage(file,offset);
		return;
	} 
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//
//void ASprite::FreeCacheData()
//{
//}

////////////////////////////////////////////////////////////////////////////////////////////////////

int ASprite::GetAFrameTime(int anim, int aframe)
{
    return _aframes_time[_anims_af_start[anim] + aframe] & 0xFF;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int ASprite::GetAFrames(int anim)
{
    return _anims_naf[anim]&0xFF;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int ASprite::GetFModules(int frame)
{
    return _frames_nfm[frame]&0xFF;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
/// Get the id of a module in a frame
/// @param frame the frame containing the module
/// @param fmodule the module to be examined
/// @returns the module id
//------------------------------------------------------------------------------
int ASprite::GetFrameModule( int frame, int fmodule )
{
    int off, fm_flags, index;
    
    off      = _frames_fm_start[ frame ] + fmodule;
    fm_flags = _fmodules_flags [ off ] & 0xFF;
    index    = _fmodules_id    [ off ] & 0xFF;
    
    index |= ((fm_flags & FLAG_INDEX_EX_MASK) << INDEX_EX_SHIFT);
    return index;
}


////////////////////////////////////////////////////////////////////////////////////////////////////

int ASprite::GetModuleX(int module)
{
	return (_modules_x[module] & MODULE_WH_MASK);
}

int ASprite::GetModuleY(int module)
{
	return (_modules_y[module] & MODULE_WH_MASK);
}

int ASprite::GetModuleWidth(int module)
{
    return _modules_w[module]&MODULE_WH_MASK;
}

int ASprite::GetModuleHeight(int module)
{
    return _modules_h[module]&MODULE_WH_MASK;
}
////////////////////////////////////////////////////////////////////////////////////////////////////

int ASprite::GetFrameWidth(int frame)
{
	if (_frames_rc == null)
		return 0;//brew:to do?
//    short temp = _frames_rc[frame*4 + 2]&MODULE_WH_MASK;
    return _frames_rc[frame*4 + 2]&MODULE_WH_MASK;
}

int ASprite::GetFrameHeight(int frame)
{
	if (_frames_rc == null)
		return 0;
    return _frames_rc[frame*4 + 3]&MODULE_WH_MASK;
}

int ASprite::GetFrameStartX(int frame)
{
    if (_frames_rc == null)
        return 0;//brew:to do?
    return _frames_rc[frame*4];
}
int ASprite::GetFrameStartY(int frame)
{
    if (_frames_rc == null)
        return 0;//brew:to do?
    return _frames_rc[frame*4 + 1];
}
CCPoint ASprite::GetFramePoint(int frame)
{
    if(frame >= m_nFrames || frame < 0)
    {
        return CCPointMake(0, 0);
    }
    float x = GetFrameStartX(frame);
    float y = -GetFrameStartY(frame);

	float deviceScale = CC_CONTENT_SCALE_FACTOR();

    CCPoint point;
	point.x = x / deviceScale;
	point.y = y / deviceScale;
    
    return point;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//opa --- #ifdef UNUSED_FUNCTIONS
int ASprite::GetFrameModuleX(int frame, int fmodule)
{
    return _fmodules_ox[_frames_fm_start[frame] + fmodule];
}

int ASprite::GetFrameModuleY(int frame, int fmodule)
{
    return _fmodules_oy[_frames_fm_start[frame] + fmodule];
}

int ASprite::GetFrameModuleWidth(int frame, int fmodule)
{
    int index = _fmodules_id[_frames_fm_start[frame] + fmodule]&0xFF;
    return _modules_w[index]&MODULE_WH_MASK;
}

int ASprite::GetFrameModuleHeight(int frame, int fmodule)
{
    int index = _fmodules_id[_frames_fm_start[frame] + fmodule]&0xFF;
    return _modules_h[index]&MODULE_WH_MASK;
}
//opa --- #endif

////////////////////////////////////////////////////////////////////////////////////////////////////

int ASprite::GetAnimFrame(int anim, int aframe)
{
    return _aframes_frame[_anims_af_start[anim] + aframe]&0xFF;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// get index of aframe
int ASprite::GetIdxAFrame(int anim, int aframe )
{    
    int off = _anims_af_start[anim] + aframe;
    int frame = _aframes_frame[off]&0xFF;
    
#ifdef USE_INDEX_EX_AFRAMES
    frame |= ((_aframes_flags[off]&FLAG_INDEX_EX_MASK)<<INDEX_EX_SHIFT);
#endif //USE_INDEX_EX_AFRAMESF
    
    return frame;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// get index of FModule
int ASprite::GetIdxFModule( int frame, int fmodule )
{
    int off = _frames_fm_start[frame] + fmodule;
    int fm_flags = _fmodules_flags[off]&0xFF;
    int index = _fmodules_id[off]&0xFF;
    
    index |= ((fm_flags&FLAG_INDEX_EX_MASK)<<INDEX_EX_SHIFT);
    
    return index;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int ASprite::GetFModuleFlags( int frame, int fmodule )
{
    int off = _frames_fm_start[frame] + fmodule;
    int fm_flags = _fmodules_flags[off]&0xFF;
    
    return fm_flags;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// get animation's time
int ASprite::GetAnimTime( int anim )
{
    int time = 0;
    for( int  i = 0; i < GetAFrames(anim); i++ )
    {
        time += GetAFrameTime( anim, i );
    }
    return time;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// 获得aframe的y 向偏移位置
int ASprite::GetAFrameOffsetY( int anim, int aframe, int flags )
{
    
    int off = _anims_af_start[anim] + aframe;
    int frame = _aframes_frame[off]&0xFF;
    
#ifdef USE_INDEX_EX_AFRAMES
    frame |= ((_aframes_flags[off]&FLAG_INDEX_EX_MASK)<<INDEX_EX_SHIFT);
#endif //USE_INDEX_EX_AFRAMES
    
#ifdef USE_TRANSF_FLIP
    return (flags & FLAG_FLIP_Y) != 0 ? -_aframes_oy[off] : _aframes_oy[off];
#else //USE_TRANSF_FLIP
    return _aframes_oy[off];
#endif //USE_TRANSF_FLIP
    
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// 获得aframe的x 向偏移位置
int ASprite::GetAFrameOffsetX( int anim, int aframe, int flags )
{
    
    int off = _anims_af_start[anim] + aframe;
    int frame = _aframes_frame[off]&0xFF;
    
#ifdef USE_INDEX_EX_AFRAMES
    frame |= ((_aframes_flags[off]&FLAG_INDEX_EX_MASK)<<INDEX_EX_SHIFT);
#endif //USE_INDEX_EX_AFRAMES
    
#ifdef USE_TRANSF_FLIP
    return (flags & FLAG_FLIP_X) != 0 ? - _aframes_ox[off] : _aframes_ox[off];
#else //USE_TRANSF_FLIP
    return _aframes_ox[off];
#endif //USE_TRANSF_FLIP
    
}
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

// ASprite_Paint.hxx
////////////////////////////////////////////////////////////////////////////////////////////////////

void ASprite::clearParent()
{
//	return;
}

//void ASprite::SetModulesFilterFlags(int flags)
//{
//    _modulesFilterFlags = flags;
//}

////////////////////////////////////////////////////////////////////////////////////////////////////
// 描绘AFrame
// flags ： aframe 的标记
// poxX : X 方向位置
// poxY : Y 方向位置
void ASprite::PaintAFrame(CCSprite *parent, CCSpriteBatchNode *batchnode, int anim, int aframe, int posX, int posY, int flags)
{    
    if(parent)    
        parent->removeAllChildrenWithCleanup(true);
    
    if(anim >= m_nAnims || anim < 0 || aframe < 0 || aframe >= m_nAFrames)
    {
        return;
    }
    
    int off = _anims_af_start[anim] + aframe; // 获得当前 aframe的信息
    int frame = _aframes_frame[off]&0xFF;     // 每个动画包含的frame 不能大于255个
    
#ifdef USE_INDEX_EX_AFRAMES
    frame |= ((_aframes_flags[off]&FLAG_INDEX_EX_MASK)<<INDEX_EX_SHIFT);
#endif //USE_INDEX_EX_AFRAMES
    
	//	if ((flags & FLAG_OFFSET_AF) != 0)
    // 计算aframe的偏移位置
    {
#ifdef USE_TRANSF_ROT
        if ((flags & FLAG_ROT_90) != 0)
        {
#ifdef USE_TRANSF_FLIP
            if ((flags & FLAG_FLIP_X) != 0) 
                posX -= _aframes_ox[off];
            else					
                posX += _aframes_ox[off];
            if ((flags & FLAG_FLIP_Y) != 0) 
                posY -= _aframes_oy[off];
            else							
                posY += _aframes_oy[off];
#else //USE_TRANSF_FLIP
            posX += _aframes_ox[off];
            posY += _aframes_oy[off];
#endif //USE_TRANSF_FLIP
        }
        else
#endif //USE_TRANSF_ROT
        {
#ifdef USE_TRANSF_FLIP
            if ((flags & FLAG_FLIP_X) != 0)	
                posX -= _aframes_ox[off];
            else	
                posX += _aframes_ox[off];
            if ((flags & FLAG_FLIP_Y) != 0)
                posY -= -_aframes_oy[off];
            else	
                posY += -_aframes_oy[off];
#else //USE_TRANSF_FLIP
            posX += _aframes_ox[off];
            posY += _aframes_oy[off];
#endif //USE_TRANSF_FLIP
        }
		//	if ((flags & FLAG_FLIP_X) != 0)	posX -= _frames_w[frame]&0xFF;
		//	if ((flags & FLAG_FLIP_Y) != 0)	posY -= _frames_h[frame]&0xFF;
    }
    
    //描绘具体的frame
    PaintFrame(parent, batchnode, frame, posX, posY, flags ^ (_aframes_flags[off]&0x0F));    
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// 描绘frame angle默认为0
void ASprite::PaintFrame(CCSprite *parent, CCSpriteBatchNode *batchnode, int frame, int posX, int posY, int flags,int angle)
{
    if( frame >= m_nFrames || frame < 0)
    {
        return;
    }
    int nFModules = 0;
    if( _frames_nfm != null )
        nFModules = _frames_nfm[frame]&0xFF;
    
    //描绘frame包含的所有module
    for (int fmodule = 0; fmodule < nFModules; fmodule++)
        PaintFModule(parent, batchnode, frame, fmodule, posX, posY, flags);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//描绘FModule
void ASprite::PaintFModule(CCSprite *parent, CCSpriteBatchNode *batchnode, int frame, int fmodule, int posX, int posY, int flags,int angle)
{
    int off = _frames_fm_start[frame] + fmodule;
    int fm_flags = _fmodules_flags[off]&0xFF;
    int index = _fmodules_id[off]&0xFF;
    
    index |= ((fm_flags&FLAG_INDEX_EX_MASK)<<INDEX_EX_SHIFT);    
    
    if ((flags & FLAG_FLIP_X) != 0) 
        posX -= _fmodules_ox[off] + (_modules_w[index]&MODULE_WH_MASK);
    else							
        posX += _fmodules_ox[off];
    if ((flags & FLAG_FLIP_Y) != 0)
        posY -= -_fmodules_oy[off] - (_modules_h[index]&MODULE_WH_MASK);
    else							
        posY += -_fmodules_oy[off];
    
    flags = flags ^ (fm_flags&0x0F); 
    
    //描绘具体的module
    PaintModule(parent, batchnode, index, posX, posY, flags);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////


ASprite::ASprite()
: _temp_buff(NULL)
, _modules_w(NULL)
, _modules_h(NULL)
, _modules_x(NULL)
, _modules_y(NULL)
, _modules_image(NULL)
, _module_textures_w(NULL)
, _module_textures_h(NULL)    
, _module_types(NULL)
, _module_colors(NULL)
, _frames_nfm(NULL)
, _frames_fm_start(NULL)
, _frames_rc(NULL)
, _frames_col(NULL)
, _fmodules(NULL)
, _fmodules_id(NULL)
, _fmodules_ox(NULL)
, _fmodules_oy(NULL)
, _fmodules_pal(NULL)
, _fmodules_flags(NULL)
, _anims_naf(NULL)
, _anims_af_start(NULL)
, _aframes(NULL)
, _aframes_frame(NULL)
, _aframes_time(NULL)
, _aframes_ox(NULL)
, _aframes_oy(NULL)
, _aframes_flags(NULL)
, _modules_data(NULL)
, _modules_data_off(NULL)  
{  

}

ASprite::~ASprite()
{    
	//CCLOG("release ASprite");
	FreeSprite();
}

void ASprite::FreeSprite()
{       
	SAFE_DEL_ARRAY(_modules_w);
	SAFE_DEL_ARRAY(_modules_h);    
	SAFE_DEL_ARRAY(_modules_x);
	SAFE_DEL_ARRAY(_modules_y);  
	SAFE_DEL_ARRAY(_modules_image);    
	SAFE_DEL_ARRAY(_module_textures_w);
	SAFE_DEL_ARRAY(_module_textures_h);    
    SAFE_DEL_ARRAY(_module_types);
	SAFE_DEL_ARRAY(_module_colors);    
    SAFE_DEL_ARRAY(_frames_nfm);
    SAFE_DEL_ARRAY(_frames_fm_start);
    SAFE_DEL_ARRAY(_frames_rc);
    SAFE_DEL_ARRAY(_frames_col);
    SAFE_DEL_ARRAY(_fmodules);
    SAFE_DEL_ARRAY(_fmodules_id);
    SAFE_DEL_ARRAY(_fmodules_ox);
    SAFE_DEL_ARRAY(_fmodules_oy);
    SAFE_DEL_ARRAY(_fmodules_pal);
    SAFE_DEL_ARRAY(_fmodules_flags);
    SAFE_DEL_ARRAY(_anims_naf);
    SAFE_DEL_ARRAY(_anims_af_start);
    SAFE_DEL_ARRAY(_aframes);
    SAFE_DEL_ARRAY(_aframes_frame);
    SAFE_DEL_ARRAY(_aframes_time);
    SAFE_DEL_ARRAY(_aframes_ox);
    SAFE_DEL_ARRAY(_aframes_oy);
    SAFE_DEL_ARRAY(_aframes_flags);
    SAFE_DEL_ARRAY(_modules_data);
    SAFE_DEL_ARRAY(_modules_data_off); 
}

void ASprite::setParent(CCSprite *sprite)
{
    //m_parent = sprite;
}


// ASprite_PaintModule.hxx
////////////////////////////////////////////////////////////////////////////////////////////////////
//描绘具体的module
void ASprite::PaintModule(CCSprite* parent, CCSpriteBatchNode *batchnode, int module, int posX, int posY, int flags,int angle)
{    
    if(module >= _nModules || module < 0)
    {
        return;
    }
    
    float x = posX;
    float y = posY;
//    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    float sizeX = (_modules_w[module] & MODULE_WH_MASK);
	float sizeY = (_modules_h[module] & MODULE_WH_MASK);
	if (sizeX <= 0 || sizeY <= 0) return;
    
	float texX = (_modules_x[module] & MODULE_WH_MASK);
	float texY = (_modules_y[module] & MODULE_WH_MASK);
    
    
    float currentDeviceScale = CCDirector::sharedDirector()->getContentScaleFactor();
    
    x /= currentDeviceScale;
    y /= currentDeviceScale;
    
    CCRect rect = CCRectMake(texX / currentDeviceScale * ANIM_SCALE,
                             texY / currentDeviceScale * ANIM_SCALE,
                             sizeX / currentDeviceScale * ANIM_SCALE,
                             sizeY/ currentDeviceScale * ANIM_SCALE);
    
    sizeX /= currentDeviceScale;
    sizeY /= currentDeviceScale;
    
    //CCSprite* sprite = CCSprite::spriteWithBatchNode(batchnode, rect);
	CCSprite* sprite = CCSprite::createWithTexture(batchnode->getTexture(), rect);
	sprite->setScale(1 / ANIM_SCALE);
    
    sprite->setContentSize(rect.size);
    sprite->setAnchorPoint(ccp(0, 1));
    sprite->setOpacity(parent->getOpacity());
    
    
    float lastX = x;
    float lastY = y;
    
    flags = flags & 0x07;    
    if(flags == 1) 
    {
        sprite->setPosition(ccp(lastX, lastY));
        sprite->setFlipX(true);
    }
    else if(flags == 2)
    {
        sprite->setPosition(ccp(lastX, lastY));
        sprite->setFlipY(true);
    }
    else if(flags == 3)
    {
        sprite->setPosition(ccp(lastX, lastY));
        sprite->setFlipX(true);
        sprite->setFlipY(true);
    }
    else if(flags == 4)
    {
        sprite->setRotation(90);
        sprite->setPosition(ccp(lastX + sizeY, lastY)); //因为 roratation是绕 （0， 1） 旋转 所以要 向右移动 h 
    }
    else if(flags == 5)
    {
        sprite->setRotation(90);
        sprite->setFlipX(true);
        sprite->setPosition(ccp(lastX + sizeY, lastY));
    }
    else if(flags == 6)
    {
        sprite->setRotation(90);     
        sprite->setPosition(ccp(lastX + sizeY, lastY));
        sprite->setFlipY(true);        
        
    }
    else if(flags == 7)
    {
        sprite->setFlipY(true);
        sprite->setFlipX(true);
        sprite->setRotation(90);
        sprite->setPosition(ccp(lastX + sizeY, lastY));
    }
    else
    {
        sprite->setPosition(ccp(lastX, lastY));
    }
    
    parent->addChild(sprite);    
}

// 获得最基础的 module 并add进父节点
// posX , posY 为父节点位置
// offX , offY 为module的位置
void ASprite::getModule(CCSprite* parent, int module, int posX, int posY, float offX, float offY, int flags)
{
    if(module >= _nModules || module < 0)
    {
        return;
    }
    
    int  index = module;    
    
    int sizeX = _modules_w[index] & MODULE_WH_MASK;
	int sizeY = _modules_h[index] & MODULE_WH_MASK;
	if (sizeX <= 0 || sizeY <= 0) 
        return;
    
	int texX = _modules_x[index] & MODULE_WH_MASK;
	int texY = _modules_y[index] & MODULE_WH_MASK;
    
    float deviceScale = CC_CONTENT_SCALE_FACTOR();
    
    float x = texX / deviceScale;
    float y = texY / deviceScale;
    float w = sizeX / deviceScale;
    float h = sizeY / deviceScale;
    
    float px = posX / deviceScale;
    float py = posY / deviceScale;
    
    CCRect recttemp = CCRectMake(x, y, w, h);  

	// create image sprite
	int imageIndex = _modules_image[index];    
    CCSprite* sprite = CCSprite::create(getImageName(imageIndex).c_str(), recttemp);
    //sprite->initWithTexture(sprite->getTexture(),sprite->getTextureRect());
    
    //    if((FLAG_FLIP_X & flags) != 0)
    //    {
    //        sprite->setFlipX(true);
    //    }
    //    if((FLAG_FLIP_Y & flags) != 0)
    //    {
    //        sprite->setFlipY(true);
    //    }
    //    if ((flags & FLAG_ROT_90) != 0)
    //    {
    //        sprite->setFlipY(true);
    //    }
    sprite->setAnchorPoint(ccp(0, 1)); 
    
//    CCSize size = parent->getContentSize();
    //sprite->setPosition(ccp(px - offX , fabs(py) - offY));
    
    float lastX = px - offX;
    float lastY = fabs(py) - offY;
    
    flags = flags & 0x07;    
    if(flags == 1) 
    {
        sprite->setPosition(ccp(lastX, lastY));
        sprite->setFlipX(true);
    }
    else if(flags == 2)
    {
        sprite->setPosition(ccp(lastX, lastY));
        sprite->setFlipY(true);
    }
    else if(flags == 3)
    {
        sprite->setPosition(ccp(lastX, lastY));
        sprite->setFlipX(true);
        sprite->setFlipY(true);
    }
    else if(flags == 4)
    {
        sprite->setRotation(90);
        sprite->setPosition(ccp(lastX + h, lastY)); //因为 roratation是绕 （0， 1） 旋转 所以要 向右移动 h 
    }
    else if(flags == 5)
    {
        sprite->setRotation(90);
        sprite->setFlipX(true);
        sprite->setPosition(ccp(lastX + h, lastY));
    }
    else if(flags == 6)
    {
        sprite->setRotation(90);     
        sprite->setPosition(ccp(lastX + h, lastY));
        sprite->setFlipY(true);        
        
    }
    else if(flags == 7)
    {
        sprite->setFlipY(true);
        sprite->setFlipX(true);
        sprite->setRotation(90);
        sprite->setPosition(ccp(lastX + h, lastY));
    }
    else
    {
        sprite->setPosition(ccp(lastX, lastY));
    }
    
    parent->addChild(sprite);
    //return sprite;
}

// 获得 frame内的每一个 module
// posX , posY 为父亲节位置
// parent 父亲节点
void ASprite::getFModule(CCSprite* parent, int frame, int fmodule, float posX, float posY, int flags)
{
    int off = _frames_fm_start[frame] + fmodule;
    int fm_flags = _fmodules_flags[off]&0xFF;
    int index = _fmodules_id[off]&0xFF;
    
    int offX = 0;
    int offY = 0;
    
    index |= ((fm_flags&FLAG_INDEX_EX_MASK)<<INDEX_EX_SHIFT);    
    
    if ((flags & FLAG_FLIP_X) != 0) 
        offX -= _fmodules_ox[off] + (_modules_w[index]&MODULE_WH_MASK);
    else							
        offX += _fmodules_ox[off];
    if ((flags & FLAG_FLIP_Y) != 0)
        offY -= -_fmodules_oy[off] - (_modules_h[index]&MODULE_WH_MASK);
    else							
        offY += -_fmodules_oy[off];
    
    flags = flags ^ (fm_flags&0x0F);     
    
    getModule(parent, index, offX, offY, posX, posY, flags);
}

//获得frame， anchopoint为 （0， 0）
// flags 为 frame的变化标记， 例如 flipx flipy ，以及旋转等。
CCSprite* ASprite::getSpriteFromFrame(int frame, int flags, CCPoint &point)
{    
    if(frame >= m_nFrames || frame < 0)
    {
        return null;
    }
    
    int nFModules = 0;
    if( _frames_nfm != null )
        nFModules = _frames_nfm[frame]&0xFF;
    
    CCSprite* parent = CCSprite::create();
	
	CCRect frameRect = this->getframeRect(frame);
    parent->initWithTexture(NULL,CCRectZero);
	point = frameRect.origin;
    
    parent->setContentSize(frameRect.size);
    
    for (int fmodule = 0; fmodule < nFModules; fmodule++)
        getFModule(parent, frame, fmodule, point.x, point.y, flags);
    
    parent->setAnchorPoint(ccp(0, 0));
	parent->setPosition(CCPointZero);
    
    return parent;    
}


// 获得动画中指定frame的框体
CCRect ASprite::getCurAframeRect(int anim, int aframe)
{
    int off = _anims_af_start[anim] + aframe;
    int frame = _aframes_frame[off]&0xFF;
    
#ifdef USE_INDEX_EX_AFRAMES
    frame |= ((_aframes_flags[off]&FLAG_INDEX_EX_MASK)<<INDEX_EX_SHIFT);
#endif  
    
    float w = GetFrameWidth(frame);
    float h = GetFrameHeight(frame);
    
    float x = GetFrameStartX(frame) + _aframes_ox[off];
    float y = -GetFrameStartY(frame) + (-_aframes_oy[off]) - h; // anchpoint (0,0)
    
    
    float deviceScale = CC_CONTENT_SCALE_FACTOR();
    
    return CCRectMake(x / deviceScale,
                      y / deviceScale,
                      w / deviceScale,
                      h / deviceScale);
    
    
    // in orora 
    
    //  ^ -y
    //  | -----------
    //  | |         |
    //  | |         |
    //  | -----------
    //  |
    //--|-----------------------> x
    //  |
}

// 返回一个frame的 rect
CCRect ASprite::getframeRect(int frame)
{ 
    float x = GetFrameStartX(frame);
    float y = -GetFrameStartY(frame);
    float w = GetFrameWidth(frame);
    float h = GetFrameHeight(frame);

	float deviceScale = CC_CONTENT_SCALE_FACTOR();
    
    return CCRectMake(x / deviceScale,
                      (y - h) / deviceScale,
                      w / deviceScale,
                      h / deviceScale);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

//获得module， 
//ptx， pty 为父节点的中心位置
void ASprite::getModule_new(CCSprite* parent, int module, int posX, int posY, float ptx, float pty, int flags)
{
    if(module >= _nModules || module < 0)
    {
        return;
    }
    
    int index = module;
    
    int sizeX = _modules_w[index] & MODULE_WH_MASK;
	int sizeY = _modules_h[index] & MODULE_WH_MASK;
	if (sizeX <= 0 || sizeY <= 0) 
        return;
    
	int texX = _modules_x[index] & MODULE_WH_MASK;
	int texY = _modules_y[index] & MODULE_WH_MASK;
    
    float deviceScale = CC_CONTENT_SCALE_FACTOR();
    
    float x = texX / deviceScale;
    float y = texY / deviceScale;
    float w = sizeX / deviceScale;
    float h = sizeY / deviceScale;
    
    float px = posX / deviceScale;
    float py = posY / deviceScale;
    
    CCRect recttemp = CCRectMake(x, y, w, h);

	int image = _modules_image[index];
    CCSprite* sprite = CCSprite::create(getImageName(image).c_str(), recttemp);
	CCAssert(sprite != NULL,"create CCSprite failed");
	if(sprite == NULL)
	{
		// 防止 crash
		sprite = CCSprite::create();
	}

    sprite->setAnchorPoint(ccp(0.5, 0.5));
    
    sprite->setContentSize(CCSizeMake(w,h));
    CCSize size = parent->getContentSize();

    float lastX = px - ptx + w + size.width/2.0f - w/2.0f;
    float lastY = py - pty + size.height/2.0f - h/2.0f;
    
    //因为 roratation是绕 （0.5， 0.5） 旋转 所以要 向右移动 offset, 在 aurora中是以（0，1）为旋转点的。
    float offsetRotation =  w/2 - h/2;
    
    flags = flags & 0x07;    
    if(flags == 1) 
    {
        sprite->setPosition(ccp(lastX, lastY));
        sprite->setFlipX(true);
    }
    else if(flags == 2)
    {
        sprite->setPosition(ccp(lastX, lastY));
        sprite->setFlipY(true);
    }
    else if(flags == 3)
    {
        sprite->setPosition(ccp(lastX, lastY));
        sprite->setFlipX(true);
        sprite->setFlipY(true);
    }
    else if(flags == 4)
    {
        sprite->setRotation(90);
        sprite->setPosition(ccp(lastX - offsetRotation, lastY - offsetRotation));
    }
    else if(flags == 5)
    {
        sprite->setRotation(90);
        sprite->setFlipX(true);
        sprite->setPosition(ccp(lastX - offsetRotation, lastY - offsetRotation));
    }
    else if(flags == 6)
    {
        sprite->setRotation(90);     
        sprite->setPosition(ccp(lastX - offsetRotation, lastY - offsetRotation));
        sprite->setFlipY(true);        
        
    }
    else if(flags == 7)
    {
        sprite->setFlipY(true);
        sprite->setFlipX(true);
        sprite->setRotation(90);
        sprite->setPosition(ccp(lastX - offsetRotation, lastY - offsetRotation));
    }
    else
    {
        sprite->setPosition(ccp(lastX, lastY));
    }
    parent->addChild(sprite);
    //return sprite;
}

//获得 fmodule  是 getSpriteFromFrame_Middle 的 子方法。
//ptx， pty 为父节点的中心位置
void ASprite::getFModule_new(CCSprite* parent, int frame, int fmodule, float ptx, float pty, int flags)
{
    int off = _frames_fm_start[frame] + fmodule;
    int fm_flags = _fmodules_flags[off]&0xFF;
    int index = _fmodules_id[off]&0xFF;
    float posX = 0;
    float posY = 0;
    
    index |= ((fm_flags&FLAG_INDEX_EX_MASK)<<INDEX_EX_SHIFT);    
    
    if ((flags & FLAG_FLIP_X) != 0) 
        posX -= _fmodules_ox[off] + (_modules_w[index]&MODULE_WH_MASK);
    else							
        posX += _fmodules_ox[off];
    if ((flags & FLAG_FLIP_Y) != 0)
        posY -= -_fmodules_oy[off] + (_modules_h[index]&MODULE_WH_MASK);
    else							
        posY += -_fmodules_oy[off];
    
    flags = flags ^ (fm_flags&0x0F);     
    
    getModule_new(parent, index, posX, posY, ptx, pty, flags);
}


CCSprite* ASprite::copySprite(CCSprite* sprite)
{
    CCTexture2D *tex = sprite->getTexture();
    bool isRotate = sprite->isTextureRectRotated();
    CCRect rectInPixels = CC_RECT_POINTS_TO_PIXELS(sprite->getTextureRect());
    CCSprite *ps = CCSprite::createWithTexture(tex);
    ps->setTextureRect(rectInPixels, isRotate, rectInPixels.size);
    
    bool flx = sprite->isFlipX();
    bool fly = sprite->isFlipY();
    
    ps->setFlipX(flx);
    ps->setFlipY(fly);
    return ps;
}


//if u call this function, ccsprite must have the same textures and ccsprite only have one child
CCSpriteBatchNode* ASprite::getSpriteBatchNodeFromFrame_Middle(int frame, int flags, CCPoint &point)
{
    CCSprite *parent = getSpriteFromFrame_Middle(frame, flags, point);
    
    CCSpriteBatchNode *pbatch = NULL;
    CCObject *pObject;
    CCArray *pchild = parent->getChildren();
    CCARRAY_FOREACH(pchild, pObject)
    {
        CCSprite* ss = (CCSprite*)pObject;
        CCSprite *ps = copySprite(ss);
        ps->setPosition(ss->getPosition());
        
        if(!pbatch)
        {
            pbatch = CCSpriteBatchNode::createWithTexture(ps->getTexture(), 10);
        }
        pbatch->addChild(ps);
    }
    return pbatch;
}



// 获得一个frame， 
// frame的 AnchorPoint 为（0。5， 0。5）
CCSprite* ASprite::getSpriteFromFrame_Middle(int frame, int flags, CCPoint &point)
{    
    if(frame >= m_nFrames || frame < 0)
    {
        return null;
    }

    int nFModules = 0;
    if( _frames_nfm != null )
        nFModules = _frames_nfm[frame]&0xFF;
    
    CCSprite* parent = CCSprite::create();
	CCRect frameRect = this->getframeRect(frame);
	parent->initWithTexture(NULL,CCRectZero);
	point = this->GetFramePointMiddle(frame);
    
    parent->setContentSize(frameRect.size);
    parent->setAnchorPoint(ccp(0.5, 0.5));

    for (int fmodule = 0; fmodule < nFModules; fmodule++)
        getFModule_new(parent, frame, fmodule, point.x, point.y, flags);
    
    parent->setPosition(ccp(0,0));
    return parent;    
}

//获得一个frame的中心点
CCPoint ASprite::GetFramePointMiddle(int frame)
{
    if(frame >= m_nFrames || frame < 0)
    {
        return CCPointMake(0, 0);
    }
    float x = GetFrameStartX(frame);
    float y = -GetFrameStartY(frame);
    float w = GetFrameWidth(frame);
	float h = GetFrameHeight(frame);

	float deviceScale = CC_CONTENT_SCALE_FACTOR();

	x /= deviceScale;
	y /= deviceScale;
	w /= deviceScale;
	h /= deviceScale;

    CCPoint point;
    
    point.x = x + w * 0.5f;
    point.y = y - h * 0.5f;
    
    return point;
}

