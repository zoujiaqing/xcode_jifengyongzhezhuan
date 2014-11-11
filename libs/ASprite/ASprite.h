#ifndef ASPRITE_H
#define ASPRITE_H


//////////////////////////////////////////////////////////////////////////

#define MAX_MODULE_MAPPINGS	10

//////////////////////////////////////////////////////////////////////////

// Constants
#define OPERATION_DRAW			0
#define OPERATION_COMPUTERECT	1
#define OPERATION_RECORD		2
#define OPERATION_MARK			3


//////////////////////////////////////////////////////////////////////////


#define FLAG_FLIP_X	 0x01
#define FLAG_FLIP_Y	 0x02
#define FLAG_ROT_90	 0x04

#include <vector>
#include <string>

#include "ASprite_config.h"
#include "ASprite_defines.h"
#include "ASprite_Flags.h"
#include "cocos2d.h"

#ifndef ARRC
#define ARRC
#define ArrayCopy(a,b,c,d,e) {for(int _idx=0;_idx<e;_idx++)c[d+_idx]=a[b+_idx];}
#endif



////////////////////////////////////////////////////////////////////////////////////////////////////
using namespace cocos2d;

class ASprite
{
public:
	ASprite();
	~ASprite();

	void FreeSprite();
    
    void loadFile(const char *path, const char* pngname);
    void loadFile(const char *path);
    void Load(BBYTE* file, int offset);
	void loadRGBImage(BBYTE* file, int offset);

//	void SetModulesFilterFlags(int flags);

//	void BuildCacheImages(int pal, int m1, int m2, int pal_copy);
//	void FreeModuleImage(int nPal, int nMod);
//	void FreeCacheData();
//
//	int  GetLineSpacing();
//	void SetLineSpacing(int spacing);
//	void SetLineSpacingToDefault();
//	int GetCharactersSpacing();
//	void SetCharactersSpacing(int spacing);
//	int GetFontHeight();
//	int GetFontWidth( int charId );
//opa ---
	//void GetFrameRect(int* rc, int frame, int posX, int posY, int flags, int hx, int hy);
	//void GetAFrameRect(int* rc, int anim, int aframe, int posX, int posY, int flags, int hx, int hy);
//~opa ---
   
	int GetAFrameTime(int anim, int aframe);
	int GetAFrames(int anim);
	int GetFModules(int frame);
	int GetFrameModule( int frame, int fmodule );
	int GetModuleCount() { return _nModules; }
//opa --- #ifdef UNUSED_FUNCTIONS
//opa --- #ifdef UNUSED_FUNCTIONS
	int GetModuleX(int module);
	int GetModuleY(int module);
    int GetModuleWidth(int module);
	int GetModuleHeight(int module);
//opa --- #endif
	int GetFrameWidth(int frame);
	int GetFrameHeight(int frame);
	int GetFrameStartX(int frame);
	int GetFrameStartY(int frame);
    
    CCPoint GetFramePoint(int frame);
//opa --- #ifdef UNUSED_FUNCTIONS
	int GetFrameModuleX(int frame, int fmodule);
	int GetFrameModuleY(int frame, int fmodule);
	int GetFrameModuleWidth(int frame, int fmodule);
	int GetFrameModuleHeight(int frame, int fmodule);
//opa --- #endif
    
//opa ---
	int GetAnimFrame(int anim, int aframe);

#ifdef UNUSED_FUNCTIONS
	int CountFrameModules(int frame);
#endif

	int GetNumFrames() { return m_nFrames; }
    int GetAnimationNum() { return m_nAnims; }

	int GetIdxFModule( int frame, int fmodule );
	int GetFModuleFlags( int frame, int fmodule );
	int GetIdxAFrame( int anim, int aframe );
	int GetAnimTime( int anim );
	int GetAFrameOffsetY( int anim, int aframe, int flags );
	int GetAFrameOffsetX( int anim, int aframe, int flags );

	void clearParent(void);
	void PaintAFrame(CCSprite *parent, CCSpriteBatchNode *batchnode, int anim, int aframe, int posX, int posY, int flags);
	void PaintFrame(CCSprite *parent, CCSpriteBatchNode *batchnode, int frame, int posX, int posY, int flags,int angle=0);
	void PaintFModule(CCSprite *parent, CCSpriteBatchNode *batchnode, int frame, int fmodule, int posX, int posY, int flags,int angle=0);	
	void PaintModule(CCSprite *parent,  CCSpriteBatchNode *batchnode, int module, int posX, int posY, int flags,int angle=0);
    
//	#define GetModuleX(module)    _modules_x[module]
//	#define GetModuleY(module)    _modules_y[module]
//	#define GetFModuleOX(module)  _fmodules_ox[module]
//	#define GetFModuleOY(module)  _fmodules_oy[module]
//	#define GetAFramesOX(aFrame)  _aframes_ox[aFrame]
//	#define GetAFramesOY(aFrame)  _aframes_oy[aFrame]
    
