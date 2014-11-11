--Note: Game First Config File 
--Note£ºusage: use for project config ; download resource flag ; download server list flag ; 

--Note: NetWork Flag "InnerNet" or "OutNet" or "AllOutNet"
--Note: WARNNING -> This Code Must be First Line in this file
G_GameNetWorkFlag = "CKNet"


--for 360 network
--G_GameChannelID = "1"
--for self ios network
G_GameChannelID = "60"
--for android network
--G_GameChannelID = "94"

G_GamePackageName = "com.taomee.adventure"
G_GameMainVersion = "6"

G_GameNetWorkGetServerConfigUrl =
"http://ahero.login1.61.com/version/router.php"
--"http://10.1.1.155//version/router.php";
G_GameDownLoadShopItemInfoConfigUrl =
"http://wlpay.61.com/iap/index.php?m=ahero&a=getItemJson&category=1";
G_GameLaunchStatConfigUrl ="http://ck_stat.61.com/detect/ck_stat.php";

