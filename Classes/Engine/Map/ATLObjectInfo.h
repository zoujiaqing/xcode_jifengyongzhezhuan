#ifndef ATLOBJECTINFO_2013_1_10_H
#define ATLOBJECTINFO_2013_1_10_H


#include <vector>
#include "../../../cocos2dx/cocos2dx/textures/CCTexture2D.h"
#include "../../../libs/ASprite/ASprite.h"

class CATLObjectInfo
{
public:
	struct VECTOR2D
	{
		VECTOR2D()
			:x(0)
			,y(0)
		{
		}

		int x;
		int y;
	};

	struct CAMERA
	{
		CAMERA()
			:flags(0)
		{
		}

		int flags;
		VECTOR2D pos;
		VECTOR2D size;

		void Read(char*&);
	};

	struct LAYER
	{
		enum TYPE
		{

			E_TILED,
			E_DUMMY,
			E_OBJECT,

		};

		struct BASE
		{
			BASE()
				:flags(0)
				,id(0)
			{
			}

			virtual ~BASE()
			{
				for(size_t i = 0 ;i < childrens.size(); i++)
				{
					delete childrens[i];
				}
			}

			int flags;
			int id;
			VECTOR2D pos;

			std::vector<LAYER::BASE*> childrens;

			virtual void Read(char*&);
		};

		struct TILED : BASE
		{
			std::string templateFile;	// 实际上就是sprite文件
			std::string mapFile;		// 实际上就是aTLMap文件
			virtual void Read(char*&);
		};
		
		struct DUMMY : BASE
		{

		};

		struct OBJECT : BASE
		{
			enum RENDER_FLAG
			{
				E_NULL,
				E_FLIP_X,
				E_FLIP_Y,
				E_FLIP_XY,
			};

			std::string templateName;
			
			bool enable;	// 1
			bool visible;	// 2
			std::string layerName;	// 3
			std::string tagName;	// 4
			std::string spriteFile;	// 5
			RENDER_FLAG renderFlag;	// 6
			int animID;				// 7

			int zOrder;

			OBJECT()
			{
				
			}

			virtual ~OBJECT()
			{

			}

			virtual void Read(char*&);

		};
		
	};

	struct LEVEL
	{
		LEVEL()
			:flags(0)
			,id(0)
		{
		}

		virtual ~LEVEL()
		{
			for(size_t i = 0; i < layers.size(); i++)
			{
				delete layers[i];
				layers[i] = NULL;
			}
		}

		std::string name;
		int flags;
		int id;
		VECTOR2D pos;

		std::vector<LAYER::BASE*> layers;

		virtual void Read(char*&);

	};

	std::vector<LEVEL*> m_levels;

public:
	CATLObjectInfo();
	~CATLObjectInfo();

	void Load(const char* pFileName);

};

#endif