    void initSpriteFrame();
    void setParent(CCSprite *sprite);
    CCSprite* getSpriteFromFrame(int frame, int flags, CCPoint &point);
    CCRect getCurAframeRect(int anim, int aframe);
    void getFModule(CCSprite* parent, int frame, int fmodule, float offX, float offY, int flags);
    void getModule(CCSprite* parent, int module, int posX, int posY, float offX, float offY, int flags);
    CCRect getframeRect(int frame);
    CCPoint GetFramePointMiddle(int frame);
    
    CCSpriteBatchNode* getSpriteBatchNodeFromFrame_Middle(int frame, int flags, CCPoint &point);
    CCSprite* getSpriteFromFrame_Middle(int frame, int flags, CCPoint &point);
    void getFModule_new(CCSprite* parent, int frame, int fmodule, float offX, float offY, int flags);
    void getModule_new(CCSprite* parent, int module, int posX, int posY, float offX, float offY, int flags);
    
    /// image name settings
	std::string getImageName() { return m_strImageName[0]; }
    
	std::string& getImageName(int index);

	/// set images name
	void setImageName(int index, std::string ImageName);
    
private:
    CCSprite* copySprite(CCSprite* sprite);

private:
    
    // Redimension it depending on the max size of the modules!
    BBYTE * _temp_buff;
    //	int _alpha_index;
    
	//////////////////////////////////////////////////
    
	// Modules...
	int					_nModules;			// number of modules	
	T_MODULE_WH*		_modules_w;			// width for each module
	T_MODULE_WH*		_modules_h;			// height for each module
    
	T_MODULE_WH*		_modules_x;			// width for each module
	T_MODULE_WH*		_modules_y;			// height for each module

	T_MODULE_WH*        _modules_image;      // image index of each module
    
	int*		_module_textures_w;			// width for each module
	int*		_module_textures_h;			// height for each module
    
    //	int			_texture_width;
    //	int			_texture_height;
    //
    //	int			_textures_w;
    //	int			_textures_h;
    
	BBYTE*				_module_types;		// MD_IMAGE, MD_RECT, MD_FILL_RECT
	int* 				_module_colors;			// colors list used for the modules of MD_RECT, MD_FILL_RECT types
    
    
	// Frames...
    int                 m_nFrames;
	BBYTE*				_frames_nfm;		// number of FModules (max 256 FModules/Frame)
	//	short*				_frames_nfm;		// number of FModules (max 65536 FModules/Frame)
	short*				_frames_fm_start;	// index of the first FModule
    
	short*				_frames_rc;			// frame bound rect (x y width height)
    
	BBYTE*				_frames_col;		// collision rect for each frame (x y width height)
    
    
	// FModules...
	BBYTE*				_fmodules;			// 4 bytes for each FModule
    
	BBYTE*				_fmodules_id;		// module/hyper frame index
	T_FMODULE_OFF*		_fmodules_ox;		// fmodule ox
	T_FMODULE_OFF*		_fmodules_oy;		// fmodule oy
	BBYTE*				_fmodules_pal;		// fmodule palette
	BBYTE*				_fmodules_flags;	// fmodule flags
    
    
	// Anims...
    int                 m_nAnims;
    int                 m_nAFrames;
	BBYTE*				_anims_naf;			// number of AFrames (max 256 AFrames/Anim)
    //	int					_anims_naf_length;
	//	short*				_anims_naf;			// number of AFrames (max 65536 AFrames/Anim)
	short*				_anims_af_start;	// index of the first AFrame
    
	// AFrames...
    
	BBYTE*				_aframes;			// 5 bytes for each AFrame
    
	BBYTE*				_aframes_frame;		// frame index
	BBYTE*				_aframes_time;		// aframe time
	T_AFRAME_OFF*		_aframes_ox;		// aframe ox
	T_AFRAME_OFF*		_aframes_oy;		// aframe oy
	BBYTE*				_aframes_flags;		// aframe flags
    
    
	// Palettes...
	//	short 				_pixel_format;		// always converted to 8888
	unsigned int		_bs_flags;			// sprite format
    
	int					_palettes;			// number of palettes
	int					_colors;			// number of colors
	int					_crt_pal;			// current palette
	boolean				_alpha;				// has transparency ?
	//	int					_flags;				// transparency, etc.
    
	// Graphics data (for each module)...
	short 				_data_format;
	int					_i64rle_color_mask;	// used by ENCODE_FORMAT_I64RLE
	int					_i64rle_color_bits;	// used by ENCODE_FORMAT_I64RLE
	BBYTE*				_modules_data;		// encoded image data for all modules
	int					_modules_data_length;
	int*				_modules_data_off;	// offset for the image data of each module
    
	std::string m_imagePath;

    //	int _index1;
    //	int _index2;   
    //    bool _modulesFilterFlags;
    
    //	static const int midp2_flags[];
    
	////////////////////////////////////////////////////////////////////////////////////////////////////
    std::map<int, std::string> m_strImageName;
};

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
