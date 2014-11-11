require("Script/Fairy/FairyConfigTable")
require("Script/Equipment/TipColor")

FairyContactListItem = {
	MINSCALE = 0.45,
	baseNode = nil,
	labelNode = nil,
	labelScale = 1,
}

function FairyContactListItem:create(parentList,fairyInfo)
	local item = setmetatable({},self)
	self.__index = self
	item:init(parentList,fairyInfo)
	return item
end

-- 重置参数
function FairyContactListItem:resetValue()
	self.baseNode = nil
	self.labelNode = nil
	self.labelScale = 1
end

function FairyContactListItem:createLabelNode(fairyInfo,fairyBasicInfo)
	local fetters = getFettersByFairyId(fairyInfo.fairyId)
	local num = table.getn(fetters)
	local contentRect,contentlabelPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_CONTACT_TEXT_ALL")
	local rectSize = contentRect:getContentSize()
	if num == 0 then
		self.labelNode = CCSprite:create()
		local str = LanguageLocalization:GetLocalization("M_FAIRY_NOLINK")
		local label = TXGUI.UILabelTTF:create(str,KJLinXin,20,CCSizeMake(rectSize.width,0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop)
		label:setPosition(ccp(rectSize.width * 0.5,0 - label:getContentSize().height * 0.5))
		self.labelNode:addChild(label)
		self.labelNode:setContentSize(CCSizeMake(rectSize.width,label:getContentSize().height))
	else
		self.labelNode = CCSprite:create()
		local heightSum = 0
		local lineOffset = 10 -- 行距
		for k,v in pairs(fetters) do
			local fetterName = getFetterNameById(v)
			local fetterDes = getFetterDescribeById(v)
			local str = fetterName .. ":" .. fetterDes
			local contentLabel = TXGUI.UILabelTTF:create(str,KJLinXin,20,CCSizeMake(rectSize.width,0),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop)
			if fairyInfo.fairyFetters ~= nil and fairyInfo.fairyFetters[v] ==  1 then
				contentLabel:setColor(TipColorGreen)
			else
				contentLabel:setColor(ccc3(255,255,255))
			end
			contentLabel:setPosition(ccp(rectSize.width * 0.5,0 - heightSum - contentLabel:getContentSize().height * 0.5))
			self.labelNode:addChild(contentLabel)
			heightSum = heightSum + contentLabel:getContentSize().height + lineOffset
		end
		self.labelNode:setContentSize(CCSizeMake(rectSize.width,heightSum))
	end
	self.labelScale = self.labelNode:getContentSize().height / rectSize.height
	if self.labelScale < self.MINSCALE then
		self.labelScale = self.MINSCALE
	end
	CCLuaLog("labelScale: " .. self.labelScale)
end

function FairyContactListItem:init(parentList,fairyInfo)
	--CCLuaLog("FairyContactListItem:init(parentList,fairyInfo)")
	if parentList ~= nil and fairyInfo ~= nil then
		local fairyBasicInfo = getFairyBasicInfo(fairyInfo.fairyId)
		if fairyBasicInfo ~= nil then
			local baseNode = CCSprite:create()
			local bgFrame,bgPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_CONTACT_FRAME")
			--local clickedFrame = AspriteManager:getInstance():getOneFrame("UI/ui.bin","map_ui_FRAME_FAIRY_BAG_FRAME_FAIRY1_CLICKED")
			--local button = parentList:AddNewItem(bgFrame,clickedFrame,"","",ccc3(255,255,255),-130)
			self:createLabelNode(fairyInfo,fairyBasicInfo)
			local labelSize = self.labelNode:getContentSize()

			local itemSize = CCSizeMake(bgFrame:getContentSize().width,bgFrame:getContentSize().height * self.labelScale)
			local control = parentList:AddBaseItem(itemSize,baseNode)
			control:setTag(fairyInfo.pos)

			local bgPic,bgPicPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_CONTACT_BACKGROUND3")
			baseNode:addChild(bgPic,0)
			bgPic:setScaleY(self.labelScale)
			bgPic:setPosition(ccpSub(bgPicPt,bgPt))
			
			local bg2Pic,bg2PicPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_CONTACT_BACKGROUND4")
			baseNode:addChild(bg2Pic,0)
			bg2Pic:setScaleY(self.labelScale)
			bg2Pic:setPosition(ccpSub(bg2PicPt,bgPt))
			

			local namePic,namePicPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_CONTACT_NAME1")
			baseNode:addChild(namePic,0)
			namePic:setPosition(ccp(namePicPt.x - bgPt.x,namePicPt.y - bgPt.y + bgPic:getContentSize().height * (self.labelScale - 1) * 0.5))


			local headBgPic,headBgPicPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_CONTACT_ICON1_FRAME")
			baseNode:addChild(headBgPic,0)
			headBgPic:setPosition(ccpSub(headBgPicPt,bgPt))

			local headFrameRect,headFramePt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_CONTACT_ICON1")
			local headIcon = fairyBasicInfo:getFairyIcon()
			if headIcon ~= nil then
				headIcon:setPosition(ccpSub(headFramePt,bgPt))
				baseNode:addChild(headIcon,10)
			end
			local nameRect,namelabelPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_CONTACT_NAME1_TEXT")
			if nameRect ~= nil then
				local nameLabel = TXGUI.UILabelTTF:create(fairyBasicInfo.name,KJLinXin,20)
				nameLabel:setPosition(ccp(namelabelPt.x - bgPt.x,namelabelPt.y - bgPt.y + bgPic:getContentSize().height * (self.labelScale - 1) * 0.5))
				nameLabel:setColor(ItemManager:Get():getFairyLabelColorByQuality(fairyBasicInfo.quality))
				baseNode:addChild(nameLabel,12)
			end

			local contentRect,contentlabelPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_CONTACT_TEXT_ALL")
			--self.labelNode:setPosition(ccp(contentlabelPt.x - bgPt.x,bgPic:getPositionY()+bgPic:getContentSize().height * self.labelScale * 0.5))
			--self.labelNode:setPosition(ccp(contentlabelPt.x - bgPt.x,bgPic:getPositionY() + contentlabelPt.y - bgPt.y + bgPic:getContentSize().height * (self.labelScale - 1)))
			--self.labelNode:setPosition(ccpSub(contentlabelPt,bgPt))
			self.labelNode:setPosition(ccp(contentlabelPt.x - bgPt.x,contentlabelPt.y - bgPt.y + labelSize.height))

			baseNode:addChild(self.labelNode,12)
			--self:initContactInfo(fairyInfo,fairyBasicInfo,baseNode,bgPt)
			--local contentRect,contentlabelPt = AspriteManager:getInstance():getOneFrameAndPosition("UI/ui.bin","map_ui_FRAME_FAIRY_CONTACT_TEXT_ALL")
			--if contentRect ~= nil then
			--	local contentLabel = TXGUI.UILabelTTF:create("",KJLinXin,20)
			--	contentLabel:setPosition(ccpSub(contentlabelPt,bgPt))
			--	contentLabel:setColor(ccc3(255,255,255))
			--	baseNode:addChild(contentLabel,12)
			--end
		end

	end
end