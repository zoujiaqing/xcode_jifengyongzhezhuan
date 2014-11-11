#include "TutorialBase.h"

TutorialBase::TutorialBase()
{
	m_eTutorialType = kTutorialUnKnow;
	m_nTutorialID = 0;
}

TutorialBase::TutorialBase(unsigned int id)
{
	m_nTutorialID = id;
}

TutorialBase::~TutorialBase()
{

}