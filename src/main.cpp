#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
using namespace geode::prelude;

bool reload = false;

class $modify(AltMenuLayer, MenuLayer) {
	static CCScene* scene(bool p0) {
		if (!reload) return MenuLayer::scene(p0);
		else {
			reload = false;
			return MenuLayer::scene(false);
		}
	}
	bool init() {
		if (!MenuLayer::init()) return false;
		log::info("menulayer init!!");
		auto winSize = CCDirector::get()->getWinSize();

		if (!this->getChildByID("close-menu")) {
			auto menu = CCMenu::create();
			menu->setID("close-menu");
			menu->setContentSize(CCSize(200, 33.25));
			menu->ignoreAnchorPointForPosition(false);
			this->addChild(menu);
		}
			
		auto menu = this->getChildByID("close-menu");
		menu->setAnchorPoint(CCPoint(0, 1));
		menu->setPosition(winSize.width/300.f, winSize.height - winSize.width/300.f);
		auto layout = ColumnLayout::create()
					->setAxisReverse(true)
					->setAxisAlignment(AxisAlignment::End)
					->setGap(4.f)
					->setCrossAxisOverflow(false);

		menu->setLayout(layout);
		menu->setContentSize({menu->getContentSize().height, menu->getContentSize().width});

		// Exit button
		if (Mod::get()->getSettingValue<bool>("exit")) {
			if (!menu->getChildByID("close-button")) {
				auto cls_spr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
				cls_spr->setScale(0.7);
				auto cls_btn = CCMenuItemSpriteExtra::create(
					cls_spr, 
					this, 
					menu_selector(MenuLayer::onQuit));
				cls_btn->setID("close-button");
				menu->addChild(cls_btn);
			}
		} else if (menu->getChildByID("close-button")) {
			menu->removeChildByID("close-button");
		}
		
		// Restart button
		if (Mod::get()->getSettingValue<bool>("restart")) {
			auto res_spr = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
			res_spr->setScale(0.7);
			auto res_btn = CCMenuItemSpriteExtra::create(
				res_spr, 
				this, 
				menu_selector(AltMenuLayer::onRestart));
			res_btn->setID("restart-button");
			menu->addChild(res_btn);
		}

		// Reload textures button
		if (Mod::get()->getSettingValue<bool>("reload-textures")) {
			auto rel_spr = CCSprite::createWithSpriteFrameName("GJ_replayBtn_001.png");
			rel_spr->setScale(0.525);
			auto rel_btn = CCMenuItemSpriteExtra::create(
				rel_spr, 
				this, 
				menu_selector(AltMenuLayer::onReload));
			rel_btn->setID("restart-button");
			menu->addChild(rel_btn);
		}

		auto scale = Mod::get()->getSettingValue<double>("size");
		menu->setScale(scale);
		menu->updateLayout();

		return true;
	}

	void onRestart(CCObject* sender) {
		geode::createQuickPopup(
			"Restart Game",
			"Are you sure you want to <cg>restart</c>?",
			"Cancel", "Yes",
			[](auto, bool btn2) {
				if (btn2) {
					utils::game::restart();
				}
			}
		);
	}

	void onReload(CCObject* sender) {
		reload = true;
		geode::createQuickPopup(
			"Reload Textures",
			"Are you sure you want to <cj>reload textures</c>?",
			"Cancel", "Yes",
			[](auto, bool btn2) {
				if (btn2) {
					GameManager::get()->reloadAll(false, false, true);
					
				}
			}
		);
	}
};