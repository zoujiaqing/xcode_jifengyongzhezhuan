#ifndef __COCOSPLAYCLIENT_H__
#define __COCOSPLAYCLIENT_H__

#include <string>

namespace cocosplay {

/**
 *  Checks whether CocosPlay is enabled
 */
bool isEnabled();

/**
 *  Checks whether CocosPlay is in demo mode
 */
bool isDemo();

/**
 *  Updates assets by filePath, if the file doesn't exist, CocosPlay will show a progress page of downloading.
 *  And this interface will be hung up until the scene package was downloaded.
 */
void updateAssets(const std::string& filePath);

/**
 *  Checks whether the file exists
 */
bool fileExists(const std::string& filePath);

/**
 *  Notifies to Cocos Play SDK that a file was loaded
 *  It will do nothing if game doesn't run on Cocos Play
 */
void notifyFileLoaded(const std::string& filePath);

/**
 *  Gets the resource root path of current game
 *  @return A writable path of current game
 */
std::string getGameRoot();

/**
 *  Purges the file searching cache.
 *
 *  @note It should be invoked after the resources were updated.
 *        For instance, it could be used when there is a small update in games.
 */
void purgeCachedEntries();

/**
 *  Notifies that the game demo was ended
 */
void notifyDemoEnded();

} // namespace cocosplay {

#endif // __COCOSPLAYCLIENT_H__
